# Releases.json Test App

This app explores the convenience spectrum of System.Text.Json APIs. It does that via processing [JSON documents](fakejson) of varying lengths. The JSON documents are fake test versions of the official [release.json](https://github.com/dotnet/core/blob/main/release-notes/releases-index.json) documents maintained by the .NET Team at Microsoft.

The app is implemented multiple times -- in the `*Benchmark.cs` files -- using various JSON APIs. The app is tested in three ways, accessing the JSON documents from different sources: remotely via Github `raw.` repo URLs, locally via a static files [web app](fakejsonweb), and locally via the file system.

A more complete version of the app is available at [dotnet/dotnet-docker](https://github.com/dotnet/dotnet-docker/blob/main/samples/releasesapp/README.md).

The results are discussed on the [.NET blog](https://devblogs.microsoft.com/dotnet/).

## Running the app

The app can be run with `dotnet run -c Release`.

In in its default configuration, it will run all the implementations `16` times, calling the remote URL for one of the documents.

The app takes an integer value as input, such as `dotnet run -c Release -- 0`

- `0`-`7` -- Runs a specific benchmark, and collects memory use information. Elapsed time is also provided, but isn't a useful number, since JIT cost is dominant
- `>= 10` -- Runs all the benchmarks n times, using a web URL.
- `100` -- Runs all the benchmarks once, using a web URL.
- `> 100` -- Runs all the benchmarks n times, using a local URL (need to start the [webserver](../fakejsonweb) first).
- `> 200` -- Runs all the benchmarks n times, using a local file.
- `> 300` -- Runs all the benchmarks n times, using a string (acquired from a local file).

The file used can be changed by specifying a second integer value as input, which reference an array in [`BenchmarkData.cs`](BenchmarkData.cs).

In the cases that the benchmarks are run multiple times, a certain number of runs are thrown away -- for producing an elapsed time average -- for warmup and to remove outliers. The elapsed time is collected in ticks, since some of the values collected may be around or below 1ms.

For example:

```
dotnet run -c Release 216 3
```

This will run all the benchmarks 16 times with a local file, using the document referenced by `FakeReleaseJson[3]`.
