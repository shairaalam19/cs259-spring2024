import mlir.ir as ir

# Create an MLIR context
with ir.Context() as context:
    # Establish a default location
    loc = ir.Location.unknown()

    # Create an MLIR module with the default location
    with loc:
        module = ir.Module.create()
    
    print(module)