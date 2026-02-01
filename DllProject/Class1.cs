namespace DllProject;

public static class Greeter
{
    public static string GetGreeting(string? name)
    {
        var safeName = string.IsNullOrWhiteSpace(name) ? "world" : name.Trim();
        return $"Hello, {safeName} from DllProject!";
    }
}
