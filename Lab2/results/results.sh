# sgemm8 
# ./ci/blackbox.sh --driver=rtlsim --app=sgemm8  --args="-n128" --warps=4 --threads=4 --cores=1 >> ../results/4x4/rtlsim_1.txt
# ./ci/blackbox.sh --driver=rtlsim --app=sgemm8  --args="-n128" --warps=4 --threads=4 --cores=4 >> ../results/4x4/rtlsim_4.txt
# ./ci/blackbox.sh --driver=simx --app=sgemm8  --args="-n128" --warps=4 --threads=4 --cores=1 >> ../results/4x4/simx_1.txt
# ./ci/blackbox.sh --driver=simx --app=sgemm8  --args="-n128" --warps=4 --threads=4 --cores=4 >> ../results/4x4/simx_4.txt

# ./ci/blackbox.sh --driver=rtlsim --app=sgemm8  --args="-n128" --warps=16 --threads=16 --cores=1 >> ../results/16x16/rtlsim_1.txt
# ./ci/blackbox.sh --driver=rtlsim --app=sgemm8  --args="-n128" --warps=16 --threads=16 --cores=4 >> ../results/16x16/rtlsim_4.txt
# ./ci/blackbox.sh --driver=simx --app=sgemm8  --args="-n128" --warps=16 --threads=16 --cores=1 >> ../results/16x16/simx_1.txt
# ./ci/blackbox.sh --driver=simx --app=sgemm8  --args="-n128" --warps=16 --threads=16 --cores=4 >> ../results/16x16/simx_4.txt

# sgemmx
# ./ci/blackbox.sh --driver=rtlsim --app=sgemmx  --args="-n128" --warps=4 --threads=4 --cores=1 >> ../results/sgemmx/4x4/rtlsim_1.txt
# ./ci/blackbox.sh --driver=rtlsim --app=sgemmx  --args="-n128" --warps=4 --threads=4 --cores=4 >> ../results/sgemmx/4x4/rtlsim_4.txt
# ./ci/blackbox.sh --driver=simx --app=sgemmx  --args="-n128" --warps=4 --threads=4 --cores=1 >> ../results/sgemmx/4x4/simx_1.txt
# ./ci/blackbox.sh --driver=simx --app=sgemmx  --args="-n128" --warps=4 --threads=4 --cores=4 >> ../results/sgemmx/4x4/simx_4.txt

# ./ci/blackbox.sh --driver=rtlsim --app=sgemmx  --args="-n128" --warps=16 --threads=16 --cores=1 >> ../results/sgemmx/16x16/rtlsim_1.txt
# ./ci/blackbox.sh --driver=rtlsim --app=sgemmx  --args="-n128" --warps=16 --threads=16 --cores=4 >> ../results/sgemmx/16x16/rtlsim_4.txt
# ./ci/blackbox.sh --driver=simx --app=sgemmx  --args="-n128" --warps=16 --threads=16 --cores=1 >> ../results/sgemmx/16x16/simx_1.txt
# ./ci/blackbox.sh --driver=simx --app=sgemmx  --args="-n128" --warps=16 --threads=16 --cores=4 >> ../results/sgemmx/16x16/simx_4.txt

# bonus 
./ci/blackbox.sh --driver=rtlsim --app=sgemm8_sm  --args="-n64" --warps=16 --threads=16 >> ../results/bonus/sgemm8_sm.txt
./ci/blackbox.sh --driver=rtlsim --app=sgemm8  --args="-n64" --warps=16 --threads=16 >> ../results/bonus/sgemm8.txt