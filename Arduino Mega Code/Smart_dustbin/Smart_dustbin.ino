/* Things to do-
 *  calliberate moisture sensor
 *  calliberate weight sensor
 *  calliberate photoelectric sensor
 *  set height
 */
#include <Servo.h>
#include "HX711.h"
#include <SoftwareSerial.h>

#define LOADCELL_DOUT_PIN  2
#define LOADCELL_SCK_PIN  1

HX711 scale;
SoftwareSerial espSerial(34, 35);

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup

int servopin1 = 8;
int servopin2 = 7;
Servo myservo1, myservo2;

const int AirValue1 = 850;   //you need to replace this value with Value_1
const int WaterValue1 = 450;  //you need to replace this value with Value_2
const int AirValue2 = 820;
const int WaterValue2 = 390;

int moisturepin1 = A0;
int moisturepin2 = A1;

int trigpin = 12;
int echopin = 11;
int trigpin2 = 10;
int echopin2 = 9;

int traveltime;
float distance;

int inductive1 = 6;
int inductive2 = 5;
int inductive3 = 4;
int inductive4 = 3;
//int inductive5 = 2;

//int photoelectric1 = A4;
int photoelectric2 = A5;
int photoelectric3 = A6;
int photoelectric4 = A7;

int threshold = 800;//threshold to detect non organic substance
float weightthreshold;//weight of platform without any waste
float height = 100; //set the height of the bin

float leftweight = 0;
float rightweight = 0;
float currweight;

String str;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  delay(2000);
  myservo1.attach(servopin1);
  myservo2.attach(servopin2);
  myservo1.write(90);
  myservo2.write(90);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();
  
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(trigpin2, OUTPUT);
  pinMode(echopin2, INPUT);

  pinMode(inductive1, INPUT);
  pinMode(inductive2, INPUT);
  pinMode(inductive3, INPUT);
  pinMode(inductive4, INPUT);

  //pinMode(photoelectric1, INPUT);
  pinMode(photoelectric2, INPUT);
  pinMode(photoelectric3, INPUT);
  pinMode(photoelectric4, INPUT);
  
  float weightthreshold = weight();
}

void loop() {
  //float currweight = weight();
  Serial.println("Measuring");
  delay(2000);
  
  if (currweight > weightthreshold)
  {
    bool metal1 = inductive(inductive1);
    bool metal2 = inductive(inductive2);
    bool metal3 = inductive(inductive3);
    bool metal4 = inductive(inductive4);
    //Serial.println(metal2);
  
    //bool nonorg1 = photoelectric(photoelectric1);
    bool nonorg2 = photoelectric(photoelectric2);
    bool nonorg3 = photoelectric(photoelectric3);
    bool nonorg4 = photoelectric(photoelectric4);
    //Serial.println(nonorg2);
    //Serial.println(nonorg3);
    //Serial.println(nonorg4);
    
    if (!(metal1 && metal2 && metal3 && metal4))
    {
      Serial.println("Metal detected");
      Serial.println("Putting in right bin");
      rightTilt();
      //delay(5000);
      rightweight += currweight - weightthreshold;
    }else if (nonorg2 || nonorg3 || nonorg4)
    {
      Serial.println("Non organic substances detected");
      Serial.println("Putting in right bin");
      rightTilt();
      delay(3000);
      rightweight += currweight - weightthreshold;
    }else
    {
      Serial.println("Organic waste");
      Serial.println("Putting in left bin");
      leftTilt();
      leftweight += currweight - weightthreshold;
    }
  }
  delay(10000);
  Serial.println("Resetting platform");
  myservo1.write(90);
  myservo2.write(90);
  
  float orgdist = ultrasonic(trigpin, echopin);
  float nonorgdist = ultrasonic(trigpin2, echopin2);
  int moisture1 = moisture(moisturepin1, AirValue1, WaterValue1);
  int moisture2 = moisture(moisturepin2, AirValue2, WaterValue2);
  float leftcapacity = 100*(1 - (orgdist / height));
  float rightcapacity = 100*(1 - (nonorgdist / height));

  Serial.print("Weight in left bin = ");
  Serial.println(leftweight);
  Serial.print("Weight in right bin = ");
  Serial.println(rightweight);
  Serial.print("Moisture in left bin = ");
  Serial.print(moisture1);
  Serial.println("%");
  Serial.print("Moisture in right bin = ");
  Serial.print(moisture2);
  Serial.println("%");
  Serial.print("Capacity of left bin = ");
  Serial.print(leftcapacity);
  Serial.println("%");
  Serial.print("Capacity of right bin = ");
  Serial.print(rightcapacity);
  Serial.println("%");
  Serial.println();
  Serial.println();
  str =String(leftweight)+String(", ")+String(rightweight)+String(", ")+String(moisture1)+String(", ")+String(moisture2)+String(", ")+String(leftcapacity)+String(", ")+String(rightcapacity);
  espSerial.println(str);
  delay(10000);
}

float ultrasonic (int trig, int echo)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  traveltime =  pulseIn(echo, HIGH);
  distance = ((traveltime / 2.0) * 343.0) / 10000.0;
  delay(10);
  return distance;
  //Serial.print("Distance in cms: ");
  //Serial.println(distance);
  //delay(10);
}

int moisture (int pin, int air, int water)
{
  int soilMoistureValue = analogRead(pin);  //put Sensor insert into soil
  //Serial.println(soilMoistureValue);
  int soilmoisturepercent = map(soilMoistureValue, air, water, 0, 100);
  if(soilmoisturepercent >= 100)
  {
    return 100;
  }
  else if(soilmoisturepercent <=0)
  {
    return 0;
  }
  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  {
    return soilmoisturepercent;
  //Serial.println("%");
  }
  delay(150);
}

float weight (void)
{
  float weight = scale.get_units();
  return weight;
}

void leftTilt(void)
{
  myservo1.write(10);
  myservo2.write(170);
}

void rightTilt(void)
{
  myservo1.write(150);
  myservo2.write(30);
}

bool inductive (int pin)
{
  bool metal = digitalRead(pin);
  return metal;
}

bool photoelectric (int pin)
{
  float nonorg = analogRead(pin);
  //Serial.println(nonorg);
  if (nonorg < threshold)
  {
    return true;
  } else
  {
    return false;
  }
}
