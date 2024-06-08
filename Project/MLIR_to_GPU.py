from mlir.ir import *
import mlir.dialects.builtin as builtin
import mlir.dialects.func as func
import mlir.dialects.linalg as linalg
from mlir.dialects.linalg.opdsl.lang import *

from mlir.passmanager import *

import numpy as np
import subprocess as sp
import pyopencl as cl

ctx = Context()

# A module that performs elementwise addition using linalg MLIR
def testMemrefAdd():
    with ctx, Location.unknown() as loc:
        m = builtin.ModuleOp()
        f32 = F32Type.get()

        with InsertionPoint(m.body):
            @func.FuncOp.from_py_func(
                MemRefType.get([1024], f32),
                MemRefType.get([1024], f32),
                MemRefType.get([1024], f32)
            )
            def vadd(bufA, bufB, outBuf):
                result = linalg.elemwise_binary(bufA, bufB, outs=[outBuf], fun=BinaryFn.add)
                return result
    return m


# Lowering linalg ops to the gpu dialect.
def gpu_frontend(module):
    with ctx:
        pm = PassManager("any")
        pm.add("convert-linalg-to-parallel-loops")
        pm.add("func.func(gpu-map-parallel-loops)")
        pm.add("func.func(convert-parallel-loops-to-gpu)")
        pm.add("convert-scf-to-cf")
        pm.add("canonicalize")
        pm.add("gpu-kernel-outlining")
        pm.add("canonicalize")
        pm.run(module.operation)
    return module

# getting linalg MLIR
m0 = testMemrefAdd()
# The below should print MLIR to screen
print(m0)

# lowering above to GPU dialect
m1 = gpu_frontend(m0)
# The below should print the gpu dialect.
print(m1)

# TODO: if all of the above works add and call and additional function that lowers gpu dialect to llvm host code.