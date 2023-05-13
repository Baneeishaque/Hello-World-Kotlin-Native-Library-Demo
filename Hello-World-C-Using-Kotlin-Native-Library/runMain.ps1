Import-Module "$env:ProgramFiles\Microsoft Visual Studio\2022\Preview\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath "$env:ProgramFiles\Microsoft Visual Studio\2022\Preview" -DevCmdArguments '-arch=x64'
Copy-Item ..\Hello-World-Library-Kotlin-Native-Gradle\build\bin\native\debugShared\* .
lib /def:libnative.def /out:libnative.lib
cl.exe main.c libnative.lib
.\main.exe
