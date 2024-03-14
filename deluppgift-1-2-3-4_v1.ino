#define RED_LED_PIN 9
#define GREEN_LED_PIN 10
#define BLUE_LED_PIN 11
#define POTENTIOMETER_PIN 3 
#define ANALOG_PIN A0
#define RED_BUTTON_PIN 8
#define GREEN_BUTTON_PIN 12
#define BLUE_BUTTON_PIN 7
#define DEBOUNCE_DELAY 180 // button to breadboard connections is terrible!!!
#define INTERUPT_PIN 2

bool turnLedLoopOn = false;
bool isPotentPwmControlOn = false;
bool showVoltage = false;
bool enableLedButtons = false;
bool isRedLedOn = false;
bool isGreenLedOn = false;
bool isBlueLedOn = false;
bool isDebouncerOn = false;
bool isRedButtonEnabled = true;
bool isGreenButtonEnabled = true;
bool isBlueButtonEnabled = true;
bool enableInterupt = false;

unsigned long lastDebounceTimeRedBtn = 0;
unsigned long lastDebounceTimeGreenBtn = 0;
unsigned long lastDebounceTimeBlueBtn = 0;




void setup(){
  pinMode(RED_LED_PIN,OUTPUT);
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(BLUE_LED_PIN,OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(POTENTIOMETER_PIN,OUTPUT);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INTERUPT_PIN,INPUT);

  attachInterrupt(digitalPinToInterrupt(INTERUPT_PIN), interuptHandler, CHANGE);

  Serial.begin(9600);

  Serial.println("Type \"HELP\" for avaliable Commands!");
}


void displayCommands(){
  Serial.println("COMMANDS: ");

  Serial.println("\nPART 1:");
  Serial.println("LED LOOP ON");
  Serial.println("LED LOOP OFF");
  Serial.println("ENABLE BUTTONS");
  Serial.println("DISABLE BUTTONS");
   Serial.println("CONTROL (PIN) WITH POTENTIOMETER");

  Serial.println("\nPART 2:");
  Serial.println("ENABLE DEBOUNCER");
  Serial.println("DISABLE DEBOUNCER");

  Serial.println("\nPART 3:");
  Serial.println("RED LED ON");
  Serial.println("RED LED OFF");
  Serial.println("GREEN LED ON");
  Serial.println("GREEN LED OFF");
  Serial.println("BLUE LED ON");
  Serial.println("BLUE LED OFF");
  Serial.println("ENABLE RED BUTTON");
  Serial.println("DISABLE RED BUTTON");
  Serial.println("ENABLE GREEN BUTTON");
  Serial.println("DISABLE GREEN BUTTON");
  Serial.println("ENABLE BLUE BUTTON");
  Serial.println("DISABLE BLUE BUTTON");
  Serial.println("SHOW VOLTAGE");
  Serial.println("HIDE VOLTAGE");
  Serial.println("ENABLE POTENTIOMETER");
  Serial.println("DISABLE POTENTIOMETER");
  Serial.println("set red 0-255");
  Serial.println("set green 0-255");
  Serial.println("set blue 0-255");

  Serial.println("\nPART 4:");
  Serial.println("ENABLE INTERUPT");
  Serial.println("DISABLE INTERUPT");

}

void ledLoop(){

  digitalWrite(RED_LED_PIN,HIGH);
  delay(200);
  digitalWrite(GREEN_LED_PIN,HIGH);
  delay(200);
  digitalWrite(BLUE_LED_PIN,HIGH);
  delay(200);
  digitalWrite(RED_LED_PIN,LOW);
  delay(200);
  digitalWrite(GREEN_LED_PIN,LOW);
  delay(200);
  digitalWrite(BLUE_LED_PIN,LOW);
  delay(200);

}

void controlPwmWithPotentiometer(int pin){

  int numericalValue = analogRead(ANALOG_PIN);

  int pwm = map(numericalValue, 0, 1023, 0, 255);

  analogWrite(pin, pwm);

}

void setPinPWM(int pin, int power){

  analogWrite(pin,power);

}

void showAnalogVoltage(){

  int numericalValue = analogRead(ANALOG_PIN);
  float voltage = numericalValue * (5.0 / 1023.0);

  Serial.print("Numerical Value: ");
  Serial.print(numericalValue);
  Serial.println("\n");
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("\n");
  delay(300);

}

void controlPotentiometer(int onOff){

  if(onOff == 1){

    digitalWrite(POTENTIOMETER_PIN,HIGH);

  }else if(onOff == 0){

    digitalWrite(POTENTIOMETER_PIN,LOW);

  }


}

void interuptHandler(){

  if(enableInterupt){

    isDebouncerOn = true;
    ledBtnWithDebouncer();

  }else{
    return;
  }

}

