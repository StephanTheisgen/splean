// (C) Copyright by Stephan Theisgen
//
// This is the main file for the splean miner approach.
// It is an attempt to claim the Linear Time-Memory Trade-Off Bounty from John Tromp
// (https://github.com/tromp/cuckoo?tab=readme-ov-file#linear-time-memory-trade-off-bounty):
//
// 		"$10000 for an open source implementation that uses at most N/k bits while finding 42-cycles
// 		up to 10 k times slower, for any k>=2.
// 		All of these bounties require n ranging over {27,29,31} and #threads ranging over {1,2,4,8},
//	 	and further assume a high-end Intel Core i7 or Xeon and recent gcc compiler with regular
//		flags as in my Makefile."
//
// The main function, its status output and command line arguments are kept very close to
// the lean miner approach from John Tromp (2019) (see https://github.com/tromp/cuckoo).
// Biggest exception is that multi-threading is controlled with a compilation parameter.
//
// It contains the "trim", "walk" and "check" functions to first trim a subset of the edges
// on one side, and then walk along a path up to PROOF_SIZE length on the other side.
// Afterwards it tests if the path indeed forms a cycle. And verifies that it is a valid
// Cuckatoo cycle of the correct length. It uses probabilistic (Bloom) filter data structures
// for space and time efficiency in the "walking" part of the algorithm, and it
// implements multi-threading and SIMD to speed up the calculations.
//
// Compilation instruction:
//
// 		For example use the following command to compile with GCC:
//	
// 		g++ splean.cpp blake2b-ref.c -O3 -std=c++11 -fno-strict-aliasing -march=native -pthread -o splean -DKFACTLOG=9 -DNSIPHASH=8 -DNTHREADS=8 -DNBATCHES=8
//
// 		-fno-strict-aliasing is important for e.g. SIMD Bloom filter implementation (in bloom.hpp), because
// 		reinterpret_casts used there technically violate C++'s strict aliasing rules, the same goes in helper.hpp
//		for SIMD Siphash calculations.
//
// 		-DKFACTLOG chooses the amount of memory-reduction (log2 of reduction factor)
//
// 		-DNSIPHASH option chooses how much SipHash calculations are performed in parallel:
//			  1, 2 (SSE2), 4 (SSE2/AVX2) and 8 (AVX2) are currently supported,
//			  choose -march accordingly.
//
// 		-DNTHREADS options activates multi-threading and chooses the amounts of parallel threads
// 
// 		-DVERBOSE option activate extensive (debug) output
//
// 		please see the header file "splean.hpp" for more fine-tuning options

#include <unistd.h>
#include <pthread.h>

#include <algorithm>
#include <new>

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cmath>

#include "splean.hpp"
#include "siphash.hpp"
#include "helper.hpp"

//solution data
uint32_t solution[PROOF_SIZE];
bool solved = false;
uint64_t solution_count = 0;

//thread data
pthread_barrier_t barrier;
struct thread_data {
	siphasher::keys keys;
	uint32_t id;
	pthread_t thread;
	walk_flt *walk;
	trim_flt *trim[2];
	bfs_t *bfs;
	edge_strm *edges;
#ifdef VERBOSE
	uint64_t hash_count;
#endif
};

//verify and report solution
inline bool verify(uint32_t* solution, const thread_data *data, siphasher& hasher) {
	std::sort(solution, solution+PROOF_SIZE);
	printf("Solution");
	for(uint32_t s = 0; s < PROOF_SIZE; s++) printf(" %x", solution[s]);
	printf("\n");
	if(verify(solution, hasher)) {
        unsigned char cyclehash[32];
        blake2b((void *)cyclehash, sizeof(cyclehash), solution, PROOF_SIZE*sizeof(uint32_t), 0, 0);
		printf("Verified with cyclehash ");
		for (int i = 0; i < 32; i++) printf("%02x", cyclehash[i]);
		printf("\n");
		solution_count++;
		return true;
	} else {
		printf("Verification FAILED!\n");
		return false;
	}	
}

