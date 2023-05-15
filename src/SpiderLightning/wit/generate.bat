@echo off

echo Ensure you have wit-bindgen 0.2.0
echo Install with: cargo install --git https://github.com/bytecodealliance/wit-bindgen wit-bindgen-cli --tag v0.2.0

for /f %%f in ('dir *.wit /b') do (
    wit-bindgen c --import %%f --out-dir generated
)
