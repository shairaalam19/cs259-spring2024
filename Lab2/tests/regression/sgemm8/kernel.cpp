#include <stdint.h>
#include <vx_intrinsics.h>
#include <vx_spawn.h>
#include "common.h"

inline char is_log2(uint32_t x) {
    return ((x & (x-1)) == 0);
}

void kernel_body(uint32_t task_id, kernel_arg_t* __UNIFORM__ arg) {
	auto A = reinterpret_cast<int8_t*>(arg->A_addr);
	auto B = reinterpret_cast<int8_t*>(arg->B_addr);
	auto C = reinterpret_cast<int32_t*>(arg->C_addr);
    auto size = arg->size;

    uint32_t row, col;
    if (is_log2(size)) {
        row = task_id >> arg->log2_size;
        col = task_id & (size-1);
    } else {
        row = task_id / size;
        col = task_id % size;
    }

    // int32_t sum(0);
    // for (int e = 0; e < size; ++e) {
    //     sum += A[row * size + e] * B[e * size + col];
    // }

    int32_t sum(0);
    for (int e = 0; e < size; e += 4) {
        uint32_t packed_a = *(uint32_t*)&A[row * size + e + 0];

        uint8_t b0 = B[(e + 0) * size + col];
        uint8_t b1 = B[(e + 1) * size + col];
        uint8_t b2 = B[(e + 2) * size + col];
        uint8_t b3 = B[(e + 3) * size + col];
        uint32_t packed_b = ((uint32_t)b3 << 24) | ((uint32_t)b2 << 16) | ((uint32_t)b1 << 8) | (uint32_t)b0;

        sum += vx_dot8(packed_a, packed_b);
    }

    C[task_id] = sum;
}

int main() {
	kernel_arg_t* arg = (kernel_arg_t*)csr_read(VX_CSR_MSCRATCH);
	vx_spawn_tasks(arg->num_tasks, (vx_spawn_tasks_cb)kernel_body, arg);
	return 0;
}
