const unsigned long waiting_time_ms = 3000;   // 3s
unsigned long saved_moment = 0;
int state = 0;

void setup() { Serial.begin(9600); }

void loop()
{
  unsigned long current_moment = millis();
  if (current_moment - saved_moment >= waiting_time_ms)
  {
    saved_moment = current_moment;
    state = ++state % 2;        // 0 -> 1 -> 0 -> 1 -> 0 -> 1 -> 0 ...
    do_action();
  }

  stop_in_case_of_emergency();  // This is checked every few milliseconds
}

void do_action()
{
  switch(state)
  {
    case 0:
    {
      turn_laser_off();
      turn_motor_on();
      break;
    }
    case 1:
    {
      turn_motor_off();
      turn_laser_on();
      break;
    }
  }
}

void turn_laser_on()  { Serial.println("laser on"); }
void turn_laser_off() { Serial.println("laser off"); }
void turn_motor_on()  { Serial.println("motor on"); }
void turn_motor_off() { Serial.println("motor off"); }

bool is_emergency_button_pressed()
{ 
  // Serial.println("Checking emergency button");
  if(Serial.available())
  {
    Serial.println("Stopping due to emergency: " + Serial.readString());
    Serial.println("Please reset");
    return true;
  }

  return false;
}

void stop_in_case_of_emergency()
{
  if (is_emergency_button_pressed())
  {
    turn_motor_off();
    turn_laser_off();

    while (0 != 1) { }
  }
}
