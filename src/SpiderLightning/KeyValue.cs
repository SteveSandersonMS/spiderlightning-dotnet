using System.Runtime.InteropServices;
using System.Text;

namespace SpiderLightning;

public class KeyValue : IDisposable
{
    readonly int _keyValueIndex;

    public KeyValue(string name)
    {
        keyvalue_keyvalue_open(new WitString(name), out var result);
        _keyValueIndex = result.Unwrap();
    }

    public byte[] Get(string key)
    {
        keyvalue_keyvalue_get(_keyValueIndex, new WitString(key), out var result);
        return result.Unwrap().ToArray();
    }

    public string GetString(string key)
    {
        keyvalue_keyvalue_get(_keyValueIndex, new WitString(key), out var result);
        return result.Unwrap().ToStringFromUTF8();
    }

    public void Set(string key, byte[] value)
    {
        keyvalue_keyvalue_set(_keyValueIndex, new WitString(key), new WitByteArray(value), out var result);
        result.Unwrap();
    }

    public void Set(string key, string value)
        => Set(key, Encoding.UTF8.GetBytes(value));

    public void Dispose()
        => keyvalue_keyvalue_free(_keyValueIndex);

    const string LibraryName = "libSystem.Native"; // It just has to be any name that the pinvoke generator knows about

    [DllImport(LibraryName)]
    static extern void keyvalue_keyvalue_open(WitString name, out WitResult<int> result);

    [DllImport(LibraryName)]
    static extern void keyvalue_keyvalue_get(int index, WitString name, out WitResult<WitBuffer> result);

    [DllImport(LibraryName)]
    static extern void keyvalue_keyvalue_set(int index, WitString name, WitByteArray value, out WitResult<int> result);

    [DllImport(LibraryName)]
    static extern void keyvalue_keyvalue_free(int index);
}
