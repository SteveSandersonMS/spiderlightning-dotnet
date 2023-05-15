using System.Runtime.InteropServices;

namespace SpiderLightning;

public class HttpRouter : IDisposable
{
    readonly int _routerIndex;

    public HttpRouter()
    {
        http_server_router_new(out var result);
        _routerIndex = result.Unwrap();
    }

    private HttpRouter(int routerIndex)
    {
        _routerIndex = routerIndex;
    }

    public HttpServer Serve(string address)
        => new HttpServer(address, _routerIndex);

    public void Dispose()
       => http_server_router_free(_routerIndex);

    public HttpRouter Get(string route)
    {
        http_server_router_get(_routerIndex, new WitString(route), new WitString("handle-http"), out var result);
        return new HttpRouter(result.Unwrap());
    }

    const string LibraryName = "libSystem.Native"; // It just has to be any name that the pinvoke generator knows about

    [DllImport(LibraryName)]
    static extern void http_server_router_new(out WitResult<int> result);

    [DllImport(LibraryName)]
    static extern void http_server_router_free(int routerIndex);

    [DllImport(LibraryName)]
    static extern void http_server_router_get(int routerIndex, WitString route, WitString handlerName, out WitResult<int> result);
}