inline void trim(const uint32_t begin, const uint32_t end, const thread_data *data, siphasher& hasher) noexcept {

	//shortcuts to data and values
	const uint32_t id = data->id;
	edge_strm& edges = *(data->edges);
	trim_flt& flt0 = *(data->trim[0]);
	trim_flt& flt1 = *(data->trim[1]);
#if (NBATCHES > 1) //checking potential cycle from previous batch round is only needed if batching is actually used
	bfs_t& bfs = *(data->bfs);
#endif
	const uint32_t beg_edgs = (id+0)*(EDGE_SIZE/NTHREADS); //must be multiple of NSIPHASH
	const uint32_t end_edgs = (id+1)*(EDGE_SIZE/NTHREADS);
	alignas(32) uint64_t ns[NSIPHASH]; //alignment is important here or SIMD parallel SipHash calculations will not work properly

	//create data structures (needed for trimming stage) in place
	if(0 == data->id) { //only one thread needs to initialize the common data structures
		new (data->trim[0]) trim_flt();
		new (data->trim[1]) trim_flt();
	}
	pthread_barrier_wait(&barrier);

	//load edges to trim
	uint8_t uorv = 1; //trim on the the right side
	for(uint32_t e = begin; e < end; e += NSIPHASH) {
		calc_nodes(e, uorv, ns, hasher);
		for(uint32_t i = 0; i < NSIPHASH; i++) flt0.insert(ns[i]);
	}
#if (NBATCHES > 1) //checking potential cycle from previous batch round is only needed if batching is actually used
	if(0 == id) bfs.walk(uorv, hasher);
#endif
	pthread_barrier_wait(&barrier);

	//mark nodes (in second filter) which are hit by another edge
	for(uint32_t e = beg_edgs; e < end_edgs; e += NSIPHASH) {
		calc_nodes(e, uorv, ns, hasher);
		for(uint32_t i = 0; i < NSIPHASH; i++) {
			if(flt0.contains(ns[i]^1)) flt1.insert(ns[i]^1);
#if (NBATCHES > 1) //checking potential cycle from previous batch round is only needed if batching is actually used
			bfs.push(e+i, ns[i]);
#endif
		}
	}
	pthread_barrier_wait(&barrier);

	//trim edges based on marked nodes
	edges.clear();	
	for(uint32_t e = begin; e < end; e += NSIPHASH) {
		calc_nodes(e, uorv, ns, hasher);
		for(uint32_t i = 0; i < NSIPHASH; i++) {
			if(flt1.contains(ns[i])) edges.write(e+i);
		}
	}
	pthread_barrier_wait(&barrier);

	//print statistics
#ifdef VERBOSE
	if(0 == id) {
		printf("Trim Filter 1 usage is %.3f (%u items)\n", flt0.usage(), flt0.count());
		printf("Trim Filter 2 usage is %.3f (%u items)\n", flt1.usage(), flt1.count());
	}
	printf("Thread %u:: Edge stream usage is %.3f (%u items)\n", id, edges.usage(), edges.count());
#endif

}

