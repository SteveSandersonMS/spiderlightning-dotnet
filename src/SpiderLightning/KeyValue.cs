using System.Runtime.InteropServices;

namespace SpiderLightning;

public partial struct KeyValue
{
    private int _index;

    private KeyValue(int index)
    {
        _index = index;
    }

    public static KeyValue Open(string name)
    {
        Open(name, name.Length, out var result);
        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Open failed with error {result.ErrorTag}");
        }

        return new KeyValue(result.KeyValueIndex);
    }

    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_open")]
    private static unsafe partial void Open([MarshalAs(UnmanagedType.LPUTF8Str)] string name, int nameLength, out KeyOpenResult result);

    [StructLayout(LayoutKind.Explicit)]
    public struct KeyOpenResult
    {
        [FieldOffset(0)] public byte IsError;
        [FieldOffset(4)] public int KeyValueIndex;
        [FieldOffset(4)] public int ErrorTag;
    }
}
