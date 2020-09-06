const int waitingTime = 2000;   // 2 seconds
bool ledState = LOW;

void setup() { pinMode(LED_BUILTIN, OUTPUT); }

void loop()
{
  digitalWrite(LED_BUILTIN, ledState);
  delay(waitingTime);

  ledState = !ledState;
}