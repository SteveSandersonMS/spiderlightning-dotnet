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

## Run samples

You can build and run the samples in a container with the following pattern (from the repo root directory).

```bash
$ docker run --rm -it -v $(pwd):/source -w /source/sample/ConsoleApp wasi-sdk dotnet run -c Release
Hello from .NET. This value is from a SpiderLightning key-value store.
```

On Windows (using [PowerShell 7.x](https://github.com/PowerShell/PowerShell)), the volume mount syntax is slightly different.

```bash
-v ${pwd}:/source
```

## Build samples

You can use the container image to build the samples (from the repo root directory), creating a `.wasm` artifact on your machine.

```bash
docker run --rm -v $(pwd):/source -w /source/sample/ConsoleApp wasi-sdk dotnet publish 
```
You should now have a `.wasm` file within the build output of the `ConsoleApp` directory.

```bash
$ find sample/ConsoleApp/ | grep AppBundle/ConsoleApp.wasm
sample/ConsoleApp/bin/Release/net8.0/wasi-wasm/AppBundle/ConsoleApp.wasm
```

You can then run the sample with `slight`, installed on your machine or with the same container.

```bash
docker run --rm -v $(pwd):/app -w /app/sample/ConsoleApp wasi-sdk slight -c slightfile.toml run bin/Release/net8.0/wasi-wasm/AppBundle/ConsoleApp.wasm -l
```

A similar process can be used with the `WebServer` sample. It exposes a `localhost` port, which cannot be port mapped outside of the container so is intended to be run locally.

```bash
$ docker run --rm -v $(pwd):/source -w /source/sample/WebServer wasi-sdk dotnet publish
$ slight -c slightfile.toml run bin/Release/net8.0/wasi-wasm/AppBundle/WebServer.wasm -l
Listening on http://localhost:3000
```

In another terminal:

```bash
$ curl http://localhost:3000/hi-dotnet
<h1>Hello!</h1> You did a Get request to /hi-dotnet with 3 headers
```