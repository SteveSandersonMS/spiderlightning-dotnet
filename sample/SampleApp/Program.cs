using SpiderLightning;

var keyValue = new KeyValue("placeholder-name");
keyValue.Set("somekey", "Hello from .NET");
Console.WriteLine(keyValue.GetString("somekey"));
