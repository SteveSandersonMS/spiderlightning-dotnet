using System.Runtime.CompilerServices;
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
        _index = result.Unwrap();
    }

    public unsafe byte[] Get(string key)
    {
        Get(_index, new WasiString(key), out var result);
        return result.Unwrap().ToArray();
    }

    public string GetString(string key)
    {
        Get(_index, new WasiString(key), out var result);
        return result.Unwrap().ToStringFromUTF8();
    }

    public void Set(string key, byte[] value)
    {
        Set(_index, new WasiString(key), new WasiByteArray(value), out var result);
        result.Unwrap();
    }

    public void Set(string key, string value)
        => Set(key, Encoding.UTF8.GetBytes(value));

    [DllImport(LibraryName, EntryPoint = "keyvalue_keyvalue_open")]
    private static extern void Open(WasiString name, out OkResult<int> result);

    [DllImport(LibraryName, EntryPoint = "keyvalue_keyvalue_get")]
    private static extern void Get(int index, WasiString name, out OkResult<WasiResultBuffer> result);

    [DllImport(LibraryName, EntryPoint = "keyvalue_keyvalue_set")]
    private static extern void Set(int index, WasiString name, WasiByteArray value, out OkResult<int> result);

    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct WasiString
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
    private unsafe struct WasiByteArray
    {
        public byte[] Data;
        public int DataLength;

        public WasiByteArray(byte[] data)
        {
            Data = data;
            DataLength = data.Length;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct WasiResultBuffer
    {
        public nint Data;
        public int DataLength;

        public string ToStringFromUTF8()
            => Marshal.PtrToStringUTF8(Data, DataLength);

        public byte[] ToArray()
            => new Span<byte>((void*)Data, DataLength).ToArray();
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct OkResult<T> where T : struct
    {
        public byte IsError;
        public T ResultOrError;

        public unsafe T Unwrap()
        {
            if (IsError != 0)
            {
                // We can't use fixed layout for generic structs, so manually reinterpret the ResultOrError field as an int
                var errorPtr = Unsafe.AsPointer(ref ResultOrError);
                var error = Marshal.PtrToStructure<WasiError>((nint)errorPtr);
                throw new InvalidOperationException($"The operation failed with error code {error.Tag} and message '{error.ReadMessage()}'.");
            }

            return ResultOrError;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct WasiError
    {
        public byte Tag;
        public nint MessageUtf8;
        public int MessageLength;

        public string ReadMessage()
            => Marshal.PtrToStringUTF8(MessageUtf8, MessageLength);
    }
}
