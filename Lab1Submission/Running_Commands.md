# Running Commands:

## Demo

./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 >> log1.txt
(Input Size: n=32)

./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 >> log2.txt
(Input Size: n=16)

CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=2 >> eval1_basic_test.txt
(Input Size: n=64)

CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=1 >> eval2_basic_test.txt
(Input Size: n=64)

## Evaluation

### Plot 1

```bash 
CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=1 >> cores_1.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=2 >> cores_2.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=4 >> cores_4.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=8 >> cores_8.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=16 >> cores_16.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=1 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=16 >> cores_16.txt
```
(Input Size: n=16384)

### Plot 2
```bash 
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=1 --rebuild=1 >> ../results/Evaluation/Plot2Results/4x4.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=8 --threads=8 --cores=1 --rebuild=1 >> ../results/Evaluation/Plot2Results/8x8.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=16 --threads=16 --cores=1 --rebuild=1 >> ../results/Evaluation/Plot2Results/16x16.txt
```
(Input Size: n=16384)
```bash 
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=32 --threads=32 --cores=1 --rebuild=1 >> ../results/Evaluation/Plot2Results/32x32.txt
```
(Input Size: n=16384)

## Profiling
```bash
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=1 --rebuild=1 >> profiling_basic.txt
```
(Input Size: n=32)
```bash
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=4 --threads=4 --cores=1 --rebuild=1 >> profiling_4_4.txt
```
(Input Size: n=16384)
```bash
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=8 --threads=8 --cores=1 --rebuild=1 >> profiling_8_8.txt
```
(Input Size: n=16384)
```bash
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=16 --threads=16 --cores=1 --rebuild=1 >> profiling_16_16.txt
```
(Input Size: n=16384)
```bash
CONFIGS=-DISSUE_WIDTH=4 ./ci/blackbox.sh --driver=rtlsim --app=psum --perf=1 --warps=32 --threads=32 --cores=1 --rebuild=1 >> profiling_32_32.txt
```
(Input Size: n=16384)
