#define PWM_PIN 9
#define ANALOG_PIN A0

bool showVoltage = false;

int mV;
int preferredVoltage;

void setup(){

pinMode(PWM_PIN,OUTPUT);
pinMode(ANALOG_PIN,INPUT);


Serial.begin(9600);

}


void showAnalogVoltage(){

  int pwmOnPin = analogRead(ANALOG_PIN);
  float voltage = pwmOnPin * (5.0 / 1023.0);
  mV = voltage * 1000;
  Serial.print("preferred Voltage: ");
  Serial.print(preferredVoltage);
  Serial.println("\n");
  Serial.print("mV: ");
  Serial.print(mV);
  Serial.println("\n");
  delay(1000);

}

int randomizeVoltage(int milliVolt){

  randomSeed(millis());

  int randomNumber = random(2);

  int randomVoltage;

  if(randomNumber == 1){

    randomVoltage = milliVolt * 1.1;

  }else if(randomNumber == 0){

    randomVoltage = milliVolt * 0.9;
  }

  return randomVoltage;


}

void setPinPWM(int pin, int voltage){

  int changedVoltage = randomizeVoltage(voltage);

  int pwmToPin = map(changedVoltage, 0, 5000, 0, 255);

  analogWrite(pin,pwmToPin);

}

void voltageCorrection(){

    int minAllowedVoltageDiff = preferredVoltage - 20;
    int maxAllowedVoltageDiff = preferredVoltage + 20;

    int newPWM;

    int currentPWM = map(mV, 0, 5000, 0, 255);

    int preferredPWM = map(preferredVoltage, 0, 5000, 0, 255);

    int pwmDiff = preferredPWM - currentPWM;

    if(pwmDiff < 0){

      int absolutePWMDiff = abs(pwmDiff);

      newPWM = currentPWM - absolutePWMDiff;

    }else if(pwmDiff > 0){

      newPWM = currentPWM + pwmDiff;

    }

    int newVoltage = map(newPWM, 0, 255, 0, 5000);

    if(newVoltage > minAllowedVoltageDiff && newVoltage < maxAllowedVoltageDiff){

      analogWrite(PWM_PIN, newPWM);
      Serial.println("Voltage Corrected!");

    }else{

      Serial.println("Correction Error, Voltage not corrected!");

    }

}

void loop(){


  if(Serial.available() > 0) {

    String input = Serial.readStringUntil('\n');

    if(input == "show voltage"){
      
      showVoltage = true;

    }else if(input == "hide voltage"){

      showVoltage = false;

    }else if(input.startsWith("voltage ")){

        String voltageString = input.substring(8);
        
        preferredVoltage = voltageString.toInt();

        if (preferredVoltage >= 0 && preferredVoltage <= 4500) {
    
          setPinPWM(PWM_PIN, preferredVoltage);

          } else {
          
            Serial.println("invalid Voltage value! 0 - 4500 mV");
          }

    }else if(input == "correct voltage"){
      
      voltageCorrection();

    }else{
      Serial.println("Invalid input!");
    }
  }  

  if(showVoltage){

    showAnalogVoltage();

  }
}
