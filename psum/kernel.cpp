#include <stdint.h>
#include <vx_intrinsics.h>
#include <vx_spawn.h>
#include "common.h"

/**
 * @brief 
 * 
 * @param task_id thread ID to go to a specific partition 
 * @param arg 
 */
void kernel_body(int task_id, kernel_arg_t* __UNIFORM__ arg) {
	uint32_t num_points = arg->num_points;
	auto src_ptr = (TYPE*)arg->src_addr;
	auto dst_ptr = (TYPE*)arg->dst_addr;

	// variable to store partial sum for this partition
	int sum = 0;

	// loops through every num_points (partition) and adds all elements 
	for (uint32_t i = 0; i < num_points; ++i) {

		int index = task_id*num_points + i;
		auto cur_value = src_ptr[index];		
		sum = sum + cur_value;

	}

	// store sum at correct location of destination
	dst_ptr[task_id] = sum;
}

int main() { // executed by hardware thread 
	kernel_arg_t* arg = (kernel_arg_t*)KERNEL_ARG_DEV_MEM_ADDR; // loads arguments from KERNEL_ARG_DEV_MEM_ADDR

	/** Spawns the tasks: 
	 * arg->num_tasks is the number of logical threads (thread blocks * block size). 
	 * Arg becomes the argument “arg” for kernel_body.
	**/
	vx_spawn_tasks(arg->num_tasks, (vx_spawn_tasks_cb)kernel_body, arg); // spawn tasks
	return 0;
}
