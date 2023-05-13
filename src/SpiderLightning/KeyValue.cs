using System.Runtime.InteropServices;

namespace SpiderLightning;

public partial class KeyValue
{
    [LibraryImport("libSystem.Native", EntryPoint = "__wasm_import_keyvalue_keyvalue_open")]
    public static partial void Open(int a, int b, int c);
}
