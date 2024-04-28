#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vortex.h>
#include <vector>
#include "common.h"

#define RT_CHECK(_expr)                                         \
   do {                                                         \
     int _ret = _expr;                                          \
     if (0 == _ret)                                             \
       break;                                                   \
     printf("Error: '%s' returned %d!\n", #_expr, (int)_ret);   \
	 cleanup();			                                              \
     exit(-1);                                                  \
   } while (false)

///////////////////////////////////////////////////////////////////////////////

const char* kernel_file = "kernel.bin"; // load kernel binary to GPU 
uint32_t count = 0;

std::vector<TYPE> src_data;
std::vector<TYPE> ref_data;

vx_device_h device = nullptr;
std::vector<uint8_t> staging_buf;
kernel_arg_t kernel_arg = {};

static void show_usage() {
   std::cout << "Vortex Test." << std::endl;
   std::cout << "Usage: [-k: kernel] [-n words] [-h: help]" << std::endl;
}

static void parse_args(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "n:k:h?")) != -1) {
    switch (c) {
    case 'n':
      count = atoi(optarg);
      break;
    case 'k':
      kernel_file = optarg;
      break;
    case 'h':
    case '?': {
      show_usage();
      exit(0);
    } break;
    default:
      show_usage();
      exit(-1);
    }
  }
}

void cleanup() {
  if (device) {
    vx_mem_free(device, kernel_arg.src_addr);
    vx_mem_free(device, kernel_arg.dst_addr);
    vx_dev_close(device);
  }
}

// arg num_points: number of points in input
void gen_input_data(uint32_t num_points) {
  src_data.resize(num_points);

  std::cout << "Input Data" << std::endl;
  for (uint32_t i = 0; i < num_points; ++i) {
    auto r = static_cast<float>(std::rand()) / RAND_MAX;
    auto value = static_cast<TYPE>(r * num_points);
    src_data[i] = value;
    std::cout << std::dec << i << ": value=" << value << std::endl;
  }  
}

// arg num_points: number of points in destination
void gen_ref_data(uint32_t num_points) {
  ref_data.resize(num_points);

  int points_per_partition = src_data.size()/num_points; // could be a 1:1 ratio, or could be > 1 
  // > 1 means that you have to sum within each partition 

  std::cout << "Reference Data" << std::endl;
  
  // outer loop filling each point of ref data
  for (uint32_t i = 0; i < num_points; ++i) {
    TYPE sum = 0;
    // inner loop going through all points of a partition
    for (uint32_t j = 0; j < points_per_partition; ++j) {

      int index = (i*points_per_partition) + j;
      TYPE curr = src_data.at(index); // value 
      sum = sum + curr; // summing all values within partition 

    }

    ref_data.at(i) = sum;
    std::cout << std::dec << i << ": value=" << sum << std::endl;
  }
}

