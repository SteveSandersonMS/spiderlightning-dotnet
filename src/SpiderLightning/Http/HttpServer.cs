using System.Runtime.InteropServices;

namespace SpiderLightning;

public class HttpServer : IDisposable
{
    readonly int _serverIndex;
    private static Action? _onServerInit;
    private static Func<HttpRequest, HttpResponse>? _onIncomingRequest;

    internal HttpServer(string address, int routerIndex)
    {
        http_server_server_serve(new WitString(address), routerIndex, out var result);
        _serverIndex = result.Unwrap();
    }

    public static void OnServerInit(Action callback)
    {
        _onServerInit = callback;
    }

    public static void OnIncomingRequest(Func<HttpRequest, HttpResponse> callback)
    {
        _onIncomingRequest = callback;
    }

    public void Dispose()
    {
        http_server_server_stop(_serverIndex, out var result);
        result.Unwrap();
    }

    // TODO: Somehow make sure this doesn't get trimmed
    private static void Export_OnServerInit()
    {
        _onServerInit?.Invoke();
    }

    // TODO: Somehow make sure this doesn't get trimmed
    private static unsafe void Export_HandleIncomingRequest(ref HttpRequest request, out WitResult<HttpResponse> response)
    {
        if (_onIncomingRequest is null)
        {
            throw new InvalidOperationException($"No request handler was configured. Configure one by calling {nameof(HttpServer)}.{nameof(HttpServer.OnIncomingRequest)}");
        }

        response = new() { IsError = 0, ResultOrError = _onIncomingRequest.Invoke(request) };
    }

    const string LibraryName = "libSystem.Native"; // It just has to be any name that the pinvoke generator knows about

    [DllImport(LibraryName)]
    static extern void http_server_server_serve(WitString address, int routerIndex, out WitResult<int> result);

    [DllImport(LibraryName)]
    static extern void http_server_server_stop(int index, out WitResult<int> result);

    internal readonly struct Utf8String
    {
#pragma warning disable CS0649 // Written via interop
        readonly nint utf8_ptr;
        readonly int utf8_len;
#pragma warning restore CS0649 // Written via interop

        public override string ToString()
            => Marshal.PtrToStringUTF8(utf8_ptr, utf8_len);
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct StringPairList
    {
        nint ptr;
        int len;

        public int Length => len;

        public unsafe KeyValuePair<string, string>[] Read()
        {
            var span = new Span<(WitBuffer, WitBuffer)>((void*)ptr, len);
            var result = new KeyValuePair<string, string>[len];
            for (var i = 0; i < len; i++)
            {
                result[i] = new(span[i].Item1.ToStringFromUTF8(), span[i].Item2.ToStringFromUTF8());
            }
            return result;
        }

        internal static StringPairList Create(IEnumerable<KeyValuePair<string, string>> from)
        {
            // TODO: Make this GC-safe. The pointer needs to be pinned, but it's unclear how we get a notification to unpin it.
            var buffer = from.Select(x => (WitBuffer.FromStringAsUTF8(x.Key), WitBuffer.FromStringAsUTF8(x.Value))).ToArray();
            return new StringPairList { len = buffer.Length, ptr = Marshal.UnsafeAddrOfPinnedArrayElement(buffer, 0) };
        }
    }
}