//without debouncer for demostrational purpose
void ledBtnWithoutDebouncer(){

  int redBtnValue;
  int greenBtnValue;
  int blueBtnValue;

  if(isRedButtonEnabled){

    int tempRedBtnValue = digitalRead(RED_BUTTON_PIN);
    redBtnValue = tempRedBtnValue;
  
  }

  if(isGreenButtonEnabled){

    int tempGreenBtnValue = digitalRead(GREEN_BUTTON_PIN);
    greenBtnValue = tempGreenBtnValue;

  }
  
  if(isBlueButtonEnabled){

    int tempBlueBtnValue = digitalRead(BLUE_BUTTON_PIN);
    blueBtnValue = tempBlueBtnValue;
  
  }
  

  if(redBtnValue == LOW){
    Serial.println("RED BTN IS PRESSED!");
    if(isRedLedOn){
      digitalWrite(RED_LED_PIN, LOW);
      isRedLedOn = false;
    }else if(!isRedLedOn){
      digitalWrite(RED_LED_PIN, HIGH);
      isRedLedOn = true;
    }

  }else if(greenBtnValue == LOW){
    Serial.println("GREEN BTN IS PRESSED!");
    if(isGreenLedOn){
      digitalWrite(GREEN_LED_PIN, LOW);
      isGreenLedOn = false;
    }else if(!isGreenLedOn){
      digitalWrite(GREEN_LED_PIN, HIGH);
      isGreenLedOn = true;
    }

  }else if(blueBtnValue == LOW){
    Serial.println("BLUE BTN IS PRESSED!");
    if(isBlueLedOn){
      digitalWrite(BLUE_LED_PIN, LOW);
      isBlueLedOn = false;
    }else if(!isBlueLedOn){
      digitalWrite(BLUE_LED_PIN, HIGH);
      isBlueLedOn = true;
    }

  }

}

void ledBtnWithDebouncer(){
  
  unsigned long currentMillis = millis();

  if(isRedButtonEnabled && currentMillis - lastDebounceTimeRedBtn >= DEBOUNCE_DELAY){

    int redBtnValue = digitalRead(RED_BUTTON_PIN);

    if(redBtnValue == LOW){
      Serial.println("RED BTN IS PRESSED!");
      if(isRedLedOn){
          digitalWrite(RED_LED_PIN, LOW);
          isRedLedOn = false;
        }else{
          digitalWrite(RED_LED_PIN, HIGH);
          isRedLedOn = true;
        }

    }
  
    lastDebounceTimeRedBtn = currentMillis;

  }
  
  if(isGreenButtonEnabled && currentMillis - lastDebounceTimeGreenBtn >= DEBOUNCE_DELAY){

    int greenBtnValue = digitalRead(GREEN_BUTTON_PIN);

    if(greenBtnValue == LOW){
      Serial.println("GREEN BTN IS PRESSED!");
      if(isGreenLedOn){
        digitalWrite(GREEN_LED_PIN, LOW);
        isGreenLedOn = false;
      }else{
        digitalWrite(GREEN_LED_PIN, HIGH);
        isGreenLedOn = true;
      }

    }

    lastDebounceTimeGreenBtn = currentMillis;

   

  }
  
  if(isBlueButtonEnabled && currentMillis - lastDebounceTimeBlueBtn >= DEBOUNCE_DELAY){

    int blueBtnValue = digitalRead(BLUE_BUTTON_PIN);

    if(blueBtnValue == LOW){
      Serial.println("BLUE BTN IS PRESSED!");
      if(isBlueLedOn){
        digitalWrite(BLUE_LED_PIN, LOW);
        isBlueLedOn = false;
      }else{
        digitalWrite(BLUE_LED_PIN, HIGH);
        isBlueLedOn = true;
      }

    }

    lastDebounceTimeBlueBtn = currentMillis;
    
  }

}

void controlLeds(int color, int onOff){

  if(color == 1 && onOff == 1){

    digitalWrite(RED_LED_PIN, HIGH);

  }else if(color == 1 && onOff == 0){

    digitalWrite(RED_LED_PIN, LOW);

  }else if(color == 2 && onOff == 1){

    digitalWrite(GREEN_LED_PIN, HIGH);

  }else if(color == 2 && onOff == 0){

    digitalWrite(GREEN_LED_PIN, LOW);

  }else if(color == 3 && onOff == 1){

    digitalWrite(BLUE_LED_PIN, HIGH);

  }else if(color == 3 && onOff == 0){

    digitalWrite(BLUE_LED_PIN, LOW);

  }

}

