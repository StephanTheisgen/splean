.POSIX:
.SUFFIXES:

OPT ?= -O3

GCC_ARCH_FLAGS ?= -march=native
GPP_ARCH_FLAGS ?= -march=native

# -Wno-deprecated-declarations shuts up Apple OSX clang
FLAGS ?= -Wall -Wno-format -Wno-deprecated-declarations -D_POSIX_C_SOURCE=200112L $(OPT) -DPREFETCH -I. $(CPPFLAGS) -pthread
GPP ?= g++ $(GPP_ARCH_FLAGS) -std=c++11 $(FLAGS)
CFLAGS ?= -Wall -Wno-format -fomit-frame-pointer $(OPT)
GCC ?= gcc $(GCC_ARCH_FLAGS) -std=gnu11 $(CFLAGS)
BLAKE_2B_SRC ?= crypto/blake2b-ref.c
NVCC ?= nvcc -std=c++11

# create lean miner programs for n [27, 29, 31] with [1,2,4,8] threads to evaluate the baseline

baseline: lean27 lean29 lean31

lean27: lean27x1 lean27x2 lean27x4 lean27x8
lean29: lean29x1 lean29x2 lean29x4 lean29x8
lean31: lean31x1 lean31x2 lean31x4 lean31x8

lean27x1:	crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -DNSIPHASH=1 -DATOMIC -DEDGEBITS=27 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean27x2:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mno-avx -DNSIPHASH=2 -DATOMIC -DEDGEBITS=27 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean27x4:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mavx2 -DNSIPHASH=4 -DATOMIC -DEDGEBITS=27 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean27x8:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mavx2 -DNSIPHASH=8 -DATOMIC -DEDGEBITS=27 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean29x1:	crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -DNSIPHASH=1 -DATOMIC -DEDGEBITS=29 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean29x2:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mno-avx -DNSIPHASH=2 -DATOMIC -DEDGEBITS=29 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean29x4:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mavx2 -DNSIPHASH=4 -DATOMIC -DEDGEBITS=29 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean29x8:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mavx2 -DNSIPHASH=8 -DATOMIC -DEDGEBITS=29 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean31x1:	crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -DNSIPHASH=1 -DATOMIC -DEDGEBITS=31 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean31x2:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mno-avx -DNSIPHASH=2 -DATOMIC -DEDGEBITS=31 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean31x4:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mavx2 -DNSIPHASH=4 -DATOMIC -DEDGEBITS=31 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

lean31x8:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o $@ -mavx2 -DNSIPHASH=8 -DATOMIC -DEDGEBITS=31 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

# create mean cuda miner to find cycles quickly as test set for evaluation

verify: mean27cu mean29cu lean31cu

mean27cu:	crypto/siphash.cuh cuckatoo/mean.cu Makefile
	$(NVCC) -o $@ -DEDGEBITS=27 -arch sm_75 cuckatoo/mean.cu $(BLAKE_2B_SRC)

mean29cu:	crypto/siphash.cuh cuckatoo/mean.cu Makefile
	$(NVCC) -o $@ -DEDGEBITS=29 -arch sm_75 cuckatoo/mean.cu $(BLAKE_2B_SRC)

lean31cu:	crypto/siphash.cuh cuckatoo/lean.cu Makefile
	$(NVCC) -o $@ -DEDGEBITS=31 -arch sm_75 cuckatoo/lean.cu $(BLAKE_2B_SRC)

	
# create splean miner programs for n [27, 29, 31] with [1,2,4,8] threads and targeted reduction factor k [9, 10, 11, 12, 13, 14, 15, 16]

splean: splean27x8 splean29x8 splean31x8

nthreads := 8 4 2 1

# loop to create versions with different number of threads for splean27x8

splean27x8_k10 := $(addprefix splean27x8_k10_t,$(nthreads))
splean27x8_k11 := $(addprefix splean27x8_k11_t,$(nthreads))
splean27x8_k12 := $(addprefix splean27x8_k12_t,$(nthreads))
splean27x8_k13 := $(addprefix splean27x8_k13_t,$(nthreads))
splean27x8_k14 := $(addprefix splean27x8_k14_t,$(nthreads))

