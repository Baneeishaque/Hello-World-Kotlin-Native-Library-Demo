Import-Module "$env:ProgramFiles\Microsoft Visual Studio\2022\Preview\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath "$env:ProgramFiles\Microsoft Visual Studio\2022\Preview" -DevCmdArguments '-arch=x64'
Set-Location -Path $(Split-Path $($MyInvocation.MyCommand.Path) -Parent)
Copy-Item ..\Hello-World-Library-Kotlin-Native-Gradle\build\bin\native\debugShared\* .
lib /def:native.def /out:native.lib
cl.exe main.c native.lib
.\main.exe
