using System.Runtime.InteropServices;

namespace SpiderLightning;

public partial class KeyValue
{
    private int _index;

    public KeyValue(string name)
    {
        // TODO: length should probably be the byte length
        Open(name, name.Length, out var result);
        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Open failed with error {result.ErrorTag}");
        }

        _index = result.KeyValueIndex;
    }

    public string Get(string key)
    {
        // TODO: lengths should probably be the byte length
        Get(_index, key, key.Length, out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Get failed with error {result.ErrorTag}");
        }

        return Marshal.PtrToStringUTF8(result.ValuePtr, result.ValueLength);
    }

    public void Set(string key, string value)
    {
        // TODO: lengths should probably be the byte length
        // TODO: value should be byte[]
        Set(_index, key, key.Length, value, value.Length, out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Set failed with error {result.ErrorTag}");
        }
    }

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_open")]
    private static unsafe partial void Open([MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, out KeyOpenResult result);

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_get")]
    private static unsafe partial void Get(int index, [MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, out KeyGetResult result);

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_set")]
    private static unsafe partial void Set(int index, [MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, [MarshalAs(UnmanagedType.LPUTF8Str)] string value, int valueLength, out KeyResult result);

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