// arg num_points: number of points in destination
int run_test(const kernel_arg_t& kernel_arg,
             uint32_t buf_size, 
             uint32_t num_points) {
  // start device
  std::cout << "start device" << std::endl;
  RT_CHECK(vx_start(device));

  // wait for completion
  std::cout << "wait for completion" << std::endl;
  RT_CHECK(vx_ready_wait(device, VX_MAX_TIMEOUT));

  // download destination buffer
  std::cout << "download destination buffer" << std::endl;
  RT_CHECK(vx_copy_from_dev(device, staging_buf.data(), kernel_arg.dst_addr, buf_size));

  // verify result
  std::cout << "verify result" << std::endl;  
  {
    int errors = 0;
    auto buf_ptr = (TYPE*)staging_buf.data();
    for (uint32_t i = 0; i < num_points; ++i) {
      TYPE ref = ref_data.at(i);
      TYPE cur = buf_ptr[i];
      if (cur != ref) {
        std::cout << "error at result #" << std::dec << i
                  << std::hex << ": actual=" << cur << ", expected=" << ref << std::endl;
        ++errors;
      }
    }
    if (errors != 0) {
      std::cout << "Found " << std::dec << errors << " errors!" << std::endl;
      std::cout << "FAILED!" << std::endl;
      return 1;  
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {  
  // parse command arguments
  parse_args(argc, argv);

  if (count == 0) {
    count = 24; // just a default input size in case nothing is specified.
  }

  std::srand(50);

  /**
   * @brief calculates the num points and buffer size based on num size 
   * 
   */

  uint32_t num_total_points = count; // total input size
  std::cout << "Total number of points in input: " << num_total_points << std::endl;

  // generate input data
  gen_input_data(num_total_points); // src_data now has these many points with random values initialized

  uint32_t src_buf_size = src_data.size() * sizeof(int32_t);
  std::cout << "Source buffer size: " << src_buf_size << " bytes" << std::endl;


  // open device connection
  std::cout << "open device connection" << std::endl;  
  RT_CHECK(vx_dev_open(&device));

  // calculating good destination size by querying the device
  uint64_t num_cores, num_warps, num_threads;
  RT_CHECK(vx_dev_caps(device, VX_CAPS_NUM_CORES, &num_cores)); // # of cores
  RT_CHECK(vx_dev_caps(device, VX_CAPS_NUM_WARPS, &num_warps)); // # of warps 
  RT_CHECK(vx_dev_caps(device, VX_CAPS_NUM_THREADS, &num_threads)); // # of threads


  std::cout << "Device configuration [cores, warps, threads]: " << num_cores << ", " << num_warps << ", " << num_threads << std::endl;

  uint32_t num_tasks  = num_cores * num_warps * num_threads;
  // num_tasks is the total number of hardware threads
  // So we will use same number of logical threads for efficiency
  // This will be equal to number of points in destination


  std::cout << "Total number of points in output: " << num_tasks << std::endl;
  // Now using src_data and destination size we can calculate the reference data (ground truth)
  gen_ref_data(num_tasks); 

  uint32_t dst_buf_size = ref_data.size() * sizeof(TYPE);
  std::cout << "Destination buffer size: " << dst_buf_size << " bytes" << std::endl;


  // Dividing work between the threads
  uint32_t num_points = num_total_points/num_tasks; // size of partition - each thread will work on this - each destination value will be a sum of this chunk of points
  

  // upload program
  std::cout << "upload program" << std::endl;  
  RT_CHECK(vx_upload_kernel_file(device, kernel_file));

  // allocate device memory for source and destination and filling up their device addresses into kernel_arg.
  std::cout << "allocate device memory" << std::endl;
  RT_CHECK(vx_mem_alloc(device, src_buf_size, VX_MEM_TYPE_GLOBAL, &kernel_arg.src_addr));
  RT_CHECK(vx_mem_alloc(device, dst_buf_size, VX_MEM_TYPE_GLOBAL, &kernel_arg.dst_addr));

  std::cout << "dev_src=0x" << std::hex << kernel_arg.src_addr << std::endl;
  std::cout << "dev_dst=0x" << std::hex << kernel_arg.dst_addr << std::endl;

  // Filling up the other kernel_arg attributes
  kernel_arg.num_points = num_points; // number of points per partition of input
  kernel_arg.num_tasks = num_tasks; // number of logical threads = number of hardware threads = destination size


  // allocate staging buffer  
  {
    std::cout << "allocate staging buffer" << std::endl;    
    uint32_t staging_buf_size = std::max<uint32_t>(src_buf_size,
                                  std::max<uint32_t>(dst_buf_size, 
                                    sizeof(kernel_arg_t)));
    staging_buf.resize(staging_buf_size);
  }
  
  // upload kernel argument  
  std::cout << "upload kernel argument" << std::endl;
  memcpy(staging_buf.data(), &kernel_arg, sizeof(kernel_arg_t)); // upload into GPU 
  RT_CHECK(vx_copy_to_dev(device, KERNEL_ARG_DEV_MEM_ADDR, staging_buf.data(), sizeof(kernel_arg_t))); // determine based on location and access it 
  
  // upload source buffer
  {
    std::cout << "upload source buffer" << std::endl;
    auto buf_ptr = staging_buf.data();
    memcpy(buf_ptr, src_data.data(), num_total_points * sizeof(TYPE));      
    RT_CHECK(vx_copy_to_dev(device, kernel_arg.src_addr, staging_buf.data(), src_buf_size));
  }

  // clear destination buffer
  {
    std::cout << "clear destination buffer" << std::endl;
    auto buf_ptr = (int32_t*)staging_buf.data();
    for (uint32_t i = 0; i < num_tasks; ++i) {
      buf_ptr[i] = 0xdeadbeef;
    }    
    RT_CHECK(vx_copy_to_dev(device, kernel_arg.dst_addr, staging_buf.data(), dst_buf_size));  
  }

  // run tests
  std::cout << "run tests" << std::endl;
  RT_CHECK(run_test(kernel_arg, dst_buf_size, num_tasks));

  // cleanup
  std::cout << "cleanup" << std::endl;  
  cleanup();

  std::cout << "PASSED!" << std::endl;

  return 0;
}