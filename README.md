# Library NonBlockDelay

This NonBlockDelay Library implements a delay method that allows (Cooperative) multitasking for multiple devices and modules.

## Instructions for use:
1. Add the header to your file
   #include "NonBlockDelay.h" 
2. Instantiate NonBlockDelay element(s)
  NonBlockDelay nbDelay1(1000);
3. In your code Loop function check the NonBlockDelay element and execute you code when true
  if (nbDelay1.isIntervalComplete()) { 
      ....

### Constructor
```c++
NonBlockDelay(unsigned long interval);
```

```
This is the Arduino Blink example code.
============================================================================
/*
  https://docs.arduino.cc/built-in-examples/basics/Blink/
  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
```
This is the Blink example modified to use NonBlockDelay.
============================================================================

```
//This is the built-in-examples/basics/Blink modified to 
// use perform the delay with no delays 

// the setup function runs once when you press reset or power the board
#include "NonBlockDelay.h"    // ++++ added: no block delay header
NonBlockDelay nbDelay1(1000); // ++++ added: initialize a 1000ms delay, but will not start

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
static bool ledOn = true; // ++++ added: track state of the LED
  if (nbDelay1.isIntervalComplete()) { // ++++ added: to see if the task should be run i.e. timer time-out.
    if (ledOn)
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      // ++++ remove:remove: delay
    else
      // ++++ remove: remove delay  
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
     ledOn = !ledOn; // ++++ added: change the LED state 
  }
}
```

NonBlockDelay with two Blink tasks
============================================================================
```
// This is the built-in-examples/basics/Blink modified to 
// execute two tasks without delays

#include "NonBlockDelay.h"
NonBlockDelay nbDelay1(1000);
NonBlockDelay nbDelay2(337);

#define LED_SPARE 4 // add led and resistor for second task

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_SPARE, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  static bool ledOn = true; // track state of the LED
  if (nbDelay1.isIntervalComplete()) {
    if (ledOn)
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    else                     // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    ledOn = !ledOn;
  }
  // task 2: Notice LED_SPARE is blinking faster and not in sync with the LED_BUILTIN  
  if (nbDelay2.isIntervalComplete()) {
    digitalWrite(LED_SPARE, !digitalRead(LED_SPARE));
  }
}
```
============================================================================

