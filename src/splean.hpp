// (C) Copyright by Stephan Theisgen
//
// This file contains general parameter for the splean miner approach.
// For details see comments near each #define clause.
//
// It also contains the B(readth) f(irst) s(earch) tree structure.
// This structure is used to check if indeed a cycle was found after the "walk" step
// is completed. Ideally, only a small amount of candidate anchor edges are found at the
// end of the "walk" step. So, the data structure can be quite small.
// However, it still needs to maintain a full history while "walk"-ing the full path again,
// to later successfully retrieve all edges that constitute the cycle.
// It does this, by recording each edge and maintaining a tree structure where each node (edge)
// knows its parent (previous edge). It also maintains in a multi-hashmap all nodes'
// current leaf edges, to decide if a potential new edge is actually incident to a leaf node
// and extends the path/tree further.

#ifndef SPLEAN_HPP
#define SPLEAN_HPP

//CRYPTO PARAMETER

#ifndef HEADER_LEN
	#define HEADER_LEN (246)
#endif

//CUCKOO PARAMETER

#ifndef EDGE_BITS
	#define EDGE_BITS (19)
#endif

#ifndef PROOF_SIZE
	#define PROOF_SIZE (42)
#endif

#define EDGE_SIZE (1ULL << EDGE_BITS)
#define EDGE_MASK (EDGE_SIZE-1)
#define NODE_BITS (EDGE_BITS)
#define NODE_SIZE (1ULL << NODE_BITS)
#define NODE_MASK (NODE_SIZE-1)

//SOLVER PARAMETER

//following parameter are usually set for compiling

#ifndef NTHREADS
	#define NTHREADS (8) //how many threads to use
#endif

#ifndef NSIPHASH
	#define NSIPHASH (8) //how many parallel SipHash24 computations: 1, 2 (SSE2), 4 (SSE2/AVX2) and 8 (AVX2)
#endif

#ifndef KFACTLOG
	#define KFACTLOG (9) //log2 of reduction factor, only a fraction of 1/KFACTOR of edges will be tested for being part of a cycle.
#endif

//following parameter are usually calculated automatically from the parameter above

#ifndef KFACTOR
	#define KFACTOR (1<<KFACTLOG) //reduction factor, only a fraction of 1/KFACTOR of edges will be tested for being part of a cycle.
#endif

#define BATCH_SIZE (EDGE_SIZE/KFACTOR) //automatically calculate BATCH_SIZE

#ifndef NBATCHES
	#define NBATCHES (4) //how many consecutive batches of 1/KFACTOR edges to run before switching to next Cuckoo graph
#endif

#ifndef WALKFLT_SIZE
	#define WALKFLT_SIZE (BATCH_SIZE*64) //about 64 bits per element is a good value
#endif

#ifndef TRIMFLT_SIZE
	#define TRIMFLT_SIZE (WALKFLT_SIZE/2) //calculate from WALKFLT_SIZE to efficiently use the memory, higher false-positive rate is tolerable
#endif

#ifndef STREAM_SIZE
	#define STREAM_SIZE ((BATCH_SIZE/NTHREADS)*(KFACTLOG+2)*150/100) //apply +50% safety margin 
#endif

#ifndef STREAM_PARAM
	#define STREAM_PARAM (KFACTLOG) //calculate Rice stream parameter from memory reduction (k-) factor
#endif

#ifndef MAX_CYCLES
	#define MAX_CYCLES (512) //how many cycles candidates can be processed
#endif

#ifndef BFS_MAX_EDGES
	#define BFS_MAX_EDGES (MAX_CYCLES*PROOF_SIZE*3) //2x is a good safety margin
#endif

#ifndef BFS_MAX_NODES
	#define BFS_MAX_NODES (MAX_CYCLES*48) //48x is a good safety margin
#endif

#ifndef BFS_LINK_BITS //set to 32 to deactivate pruning (must be <= 32)
	#define BFS_LINK_BITS (18) //bits for maximum position in BFS edge array (rest to 32 bit is to store shift amount while pruning)
#endif

//some safety checks

