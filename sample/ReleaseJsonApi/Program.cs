using System.Text.Json;
using System.Text.Json.Serialization;
using ReportJson;
using SpiderLightning;

Report report = await ReleaseReport.Generator.MakeReportAsync();
string json = JsonSerializer.Serialize(report, AppJsonSerializerContext.Default.Report);

HttpServer.OnServerInit(() =>
{
    const string address = "localhost:3000";

    new HttpRouter()
        .Get("/")
        .Get("/*")
        .Serve(address);

    Console.WriteLine($"Listening on http://{address}");
});

HttpServer.OnIncomingRequest(request =>
{
    return new HttpResponse(200)
        .WithBody(json)
        .WithHeaders(new[] { KeyValuePair.Create("content-type", "text/json") });
});

[JsonSourceGenerationOptions(
    GenerationMode = JsonSourceGenerationMode.Serialization,
    PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase,
    WriteIndented = true
    )]
[JsonSerializable(typeof(Report))]
internal partial class AppJsonSerializerContext : JsonSerializerContext
{
}