void loop(){

  if(Serial.available() > 0) {

    String input = Serial.readStringUntil('\n');

    if(input == "help"){

      displayCommands();
    
    }else if(input == "led loop on"){
      
      Serial.println("\nLED LOOP ON");
      turnLedLoopOn = true;

    }else if(input == "led loop off"){
      
      Serial.println("\nLED LOOP OFF");
      turnLedLoopOn = false;

    }else if(input == "control red led with potentiometer"){
      
      isRedLedOn = true;
      isPotentPwmControlOn = true;

    }else if(input == "control green led with potentiometer"){
      
      isGreenLedOn = true;
      isPotentPwmControlOn = true;

    }else if(input == "control blue led with potentiometer"){
      
      isBlueLedOn = true;
      isPotentPwmControlOn = true;

    }else if(input == "reset potentiometer pwm control"){
      
      isRedLedOn = false;
      isGreenLedOn = false;
      isBlueLedOn = false;
      
      isPotentPwmControlOn = false;

    }else if(input == "show voltage"){

      Serial.println("\nSHOW VOLTAGE ON");
      showVoltage = true;

    }else if(input == "hide voltage"){

      Serial.println("\nSHOW VOLTAGE OFF");
      showVoltage = false;

    }else if(input == "enable potentiometer"){

      Serial.println("\nPOTENTIOMETER ON");
      controlPotentiometer(1);

    }else if(input == "disable potentiometer"){

      Serial.println("\nPOTENTIOMETER OFF");
      controlPotentiometer(0);

    }else if(input.startsWith("set ")){


      if(input.substring(4,8) == "red "){

        String numericalValueString = input.substring(8);
        
        int numericalValue = numericalValueString.toInt();

        if (numericalValue >= 0 && numericalValue <= 255) {
          
          setPinPWM(RED_LED_PIN, numericalValue);

          } else {
          
            Serial.println("invalid power value!");
          }
        
        }else if(input.substring(4,10) == "green "){

          String numericalValueString2 = input.substring(10);
          
          int numericalValue2 = numericalValueString2.toInt();

          if (numericalValue2 >= 0 && numericalValue2 <= 255) {
            
            controlPotentiometer(0);
            setPinPWM(GREEN_LED_PIN, numericalValue2);

          } else {
          
            Serial.println("invalid power value!");
          }

        }else if(input.substring(4,9) == "blue "){

          String numericalValueString3 = input.substring(9);
          
          int numericalValue3 = numericalValueString3.toInt();


          if (numericalValue3 >= 0 && numericalValue3 <= 255) {
            
            setPinPWM(BLUE_LED_PIN, numericalValue3);

          } else {
          
            Serial.println("invalid power value!");
          }
        
        }else {
          Serial.println("Invalid choice!");
        }

    }else if(input == "enable buttons"){

      Serial.println("\nBUTTONS ENABLED");
      enableLedButtons = true;

    }else if(input == "disable buttons"){

      Serial.println("\nBUTTONS DISABLED");
      enableLedButtons = false;

    }else if(input == "enable debouncer"){

      Serial.println("\nDEBOUNCER ENABLED");
      isDebouncerOn = true;

    }else if(input == "disable debouncer"){

      Serial.println("\nDEBOUNCER DISABLED");
      isDebouncerOn = false;

    }else if(input == "enable red button"){

      Serial.println("\nRED BUTTON ENABLED");
      isRedButtonEnabled = true;

    }else if(input == "disable red button"){

      Serial.println("\nRED BUTTON DISABLED");
      isRedButtonEnabled = false;

    }else if(input == "enable green button"){

      Serial.println("\nGREEN BUTTON ENABLED");
      isGreenButtonEnabled = true;

    }else if(input == "disable green button"){

      Serial.println("\nGREEN BUTTON DISABLED");
      isGreenButtonEnabled = false;

    }else if(input == "enable blue button"){

      Serial.println("\nBLUE BUTTON ENABLED");
      isBlueButtonEnabled = true;

    }else if(input == "disable blue button"){

      Serial.println("\nBLUE BUTTON DISABLED");
      isBlueButtonEnabled = false;

    }else if(input == "red led on"){

      Serial.println("\nRED LED ON");
      controlLeds(1,1);

    }else if(input == "red led off"){

      Serial.println("\nRED LED OFF");
      controlLeds(1,0);

    }else if(input == "green led on"){

      Serial.println("\nGREEN LED ON");
      controlLeds(2,1);

    }else if(input == "green led off"){

      Serial.println("\nGREEN LED OFF");
      controlLeds(2,0);

    }else if(input == "blue led on"){

      Serial.println("\nBLUE LED ON");
      controlLeds(3,1);

    }else if(input == "blue led off"){

      Serial.println("\nBLUE LED OFF");
      controlLeds(3,0);

    }else if(input == "enable interupt"){

      Serial.println("\nINTERUPT ENABLED");
      enableInterupt = true;

    }else if(input == "disable interupt"){

      Serial.println("\nINTERUPT DISABLED");
      enableInterupt = false;
      isDebouncerOn = false;// add to interupt handler.
    
    }else{

      Serial.println("Invalid input!");
    
    }

  }

  if(turnLedLoopOn){
    
    ledLoop();

  }

  if(isPotentPwmControlOn){

    if(isRedLedOn){

      controlPwmWithPotentiometer(RED_LED_PIN);

    }else if(isGreenLedOn){

      controlPwmWithPotentiometer(GREEN_LED_PIN);

    }else if(isBlueLedOn){

      controlPwmWithPotentiometer(BLUE_LED_PIN);

    }

  }
  
  if(showVoltage){

    showAnalogVoltage();

  }

  if(enableLedButtons){

    if(isDebouncerOn){

      ledBtnWithDebouncer();
    
    }else{
    
      ledBtnWithoutDebouncer();

    }

  }

}