splean27x8: $(splean27x8_k10) $(splean27x8_k11) $(splean27x8_k12) $(splean27x8_k13) $(splean27x8_k14)

$(splean27x8_k10): splean27x8_k10_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=10 -DNBATCHES=4 -DMAX_CYCLES=256 -DBFS_LINK_BITS=15 -DVERBOSE

$(splean27x8_k11): splean27x8_k11_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=11 -DNBATCHES=4 -DMAX_CYCLES=128 -DBFS_LINK_BITS=14 -DVERBOSE

$(splean27x8_k12): splean27x8_k12_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=12 -DNBATCHES=4 -DMAX_CYCLES=64 -DBFS_LINK_BITS=13 -DVERBOSE

$(splean27x8_k13): splean27x8_k13_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=13 -DNBATCHES=4 -DMAX_CYCLES=32 -DBFS_LINK_BITS=12 -DVERBOSE

$(splean27x8_k14): splean27x8_k14_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=16 -DBFS_LINK_BITS=11 -DVERBOSE

# loop to create versions with different number of threads for splean29x8

splean29x8_k10 := $(addprefix splean29x8_k10_t,$(nthreads))
splean29x8_k11 := $(addprefix splean29x8_k11_t,$(nthreads))
splean29x8_k12 := $(addprefix splean29x8_k12_t,$(nthreads))
splean29x8_k13 := $(addprefix splean29x8_k13_t,$(nthreads))
splean29x8_k14 := $(addprefix splean29x8_k14_t,$(nthreads))

splean29x8: $(splean29x8_k10) $(splean29x8_k11) $(splean29x8_k12) $(splean29x8_k13) $(splean29x8_k14)

$(splean29x8_k10): splean29x8_k10_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=10 -DNBATCHES=4 -DMAX_CYCLES=1024 -DBFS_LINK_BITS=17 -DVERBOSE

$(splean29x8_k11): splean29x8_k11_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=11 -DNBATCHES=4 -DMAX_CYCLES=512 -DBFS_LINK_BITS=16 -DVERBOSE

$(splean29x8_k12): splean29x8_k12_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=12 -DNBATCHES=4 -DMAX_CYCLES=256 -DBFS_LINK_BITS=15 -DVERBOSE

$(splean29x8_k13): splean29x8_k13_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=13 -DNBATCHES=4 -DMAX_CYCLES=128 -DBFS_LINK_BITS=14 -DVERBOSE

$(splean29x8_k14): splean29x8_k14_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=64 -DBFS_LINK_BITS=13 -DVERBOSE
	
# loop to create versions with different number of threads for splean31x8

splean31x8_k10 := $(addprefix splean31x8_k10_t,$(nthreads))
splean31x8_k11 := $(addprefix splean31x8_k11_t,$(nthreads))
splean31x8_k12 := $(addprefix splean31x8_k12_t,$(nthreads))
splean31x8_k13 := $(addprefix splean31x8_k13_t,$(nthreads))
splean31x8_k14 := $(addprefix splean31x8_k14_t,$(nthreads))

splean31x8: $(splean31x8_k10) $(splean31x8_k11) $(splean31x8_k12) $(splean31x8_k13) $(splean31x8_k14)

$(splean31x8_k10): splean31x8_k10_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=10 -DNBATCHES=4 -DMAX_CYCLES=4096 -DBFS_LINK_BITS=19 -DVERBOSE

$(splean31x8_k11): splean31x8_k11_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=11 -DNBATCHES=4 -DMAX_CYCLES=2048 -DBFS_LINK_BITS=18 -DVERBOSE

$(splean31x8_k12): splean31x8_k12_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=12 -DNBATCHES=4 -DMAX_CYCLES=1024 -DBFS_LINK_BITS=17 -DVERBOSE

$(splean31x8_k13): splean31x8_k13_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=13 -DNBATCHES=4 -DMAX_CYCLES=512 -DBFS_LINK_BITS=16 -DVERBOSE

$(splean31x8_k14): splean31x8_k14_t%:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=$* -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=256 -DBFS_LINK_BITS=15 -DVERBOSE

# make all

