if (Test-Path "./bin") {
  Remove-Item -Recurse -Path "./bin"
}

Get-ChildItem -Directory -r | ForEach-Object { if ($_ -like "*\build") { Remove-Item -Recurse -Path $_ } }
