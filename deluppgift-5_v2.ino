#define PWM_PIN 9
#define ANALOG_PIN A0

bool showVoltage = false;
bool correctVoltage = false;

int mV;
int preferredVoltage;

void setup(){

pinMode(PWM_PIN,OUTPUT);
pinMode(ANALOG_PIN,INPUT);


Serial.begin(9600);

}


void showAnalogVoltage(){

  pwmOnPin = analogRead(ANALOG_PIN);
  int voltage = pwmOnPin * (5.0 / 1023.0);
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

  float randomVoltage;

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

    if(pwmToChange < 0){

      int absolutePWMtoChange = abs(pwmToChange);

      newPWM = currentPWM - absolutePWM;

    }else if(pwmToChange > 0){

      newPWM = currentPWM + pwmToChange;

    }

    int newVoltage = map(newPWM, 0, 255, 0, 5000);

    if(newVoltage > minAllowedVoltageDiff && newVoltage < maxAllowedVoltageDiff){

      analogWrite(PWM_PIN, pwmToChange);

    }else{

      Serial.println("Correction Error!");

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
        
        preferedVoltage = voltageString.toInt();

        if (preferedVoltage >= 0 && preferedVoltage <= 4500) {
    
          setPinPWM(PWM_PIN, preferedVoltage);

          } else {
          
            Serial.println("invalid power value!");
          }

    }else if(input == "correct voltage"){
      
      correctVoltage = true;

    }else{
      Serial.println("Invalid input!");
    }
  }  

  if(showVoltage){

    showAnalogVoltage();

  }

  if(correctVoltage){

    voltageCorrection();

  }
}