all: baseline verify splean
	
# run evaluations

seed0 := 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
seed1 := 000005EED100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
seed2 := 00000000005EED2000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

runs := 4

# run baseline evaluation

eval_bl_27 := $(addprefix eval_bl_27_t,$(nthreads))
eval_bl_29 := $(addprefix eval_bl_29_t,$(nthreads))
eval_bl_31 := $(addprefix eval_bl_31_t,$(nthreads))

eval_bl: $(eval_bl_27) $(eval_bl_29) $(eval_bl_31)

$(eval_bl_27): eval_bl_27_t%:
	./lean27x8 -t $* -r $(runs) -x $(seed0) > data/lean27x8_t$*_seed0.out
	./lean27x8 -t $* -r $(runs) -x $(seed1) > data/lean27x8_t$*_seed1.out
	./lean27x8 -t $* -r $(runs) -x $(seed2) > data/lean27x8_t$*_seed2.out

$(eval_bl_29): eval_bl_29_t%:
	./lean29x8 -t $* -r $(runs) -x $(seed0) > data/lean29x8_t$*_seed0.out
	./lean29x8 -t $* -r $(runs) -x $(seed1) > data/lean29x8_t$*_seed1.out
	./lean29x8 -t $* -r $(runs) -x $(seed2) > data/lean29x8_t$*_seed2.out
	
$(eval_bl_31): eval_bl_31_t%:
	./lean31x8 -t $* -r $(runs) -x $(seed0) > data/lean31x8_t$*_seed0.out
	./lean31x8 -t $* -r $(runs) -x $(seed1) > data/lean31x8_t$*_seed1.out
	./lean31x8 -t $* -r $(runs) -x $(seed2) > data/lean31x8_t$*_seed2.out

# run verification set

run_verify: run_verify_27 run_verify_29 run_verify_31

run_verify_27:
	./mean27cu -r 32000 -h $(seed0) > data/verify_mean27cu_seed0.out
	./mean27cu -r 32000 -h $(seed1) > data/verify_mean27cu_seed1.out
	./mean27cu -r 32000 -h $(seed2) > data/verify_mean27cu_seed2.out

run_verify_29:
	./mean29cu -r 32000 -h $(seed0) > data/verify_mean29cu_seed0.out
	./mean29cu -r 32000 -h $(seed1) > data/verify_mean29cu_seed1.out
	./mean29cu -r 32000 -h $(seed2) > data/verify_mean29cu_seed2.out

run_verify_31:
	./mean31cu -r 32000 -h $(seed0) > data/verify_mean31cu_seed0.out
	./mean31cu -r 32000 -h $(seed1) > data/verify_mean31cu_seed1.out
	./mean31cu -r 32000 -h $(seed2) > data/verify_mean31cu_seed2.out

# run evaluation for splean27x8

eval_splean27x8_k10 := $(addprefix eval_splean27x8_k10_t,$(nthreads))
eval_splean27x8_k11 := $(addprefix eval_splean27x8_k11_t,$(nthreads))
eval_splean27x8_k12 := $(addprefix eval_splean27x8_k12_t,$(nthreads))
eval_splean27x8_k13 := $(addprefix eval_splean27x8_k13_t,$(nthreads))
eval_splean27x8_k14 := $(addprefix eval_splean27x8_k14_t,$(nthreads))

eval_splean27x8: $(eval_splean27x8_k10) $(eval_splean27x8_k11) $(eval_splean27x8_k12) $(eval_splean27x8_k13) $(eval_splean27x8_k14)

$(eval_splean27x8_k10): eval_splean27x8_k10_t%:
	./splean27x8_k10_t$* -r $(runs) -x $(seed0) > data/splean27x8_k10_t$*_seed0.out
	./splean27x8_k10_t$* -r $(runs) -x $(seed1) > data/splean27x8_k10_t$*_seed1.out
	./splean27x8_k10_t$* -r $(runs) -x $(seed2) > data/splean27x8_k10_t$*_seed2.out

