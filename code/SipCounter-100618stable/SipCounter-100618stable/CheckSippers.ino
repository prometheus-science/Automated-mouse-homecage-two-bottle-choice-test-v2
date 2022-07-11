/********************************************************
  Check Sippers
********************************************************/
void  CheckSippers() {
  ReadBatteryLevel();  //Read battery level
  logtimerfunction ();  // Is it time to log data? (Edit variable "logfreq" in a_header to change this timing)
  
  if (PongActive == false) {
    digitalWrite (outLeft, LOW); // hold both outputs LOW
    digitalWrite (outRight, LOW);
    
    // Monitor Left Sip
    while (digitalRead (LeftSip) == LOW) {  // if left sip beam is broken
      digitalWrite (outLeft, HIGH); //Left output HIGH
      LeftDuration += 0.000007;  //calibrated with stopwatch, it was more accurate than using millis() to time this
      if (digitalRead (LeftSip) == HIGH) {
        LeftCount++;
      }
    }

    // Monitor Right Sip
    while (digitalRead (RightSip) == LOW) {
      digitalWrite (outRight, HIGH);  // Right output HIGH
      RightDuration += 0.000007;
      if (digitalRead (RightSip) == HIGH) {
        RightCount++;
      }
    }

    // Calculate time elapsed
    calcTimeElapsed();

    // Show time elapsed when C is pressed
    while (digitalRead (buttonC) == LOW) {
      DisplayTimeElapsed();
    }
  }
}
