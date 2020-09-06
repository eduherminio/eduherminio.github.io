$path = $args[0]
$port = $args[1]

if (Test-Path $path) {
  arduino-cli compile -b arduino:avr:uno $path
  arduino-cli upload -b arduino:avr:uno -p ($port ?? "COM6") $path
}