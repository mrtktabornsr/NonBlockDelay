#define NON_BLOCK_TEST
#include "NonBlockDelay.h"

void setup() {
    Serial.begin(115200);
    delay(1000);  // let Serial initialize
    NoStopDelay_Test(); // perform test
}


void loop() {
}