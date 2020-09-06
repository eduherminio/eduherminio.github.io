const int waitingTime = 2000;   // 2 seconds
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() { pinMode(LED_BUILTIN, OUTPUT); }

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= waitingTime)
  {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}