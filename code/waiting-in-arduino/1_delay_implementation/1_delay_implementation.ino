const unsigned long waiting_time_ms = 3000;   // 3s

void setup() { Serial.begin(9600); }

void loop()
{
  turn_laser_off();
  turn_motor_on();
  delay(waiting_time_ms);
  
  stop_in_case_of_emergency();

  turn_motor_off();
  turn_laser_on();
  delay(waiting_time_ms);

  stop_in_case_of_emergency();
}

void turn_laser_on()  { Serial.println("laser on"); }
void turn_laser_off() { Serial.println("laser off"); }
void turn_motor_on()  { Serial.println("motor on"); }
void turn_motor_off() { Serial.println("motor off"); }

bool is_emergency_button_pressed()
{ 
  Serial.println("Checking emergency button");
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
