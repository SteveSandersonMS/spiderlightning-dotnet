# .NET on SpiderLightning

This is an example of how .NET can be integrated as a guest for [SpiderLightning](https://github.com/deislabs/spiderlightning). It uses the `wasi-experimental` workload which is first available in .NET 8 Preview 4.

## Prerequisites

* .NET 8 Preview 4 or later
  * If this is not yet released (which it isn't, at the time of writing), download it from [.NET Daily Builds](https://github.com/dotnet/installer/blob/main/README.md#installers-and-binaries)
  * Verify that `dotnet --version` tells you it's .NET 8 Preview 4 or later
* Install the `wasi-experimental` workload
  * Run `dotnet workload install wasi-experimental -s https://pkgs.dev.azure.com/dnceng/public/_packaging/dotnet8/nuget/v3/index.json`
* WASI SDK
  * Download from [wasi-sdk releases page](https://github.com/WebAssembly/wasi-sdk/releases). If you're using Windows, you need the one with `mingw` in its name.
  * Create an environment variable called `WASI_SDK_PATH` giving the path where you extracted the WASI SDK download, i.e., the directory containing `bin`/`lib`/`share`.
* Slight
  * Download from [SpiderLightning Releases](https://github.com/deislabs/spiderlightning/releases)
  * Once extracted, add the directory containing `slight` (or `slight.exe` on Windows) to your PATH.

## How to build

* `git clone https://github.com/SteveSandersonMS/spiderlightning-dotnet.git`
* `cd spiderlightning-dotnet`
* `dotnet build` (or open `Spiderlightning.sln` in Visual Studio)

## Samples

There are two samples. You can run them from Visual Studio via Ctrl+F5, or from the command line:

 * ConsoleApp - demonstrates KeyValue API
   * `cd sample/ConsoleApp`
   * `dotnet run`
 * WebServer - demonstrates HTTP Server
   * `cd sample/WebServer`
   * `dotnet run`

Note that `dotnet run` is configured to run the project using `slight`, so you need that to be on your PATH. Alternatively, you can build and run manually:

 * `dotnet build`
 * `slight -c slightfile.toml run bin\Debug\net8.0\wasi-wasm\AppBundle\ProjectName.wasm -l` (where `ProjectName` is `ConsoleApp` or `WebServer`).

## Publishing

When you're inside one of the sample directories, you can run `dotnet publish`. This will generate a `.wasm` file inside `bin\Release\net8.0\wasi-wasm\AppBundle`. You should be able to transfer that onto another machine/server and run it with `slight`.
