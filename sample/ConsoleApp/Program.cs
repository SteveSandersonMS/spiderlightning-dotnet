using SpiderLightning;

using var keyValue = new KeyValue("placeholder-name");
keyValue.Set("somekey", "Hello from .NET. This value is from a SpiderLightning key-value store.");

Console.WriteLine(keyValue.GetString("somekey"));
