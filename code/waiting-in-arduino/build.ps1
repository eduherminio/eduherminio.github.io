Get-ChildItem -Directory | ForEach-Object { if ($_ -notlike "*\bin") { Write-Host "`n*`tCompiling $_" ; Invoke-Expression "arduino-cli compile -b arduino:avr:uno $_ --output-dir bin" } }
