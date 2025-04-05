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
BLAKE_2B_SRC ?= blake2b-ref.c
NVCC ?= nvcc -std=c++11

splean27x8_k9_t8:	splean.cpp splean.hpp hashmap.hpp bloom.hpp rice.hpp helper.hpp siphash.hpp Makefile
	$(GPP) -o $@ -mavx2 -fno-strict-aliasing splean.cpp $(BLAKE_2B_SRC) -DEDGE_BITS=27 -DNSIPHASH=8 -DNTHREADS=8 -DKFACTLOG=9 -DNBATCHES=1 -DMAX_CYCLES=1024 -DBFS_LINK_BITS=18
