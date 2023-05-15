using System.Runtime.InteropServices;

namespace SpiderLightning;

[StructLayout(LayoutKind.Sequential)]
public struct HttpResponse
{
    // Internal representation for interop
    ushort status;
    WitOption<HttpServer.StringPairList> headers;
    WitOption<WitBuffer> body;

    public HttpResponse(int status)
    {
        this.status = (ushort)status;
    }

    public HttpResponse WithBody(string body)
    {
        this.body = WitBuffer.FromStringAsUTF8(body);
        return this;
    }

    public HttpResponse WithBody(byte[] body)
    {
        this.body = new WitBuffer
        {
            Data = Marshal.UnsafeAddrOfPinnedArrayElement(body, 0),
            DataLength = body.Length,
        };
        return this;
    }

    public HttpResponse WithHeaders(IReadOnlyCollection<KeyValuePair<string, string>> headers)
    {
        this.headers = HttpServer.StringPairList.Create(headers);
        return this;
    }
}
