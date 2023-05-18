Import-Module "$env:ProgramFiles\Microsoft Visual Studio\2022\Preview\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath "$env:ProgramFiles\Microsoft Visual Studio\2022\Preview" -DevCmdArguments '-arch=x64'
Set-Location -Path $(Split-Path $($MyInvocation.MyCommand.Path) -Parent)
lib /DEF:build\bin\native\debugShared\native.def /OUT:build\bin\native\debugShared\native.lib
