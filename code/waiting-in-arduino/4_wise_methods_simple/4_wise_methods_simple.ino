#include "limits.h"

const int time_motor_fast   = 3000;
const int time_motor_slow   = 750;
const int time_laser_on     = 3250;

const String message_motor_fast   = "[info] Turning motor fast";
const String message_motor_slow   = "[info] Turning motor slow";
const String message_laser_on     = "[info] Turning motor off and laser ON";

enum class State
{
  MotorFast,
  MotorSlow,
  LaserOn,

  Total = 3
};

unsigned long saved_time;
State current_state;

void setup()
{
  Serial.begin(9600);

  current_state = State::MotorFast;
  turn_motor_fast();
}

void loop()
{
  unsigned long current_moment = millis();
  unsigned long waiting_time = calculate_waiting_time();

  if (current_moment - saved_time >= waiting_time)
  {
    saved_time = current_moment;
    current_state = next_state();

    print_message();
    do_stuff();
  }

  stop_in_case_of_emergency();
}

State next_state()
{
  return static_cast<State>((static_cast<int>(current_state) + 1) % (static_cast<int>(State::Total)));
}

unsigned long calculate_waiting_time()
{
  switch(current_state)
  {
    case State::MotorFast:
    {
      return time_motor_fast;
    }
    case State::MotorSlow:
    {
      return time_motor_slow;
    }
    case State::LaserOn:
    {
      return time_laser_on;
    }
    default:
    {
      return ULONG_MAX;
    }
  }
}

void do_stuff()
{
  switch(current_state)
  {
    case State::MotorFast:
    {
      turn_laser_off();
      turn_motor_fast();
      break;
    }
    case State::MotorSlow:
    {
      turn_motor_slow();
      break;
    }
    case State::LaserOn:
    {
      turn_motor_off();
      turn_laser_on();
      break;
    }
    default:
    {
      turn_motor_off();
      turn_laser_off();
      break;
    }
  }
}

void print_message()
{
  switch(current_state)
  {
    case State::MotorFast:
    {
      Serial.println(message_motor_fast);
      break;
    }
    case State::MotorSlow:
    {
      Serial.println(message_motor_slow);
      break;
    }
    case State::LaserOn:
    {
      Serial.println(message_laser_on);
      break;
    }
    default:
    {
      Serial.println("Error printing message");
      break;
    }
  }
}

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

void turn_laser_on()    { Serial.println("laser on"); }
void turn_laser_off()   { Serial.println("laser off"); }
void turn_motor_fast()  { Serial.println("motor fast"); }
void turn_motor_slow()  { Serial.println("motor slow"); }
void turn_motor_off()   { Serial.println("motor off"); }
