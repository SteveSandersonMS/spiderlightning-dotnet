@echo off

echo Ensure you have wit-bindgen 0.2.0
echo Install with: cargo install --git https://github.com/bytecodealliance/wit-bindgen wit-bindgen-cli --tag v0.2.0

wit-bindgen c --export http-handler.wit --out-dir generated
wit-bindgen c --import http-server.wit --out-dir generated
wit-bindgen c --export http-server-export.wit --out-dir generated
wit-bindgen c --import http-types.wit --out-dir generated
wit-bindgen c --import keyvalue.wit --out-dir generated