inline void walk(const uint32_t begin, const uint32_t end, const thread_data *data, siphasher& hasher) noexcept {

	//shortcuts to data and values
	const uint32_t id = data->id;
	edge_strm& edges = *(data->edges);
	walk_flt& flt = *(data->walk);
	bfs_t& bfs = *(data->bfs);
	const uint32_t beg_edgs = (id+0)*(EDGE_SIZE/NTHREADS); //must be multiple of NSIPHASH
	const uint32_t end_edgs = (id+1)*(EDGE_SIZE/NTHREADS);
	alignas(32) uint64_t ns[NSIPHASH]; //alignment is important here or SIMD parallel SipHash calculations will not work properly

	//create data structures (needed for trimming stage) in place 
	if(0 == data->id) new (data->walk) walk_flt(); //only one thread needs to initialize the common data structures
	pthread_barrier_wait(&barrier);

	//walk PROOF_SIZE-1 steps
	uint8_t uorv = 0;
	for(uint32_t r = 0; r < (PROOF_SIZE-1); r++) {

		if(0 == id) flt.clear();
		pthread_barrier_wait(&barrier);

		//load edges from previous round
		edges.reset();
		uint32_t n;
		do {
			n = calc_nodes(edges, uorv, ns, hasher);
			for(uint32_t i = 0; i < n; i++) flt.insert(ns[i]);
		} while(n == NSIPHASH);
#if (NBATCHES > 1) //checking potential cycle from previous batch round is only needed if batching is actually used
		if(0 == id && r < (PROOF_SIZE-2)) bfs.walk(uorv, hasher);
#endif
		pthread_barrier_wait(&barrier);

		//advance the node by incident edges
		edges.clear();
		for(uint32_t e = beg_edgs; e < end_edgs; e += NSIPHASH) {
			calc_nodes(e, uorv, ns, hasher);
			for(uint32_t i = 0; i < NSIPHASH; i++) {
				if(flt.contains(ns[i]^1)) edges.write(e+i);
#if (NBATCHES > 1)  //checking potential cycles from previous batch round is only needed if batching is actually used
				if(r < (PROOF_SIZE-2)) bfs.push(e+i, ns[i]);
#endif
			}
			
		}
		pthread_barrier_wait(&barrier);

		//print statistics
#ifdef VERBOSE
		if(0 == id) {
			printf("Walk Filter usage in round %u is %.3f (%u items)\n", r, flt.usage(), flt.count());
#if (NBATCHES > 1) //report only if batching is used
			printf("Check potential cycles (round %u): Edge array usage is %.3f (%u items), Hashmap usage is %.3f (%u items)\n", r, bfs.edge_usage(), bfs.edge_count(), bfs.node_usage(), bfs.node_count());
#endif
		}
		printf("Thread %u:: Edge stream usage in round %u is %.3f (%u items)\n", id, r, edges.usage(), edges.count());
		pthread_barrier_wait(&barrier);
#endif		

		uorv ^= 1;
	}

#if (NBATCHES > 1) //checking potential cycles from previous batch round is only needed if batching is actually used
	//check for potential cycles
	if(0 == id) {
		if(bfs.test(solution, uorv^1, hasher)) {
#ifdef VERBOSE
			printf("Cycle found anchored at edge 0x%x!\n", solution[0]);
#endif
			solved = verify(solution, data, hasher);
		}
		bfs.clear();
	}
	pthread_barrier_wait(&barrier);
	if(solved) return;
#endif
	if(0 == id) flt.clear();
	pthread_barrier_wait(&barrier);

	//load leftover edges after PROOF_SIZE-1 steps
	edges.reset();
	uint32_t n;
	do {
		n = calc_nodes(edges, uorv, ns, hasher);
		for(uint32_t i = 0; i < n; i++) flt.insert(ns[i]);
	} while(n == NSIPHASH);
	pthread_barrier_wait(&barrier);
#if (NBATCHES == 1) //if batching is not used prepare to stream candidates edges
	edges.clear();
#endif
	//check against edges from the beginning of the walk, matches are potential cycles
	for(uint32_t e = begin; e < end; e += NSIPHASH) {
		calc_nodes(e, uorv, ns, hasher);
		for(uint32_t i = 0; i < NSIPHASH; i++) {
			if(flt.contains(ns[i]^1)) {
#if (NBATCHES == 1) //if batching is not used, first stream candidate edges to an edge buffer, so that filter and BFS-tree data structure do not co-exist and can use same memory space
				edges.write(e+i);
#else //if batching is used, filter and BFS-tree data structure co-exists and do not share memory, so we can directly insert the candidate edges
				bfs.insert(e+i);
#endif
#ifdef VERBOSE
				printf("Potential cycle found anchored at edge: 0x%x!\n", e+i);
#endif
			}
		}
	}

	pthread_barrier_wait(&barrier);

#if (NBATCHES == 1)
	if(0 == id) new (data->bfs) bfs_t(); //if batching is not used, initialize BFS-tree data structure only here (BFS-tree and filter do overlap in memory)
	pthread_barrier_wait(&barrier);
	edges.reset();
	uint32_t e;
	while(edges.read(e)) bfs.insert(e); //stream candidate edges into BFS-tree data structure
	pthread_barrier_wait(&barrier);
#endif

}

