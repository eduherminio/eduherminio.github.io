---
title: "Waiting in Arduino II: millis() function to the rescue"
date: 2020-09-07T00:00:00+00:00
categories:
  - blog
tags:
  - arduino
  - cpp
  - DIY
series: "Waiting in Arduino"
---

{% include series.html.liquid content=" " %}

## Introduction: Blinking a LED without `delay()`

In the first post we linked to an [official Arduino tutorial](https://www.arduino.cc/en/tutorial/BlinkWithoutDelay) to quote why the `delay()` pattern wasn't mean to be used for waiting between tasks. If you fully read it back then, it probably spoiled you the alternative we're going to present in this post: using [`millis()` function](https://www.arduino.cc/reference/en/language/functions/time/millis/) instead.

`millis()` "returns the number of milliseconds passed since the Arduino board began running the current program", so it cannot just replace `delay()` (otherwise they would just be the same method!).

Here's the pattern that can be used to replace `delay()` with `millis()` (which you can compare with the equivalent code [using delay](https://eduherminio.github.io/blog/delay_function_is_not_your_friend/#introduction-blinking-a-led)).

```arduino
const int waitingMs = 2000;   // 2 seconds
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() { pinMode(LED_BUILTIN, OUTPUT); }

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= waitingMs)
  {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
```

You can also find this code [here](https://github.com/eduherminio/eduherminio.github.io/tree/master/code/waiting-in-arduino/2_blink_led_without_delay/2_blink_led_without_delay.ino).

- We create a global variable to store a certain amount of milliseconds (`previousMillis`), which we initialize to 0.
- We create a global variable to store the current state of the LED (`ledState`).
- In each loop iteration we invoke `millis()` to get the number of current milliseconds and save its value to a local variable named `currentMillis`.
- We compare the difference between the current number of milliseconds (`currentMillis`) with the number of milliseconds we have saved (`previousMillis`) to see if it is bigger than the time we wanted to wait (`waitingMs`):
  - If it is, we update the global variable `previousMillis` with the current number of milliseconds (`currentMillis`) and perform whatever operation we were planning to (in this case, change the state of a LED: update the value of `ledState` variable and apply it using `digitalWrite` method).
  - Otherwise, just continue the execution of the sketch.

## Going back to our case study

Let's get back to our [case study](https://eduherminio.github.io/blog/delay_function_is_not_your_friend/#case-study): we want to build a simple the laser cutting machine.

We identified a [first solution](https://eduherminio.github.io/blog/delay_function_is_not_your_friend/#naive-solution-using-delay) based on a fairly right state machine, but it had a main flaw: it wouldn't be able to identify immediately (or at all) when the big, round, red emergency stop button was pressed.

The main issue of our first implementation was using [`delay()` method](https://www.arduino.cc/reference/en/language/functions/time/delay/), which essentially doesn't allow us to check the emergency button as often as we would like to.

## A solution using `millis()`

Let's apply the pattern we just learned to our code:

```arduino
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
```

You can also find this code [here](https://github.com/eduherminio/eduherminio.github.io/tree/master/code/waiting-in-arduino/3_basic_millis_implementation/3_basic_millis_implementation.ino).

The main functional difference between this implementation and the [`delay()` one](http://localhost:8080/blog/delay_function_is_not_your_friend/#naive-solution-using-delay) is that here each loop iteration lasts only few milliseconds (vs more than 6 seconds in the other case).

Arduino performs many more operations while running this code, but after all, that's what we have it for, right? We don't pay any extra 'price'\* for it.

In fact, it's just the opposite: for us (makers/programmers), **Arduino calculating the value of `millis()` and comparing it against the content of a saved variable every few milliseconds is way more efficient than Arduino waiting in the `delay()` method**, because the former one allows us to perform any other operation while 'waiting'.

In general terms, this `millis()` pattern is the approach that one should adopt if they want to level up their Arduino coding and/or use it for real life projects.

There will more entries in this series exploring more complex ways of tackling this problem (so stay tuned!), but all of them rely on the mentioned `millis()` pattern, so it's vital to assimilate that one first.

---

\* As a mere anecdote, here's the result of compiling both sketches (using [arduino-cli](https://github.com/arduino/arduino-cli)):

```shell
*       Compiling 1_delay_implementation
Sketch uses 3664 bytes (11%) of program storage space. Maximum is 32256 bytes.
Global variables use 302 bytes (14%) of dynamic memory, leaving 1746 bytes for local variables. Maximum is 2048 bytes.
*       Compiling 3_basic_millis_implementation
Sketch uses 3588 bytes (11%) of program storage space. Maximum is 32256 bytes.
Global variables use 282 bytes (13%) of dynamic memory, leaving 1766 bytes for local variables. Maximum is 2048 bytes.
```

Using millis is slightly more efficient, storage and memory wise! Of course, it executes many more operations per second when running, but that's a price we happily pay in these kind of devices that are based on executing the same code over and over again, providing those operations take very short time!
