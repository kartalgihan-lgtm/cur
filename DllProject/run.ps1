$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectPath = Join-Path $projectRoot "DllProject.csproj"

dotnet build $projectPath -c Release | Out-Host

$framework = "net8.0"
$dllPath = Join-Path $projectRoot "bin" "Release" $framework "DllProject.dll"
if (-not (Test-Path $dllPath)) {
    throw "Expected DLL not found at $dllPath"
}

Add-Type -Path $dllPath
$message = [DllProject.Greeter]::GetGreeting("PowerShell")
Write-Output $message