inline bool check(uint32_t* solution, const thread_data *data, siphasher& hasher) {

	//shortcuts to data
	bfs_t& bfs = *(data->bfs);
	if(!bfs.edge_count()) return false; //if no candidates, clearly no cycles possible

	//shortcuts values
	const uint32_t id = data->id;
	const uint32_t beg_edgs = (id+0)*(EDGE_SIZE/NTHREADS); //must be multiple of NSIPHASH
	const uint32_t end_edgs = (id+1)*(EDGE_SIZE/NTHREADS);
	alignas(32) uint64_t ns[NSIPHASH]; //alignment is important here or SIMD parallel SipHash calculations will not work properly

	//extend cycle until full PROOF_SIZE
	uint8_t uorv = 1;
	for(uint32_t r = 0; r < (PROOF_SIZE-1); r++) {
		
		if(0 == id) bfs.walk(uorv, hasher); //BFS-tree walk is not thread-safe, only use one thread to advance (here thread with id 0)
		pthread_barrier_wait(&barrier); //all threads wait until thread 0 has completed advancing the tree

		for(uint32_t e = beg_edgs; e < end_edgs; e += NSIPHASH) { //calculate in parallel potential next steps
			calc_nodes(e, uorv, ns, hasher);
			for(uint32_t i = 0; i < NSIPHASH; i++) bfs.push(e+i, ns[i]);
		}
		pthread_barrier_wait(&barrier);

		uorv ^= 1; //switch sides
		
#ifdef VERBOSE
		pthread_barrier_wait(&barrier);
		if(0 == id) printf("Check potential cycles (round %u): Edge array usage is %.3f (%u items), Hashmap usage is %.3f (%u items)\n", r, bfs.edge_usage(), bfs.edge_count(), bfs.node_usage(), bfs.node_count());
		pthread_barrier_wait(&barrier);
#endif
	}

	//check for potential cycles on final edge
	if(0 == id && bfs.test(solution, uorv, hasher)) { //BFS-tree test for cycle is not thread-safe, only use one thread to test (here thread with id 0)
#ifdef VERBOSE
		printf("Cycle found anchored at edge 0x%x!\n", solution[0]);
#endif
		return verify(solution, data, hasher);		
	}

	return false;
}

//main solver function for splean solver approach
void *solve(void *vp) {

	//get thread local data and prepare thread local hasher object
	thread_data *data = (thread_data *)vp;
	siphasher hasher(data->keys);

	//init values
	new (data->edges) edge_strm();
#if (NBATCHES > 1)
	if(0 == data->id) new (data->bfs) bfs_t(); //if multiple batches are used, initialize BFS-tree data structure already here (BFS-tree and filter do not overlap in this case)
#endif
	pthread_barrier_wait(&barrier);

	for(uint32_t b = 0; b < NBATCHES; b++) {

		const uint32_t beg = (b*BATCH_SIZE)+(data->id)*(BATCH_SIZE/NTHREADS); //must be multiple of NSIPHASH
		const uint32_t end = (b*BATCH_SIZE)+(data->id+1)*(BATCH_SIZE/NTHREADS);

#ifdef VERBOSE
		printf("Batch %u with Thread %u: Edges from %i -> %i.\n", b, data->id, beg, end);
#endif

		//first trim right side
		trim(beg, end, data, hasher);

		//now walk
		walk(beg, end, data, hasher);

#ifdef VERBOSE
		data->hash_count = hasher.count();
#endif
		//check if solution found in candidates from last batch
		if(solved) {
			pthread_exit(NULL);
			return 0;
		}
	}
	pthread_barrier_wait(&barrier);

	//check for cycles
	if(check(solution, data, hasher)) solved = true; //check function makes sure only thread 0 can receive a true value as return

#ifdef VERBOSE
	data->hash_count = hasher.count();
#endif

	pthread_exit(NULL); //report no cycles found
	return 0;
}

