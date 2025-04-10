This folder contains the [source code of the lean and mean solver from John Tromp for Cuckatoo](https://github.com/tromp/cuckoo/tree/master/src). The source was only slightly modified to have the same header length for the mean solver ("#define HEADERLEN 80" -> "#define HEADERLEN 246" in line 751 of the "mean.cu" file and accordingly in line 265 of the "lean.cu" file).
The [Makefile](Makefile) can be used to create and run the binary files to reproduce the evaluation presented [here](../README.md) for claiming the [bounty offered by John Tromp](https://github.com/tromp/cuckoo?tab=readme-ov-file#linear-time-memory-trade-off-bounty).

### List of all binaries and their parameter used for the evaluation

|Exec. File |Output File |Variant   |Seed|Nonces |Threads |k  |Memory used |Batches |Attempts |$k_{mem}$ |
|-----------|------------|:--------:|:--:|:-----:|:------:|:-:|:----------:|:------:|:-------:|:--------:|
| splean27x8_k10_t1 | ../eval/data/splean27x8_k10_t1_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 1 | 10 | 1.6 MiB | 4 | 6.597 | 9.869 |
| splean27x8_k10_t1 | ../eval/data/splean27x8_k10_t1_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 1 | 10 | 1.6 MiB | 4 | 6.597 | 9.869 |
| splean27x8_k10_t1 | ../eval/data/splean27x8_k10_t1_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 1 | 10 | 1.6 MiB | 4 | 6.597 | 9.869 |
| splean27x8_k10_t2 | ../eval/data/splean27x8_k10_t2_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 2 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t2 | ../eval/data/splean27x8_k10_t2_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 2 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t2 | ../eval/data/splean27x8_k10_t2_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 2 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t4 | ../eval/data/splean27x8_k10_t4_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 4 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t4 | ../eval/data/splean27x8_k10_t4_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 4 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t4 | ../eval/data/splean27x8_k10_t4_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 4 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t8 | ../eval/data/splean27x8_k10_t8_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 8 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t8 | ../eval/data/splean27x8_k10_t8_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 8 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k10_t8 | ../eval/data/splean27x8_k10_t8_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 8 | 10 | 1.6 MiB | 4 | 6.597 | 9.845 |
| splean27x8_k11_t1 | ../eval/data/splean27x8_k11_t1_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 1 | 11 | 842.1 KiB | 4 | 12.685 | 19.456 |
| splean27x8_k11_t1 | ../eval/data/splean27x8_k11_t1_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 1 | 11 | 842.1 KiB | 4 | 12.685 | 19.456 |
| splean27x8_k11_t1 | ../eval/data/splean27x8_k11_t1_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 1 | 11 | 842.1 KiB | 4 | 12.685 | 19.456 |
| splean27x8_k11_t2 | ../eval/data/splean27x8_k11_t2_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 2 | 11 | 844.1 KiB | 4 | 12.685 | 19.409 |
| splean27x8_k11_t2 | ../eval/data/splean27x8_k11_t2_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 2 | 11 | 844.1 KiB | 4 | 12.685 | 19.409 |
| splean27x8_k11_t2 | ../eval/data/splean27x8_k11_t2_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 2 | 11 | 844.1 KiB | 4 | 12.685 | 19.409 |
| splean27x8_k11_t4 | ../eval/data/splean27x8_k11_t4_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 4 | 11 | 844.2 KiB | 4 | 12.685 | 19.409 |
| splean27x8_k11_t4 | ../eval/data/splean27x8_k11_t4_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 4 | 11 | 844.2 KiB | 4 | 12.685 | 19.409 |
| splean27x8_k11_t4 | ../eval/data/splean27x8_k11_t4_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 4 | 11 | 844.2 KiB | 4 | 12.685 | 19.409 |
| splean27x8_k11_t8 | ../eval/data/splean27x8_k11_t8_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 8 | 11 | 844.2 KiB | 4 | 12.685 | 19.407 |
| splean27x8_k11_t8 | ../eval/data/splean27x8_k11_t8_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 8 | 11 | 844.2 KiB | 4 | 12.685 | 19.407 |
| splean27x8_k11_t8 | ../eval/data/splean27x8_k11_t8_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 8 | 11 | 844.2 KiB | 4 | 12.685 | 19.407 |
| splean27x8_k12_t1 | ../eval/data/splean27x8_k12_t1_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 1 | 12 | 427.1 KiB | 4 | 24.872 | 38.360 |
| splean27x8_k12_t1 | ../eval/data/splean27x8_k12_t1_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 1 | 12 | 427.1 KiB | 4 | 24.872 | 38.360 |
| splean27x8_k12_t1 | ../eval/data/splean27x8_k12_t1_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 1 | 12 | 427.1 KiB | 4 | 24.872 | 38.360 |
| splean27x8_k12_t2 | ../eval/data/splean27x8_k12_t2_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 2 | 12 | 428.1 KiB | 4 | 24.872 | 38.269 |
| splean27x8_k12_t2 | ../eval/data/splean27x8_k12_t2_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 2 | 12 | 428.1 KiB | 4 | 24.872 | 38.269 |
| splean27x8_k12_t2 | ../eval/data/splean27x8_k12_t2_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 2 | 12 | 428.1 KiB | 4 | 24.872 | 38.269 |
| splean27x8_k12_t4 | ../eval/data/splean27x8_k12_t4_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 4 | 12 | 428.2 KiB | 4 | 24.872 | 38.266 |
| splean27x8_k12_t4 | ../eval/data/splean27x8_k12_t4_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 4 | 12 | 428.2 KiB | 4 | 24.872 | 38.266 |
| splean27x8_k12_t4 | ../eval/data/splean27x8_k12_t4_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 4 | 12 | 428.2 KiB | 4 | 24.872 | 38.266 |
| splean27x8_k12_t8 | ../eval/data/splean27x8_k12_t8_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 8 | 12 | 428.2 KiB | 4 | 24.872 | 38.261 |
| splean27x8_k12_t8 | ../eval/data/splean27x8_k12_t8_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 8 | 12 | 428.2 KiB | 4 | 24.872 | 38.261 |
| splean27x8_k12_t8 | ../eval/data/splean27x8_k12_t8_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 8 | 12 | 428.2 KiB | 4 | 24.872 | 38.261 |
| splean27x8_k13_t1 | ../eval/data/splean27x8_k13_t1_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 1 | 13 | 216.6 KiB | 4 | 49.252 | 75.638 |
| splean27x8_k13_t1 | ../eval/data/splean27x8_k13_t1_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 1 | 13 | 216.6 KiB | 4 | 49.252 | 75.638 |
| splean27x8_k13_t1 | ../eval/data/splean27x8_k13_t1_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 1 | 13 | 216.6 KiB | 4 | 49.252 | 75.638 |
| splean27x8_k13_t2 | ../eval/data/splean27x8_k13_t2_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 2 | 13 | 217.1 KiB | 4 | 49.252 | 75.459 |
| splean27x8_k13_t2 | ../eval/data/splean27x8_k13_t2_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 2 | 13 | 217.1 KiB | 4 | 49.252 | 75.459 |
| splean27x8_k13_t2 | ../eval/data/splean27x8_k13_t2_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 2 | 13 | 217.1 KiB | 4 | 49.252 | 75.459 |
| splean27x8_k13_t4 | ../eval/data/splean27x8_k13_t4_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 4 | 13 | 217.2 KiB | 4 | 49.252 | 75.448 |
| splean27x8_k13_t4 | ../eval/data/splean27x8_k13_t4_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 4 | 13 | 217.2 KiB | 4 | 49.252 | 75.448 |
| splean27x8_k13_t4 | ../eval/data/splean27x8_k13_t4_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 4 | 13 | 217.2 KiB | 4 | 49.252 | 75.448 |
| splean27x8_k13_t8 | ../eval/data/splean27x8_k13_t8_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 8 | 13 | 217.2 KiB | 4 | 49.252 | 75.426 |
| splean27x8_k13_t8 | ../eval/data/splean27x8_k13_t8_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 8 | 13 | 217.2 KiB | 4 | 49.252 | 75.426 |
| splean27x8_k13_t8 | ../eval/data/splean27x8_k13_t8_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 8 | 13 | 217.2 KiB | 4 | 49.252 | 75.426 |
| splean27x8_k14_t1 | ../eval/data/splean27x8_k14_t1_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 1 | 14 | 109.9 KiB | 4 | 98.013 | 149.136 |
| splean27x8_k14_t1 | ../eval/data/splean27x8_k14_t1_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 1 | 14 | 109.9 KiB | 4 | 98.013 | 149.136 |
| splean27x8_k14_t1 | ../eval/data/splean27x8_k14_t1_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 1 | 14 | 109.9 KiB | 4 | 98.013 | 149.136 |
| splean27x8_k14_t2 | ../eval/data/splean27x8_k14_t2_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 2 | 14 | 110.1 KiB | 4 | 98.013 | 148.776 |
| splean27x8_k14_t2 | ../eval/data/splean27x8_k14_t2_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 2 | 14 | 110.1 KiB | 4 | 98.013 | 148.776 |
| splean27x8_k14_t2 | ../eval/data/splean27x8_k14_t2_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 2 | 14 | 110.1 KiB | 4 | 98.013 | 148.776 |
| splean27x8_k14_t4 | ../eval/data/splean27x8_k14_t4_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 4 | 14 | 110.2 KiB | 4 | 98.013 | 148.734 |
| splean27x8_k14_t4 | ../eval/data/splean27x8_k14_t4_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 4 | 14 | 110.2 KiB | 4 | 98.013 | 148.734 |
| splean27x8_k14_t4 | ../eval/data/splean27x8_k14_t4_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 4 | 14 | 110.2 KiB | 4 | 98.013 | 148.734 |
| splean27x8_k14_t8 | ../eval/data/splean27x8_k14_t8_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 8 | 14 | 110.2 KiB | 4 | 98.013 | 148.650 |
| splean27x8_k14_t8 | ../eval/data/splean27x8_k14_t8_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 8 | 14 | 110.2 KiB | 4 | 98.013 | 148.650 |
| splean27x8_k14_t8 | ../eval/data/splean27x8_k14_t8_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 8 | 14 | 110.2 KiB | 4 | 98.013 | 148.650 |
| splean29x8_k10_t1 | ../eval/data/splean29x8_k10_t1_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 1 | 10 | 6.5 MiB | 4 | 6.597 | 9.870 |
| splean29x8_k10_t1 | ../eval/data/splean29x8_k10_t1_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 1 | 10 | 6.5 MiB | 4 | 6.597 | 9.870 |
| splean29x8_k10_t1 | ../eval/data/splean29x8_k10_t1_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 1 | 10 | 6.5 MiB | 4 | 6.597 | 9.870 |
| splean29x8_k10_t2 | ../eval/data/splean29x8_k10_t2_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 2 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t2 | ../eval/data/splean29x8_k10_t2_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 2 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t2 | ../eval/data/splean29x8_k10_t2_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 2 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t4 | ../eval/data/splean29x8_k10_t4_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 4 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t4 | ../eval/data/splean29x8_k10_t4_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 4 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t4 | ../eval/data/splean29x8_k10_t4_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 4 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t8 | ../eval/data/splean29x8_k10_t8_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 8 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t8 | ../eval/data/splean29x8_k10_t8_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 8 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k10_t8 | ../eval/data/splean29x8_k10_t8_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 8 | 10 | 6.5 MiB | 4 | 6.597 | 9.846 |
| splean29x8_k11_t1 | ../eval/data/splean29x8_k11_t1_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 1 | 11 | 3.3 MiB | 4 | 12.685 | 19.458 |
| splean29x8_k11_t1 | ../eval/data/splean29x8_k11_t1_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 1 | 11 | 3.3 MiB | 4 | 12.685 | 19.458 |
| splean29x8_k11_t1 | ../eval/data/splean29x8_k11_t1_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 1 | 11 | 3.3 MiB | 4 | 12.685 | 19.458 |
| splean29x8_k11_t2 | ../eval/data/splean29x8_k11_t2_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 2 | 11 | 3.3 MiB | 4 | 12.685 | 19.412 |
| splean29x8_k11_t2 | ../eval/data/splean29x8_k11_t2_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 2 | 11 | 3.3 MiB | 4 | 12.685 | 19.412 |
| splean29x8_k11_t2 | ../eval/data/splean29x8_k11_t2_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 2 | 11 | 3.3 MiB | 4 | 12.685 | 19.412 |
| splean29x8_k11_t4 | ../eval/data/splean29x8_k11_t4_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 4 | 11 | 3.3 MiB | 4 | 12.685 | 19.411 |
| splean29x8_k11_t4 | ../eval/data/splean29x8_k11_t4_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 4 | 11 | 3.3 MiB | 4 | 12.685 | 19.411 |
| splean29x8_k11_t4 | ../eval/data/splean29x8_k11_t4_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 4 | 11 | 3.3 MiB | 4 | 12.685 | 19.411 |
| splean29x8_k11_t8 | ../eval/data/splean29x8_k11_t8_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 8 | 11 | 3.3 MiB | 4 | 12.685 | 19.411 |
| splean29x8_k11_t8 | ../eval/data/splean29x8_k11_t8_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 8 | 11 | 3.3 MiB | 4 | 12.685 | 19.411 |
| splean29x8_k11_t8 | ../eval/data/splean29x8_k11_t8_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 8 | 11 | 3.3 MiB | 4 | 12.685 | 19.411 |
| splean29x8_k12_t1 | ../eval/data/splean29x8_k12_t1_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 1 | 12 | 1.7 MiB | 4 | 24.872 | 38.368 |
| splean29x8_k12_t1 | ../eval/data/splean29x8_k12_t1_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 1 | 12 | 1.7 MiB | 4 | 24.872 | 38.368 |
| splean29x8_k12_t1 | ../eval/data/splean29x8_k12_t1_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 1 | 12 | 1.7 MiB | 4 | 24.872 | 38.368 |
| splean29x8_k12_t2 | ../eval/data/splean29x8_k12_t2_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 2 | 12 | 1.7 MiB | 4 | 24.872 | 38.278 |
| splean29x8_k12_t2 | ../eval/data/splean29x8_k12_t2_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 2 | 12 | 1.7 MiB | 4 | 24.872 | 38.278 |
| splean29x8_k12_t2 | ../eval/data/splean29x8_k12_t2_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 2 | 12 | 1.7 MiB | 4 | 24.872 | 38.278 |
| splean29x8_k12_t4 | ../eval/data/splean29x8_k12_t4_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 4 | 12 | 1.7 MiB | 4 | 24.872 | 38.277 |
| splean29x8_k12_t4 | ../eval/data/splean29x8_k12_t4_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 4 | 12 | 1.7 MiB | 4 | 24.872 | 38.277 |
| splean29x8_k12_t4 | ../eval/data/splean29x8_k12_t4_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 4 | 12 | 1.7 MiB | 4 | 24.872 | 38.277 |
| splean29x8_k12_t8 | ../eval/data/splean29x8_k12_t8_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 8 | 12 | 1.7 MiB | 4 | 24.872 | 38.275 |
| splean29x8_k12_t8 | ../eval/data/splean29x8_k12_t8_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 8 | 12 | 1.7 MiB | 4 | 24.872 | 38.275 |
| splean29x8_k12_t8 | ../eval/data/splean29x8_k12_t8_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 8 | 12 | 1.7 MiB | 4 | 24.872 | 38.275 |
| splean29x8_k13_t1 | ../eval/data/splean29x8_k13_t1_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 1 | 13 | 866.1 KiB | 4 | 49.252 | 75.667 |
| splean29x8_k13_t1 | ../eval/data/splean29x8_k13_t1_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 1 | 13 | 866.1 KiB | 4 | 49.252 | 75.667 |
| splean29x8_k13_t1 | ../eval/data/splean29x8_k13_t1_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 1 | 13 | 866.1 KiB | 4 | 49.252 | 75.667 |
| splean29x8_k13_t2 | ../eval/data/splean29x8_k13_t2_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 2 | 13 | 868.1 KiB | 4 | 49.252 | 75.491 |
| splean29x8_k13_t2 | ../eval/data/splean29x8_k13_t2_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 2 | 13 | 868.1 KiB | 4 | 49.252 | 75.491 |
| splean29x8_k13_t2 | ../eval/data/splean29x8_k13_t2_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 2 | 13 | 868.1 KiB | 4 | 49.252 | 75.491 |
| splean29x8_k13_t4 | ../eval/data/splean29x8_k13_t4_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 4 | 13 | 868.2 KiB | 4 | 49.252 | 75.489 |
| splean29x8_k13_t4 | ../eval/data/splean29x8_k13_t4_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 4 | 13 | 868.2 KiB | 4 | 49.252 | 75.489 |
| splean29x8_k13_t4 | ../eval/data/splean29x8_k13_t4_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 4 | 13 | 868.2 KiB | 4 | 49.252 | 75.489 |
| splean29x8_k13_t8 | ../eval/data/splean29x8_k13_t8_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 8 | 13 | 868.2 KiB | 4 | 49.252 | 75.483 |
| splean29x8_k13_t8 | ../eval/data/splean29x8_k13_t8_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 8 | 13 | 868.2 KiB | 4 | 49.252 | 75.483 |
| splean29x8_k13_t8 | ../eval/data/splean29x8_k13_t8_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 8 | 13 | 868.2 KiB | 4 | 49.252 | 75.483 |
| splean29x8_k14_t1 | ../eval/data/splean29x8_k14_t1_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 1 | 14 | 439.1 KiB | 4 | 98.013 | 149.248 |
| splean29x8_k14_t1 | ../eval/data/splean29x8_k14_t1_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 1 | 14 | 439.1 KiB | 4 | 98.013 | 149.248 |
| splean29x8_k14_t1 | ../eval/data/splean29x8_k14_t1_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 1 | 14 | 439.1 KiB | 4 | 98.013 | 149.248 |
| splean29x8_k14_t2 | ../eval/data/splean29x8_k14_t2_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 2 | 14 | 440.1 KiB | 4 | 98.013 | 148.903 |
| splean29x8_k14_t2 | ../eval/data/splean29x8_k14_t2_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 2 | 14 | 440.1 KiB | 4 | 98.013 | 148.903 |
| splean29x8_k14_t2 | ../eval/data/splean29x8_k14_t2_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 2 | 14 | 440.1 KiB | 4 | 98.013 | 148.903 |
| splean29x8_k14_t4 | ../eval/data/splean29x8_k14_t4_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 4 | 14 | 440.2 KiB | 4 | 98.013 | 148.893 |
| splean29x8_k14_t4 | ../eval/data/splean29x8_k14_t4_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 4 | 14 | 440.2 KiB | 4 | 98.013 | 148.893 |
| splean29x8_k14_t4 | ../eval/data/splean29x8_k14_t4_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 4 | 14 | 440.2 KiB | 4 | 98.013 | 148.893 |
| splean29x8_k14_t8 | ../eval/data/splean29x8_k14_t8_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 8 | 14 | 440.2 KiB | 4 | 98.013 | 148.871 |
| splean29x8_k14_t8 | ../eval/data/splean29x8_k14_t8_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 8 | 14 | 440.2 KiB | 4 | 98.013 | 148.871 |
| splean29x8_k14_t8 | ../eval/data/splean29x8_k14_t8_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 8 | 14 | 440.2 KiB | 4 | 98.013 | 148.871 |
| splean31x8_k11_t1 | ../eval/data/splean31x8_k11_t1_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 1 | 11 | 13.2 MiB | 4 | 12.685 | 19.458 |
| splean31x8_k11_t1 | ../eval/data/splean31x8_k11_t1_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 1 | 11 | 13.2 MiB | 4 | 12.685 | 19.458 |
| splean31x8_k11_t1 | ../eval/data/splean31x8_k11_t1_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 1 | 11 | 13.2 MiB | 4 | 12.685 | 19.458 |
| splean31x8_k11_t2 | ../eval/data/splean31x8_k11_t2_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 2 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t2 | ../eval/data/splean31x8_k11_t2_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 2 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t2 | ../eval/data/splean31x8_k11_t2_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 2 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t4 | ../eval/data/splean31x8_k11_t4_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 4 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t4 | ../eval/data/splean31x8_k11_t4_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 4 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t4 | ../eval/data/splean31x8_k11_t4_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 4 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t8 | ../eval/data/splean31x8_k11_t8_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 8 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t8 | ../eval/data/splean31x8_k11_t8_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 8 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k11_t8 | ../eval/data/splean31x8_k11_t8_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 8 | 11 | 13.2 MiB | 4 | 12.685 | 19.412 |
| splean31x8_k12_t1 | ../eval/data/splean31x8_k12_t1_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 1 | 12 | 6.7 MiB | 4 | 24.872 | 38.369 |
| splean31x8_k12_t1 | ../eval/data/splean31x8_k12_t1_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 1 | 12 | 6.7 MiB | 4 | 24.872 | 38.369 |
| splean31x8_k12_t1 | ../eval/data/splean31x8_k12_t1_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 1 | 12 | 6.7 MiB | 4 | 24.872 | 38.369 |
| splean31x8_k12_t2 | ../eval/data/splean31x8_k12_t2_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 2 | 12 | 6.7 MiB | 4 | 24.872 | 38.280 |
| splean31x8_k12_t2 | ../eval/data/splean31x8_k12_t2_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 2 | 12 | 6.7 MiB | 4 | 24.872 | 38.280 |
| splean31x8_k12_t2 | ../eval/data/splean31x8_k12_t2_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 2 | 12 | 6.7 MiB | 4 | 24.872 | 38.280 |
| splean31x8_k12_t4 | ../eval/data/splean31x8_k12_t4_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 4 | 12 | 6.7 MiB | 4 | 24.872 | 38.280 |
| splean31x8_k12_t4 | ../eval/data/splean31x8_k12_t4_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 4 | 12 | 6.7 MiB | 4 | 24.872 | 38.280 |
| splean31x8_k12_t4 | ../eval/data/splean31x8_k12_t4_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 4 | 12 | 6.7 MiB | 4 | 24.872 | 38.280 |
| splean31x8_k12_t8 | ../eval/data/splean31x8_k12_t8_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 8 | 12 | 6.7 MiB | 4 | 24.872 | 38.279 |
| splean31x8_k12_t8 | ../eval/data/splean31x8_k12_t8_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 8 | 12 | 6.7 MiB | 4 | 24.872 | 38.279 |
| splean31x8_k12_t8 | ../eval/data/splean31x8_k12_t8_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 8 | 12 | 6.7 MiB | 4 | 24.872 | 38.279 |
| splean31x8_k13_t1 | ../eval/data/splean31x8_k13_t1_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 1 | 13 | 3.4 MiB | 4 | 49.252 | 75.674 |
| splean31x8_k13_t1 | ../eval/data/splean31x8_k13_t1_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 1 | 13 | 3.4 MiB | 4 | 49.252 | 75.674 |
| splean31x8_k13_t1 | ../eval/data/splean31x8_k13_t1_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 1 | 13 | 3.4 MiB | 4 | 49.252 | 75.674 |
| splean31x8_k13_t2 | ../eval/data/splean31x8_k13_t2_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 2 | 13 | 3.4 MiB | 4 | 49.252 | 75.500 |
| splean31x8_k13_t2 | ../eval/data/splean31x8_k13_t2_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 2 | 13 | 3.4 MiB | 4 | 49.252 | 75.500 |
| splean31x8_k13_t2 | ../eval/data/splean31x8_k13_t2_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 2 | 13 | 3.4 MiB | 4 | 49.252 | 75.500 |
| splean31x8_k13_t4 | ../eval/data/splean31x8_k13_t4_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 4 | 13 | 3.4 MiB | 4 | 49.252 | 75.499 |
| splean31x8_k13_t4 | ../eval/data/splean31x8_k13_t4_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 4 | 13 | 3.4 MiB | 4 | 49.252 | 75.499 |
| splean31x8_k13_t4 | ../eval/data/splean31x8_k13_t4_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 4 | 13 | 3.4 MiB | 4 | 49.252 | 75.499 |
| splean31x8_k13_t8 | ../eval/data/splean31x8_k13_t8_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 8 | 13 | 3.4 MiB | 4 | 49.252 | 75.498 |
| splean31x8_k13_t8 | ../eval/data/splean31x8_k13_t8_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 8 | 13 | 3.4 MiB | 4 | 49.252 | 75.498 |
| splean31x8_k13_t8 | ../eval/data/splean31x8_k13_t8_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 8 | 13 | 3.4 MiB | 4 | 49.252 | 75.498 |
| splean31x8_k14_t1 | ../eval/data/splean31x8_k14_t1_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 1 | 14 | 1.7 MiB | 4 | 98.013 | 149.275 |
| splean31x8_k14_t1 | ../eval/data/splean31x8_k14_t1_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 1 | 14 | 1.7 MiB | 4 | 98.013 | 149.275 |
| splean31x8_k14_t1 | ../eval/data/splean31x8_k14_t1_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 1 | 14 | 1.7 MiB | 4 | 98.013 | 149.275 |
| splean31x8_k14_t2 | ../eval/data/splean31x8_k14_t2_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 2 | 14 | 1.7 MiB | 4 | 98.013 | 148.935 |
| splean31x8_k14_t2 | ../eval/data/splean31x8_k14_t2_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 2 | 14 | 1.7 MiB | 4 | 98.013 | 148.935 |
| splean31x8_k14_t2 | ../eval/data/splean31x8_k14_t2_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 2 | 14 | 1.7 MiB | 4 | 98.013 | 148.935 |
| splean31x8_k14_t4 | ../eval/data/splean31x8_k14_t4_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 4 | 14 | 1.7 MiB | 4 | 98.013 | 148.932 |
| splean31x8_k14_t4 | ../eval/data/splean31x8_k14_t4_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 4 | 14 | 1.7 MiB | 4 | 98.013 | 148.932 |
| splean31x8_k14_t4 | ../eval/data/splean31x8_k14_t4_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 4 | 14 | 1.7 MiB | 4 | 98.013 | 148.932 |
| splean31x8_k14_t8 | ../eval/data/splean31x8_k14_t8_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 8 | 14 | 1.7 MiB | 4 | 98.013 | 148.927 |
| splean31x8_k14_t8 | ../eval/data/splean31x8_k14_t8_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 8 | 14 | 1.7 MiB | 4 | 98.013 | 148.927 |
| splean31x8_k14_t8 | ../eval/data/splean31x8_k14_t8_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 8 | 14 | 1.7 MiB | 4 | 98.013 | 148.927 |
| splean31x8_k15_t1 | ../eval/data/splean31x8_k15_t1_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 1 | 15 | 938.1 KiB | 4 | 195.536 | 279.439 |
| splean31x8_k15_t1 | ../eval/data/splean31x8_k15_t1_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 1 | 15 | 938.1 KiB | 4 | 195.536 | 279.439 |
| splean31x8_k15_t1 | ../eval/data/splean31x8_k15_t1_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 1 | 15 | 938.1 KiB | 4 | 195.536 | 279.439 |
| splean31x8_k15_t2 | ../eval/data/splean31x8_k15_t2_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 2 | 15 | 940.3 KiB | 4 | 195.536 | 278.784 |
| splean31x8_k15_t2 | ../eval/data/splean31x8_k15_t2_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 2 | 15 | 940.3 KiB | 4 | 195.536 | 278.784 |
| splean31x8_k15_t2 | ../eval/data/splean31x8_k15_t2_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 2 | 15 | 940.3 KiB | 4 | 195.536 | 278.784 |
| splean31x8_k15_t4 | ../eval/data/splean31x8_k15_t4_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 4 | 15 | 940.3 KiB | 4 | 195.536 | 278.775 |
| splean31x8_k15_t4 | ../eval/data/splean31x8_k15_t4_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 4 | 15 | 940.3 KiB | 4 | 195.536 | 278.775 |
| splean31x8_k15_t4 | ../eval/data/splean31x8_k15_t4_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 4 | 15 | 940.3 KiB | 4 | 195.536 | 278.775 |
| splean31x8_k15_t8 | ../eval/data/splean31x8_k15_t8_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 8 | 15 | 940.4 KiB | 4 | 195.536 | 278.756 |
| splean31x8_k15_t8 | ../eval/data/splean31x8_k15_t8_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 8 | 15 | 940.4 KiB | 4 | 195.536 | 278.756 |
| splean31x8_k15_t8 | ../eval/data/splean31x8_k15_t8_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 8 | 15 | 940.4 KiB | 4 | 195.536 | 278.756 |


Lean solver baseline calculations:

|Exec. File |Output File |Variant   |Seed |Nonces |Threads |Memory used |$k_{mem}$ |
|-----------|------------|:--------:|:---:|:-----:|:------:|:----------:|:--------:|
| lean27x8_t1 | ../eval/data/lean27x8_t1_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 1 | 32.0 MiB | 0.250 |
| lean27x8_t1 | ../eval/data/lean27x8_t1_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 1 | 32.0 MiB | 0.250 |
| lean27x8_t1 | ../eval/data/lean27x8_t1_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 1 | 32.0 MiB | 0.250 |
| lean27x8_t2 | ../eval/data/lean27x8_t2_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 2 | 32.0 MiB | 0.250 |
| lean27x8_t2 | ../eval/data/lean27x8_t2_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 2 | 32.0 MiB | 0.250 |
| lean27x8_t2 | ../eval/data/lean27x8_t2_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 2 | 32.0 MiB | 0.250 |
| lean27x8_t4 | ../eval/data/lean27x8_t4_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 4 | 32.0 MiB | 0.250 |
| lean27x8_t4 | ../eval/data/lean27x8_t4_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 4 | 32.0 MiB | 0.250 |
| lean27x8_t4 | ../eval/data/lean27x8_t4_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 4 | 32.0 MiB | 0.250 |
| lean27x8_t8 | ../eval/data/lean27x8_t8_seed0.out | Cuckatoo27 | 00000000000000000000...0000 | 0-3 | 8 | 32.0 MiB | 0.250 |
| lean27x8_t8 | ../eval/data/lean27x8_t8_seed1.out | Cuckatoo27 | 000005eed10000000000...0000 | 0-3 | 8 | 32.0 MiB | 0.250 |
| lean27x8_t8 | ../eval/data/lean27x8_t8_seed2.out | Cuckatoo27 | 00000000005eed200000...0000 | 0-3 | 8 | 32.0 MiB | 0.250 |
| lean29x8_t1 | ../eval/data/lean29x8_t1_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 1 | 128.0 MiB | 0.250 |
| lean29x8_t1 | ../eval/data/lean29x8_t1_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 1 | 128.0 MiB | 0.250 |
| lean29x8_t1 | ../eval/data/lean29x8_t1_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 1 | 128.0 MiB | 0.250 |
| lean29x8_t2 | ../eval/data/lean29x8_t2_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 2 | 128.0 MiB | 0.250 |
| lean29x8_t2 | ../eval/data/lean29x8_t2_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 2 | 128.0 MiB | 0.250 |
| lean29x8_t2 | ../eval/data/lean29x8_t2_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 2 | 128.0 MiB | 0.250 |
| lean29x8_t4 | ../eval/data/lean29x8_t4_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 4 | 128.0 MiB | 0.250 |
| lean29x8_t4 | ../eval/data/lean29x8_t4_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 4 | 128.0 MiB | 0.250 |
| lean29x8_t4 | ../eval/data/lean29x8_t4_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 4 | 128.0 MiB | 0.250 |
| lean29x8_t8 | ../eval/data/lean29x8_t8_seed0.out | Cuckatoo29 | 00000000000000000000...0000 | 0-3 | 8 | 128.0 MiB | 0.250 |
| lean29x8_t8 | ../eval/data/lean29x8_t8_seed1.out | Cuckatoo29 | 000005eed10000000000...0000 | 0-3 | 8 | 128.0 MiB | 0.250 |
| lean29x8_t8 | ../eval/data/lean29x8_t8_seed2.out | Cuckatoo29 | 00000000005eed200000...0000 | 0-3 | 8 | 128.0 MiB | 0.250 |
| lean31x8_t1 | ../eval/data/lean31x8_t1_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 1 | 512.0 MiB | 0.250 |
| lean31x8_t1 | ../eval/data/lean31x8_t1_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 1 | 512.0 MiB | 0.250 |
| lean31x8_t1 | ../eval/data/lean31x8_t1_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 1 | 512.0 MiB | 0.250 |
| lean31x8_t2 | ../eval/data/lean31x8_t2_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 2 | 512.0 MiB | 0.250 |
| lean31x8_t2 | ../eval/data/lean31x8_t2_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 2 | 512.0 MiB | 0.250 |
| lean31x8_t2 | ../eval/data/lean31x8_t2_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 2 | 512.0 MiB | 0.250 |
| lean31x8_t4 | ../eval/data/lean31x8_t4_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 4 | 512.0 MiB | 0.250 |
| lean31x8_t4 | ../eval/data/lean31x8_t4_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 4 | 512.0 MiB | 0.250 |
| lean31x8_t4 | ../eval/data/lean31x8_t4_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 4 | 512.0 MiB | 0.250 |
| lean31x8_t8 | ../eval/data/lean31x8_t8_seed0.out | Cuckatoo31 | 00000000000000000000...0000 | 0-3 | 8 | 512.0 MiB | 0.250 |
| lean31x8_t8 | ../eval/data/lean31x8_t8_seed1.out | Cuckatoo31 | 000005eed10000000000...0000 | 0-3 | 8 | 512.0 MiB | 0.250 |
| lean31x8_t8 | ../eval/data/lean31x8_t8_seed2.out | Cuckatoo31 | 00000000005eed200000...0000 | 0-3 | 8 | 512.0 MiB | 0.250 |

### How to create and run all binary files to reproduce the full evaluation

The command

~~~
make all
~~~

can be used to compile and link all binary files with the parameter used for the evaluation. Followed by the command

~~~
make eval_all
~~~

to run these binaries and gather the output in the [data directory](data/). [Jupyter Notebook](https://jupyter.org/) files in the [image directory](../img/) can be run to create the overview graphs and images used in the presentation of the evaluation.

### How to create and run a quick test

Use the command

~~~
make quick_test
~~~

to create the files for a quick test and use

~~~
make run_quick_test
~~~

to run the test. The output will not be verbose and results will be piped to the console.

### How to create and run only a part of the evaluation

However, running the full evaluation takes quite some time. Therefore, one can create and run only a part of the evaluation by using the following commands. The format of the created executable files is  

 (type)(bits)x(parallel hashes)_k(reduction factor)  

so for example: splean31x8_k10, will execute splean solver approach for Cuckatoo31 using SIMD with 8 Siphash calculations in parallel and a k-factor of 10 (so using only a fraction $\frac{1}{2^{10}}$ of the edges to start with).

To only create all lean solver baseline binary files use

~~~
make baseline
~~~

To make lean solver baseline binaries for a certain Cuckatoo variant with a specified amount of parallel Siphash calculations, use for example (for lean solver targeting Cuckatoo27 with 4 Siphashes in parallel)

~~~
make lean27x4
~~~

To run the baseline calculations, use

~~~
make eval_bl
~~~

To create all splean solver variants, use

~~~
make splean
~~~

for a particular solver variant, use for example (for splean solver targeting Cuckatoo29 with 8 Siphash calculations in parallel):

~~~
make splean27x8
~~~
  
To run the evaluations for a part of the splean solver variants, use for example (for splean solver targeting Cuckatoo29 with 8 Siphash calculations in parallel):

~~~
make eval_splean27x8
~~~

To create the cuda variants of mean solver for the validation process, use:

~~~
make verify
~~~

or more specific for example (for mean solver targeting Cuckatoo27 using cuda):

~~~
make mean27cu
~~~

to run the validation to find all nonces that lead to 42-cycles, use

~~~
make run_verify
~~~

or more specific for example (for mean solver targeting Cuckatoo27 using cuda):

~~~
make run_verify_27
~~~
