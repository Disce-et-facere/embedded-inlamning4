## uppgift-1-2-3-4_v1<br>
Commands:<br>
  PART 1:<br>
  led loop on - starts a loop with the red, green and blue led <br>
  led loop off - stops the loop with the red, green and blue led<br>
  enable buttons - enables all buttons<br>
  disable buttons - disables all buttons<br>
  control red led with pontentiometer - enables pwm control with potentiometer<br>
  control green led with pontentiometer - enables pwm control with potentiometer<br>
  control blue led with pontentiometer - enables pwm control with potentiometer<br>
  reset potentiometer pwm control - disables/resets the potentiometer pwm control<br>

  PART 2:<br>
  enable debouncer - enables debouncer for the buttons<br>
  disable debouncer - disables debouncer for the buttons<br>

  PART 3:<br>
  red led on - turns the red led on<br>
  red led off - turns the red led off<br>
  green led on - turns the green led on<br>
  green led off - turns the green led off<br>
  blue led on - turns the blue led on<br>
  blue led off - turns the blue led off<br>
  enable red button - enables the button for the red led<br>
  disable red button - disables the button for the red led<br>
  enable green button - enables the button for the green led<br>
  disable green button - disables the button for the green led<br>
  enable blue button - enables the button for the blue led<br>
  disable blue button - disables the button for the blue led<br>
  show voltage - starts displaying the voltage on pin A0 in the serial monitor<br>
  hide voltage - stops displaying the voltage on pin A0 in the serial monitor<br>
  enable potentiometer - this just sets the potentiometer pin to HIGH<br>
  disable potentiometer - this just sets the potentiometer pin to LOW<br>
  set red 0-255 - sets the pwm value for the red led<br>
  set green 0-255 - sets the pwm value for the green led<br>
  set blue 0-255 - sets the pwm value for the blue led<br>

  PART 4:<br>
  enable interupt - enables the interupt function<br>
  disable interupt - disables the interupt function<br>


## uppgift-1-2-3-4_v2
  Wokwi: https://wokwi.com/projects/392355488200340481 <br>

  The commands are the same as in v1. <br>
  <br>
  ### 1:st small update in v2
  when sending "control red/green/blue led with pontentiometer" the pontentiometer is started automatically.<br>
  In v1 you had to start it manually.<br>

  ### 2:nd mayor update in v2 (INTERUPTER)
  My goal was to use 3 buttons and 1 interupt pin.<br>
  
  In v1 the interuptor was set to CHANGE and the pin was set as INPUT_PULLUP and was connected to the same ground as all buttons.<br>
  it "worked"(sometimes) in a mysterious wierd glitchy way. It should'nt as it should hit low as soon as the microcontroller starts.<br>
  
  I was not satisfied with the former solution, so I found(googled) a new nifty way of doing it without the use of diods or anything else.<br>

  So how it works is:<br>

  First, Insted of connecting all buttons to ground we connect all of them to the interupt pin.<br>

  Second, Change interupt mode from CHANGE to FALLING.<br>

  ```
  attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), interuptHandler, FALLING);
  ```
  
  Third, create 2 diffrent pin modes for the button pins and interupt pin.<br>

  Normal/Default Mode <br>
  ```
  void normalPinMode(){

    pinMode(INTERUPT_PIN, OUTPUT);
    digitalWrite(INTERUPT_PIN, LOW);
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);

  }
  ```
  Interupt Mode<br>
  ```
  void interuptPinMode(){

    pinMode(INTERUPT_PIN, INPUT_PULLUP);
    pinMode(RED_BUTTON_PIN, OUTPUT);
    digitalWrite(RED_BUTTON_PIN, LOW);
    pinMode(GREEN_BUTTON_PIN, OUTPUT);
    digitalWrite(GREEN_BUTTON_PIN, LOW);
    pinMode(BLUE_BUTTON_PIN, OUTPUT);
    digitalWrite(BLUE_BUTTON_PIN, LOW);

  }
  ```
  In default, the interupt pin is set as OUTPUT in LOW state and the buttons are set as INPUT_PULLUP so we are using the interupt pin as "ground".<br>
  <br>
  Now when "enable interupt" is sent we switch the pin mode.<br>
  ```
  else if(input == "enable interupt"){

    Serial.println("\nINTERUPT ENABLED");
  
    enableInterupt = true;
    interuptPinMode();

  }
  ```
  In this state we are using all the button pins as "ground" and the interupt pin is now set to INPUT_PULLUP.<br>
  <br>
  So now we get a clean signal to the interupt pin which in turns calls the interuptHandler function.<br>

  ```
  void interuptHandler(){

    if (millis() - lastInteruptFired < 50) {
      return;
    }

    lastInteruptFired = millis();
  
    normalPinMode();
  
    ledBtnWithDebouncer();
    
    interuptPinMode();
  
    if(!enableInterupt){
      normalPinMode();
    }
  
  }
  ```

  When the InteruptHandler is called it changes the pin mode back to default/normal mode and now we can read which button was pressed.<br>
  And then it changes the pin mode back to interupt mode for the next interupt event.<br>

  When we want to disable the interupt we just simply turn the pin mode back to default/normal mode.<br>
  