$(eval_splean27x8_k11): eval_splean27x8_k11_t%:
	./splean27x8_k11_t$* -r $(runs) -x $(seed0) > data/splean27x8_k11_t$*_seed0.out
	./splean27x8_k11_t$* -r $(runs) -x $(seed1) > data/splean27x8_k11_t$*_seed1.out
	./splean27x8_k11_t$* -r $(runs) -x $(seed2) > data/splean27x8_k11_t$*_seed2.out

$(eval_splean27x8_k12): eval_splean27x8_k12_t%:
	./splean27x8_k12_t$* -r $(runs) -x $(seed0) > data/splean27x8_k12_t$*_seed0.out
	./splean27x8_k12_t$* -r $(runs) -x $(seed1) > data/splean27x8_k12_t$*_seed1.out
	./splean27x8_k12_t$* -r $(runs) -x $(seed2) > data/splean27x8_k12_t$*_seed2.out

$(eval_splean27x8_k13): eval_splean27x8_k13_t%:
	./splean27x8_k13_t$* -r $(runs) -x $(seed0) > data/splean27x8_k13_t$*_seed0.out
	./splean27x8_k13_t$* -r $(runs) -x $(seed1) > data/splean27x8_k13_t$*_seed1.out
	./splean27x8_k13_t$* -r $(runs) -x $(seed2) > data/splean27x8_k13_t$*_seed2.out

$(eval_splean27x8_k14): eval_splean27x8_k14_t%:
	./splean27x8_k14_t$* -r $(runs) -x $(seed0) > data/splean27x8_k14_t$*_seed0.out
	./splean27x8_k14_t$* -r $(runs) -x $(seed1) > data/splean27x8_k14_t$*_seed1.out
	./splean27x8_k14_t$* -r $(runs) -x $(seed2) > data/splean27x8_k14_t$*_seed2.out

# run evaluation for splean29x8

eval_splean29x8_k10 := $(addprefix eval_splean29x8_k10_t,$(nthreads))
eval_splean29x8_k11 := $(addprefix eval_splean29x8_k11_t,$(nthreads))
eval_splean29x8_k12 := $(addprefix eval_splean29x8_k12_t,$(nthreads))
eval_splean29x8_k13 := $(addprefix eval_splean29x8_k13_t,$(nthreads))
eval_splean29x8_k14 := $(addprefix eval_splean29x8_k14_t,$(nthreads))

eval_splean29x8: $(eval_splean29x8_k10) $(eval_splean29x8_k11) $(eval_splean29x8_k12) $(eval_splean29x8_k13) $(eval_splean29x8_k14)

$(eval_splean29x8_k10): eval_splean29x8_k10_t%:
	./splean29x8_k10_t$* -r $(runs) -x $(seed0) > data/splean29x8_k10_t$*_seed0.out
	./splean29x8_k10_t$* -r $(runs) -x $(seed1) > data/splean29x8_k10_t$*_seed1.out
	./splean29x8_k10_t$* -r $(runs) -x $(seed2) > data/splean29x8_k10_t$*_seed2.out

$(eval_splean29x8_k11): eval_splean29x8_k11_t%:
	./splean29x8_k11_t$* -r $(runs) -x $(seed0) > data/splean29x8_k11_t$*_seed0.out
	./splean29x8_k11_t$* -r $(runs) -x $(seed1) > data/splean29x8_k11_t$*_seed1.out
	./splean29x8_k11_t$* -r $(runs) -x $(seed2) > data/splean29x8_k11_t$*_seed2.out

$(eval_splean29x8_k12): eval_splean29x8_k12_t%:
	./splean29x8_k12_t$* -r $(runs) -x $(seed0) > data/splean29x8_k12_t$*_seed0.out
	./splean29x8_k12_t$* -r $(runs) -x $(seed1) > data/splean29x8_k12_t$*_seed1.out
	./splean29x8_k12_t$* -r $(runs) -x $(seed2) > data/splean29x8_k12_t$*_seed2.out

$(eval_splean29x8_k13): eval_splean29x8_k13_t%:
	./splean29x8_k13_t$* -r $(runs) -x $(seed0) > data/splean29x8_k13_t$*_seed0.out
	./splean29x8_k13_t$* -r $(runs) -x $(seed1) > data/splean29x8_k13_t$*_seed1.out
	./splean29x8_k13_t$* -r $(runs) -x $(seed2) > data/splean29x8_k13_t$*_seed2.out

