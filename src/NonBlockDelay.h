//#include <Arduino.h>

//#define NON_BLOCK_TEST

class NonBlockDelay
{
public:
    NonBlockDelay(unsigned long interval);
    void retrigger();
    void trigger() { retrigger(); }
    bool isIntervalComplete();
    void changeInterval(unsigned long interval);
    unsigned long remainingTime();
    bool isIntervalActive();
    void stop(); // { _active = false;  }
    
private:
  unsigned long _previousMillis;
  unsigned long _interval;
  bool _active;
};

// Constructor
NonBlockDelay::NonBlockDelay(unsigned long interval)
{
    _previousMillis = 0;
    _interval = interval;
    _active = false;
}

// Retrigger the interval timer and set active
void NonBlockDelay::retrigger()
{
    _previousMillis = millis();
    _active = true;
}

// Returns true if the interval is active
bool NonBlockDelay::isIntervalActive()
{
    return _active;
}

void NonBlockDelay::stop() 
{
   _active = false; // stop the current cycle, 
}

// Check whether the interval has completed
// returns false if not complete or is not active
// return true, and restarts the interval
//
// If the interval is not active it is triggered (becomes active)
//
bool NonBlockDelay::isIntervalComplete()
{
    bool result = false;
    if (isIntervalActive())
    {
        unsigned long currentMillis = millis();
        if (currentMillis - _previousMillis >= _interval) 
        {
            retrigger(); // re-start timer
            result = true;  // return value
        }        
    }
    else 
    {
        trigger(); // not active so trigger it
    }
    return result;
}

unsigned long NonBlockDelay::remainingTime()
{
    unsigned long currentMillis = millis();
    unsigned long usedTime = currentMillis - _previousMillis;
    return _interval - usedTime;
}


// Change the interval value and is active, retrigger
void NonBlockDelay::changeInterval(unsigned long interval)
{
    _interval = interval;
    if (isIntervalActive())
        retrigger();
      
}

#ifdef NON_BLOCK_TEST
void NonBlockDelay_Test()
{
    NonBlockDelay tmrTest(500);
    NonBlockDelay tmrTest2(1000);  
    uint16_t check = 0;  
    uint32_t tmStart = 0;
    uint32_t tmEnd = 0;

    Serial.println();
    Serial.println();
    Serial.println("NonBlockDelay testing");

    Serial.printf("tmrTest should not be active: %s\n", 
                    tmrTest.isIntervalActive() ?  "FAIL" : "SUCCESS");

    tmrTest.trigger();
    delay(10);
    unsigned long tmrTemp = tmrTest.remainingTime();

    Serial.printf("Start timer by trigger: %s\n", 
                    tmrTest.isIntervalActive() ?  "SUCCESS" : "FAIL" );

    Serial.printf("Start interval of 500ms and delay 10ms.  Should have 490ms, actual %d:  %s\n", 
                    tmrTemp,
                    (tmrTemp == 590 | tmrTemp == 491)  ?  "SUCCESS" : "FAIL" );
    Serial.printf("Time remaining: %dms\n", tmrTest.remainingTime());
    tmrTest.stop();
    Serial.printf("Stop timer with stop: %s\n", 
                    tmrTest.isIntervalActive() ?  "FAIL" : "SUCCESS");

    tmrTest.isIntervalComplete();
    Serial.printf("Start timer by isIntervalComplete: %s\n", 
                    tmrTest.isIntervalActive() ?  "SUCCESS" : "FAIL" );   
    check = 0;
    tmrTest.stop();
    tmrTest.trigger();
    while(1) {
        delay(1);
        if (tmrTest.isIntervalComplete())
            break;
        else
            check++ ;
       
    }
    Serial.printf("Test delay(1) against NonBlockDelay(500).\n");
    Serial.printf("Should have 500 delay checks. Actual= %d, %s\n", check,
                    (500 == check) ?  "SUCCESS" : "FAIL"   );
    
    Serial.printf("Change interval to 750.\n");
    check = 0;
    tmrTest.changeInterval(750);
    while(1) {
         delay(1);
        if (tmrTest.isIntervalComplete())
            break;
        else
            check++ ;       
    }    
    Serial.printf("Test delay(1) against NonBlockDelay(750).\n");
    Serial.printf("Should have 749 to 750 delay checks. Actual= %d, %s\n", check,
                    (750 == check) || 749 == check ?  "SUCCESS" : "FAIL"   );

    Serial.printf("Change interval to 100. Check 10 intervals\n");
    check = 0;
    tmrTest.changeInterval(100);
    tmStart = millis();
    while(1) {
        if (tmrTest.isIntervalComplete()) check++ ;
        if (check == 10) break;
    }
    tmEnd = millis();
    Serial.printf("Should have 1000 ms. Actual= %d, %s\n", (tmEnd - tmStart),
                (1000 == (tmEnd - tmStart)) || (999 == (tmEnd - tmStart)) ?  "SUCCESS" : "FAIL"   );
    tmrTest.stop();
    tmrTest2.stop();
    Serial.println("Start two timers");
    tmrTest2.changeInterval(10);
    tmrTest.changeInterval(500);
    tmrTest2.trigger();
    tmrTest.trigger();
    check = 0;
    tmStart = millis();
    while(1){
        if (tmrTest2.isIntervalComplete()) check++ ;
        if (check == 100) break;       
        if (tmrTest.isIntervalComplete())
            break;
    }
    tmEnd = millis();
    Serial.printf("Time: %dms\n", tmEnd-tmStart);
    Serial.printf("Should have 50 10ms intervals in 500ms interval. Actual= %d, %s\n", check,
                    (50 == check) || 49 == check ?  "SUCCESS" : "FAIL"   );

 
    

    while(1)
        delay(500);
}    

#endif