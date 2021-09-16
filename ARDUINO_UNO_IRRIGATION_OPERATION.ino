


//MD MAHBUB ALI

#include <Wire.h>


#include<dht.h>
dht DHT;
int airquality = 0;
int hydrogen = 0;
const int relay = D6;
int moistureStatus = 0;
const int moisture1 = D2;
int ldrPercentage = 0;
int airpin =D5;
int H2pin=D4;
int ldrValue = 0;
int ldr = A0;
#define DHT11_PIN D0

void setup() {
  

  pinMode(airpin, INPUT);
  pinMode(H2pin, INPUT);



  pinMode(relay, OUTPUT);

  pinMode(moisture1, INPUT);



  Serial.begin(115200);

  //Serial.println("welcome to TechPonder Humidity and temperature Detector");
}







void hum()
{
  int chk = DHT.read11(DHT11_PIN);

  Serial.println(" Humidity " );

  Serial.println(DHT.humidity, 1);

  Serial.println(" Temparature ");

  Serial.println(DHT.temperature, 1);
  
  delay(500);
  


  delay(500);



}






void gas()
{
  int sensorValue1 = analogRead(airpin);
  int sensor1Value = analogRead(H2pin);

  Serial.print("Air Quality = ");
int sensorValue=map(sensorValue,0,1000,0,90);
  Serial.print(sensorValue);

  Serial.print("*PPM");

  Serial.println();

  //delay(1000);
  Serial.print("H2 Quality = ");

  Serial.print(sensor1Value);

  Serial.print("*PPM");

  Serial.println();
  

  delay(500);
  


  delay(500);
  


}

void moisturew()
{
  moistureStatus = analogRead(moisture1);

  if (moistureStatus <= 290) {

    digitalWrite(relay, HIGH);

    Serial.print("moisture low motor On ");

    Serial.println(moistureStatus);

  }
  else {

    digitalWrite(relay, LOW);

    Serial.print("motor off : ");

    Serial.println(moistureStatus);
  }
  

  delay(500);
  

}


void density()
{
    ldrValue = analogRead(ldr);

  Serial.println(ldrValue);
  ldrPercentage = (( (1/(ldrValue / 1023))) * 100);
  Serial.println("ldr");
  Serial.println(ldrPercentage);
  Serial.print("%");
  

  delay(500);
  


}



void loop()
{


 hum() ;
 delay(250);
// gas() ;
// delay(250);
//
////  moisturew() ;
////  delay(250);
////
////  density() ;
////  delay(250);

  // put your main code here, to run repeatedly:

}
