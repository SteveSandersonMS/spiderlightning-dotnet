FROM alpine AS installer

RUN wget https://github.com/deislabs/spiderlightning/releases/download/v0.5.0/slight-linux-x86_64.tar.gz
RUN mkdir slight
RUN tar -xf slight-linux-x86_64.tar.gz -C slight

RUN wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/wasi-sdk-20.0-linux.tar.gz
RUN mkdir wasi-sdk
RUN tar -xf wasi-sdk-20.0-linux.tar.gz -C wasi-sdk

FROM mcr.microsoft.com/dotnet/sdk:8.0-preview-jammy

ENV WASI_SDK_PATH=/wasi-sdk/
COPY --from=installer /wasi-sdk/wasi-sdk-20.0/ $WASI_SDK_PATH

COPY --from=installer /slight/release/slight /usr/bin/

RUN dotnet workload install wasi-experimental
