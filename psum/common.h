#ifndef _COMMON_H_
#define _COMMON_H_

#define KERNEL_ARG_DEV_MEM_ADDR 0x7ffff000 // kernel argument for where it is stored

#ifndef TYPE
#define TYPE int
#endif

typedef struct {
  uint32_t num_points; // size of partition 
  uint32_t num_tasks; // # of logical threads = hardware threads 
  uint64_t src_addr;
  uint64_t dst_addr;  
} kernel_arg_t;

#endif