$(eval_splean29x8_k14): eval_splean29x8_k14_t%:
	./splean29x8_k14_t$* -r $(runs) -x $(seed0) > data/splean29x8_k14_t$*_seed0.out
	./splean29x8_k14_t$* -r $(runs) -x $(seed1) > data/splean29x8_k14_t$*_seed1.out
	./splean29x8_k14_t$* -r $(runs) -x $(seed2) > data/splean29x8_k14_t$*_seed2.out
	
# run evaluation for splean31x8

eval_splean31x8_k10 := $(addprefix eval_splean31x8_k10_t,$(nthreads))
eval_splean31x8_k11 := $(addprefix eval_splean31x8_k11_t,$(nthreads))
eval_splean31x8_k12 := $(addprefix eval_splean31x8_k12_t,$(nthreads))
eval_splean31x8_k13 := $(addprefix eval_splean31x8_k13_t,$(nthreads))
eval_splean31x8_k14 := $(addprefix eval_splean31x8_k14_t,$(nthreads))

#eval_splean31x8: $(eval_splean31x8_k10) $(eval_splean31x8_k11) $(eval_splean31x8_k12) $(eval_splean31x8_k13) $(eval_splean31x8_k14)
eval_splean31x8: $(eval_splean31x8_k11) $(eval_splean31x8_k10)

$(eval_splean31x8_k10): eval_splean31x8_k10_t%:
	./splean31x8_k10_t$* -r $(runs) -x $(seed0) > data/splean31x8_k10_t$*_seed0.out
	./splean31x8_k10_t$* -r $(runs) -x $(seed1) > data/splean31x8_k10_t$*_seed1.out
	./splean31x8_k10_t$* -r $(runs) -x $(seed2) > data/splean31x8_k10_t$*_seed2.out

$(eval_splean31x8_k11): eval_splean31x8_k11_t%:
	./splean31x8_k11_t$* -r $(runs) -x $(seed0) > data/splean31x8_k11_t$*_seed0.out
	./splean31x8_k11_t$* -r $(runs) -x $(seed1) > data/splean31x8_k11_t$*_seed1.out
	./splean31x8_k11_t$* -r $(runs) -x $(seed2) > data/splean31x8_k11_t$*_seed2.out

$(eval_splean31x8_k12): eval_splean31x8_k12_t%:
	./splean31x8_k12_t$* -r $(runs) -x $(seed0) > data/splean31x8_k12_t$*_seed0.out
	./splean31x8_k12_t$* -r $(runs) -x $(seed1) > data/splean31x8_k12_t$*_seed1.out
	./splean31x8_k12_t$* -r $(runs) -x $(seed2) > data/splean31x8_k12_t$*_seed2.out

$(eval_splean31x8_k13): eval_splean31x8_k13_t%:
	./splean31x8_k13_t$* -r $(runs) -x $(seed0) > data/splean31x8_k13_t$*_seed0.out
	./splean31x8_k13_t$* -r $(runs) -x $(seed1) > data/splean31x8_k13_t$*_seed1.out
	./splean31x8_k13_t$* -r $(runs) -x $(seed2) > data/splean31x8_k13_t$*_seed2.out

$(eval_splean31x8_k14): eval_splean31x8_k14_t%:
	./splean31x8_k14_t$* -r $(runs) -x $(seed0) > data/splean31x8_k14_t$*_seed0.out
	./splean31x8_k14_t$* -r $(runs) -x $(seed1) > data/splean31x8_k14_t$*_seed1.out
	./splean31x8_k14_t$* -r $(runs) -x $(seed2) > data/splean31x8_k14_t$*_seed2.out

# run splean miner evaluation

eval: eval_splean27x8 eval_splean29x8 eval_splean31x8

# run all evaluation

eval_all: eval eval_bl

# quick test

