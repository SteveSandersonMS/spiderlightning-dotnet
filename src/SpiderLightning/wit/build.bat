@echo off

echo Ensure you have wit-bindgen 0.2.0
echo Install with: cargo install --git https://github.com/bytecodealliance/wit-bindgen wit-bindgen-cli --tag v0.2.0

wit-bindgen c --import keyvalue_0.5.0\keyvalue.wit --out-dir generated
