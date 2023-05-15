using System.Runtime.InteropServices;

namespace SpiderLightning;

[StructLayout(LayoutKind.Sequential)]
public readonly struct HttpRequest
{
    // Internal representation for interop
    internal readonly HttpMethod method;
    internal readonly HttpServer.Utf8String uri;
    internal readonly HttpServer.StringPairList headers;
    internal readonly HttpServer.StringPairList @params;
    internal readonly WitOption<WitBuffer> body;

    // Friendly .NET APIs
    public HttpMethod Method => method;
    public string Uri => uri.ToString();
    public IReadOnlyList<KeyValuePair<string, string>> Headers => headers.Read();
    public IReadOnlyList<KeyValuePair<string, string>> Parameters => @params.Read();
    public byte[]? Body => body.Value.HasValue ? body.Value.Value.ToArray() : default;
}

public enum HttpMethod : byte
{
    Get = 0,
    Post = 1,
    Put = 2,
    Delete = 3,
    Patch = 4,
    Head = 5,
    Options = 6,
}
