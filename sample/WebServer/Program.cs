using SpiderLightning;

HttpServer.OnServerInit(() =>
{
    const string address = "0.0.0.0:3000";

    new HttpRouter()
        .Get("/")
        .Get("/*")
        .Serve(address);

    Console.WriteLine($"Listening on http://{address}");
});


HttpServer.OnIncomingRequest(request =>
{
    return new HttpResponse(200)
        .WithBody($"<h1>Hello!</h1> You did a {request.Method} request to {request.Uri} with {request.Headers.Count} headers")
        .WithHeaders(new[] { KeyValuePair.Create("content-type", "text/html") });
});
