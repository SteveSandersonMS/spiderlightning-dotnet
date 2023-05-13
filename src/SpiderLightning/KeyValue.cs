using System.Runtime.InteropServices;
using System.Text;

namespace SpiderLightning;

public partial class KeyValue
{
    private int _index;

    public KeyValue(string name)
    {
        Open(name, Encoding.UTF8.GetByteCount(name), out var result);
        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Open failed with error {result.ErrorTag}");
        }

        _index = result.KeyValueIndex;
    }

    public unsafe byte[] Get(string key)
    {
        Get(_index, key, Encoding.UTF8.GetByteCount(key), out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Get failed with error {result.ErrorTag}");
        }

        return new Span<byte>((void*)result.ValuePtr, result.ValueLength).ToArray();
    }

    public string GetString(string key)
    {
        Get(_index, key, Encoding.UTF8.GetByteCount(key), out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Get failed with error {result.ErrorTag}");
        }

        return Marshal.PtrToStringUTF8(result.ValuePtr, result.ValueLength);
    }

    public void Set(string key, byte[] value)
    {
        Set(_index, key, Encoding.UTF8.GetByteCount(key), value, value.Length, out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Set failed with error {result.ErrorTag}");
        }
    }

    public void Set(string key, string value)
        => Set(key, Encoding.UTF8.GetBytes(value));

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_open")]
    private static unsafe partial void Open([MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, out KeyOpenResult result);

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_get")]
    private static unsafe partial void Get(int index, [MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, out KeyGetResult result);

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_set")]
    private static unsafe partial void Set(int index, [MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, byte[] value, int valueLength, out KeyResult result);

    [StructLayout(LayoutKind.Explicit)]
    public struct KeyResult
    {
        [FieldOffset(0)] public byte IsError;
        [FieldOffset(4)] public int ErrorTag;
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct KeyOpenResult
    {
        [FieldOffset(0)] public byte IsError;
        [FieldOffset(4)] public int KeyValueIndex;
        [FieldOffset(4)] public int ErrorTag;
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct KeyGetResult
    {
        [FieldOffset(0)] public byte IsError;
        [FieldOffset(4)] public int ValuePtr;
        [FieldOffset(8)] public int ValueLength;
        [FieldOffset(4)] public int ErrorTag;
    }
}
