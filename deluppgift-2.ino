#define PWM_PIN 9
#define ANALOG_PIN A0

bool showVoltage = false;
bool correctVoltage = false;
int mV;
int truemV;
int pwmValue;
int preferedVoltage;

void setup(){

pinMode(PWM_PIN,OUTPUT);
pinMode(ANALOG_PIN,INPUT);


Serial.begin(9600);

}

void showAnalogVoltage(){

 int numericalValue = analogRead(ANALOG_PIN);
  float voltage = numericalValue * (5.0 / 1023.0);
  truemV = voltage * 1000;
  mV = truemV * 0.9;
  Serial.print("prefered Voltage: ");
  Serial.print(preferedVoltage);
  Serial.println("\n");
  Serial.print("mV: ");
  Serial.print(mV);
  Serial.println("\n");
  delay(1000);

}

void setPinPWM(int pin, int voltage){

  pwmValue = map(voltage, 0, 5000, 0, 255);
  analogWrite(pin,pwmValue);

}
void voltageCorrection(){

    int minAllowedVoltageDiff = preferedVoltage - 20;

    if(mV < minAllowedVoltageDiff){
      Serial.println("working on it!");

      pwmValue++;
      analogWrite(PWM_PIN, pwmValue);

    }else if(mV >= minAllowedVoltageDiff){

      Serial.println("DONE!");
      correctVoltage = false;
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

        if (preferedVoltage >= 0 && preferedVoltage <= 5000) {
    
          setPinPWM(PWM_PIN, preferedVoltage);

          } else {
          
            Serial.println("invalid power value!");
          }

    }else if(input == "start correcting voltage"){
      
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