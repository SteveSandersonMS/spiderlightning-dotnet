using SpiderLightning;

var keyValue = KeyValue.Open("placeholder-name");
keyValue.Set("somekey", "Hello from .NET");

Console.WriteLine(keyValue.Get("somekey"));
