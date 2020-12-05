---
title: "Waiting in Arduino III: wise methods (simple example)"
excerpt: "Usage of 'wise' methods pattern to handle states in Arduino."
date: 2020-09-30T00:00:00+00:00
categories:
  - blog
tags:
  - arduino
  - cpp
  - DIY
series: "Waiting in Arduino"
---

{% include series.html.liquid content=" " %}

## Introduction: changes in our laser cutting machine project

Let's imagine we started building [our laser cutting machine](https://eduherminio.github.io/blog/delay-function-is-not-your-friend/#case-study) and just realized that the [initial state machine](https://eduherminio.github.io/blog/delay-function-is-not-your-friend/#naive-solution-using-delay) we came up with was too simple to be true:

* There's an extra state we need to take into account: the motor moving at a reduced speed before completely stopping.

* Not every state lasts the same amount of time.

Here's a diagram of how we want our DIY project to behave with time, according to our now estimations:

![Simple laser cutting machine behavior diagram](/assets/images/2020-09-30-wise-methods-simple/laser-cutting-simple.png)

## 'Wise' methods approach

In the previous diagram we can observe multiple different states, which we need to associate with different constants or behaviors.

An approach we can use in such cases is following a **'wise' methods pattern<sup>TM</sup>**, which simply consists of using methods who are fully responsible for acting according to a given state.

Some examples of those methods are, `calculate_waiting_time()`, `do_stuff()` and `print_message()`: their behaviors depend on the value of `current_state`. For instance, this is our implementation of `do_stuff()` using `switch` statements:

```c++
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
```

It's also worth mentioning that we handle the application state by defining `enum class State` and storing an instance of it as a global variable (`current_state`).

`next_state()` method takes care of calculating the next state in our cycle using [modulus operator](https://www.cprogramming.com/tutorial/modulus.html) and [`static_cast`](https://en.cppreference.com/w/cpp/language/static_cast). We can use the small trick of storing the number of `enum` elements as an element itself to keep it as close to the `enum` as possible, with the only purpose of trying to avoid forgetting to update it if we add or remove states.

```c++
enum class State
{
  MotorFast,
  MotorSlow,
  LaserOn,

  Total = 3
};

State current_state;

State next_state()
{
  return static_cast<State>((static_cast<int>(current_state) + 1) % (static_cast<int>(State::Total)));
}
```

Here is the complete, full implementation of our brand new laser cutting machine:

```c++
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
    current_state = next_state()

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
```

You can also find this code [here](https://github.com/eduherminio/eduherminio.github.io/tree/master/code/waiting-in-arduino/4_wise_methods_simple/4_wise_methods_simple.ino).

## Conclusion

As we already learned, the [the usage of `millis()`](https://eduherminio.github.io/blog/millis-to-the-rescue/) allows us to keep checking the emergency stop button very often.

The combination of that technique with this 'wise' methods pattern provides us with a clear separation of concerns, allowing us to forget about states when coding/reviewing the main logic flow of the application.

This helps us to have a very easy to follow logic in our `loop()`:

```c++
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
```

In the next entries of this series we'll have a look at more complex examples of this 'wise' methods pattern, as well as at other alternative patterns to use in combination with `millis()` that can be followed to achieve similar clean code.
