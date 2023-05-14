using System.Runtime.InteropServices;
using System.Text;

namespace SpiderLightning;

public class KeyValue
{
    private const string LibraryName = "libSystem.Native"; // It just has to be any name that the pinvoke generator knows about

    private readonly int _index;

    public KeyValue(string name)
    {
        Open(new WasiString(name), out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Open failed with error {result.ErrorTag}");
        }

        _index = result.KeyValueIndex;
    }

    public unsafe byte[] Get(string key)
    {
        Get(_index, new WasiString(key), out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Get failed with error {result.ErrorTag}");
        }

        return new Span<byte>((void*)result.ValuePtr, result.ValueLength).ToArray();
    }

    public string GetString(string key)
    {
        Get(_index, new WasiString(key), out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Get failed with error {result.ErrorTag}");
        }

        return Marshal.PtrToStringUTF8(result.ValuePtr, result.ValueLength);
    }

    public void Set(string key, byte[] value)
    {
        Set(_index, new WasiString(key), new WasiByteArray(value), out var result);

        if (result.IsError != 0)
        {
            throw new InvalidOperationException($"KeyValue.Set failed with error {result.ErrorTag}");
        }
    }

    public void Set(string key, string value)
        => Set(key, Encoding.UTF8.GetBytes(value));

    [DllImport(LibraryName, EntryPoint = "keyvalue_keyvalue_open")]
    private static unsafe extern void Open(WasiString name, out KeyOpenResult result);

    [DllImport(LibraryName, EntryPoint = "keyvalue_keyvalue_get")]
    private static unsafe extern void Get(int index, WasiString name, out KeyGetResult result);

    [DllImport(LibraryName, EntryPoint = "keyvalue_keyvalue_set")]
    private static unsafe extern void Set(int index, WasiString name, WasiByteArray value, out KeyResult result);

    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct WasiString
    {
        [MarshalAs(UnmanagedType.LPUTF8Str)]
        public string String;

        public int StringByteLength;

        public WasiString(string value)
        {
            String = value;
            StringByteLength = Encoding.UTF8.GetByteCount(value);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct WasiByteArray
    {
        public byte[] Data;
        public int DataLength;

        public WasiByteArray(byte[] data)
        {
            Data = data;
            DataLength = data.Length;
        }
    }

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
    public unsafe struct KeyGetResult
    {
        [FieldOffset(0)] public byte IsError;
        [FieldOffset(4)] public nint ValuePtr;
        [FieldOffset(8)] public int ValueLength;
        [FieldOffset(4)] public int ErrorTag;
    }
}