int main(int argc, char **argv) {

	//default parameter
	uint32_t nonce = 0;
	uint32_t range = 1;
	char header[HEADER_LEN] = {0};

	//parse command line parameter
	int opt;
	while((opt = getopt(argc, argv, "h:m:n:r:t:x:")) != -1) {
		switch(opt) {
			case 'h': //set header
				memcpy(header, optarg, strlen(optarg));
				break;
			case 'n': //set nonce to start
				nonce = strtoul(optarg, nullptr, 0);
				break;
			case 'r': //set range (how many nonces, starting at nonce to test)
				range = strtoul(optarg, nullptr, 0);
				break;
			case 'x': //set header in hex format
				for(int i = 0, len = strlen(optarg)/2; i < len; i++) sscanf(optarg+2*i, "%2hhx", header+i);
				break;
		}
	}

	//print info
	printf("Looking for %d-cycle on cuckatoo%d(\"", PROOF_SIZE, EDGE_BITS);
	for(int i = 0; i < HEADER_LEN; i++) printf("%02x", (unsigned char) header[i]);
	printf("\"");
	if(range > 1) {
		printf(",%d-%d", nonce, nonce+range-1);
	} else if(nonce) {
		printf(",%d", nonce);
	}
	printf("), with %d threads\n", NTHREADS);

	//reserve and set up memory, prepare threads
	pthread_barrier_init(&barrier, NULL, NTHREADS);
	thread_data *threads = new thread_data[NTHREADS];
	uint64_t bfs_size = sizeof(bfs_t);
	uint64_t walk_size = sizeof(walk_flt);
	uint64_t trim_size = sizeof(trim_flt)*2;
	uint64_t strm_size = sizeof(edge_strm)*NTHREADS;
	uint64_t flt_size = std::max(walk_size, trim_size);
#if (NBATCHES == 1)
	uint64_t flt_bfs_size = std::max(flt_size, bfs_size);
	uint64_t mem_size = flt_bfs_size+strm_size; //if only one batch is used, BFS tree can overlap with filter in memory. They are never used at the same time.
#else
	uint64_t mem_size = flt_size+bfs_size+strm_size; //with batching BFS tree is non-overlapping with filters and uses extra memory
#endif
	void *mem = aligned_alloc(32, mem_size); //RESERVE ENTIRE MEMORY HERE (respect alignment for SIMD version of filter!)
	if(!mem) {
		printf("PANIC: Cannot reserve memory!\n");
		return -1;
	}
	for(uint32_t t = 0; t < NTHREADS; t++) {
		threads[t].walk = (walk_flt *)mem; //place walk filter first to respect alignment
		threads[t].trim[0] = ((trim_flt *)mem)+0; //share space with walk filter, walk filter and trim filter are never used at the same time
		threads[t].trim[1] = ((trim_flt *)mem)+1; //Two trim filter fit into the space of one walk filter
#if (NBATCHES == 1)
		threads[t].bfs = ((bfs_t *)(((char *)mem))); //if only one batch is used, BFS tree can overlap with filter in memory. They are never used at the same time.
		threads[t].edges = ((edge_strm *)(((char *)mem)+flt_bfs_size))+t; //place edge streams after filter/BFS, each thread has its own edge filter
#else
		threads[t].bfs = ((bfs_t *)(((char *)mem)+flt_size)); //BFS tree data structure (if batching is used, extra space is needed)
		threads[t].edges = ((edge_strm *)(((char *)mem)+flt_size+bfs_size))+t; //place edge streams after filter and BFS, each thread has its own edge filter
#endif
	}
	int mem_unit = std::min(floor((double)log(mem_size)/log(1024.0)), 4.0);
	double kfact = (double)EDGE_SIZE/(mem_size*8);
	double attempts = 1.0/(1.0-std::pow(1.0-((double)BATCH_SIZE*NBATCHES/EDGE_SIZE), 42.0));	
	const char *mem_unit_str[] = { "B", "KiB", "MiB", "GiB", "TiB" };
#if (NBATCHES == 1)
	printf("Using %lu bits (%.1f %s) of max. memory (k-Factor: %.3f, attempts: %.3f, batch size: %llu, batches: %u), %lu bits for trim filter / walk filter / BFS tree and %lu bits for edge streams, using %u-way siphash\n", mem_size*8, (double)mem_size/pow(1024,mem_unit), mem_unit_str[mem_unit], kfact, attempts, BATCH_SIZE, NBATCHES, flt_bfs_size*8, strm_size*8, NSIPHASH);
#else
	printf("Using %lu bits (%.1f %s) of max. memory (k-Factor: %.3f, attempts: %.3f, batch size: %llu, batches: %u), %lu bits for trim/walk filter, %lu bits for BFS-tree and %lu bits for edge streams, using %u-way siphash\n", mem_size*8, (double)mem_size/pow(1024,mem_unit), mem_unit_str[mem_unit], kfact, attempts, BATCH_SIZE, NBATCHES, flt_size*8, bfs_size*8, strm_size*8, NSIPHASH);
#endif
#ifdef VERBOSE
	printf("Trim filters:   %16lu bits\n", trim_size*8);
	printf("Walk filter:    %16lu bits\n", walk_size*8);
	printf("BFS tree:       %16lu bits\n", bfs_size*8);
	printf("Edge streams:   %16lu bits\n", strm_size*8);
#endif
	//set up and run solver
	for(uint32_t r = nonce; r < (nonce+range); r++) {
    	uint64_t start = timestamp();
		//get siphash keys from header
		set_header_nonce(header, r);
		siphasher::keys keys;
		get_siphash_keys(header, keys);
		printf("nonce %d k0 k1 k2 k3 %lx %lx %lx %lx\n", r, keys.v0, keys.v1, keys.v2, keys.v3);
		//set up threads
		for(uint32_t t = 0; t < NTHREADS; t++) {
			threads[t].id = t;
			threads[t].keys = keys;
			if(pthread_create(&threads[t].thread, NULL, solve, (void *)&threads[t])) {
				printf("PANIC: Cannot create threads!\n");
				return -1;
			}
		}
		//join threads
		for(uint32_t t = 0; t < NTHREADS; t++) {
			pthread_join(threads[t].thread, NULL);
		}
		uint64_t stop = timestamp();
		//report statistics
		//double attempts = 1.0/(1.0-std::pow(1.0-((double)BATCH_SIZE*NBATCHES/EDGE_SIZE), 42.0));
		double duration = (double)(stop - start) / 1000000.0;
		double equi_time = ((duration*attempts)/(10*kfact));
	    printf("Time: %.2f ms with %.2f attempts needed (equivalent to lean miner: %.2f ms, k-factor: %.3f)\n", duration, attempts, equi_time, kfact);
#ifdef VERBOSE
		uint64_t hcnt = 0;
		for(uint32_t t = 0; t < NTHREADS; t++) {
			uint64_t hc = threads[t].hash_count;
			printf("Thread %u: %lu hashes (%0.2f per edge).\n", t, hc, (double) hc/EDGE_SIZE);
			hcnt += hc;
		}
		printf("Total hashes: %lu (%0.2f per edge).\n", hcnt, (double) hcnt/EDGE_SIZE);
#endif
		//report cycle if one was found
		if(solved) {
			static siphasher hasher(keys);

			solved = false;
		}
	}
	printf("%lu total solutions\n", solution_count);

	//clean up
	delete[] threads;
	free(mem);

	return 0;
}
