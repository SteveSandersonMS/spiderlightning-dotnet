# Spiderlighting with Docker

The combination of .NET SDK and Spiderlighting has multiple dependencies that you may not want to install on your machine. You can use Docker to avoid that with a [Dockerfile](Dockerfile) that provides a known (working) environment.

The following instructions assume that you've cloned the repo and are in the root directory. They also assume you have Docker installed.

Multiple of the dependencies are x64-specific. You must build and use this Dockerfile on an x64 machine. It will not work on a Mac M1 machine. We hope that changes in the future.

## Build the Dockerfile

```bash
cd docker
docker build --pull -t wasi-sdk .
```

You should now have an Ubuntu x64 container image, with the tag `wasi-sdk`.

## Build a sample

You can use the container image to build the samples (from the repo root directory).

On Linux or macOS:

```bash
docker run --rm -v $(pwd):/source -w /source wasi-sdk dotnet build sample/ConsoleApp/
```

On Windows (using [PowerShell 7.x](https://github.com/PowerShell/PowerShell)):

```bash
docker run --rm -v ${pwd}:/source -w /source wasi-sdk dotnet build sample/ConsoleApp/
```

You should now have a `.wasm` file within the build output of the `ConsoleApp` directory.

## Run the sample

You can use the same container image to run the sample.

On Linux or macOS:

```bash
docker run --rm -v $(pwd):/app -w /app wasi-sdk slight -c sample/ConsoleApp/slightfile.toml run sample/ConsoleApp/bin/Debug/net8.0/wasi-wasm/AppBundle/ConsoleApp.wasm -l
```

On Windows (using [PowerShell 7.x](https://github.com/PowerShell/PowerShell)):

```bash
docker run --rm -v ${pwd}:/app -w /app wasi-sdk slight -c sample/ConsoleApp/slightfile.toml run sample/ConsoleApp/bin/Debug/net8.0/wasi-wasm/AppBundle/ConsoleApp.wasm -l
```

Note: On Windows, only the volume mount syntax is different.
