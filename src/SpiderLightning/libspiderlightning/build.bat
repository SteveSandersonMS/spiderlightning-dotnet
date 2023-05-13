@echo off

echo Ensure you have wit-bindgen 0.2.0
echo Install with: cargo install --git https://github.com/bytecodealliance/wit-bindgen wit-bindgen-cli --tag v0.2.0

wit-bindgen c --import wit\keyvalue_0.5.0\keyvalue.wit --out-dir bindings

%WASI_SDK_PATH%\bin\clang bindings\*.c -Ibindings -nostdlib -Wl,--relocatable -o libspiderlightning.wasm
