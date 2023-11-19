# syntax=docker/dockerfile:1
FROM scratch AS installer

ADD https://github.com/deislabs/spiderlightning/releases/download/v0.5.1/slight-linux-x86_64.tar.gz /sdk/
ADD https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/wasi-sdk-20.0-linux.tar.gz /sdk/


FROM mcr.microsoft.com/dotnet/sdk:8.0-jammy

ENV WASI_SDK_PATH=/wasi-sdk/wasi-sdk-20.0/
ENV SLIGHT_SDK_PATH=/slight/release/

RUN dotnet workload install wasi-experimental
RUN --mount=type=bind,from=installer,source=/sdk,target=/sdk \
<<EOF
mkdir /slight
mkdir /wasi-sdk
tar -xf /sdk/slight-linux-x86_64.tar.gz -C slight
tar -xf /sdk/wasi-sdk-20.0-linux.tar.gz -C wasi-sdk
ln -s $SLIGHT_SDK_PATH/slight /usr/bin/slight
EOF