#if ((EDGES_SIZE/NTHREADS) % NSIPHASH) != 0
	#error EDGE_SIZE/NTHREADS needs to be a multiple of NSIPHASH
#endif

#if ((BATCH_SIZE/NTHREADS) % NSIPHASH) != 0
	#error BATCH_SIZE/NTHREADS needs to be a multiple of NSIPHASH
#endif

#if (1<<BFS_LINK_BITS) < BFS_MAX_EDGES
	#error Not enough link bits (BFS_LINK_BITS) to cover the full BFS edge array (see BFS_MAX_EDGES)
#endif

//SOLVER

#include <atomic>
#include <algorithm>
#include <cstdio>

#include "rice.hpp"
#include "bloom.hpp"
#include "hashmap.hpp"
#include "siphash.hpp"
#include "helper.hpp"

//BREADTH FIRST SEARCH TREE structures

template<uint32_t MAX, uint32_t SIZE, uint32_t LNKB> //MAX edges in tree, SIZE nodes at tree's current leaf level, 2^LNK is maximum distance allowed in link
class bfs_tree {

	public:

		bfs_tree() { clear(); };

#if (LNKB < 32) //pruning

	private:

		static const uint64_t PRUN_SHFT = LNKB;
		static const uint64_t PRUN_MASK = (1llu<<(32-LNKB))-1;		

		inline static uint32_t get_shift(const uint64_t item) noexcept { return ((item >> PRUN_SHFT) & PRUN_MASK); }; //get shift for pruning purpose
		inline static void set_shift(uint64_t& item, const uint32_t shift) { //store shift for pruning purpose
			item &= ~(PRUN_MASK << PRUN_SHFT); //clear shift
			item |= ((shift & PRUN_MASK) << PRUN_SHFT); //store new shift
		};

	public:

		inline void prune() { //keeps only edges that are still part of a path (reduces memory demand on BFS tree structure); NOT THREAD-SAFE, call only from one defined thread
			//start pruning
			for(uint32_t p = cur; p < cnt; p++) { //start at leaf-edges
				uint32_t q = p;
				while(q != VAL_MASK) { //current edge has precursor
					set_shift(edges[q], PRUN_MASK); //mark current edge as active
					q = get_value(edges[q]); //walk further backwards
				}
			}
			//update links respecting inactive edges that will be removed in the next step
			uint32_t shift = 0;
			for(uint32_t p = 0; p < cnt; p++) {
				uint64_t& e = edges[p];
				if(get_shift(e) == PRUN_MASK) { //actual edge is active
					set_shift(e, shift); //update shift respecting removal of inactive edges
					uint32_t l = get_value(e); //get link to precursor edge
					if(l != VAL_MASK) set_value(e, l - get_shift(edges[l])); //if not starting edge, update link with calculated updated shift respecting removed edges
				} else { //actual edge is inactive
					set_shift(e, 0); //do not shift inactive edge, so it gets overwritten
					shift++; //increase shift
					if(shift >= PRUN_MASK) { printf("ALERT: Shift too far in BFS tree pruning --> pruning deactivated.\n"); return; } //if we try to shift too far, break to not cause trouble
				}
			}
			//update positions and removing/overwriting inactive edges
			for(uint32_t p = 0; p < cnt; p++) edges[p-get_shift(edges[p])] = edges[p];
			cnt -= shift; //update max position respecting removed edges
			cur -= shift; //update current position respecting removed edges
		}
#else //no pruning
		inline void prune() {};
#endif

		inline void walk(uint8_t uorv, siphasher& hasher) { //advance to next round, NOT THREAD-SAFE, call only from one defined thread
			//prune dead-end edges to save some memory
			prune();
			//calculate for all current (leaf) edges (cur to cnt) the (leaf) nodes
			nodes.clear();
			for(uint32_t p = cur; p < cnt; p++) {
				uint64_t n = calc_node(get_key(edges[p]), uorv, hasher);
				nodes.insert(n, p);
			}
			cur = cnt; //advance
		};