quick_test_bl:	threads/barrier.hpp crypto/siphash.hpp cuckatoo/cuckatoo.h  cuckatoo/bitmap.hpp cuckatoo/compress.hpp cuckatoo/graph.hpp cuckatoo/lean.hpp cuckatoo/lean.cpp Makefile
	$(GPP) -o quick_lean27x8 -mavx2 -DNSIPHASH=8 -DATOMIC -DEDGEBITS=27 cuckatoo/lean.cpp $(BLAKE_2B_SRC)
	$(GPP) -o quick_lean29x8 -mavx2 -DNSIPHASH=8 -DATOMIC -DEDGEBITS=29 cuckatoo/lean.cpp $(BLAKE_2B_SRC)
	$(GPP) -o quick_lean31x8 -mavx2 -DNSIPHASH=8 -DATOMIC -DEDGEBITS=31 cuckatoo/lean.cpp $(BLAKE_2B_SRC)

quick_test_splean:	../src/splean.cpp ../src/splean.hpp ../src/hashmap.hpp ../src/bloom.hpp ../src/rice.hpp ../src/helper.hpp ../src/siphash.hpp Makefile
	$(GPP) -o quick_splean27x8_k14_t8 -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=8 -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=16 -DBFS_LINK_BITS=11
	$(GPP) -o quick_splean27x8_k14_t1 -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=1 -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=16 -DBFS_LINK_BITS=11
	$(GPP) -o quick_splean29x8_k14_t8 -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=8 -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=64 -DBFS_LINK_BITS=13
	$(GPP) -o quick_splean29x8_k14_t1 -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=29 -DNSIPHASH=8 -DNTHREADS=1 -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=64 -DBFS_LINK_BITS=13
	$(GPP) -o quick_splean31x8_k14_t8 -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=8 -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=256 -DBFS_LINK_BITS=15
	$(GPP) -o quick_splean31x8_k14_t1 -mavx2 -fno-strict-aliasing ../src/splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=31 -DNSIPHASH=8 -DNTHREADS=1 -DKFACTLOG=14 -DNBATCHES=4 -DMAX_CYCLES=256 -DBFS_LINK_BITS=15
	
quick_test:	quick_test_bl quick_test_splean

run_quick_test_bl:
	@echo "Lean baseline with 8 threads:"
	./quick_lean27x8 -t 8 -x $(seed2)
	./quick_lean29x8 -t 8 -x $(seed2)
	./quick_lean31x8 -t 8 -x $(seed2)
	@echo "Lean baseline with 1 thread:"
	./quick_lean27x8 -t 1 -x $(seed2)
	./quick_lean29x8 -t 1 -x $(seed2)
	./quick_lean31x8 -t 1 -x $(seed2)
	
run_quick_test_splean:
	@echo "Splean approach with 8 threads:"
	./quick_splean27x8_k14_t8
	./quick_splean29x8_k14_t8
	./quick_splean31x8_k14_t8
	@echo "Splean approach with 1 thread:"
	./quick_splean27x8_k14_t1
	./quick_splean29x8_k14_t1
	./quick_splean31x8_k14_t1
	
run_quick_test:	run_quick_test_bl run_quick_test_splean

cont1:
#	./splean31x8_k12_t4 -r $(runs) -x $(seed0) > data/splean31x8_k12_t4_seed0.out
#	./splean31x8_k12_t4 -r $(runs) -x $(seed1) > data/splean31x8_k12_t4_seed1.out
#	./splean31x8_k12_t4 -r $(runs) -x $(seed2) > data/splean31x8_k12_t4_seed2.out
#	./splean31x8_k12_t2 -r $(runs) -x $(seed0) > data/splean31x8_k12_t2_seed0.out
#	./splean31x8_k12_t2 -r $(runs) -x $(seed1) > data/splean31x8_k12_t2_seed1.out
#	./splean31x8_k12_t2 -r $(runs) -x $(seed2) > data/splean31x8_k12_t2_seed2.out
#	./splean31x8_k12_t1 -r $(runs) -x $(seed0) > data/splean31x8_k12_t1_seed0.out
	./splean31x8_k12_t1 -r $(runs) -x $(seed1) > data/splean31x8_k12_t1_seed1.out
	./splean31x8_k12_t1 -r $(runs) -x $(seed2) > data/splean31x8_k12_t1_seed2.out
	
continue: cont1 eval_splean31x8
