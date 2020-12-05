---
title: "Waiting in Arduino I: delay() function is not your friend"
excerpt: "Why delay() function isn't the recommended approach for waiting in Arduino"
date: 2020-09-06T00:00:00+00:00
categories:
  - blog
tags:
  - arduino
  - cpp
  - DIY
series: "Waiting in Arduino"
---

{% include series.html.liquid content=" " %}

## Introduction: Blinking a LED

Displaying ['Hello, world!'](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) is usually the first thing you learn (or get taught) when you're getting familiar with a new programming language.

An exception to that statement shows up when you start programming devices that aren't necessarily designed to directly print information on a screen, such as microcontrollers. That's why, **in the case of Arduino, displaying 'Hello, world!' is effectively replaced with blinking a LED**.

This is typically the first Arduino program (or [sketch](https://www.arduino.cc/en/tutorial/sketch)) that Arduino enthusiasts write (simpler code can be found in [this official tutorial](https://www.arduino.cc/en/Tutorial/Blink)):

```c++
const int waitingTime = 2000;   // 2 seconds
bool ledState = LOW;

void setup() { pinMode(LED_BUILTIN, OUTPUT); }

void loop()
{
  digitalWrite(LED_BUILTIN, ledState);
  delay(waitingTime);

  ledState = !ledState;
}
```

You can also find this code [here](https://github.com/eduherminio/eduherminio.github.io/tree/master/code/waiting-in-arduino/0_blink_led/0_blink_led.ino).

For those readers that are not well versed in Arduino (and that haven't already closed this tab), `setup()` method is executed once (at the beginning of the program, i.e., when the Arduino board is powered on or restarted) and the `loop()` method is executed over and over again.

Which means that an Arduino board with this program loaded would execute the following instructions indefinitely:

1. Configures the pin where the LED is connected as output.
2. Turns the LED on or off according to the value of `ledState` (initially off).
3. Waits two seconds.
4. Updates the value of `ledState`.
5. Goes back to step 2.

## Understanding why using `delay()` is not always a good idea

So what's all this wording about? Is there something wrong the the blinking LED program shown above? No, absolutely no (and whoever tells you the opposite is a liar 😛). As in any other programming language, each Arduino program/sketch has a very concrete purpose, which in this case is serving as 'Hello, world!' or welcoming sample for beginners.

That example uses the [`delay()` function](https://www.arduino.cc/reference/en/language/functions/time/delay/) in order to wait for things to happen for the sake of simplicity, but that's in general not a good pattern to follow.

The main reason for such a hard statement is that while your Arduino board is stuck performing a `delay()`, it cannot do (almost) anything else. In [this tutorial in Arduino official page](https://www.arduino.cc/en/tutorial/BlinkWithoutDelay) that is explained with the following example:

> Sometimes you need to do two things at once. For example you might want to blink an LED while reading a button press. In this case, you can't use `delay()`, because Arduino pauses your program during the `delay()`. If the button is pressed while Arduino is paused waiting for the `delay()` to pass, your program will miss the button press.
>
> An analogy would be warming up a pizza in your microwave, and also waiting some important email. You put the pizza in the microwave and set it for 10 minutes. The analogy to using `delay()` would be to sit in front of the microwave watching the timer count down from 10 minutes until the timer reaches zero. If the important email arrives during this time you will miss it.
>
> What you would do in real life would be to turn on the pizza, and then check your email, and then maybe do something else (that doesn't take too long!) and every so often you will come back to the microwave to see if the timer has reached zero, indicating that your pizza is done.

Another practical example can be:

You have two LEDs, a red one and a green one. You want your red LED to blink every 2 seconds, and your green one every 7 seconds.

If you use `delay()` method for your green LED, how is the red LED supposed to blink every two seconds if Arduino just waits doing almost literally nothing for seven seconds?

In short, **`delay()` function in Arduino is (generally) not your friend.**

## Case study

Let's imagine that we are building a laser cutting machine as our side project and our aim is to control it using an Arduino board.

We want to be able to:

- Move pieces towards a laser using a DC motor that powers a conveyor belt. Stop the motor when the pieces are in front the laser.
- Turn the laser on when the pieces are in the right position. Turn it off when the operation is complete.
- Be able to stop the machine using a big, red 'Emergency stop' button if needed.

![Conveyor belt DIY project](/assets/images/2020-09-06-delay-function-is-not-your-friend/conveyor_belt.jpg)
Conveyor belt DIY project ([source](https://www.instructables.com/id/MODULAR-PORTABLE-CONVEYOR-BELT-SPEED-CONTROL-BY-AR/))

Let's assume for now that it takes three seconds to move a piece from the beginning of the conveyor belt to the laser position and another three seconds to cut the piece using the laser.

_All the code that appears in this series is simplified so that it can be run without any actuators, just connecting your Arduino to your laptop, loading the sketch and looking at the serial monitor output. Pressing the emergency button can be simulated by sending anything through the serial port using Arduino's serial monitor window._

![Simulating an emergency stop](/assets/images/2020-09-06-delay-function-is-not-your-friend/stop_button.png)

## Naive solution: using `delay()`

After analyzing our goal, one can conclude that it can be achieved by creating a [state machine](https://en.wikipedia.org/wiki/Finite-state_machine) with three different states:

- State 1: Motor on, laser off. The piece is moving through the conveyor belt.
- State 2: Motor off, laser on. The laser is cutting the piece.
- State 3: Motor off, laser off. Emergency mode (something went wrong), manual intervention is required.

![State machine](/assets/images/2020-09-06-delay-function-is-not-your-friend/state_machine.png)

This first implementation uses `delay()` for handling the amount of time between states 1 and 2.

```c++
const unsigned long waiting_time_ms = 3000;   // 3s

void setup() { Serial.begin(9600); }

void loop()
{
  turn_laser_off();
  turn_motor_on();
  delay(waiting_time_ms);

  stop_in_case_of_emergency();  // This is checked every three seconds

  turn_motor_off();
  turn_laser_on();
  delay(waiting_time_ms);

  stop_in_case_of_emergency();  // This is checked every three seconds
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
```

You can also find this code [here](https://github.com/eduherminio/eduherminio.github.io/tree/master/code/waiting-in-arduino/1_delay_implementation/1_delay_implementation.ino).

The main issue with this piece of code is that when the microcontroller executes our `delay(3000)`, it pauses the program for 3 seconds; therefore `stop_in_case_of_emergency` method is only invoked every 3 seconds, which implies:

- If someone kept the emergency button pressed, our machine wouldn't immediately stop: it could take it up to 3 seconds to do it.
- If someone just pressed the emergency button and released it, it could be easily missed by our program if that didn't happen just before the emergency check.

Jump straight to [Waiting in Arduino II: millis() function to the rescue](https://eduherminio.github.io/blog/millis-to-the-rescue/) to analyze a better way of coding this.