		bool test(uint32_t* sol, uint8_t uorv, siphasher& hasher) { //check for cycle, NOT THREAD-SAFE call only from one defined thread
			if(!cnt) return false;
			//check for all final (leaf) edges (cur to cnt) if they form a cycle if PROOF_SIZE length
			for(uint32_t i = cur; i < cnt; i++) {
				uint32_t s = 0;
				uint64_t edg = edges[i];
				//traverse backwards to record all edges for this potential cycle
				while(get_value(edg) != VAL_MASK) {
					uint32_t e = get_key(edg);
					sol[s++] = e;
					edg = edges[get_value(edg)];
				}
				sol[s++] = get_key(edg);
				//calculate nodes of potential cycle closing edge
				uint64_t n0 = calc_node(get_key(edges[i]), uorv, hasher);
				uint64_t n1 = calc_node(get_key(edg), uorv, hasher);
				if(n0 == (n1^1)) return true; //indeed we found a cycle
			}
			return false; //it was only a path of length PROOF_SIZE but no cycle
		}

		inline void clear() noexcept { cnt = 0; cur = 0; nodes.clear(); }; //NOT THREAD-SAFE call only from one defined thread

		inline uint32_t edge_count() const noexcept { return cnt; }; //implicit cnt.load with strong memory ordering if cnt is atomic
		inline double edge_usage() const noexcept { return (double)cnt/MAX; };

		inline uint32_t node_count() const noexcept { return nodes.count(); };
		inline double node_usage() const noexcept { return nodes.usage(); };

		static uint64_t size() noexcept { return MAX*8*sizeof(uint64_t)+hashmap<SIZE>::size(); }; //size in bits

	private:

		static const uint64_t VAL_MASK = (1llu<<LNKB)-1;

		inline static uint64_t make_item(const uint32_t key, const uint32_t value) noexcept { return (((uint64_t)key<<32) | (value & VAL_MASK)); }; //value should never be > VAL_MASK
		inline static uint32_t get_key(const uint64_t item) noexcept  { return (item>>32); };
		inline static uint32_t get_value(const uint64_t item) noexcept  { return (item & VAL_MASK); };
		inline static void set_value(uint64_t& item, const uint32_t value) {
			item &= ~VAL_MASK; //clear value;
			item |= (value & VAL_MASK); //store new value
		};

		hashmap<SIZE> nodes;
		uint64_t edges[MAX];
		uint32_t cur;

#if (NTHREADS > 1) //concurrent version

	public:

		inline void insert(uint32_t edge, uint32_t parent = VAL_MASK) noexcept { //insert edge into array, VAL_MASK means no parent
			uint32_t i = cnt.fetch_add(1, std::memory_order_relaxed); //get free spot in edge array
			if(i < MAX) {
				edges[i] = make_item(edge, parent); //make and insert item on next free spot, -1 marks no parent
			} else {
				cnt.fetch_sub(1, std::memory_order_relaxed); //we went too far, so correct
				printf("PANIC: BFS-tree edge capacity reached!\n");
			}
		};

		inline void push(uint32_t edge, uint32_t node) { //check and insert edge if it fits
			if(!cnt.load(std::memory_order_relaxed)) return;
			uint32_t pos = 0, parent;
			while(nodes.get(node^1, parent, pos)) insert(edge, parent);
		};

	private:

		std::atomic<uint32_t> cnt;

#else //non-concurrent version

	public:

		inline void insert(uint32_t edge, uint32_t parent = VAL_MASK) noexcept { //insert edge into array, VAL_MASK means no parent
			if(cnt < MAX) {
				edges[cnt++] = make_item(edge, parent); //make and insert item on next free spot, -1 marks no parent
			} else {
				printf("PANIC: BFS-tree edge capacity reached!\n");
			}
		};

		inline void push(uint32_t edge, uint32_t node) noexcept { //check and insert edge if it fits
			if(!cnt) return;
			uint32_t pos = 0, parent;
			while(nodes.get(node^1, parent, pos)) insert(edge, parent);
		};

	private:

		uint32_t cnt;
		
#endif

};

typedef bfs_tree<BFS_MAX_EDGES, BFS_MAX_NODES, BFS_LINK_BITS> bfs_t;

#endif
