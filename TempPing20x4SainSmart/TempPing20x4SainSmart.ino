// Using the SainSmart 20x4 LCD through the I2C interface of
// SDA and SCL found on the Arduino boards UNO and MEGA

//Reading Temperature with LM35 Sensor
//Measuring in both scales Celsius and Fahrenheit
//Arduino Thermometer Shield
//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1
/* Ping))) Sensor
  
This sketch reads a PING))) ultrasonic rangefinder and returns the
distance to the closest object in range. To do this, it sends a pulse
to the sensor to initiate a reading, then listens for a pulse 
to return.  The length of the returning pulse is proportional to 
the distance of the object from the sensor.
     
The circuit:
* +V connection of the PING))) attached to +5V
* GND connection of the PING))) attached to ground
* SIG connection of the PING))) attached to digital pin 11

http://www.arduino.cc/en/Tutorial/Ping
   
created 3 Nov 2008
by David A. Mellis
modified 30 Aug 2011
by Tom Igoe
modified 3 June 2016
by Patrick Scott
 
This example code is in the public domain.

*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// this constant won't change.  It's the pin number
// of the sensor's output:
const int pingPin = 11;
const int shifter = 2;
long duration, avg, sum;
double cm, inches;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int AvgC, TotC, TC;
float tempC;
float tempF;
int tempPin = 3,  // A1 => analog pin 1
    ShiftSz = 2;  // number of samples for average 2^n, 2 = 4, 3 = 8

void setup()
{
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  sum = duration << shifter;
  avg = duration;
  delay(100);

  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  TC = analogRead(tempPin);
  TotC = TC << ShiftSz;  // form an average
}
void loop()
{
  TC = analogRead(tempPin);
  TotC -= TotC >> ShiftSz;  // make a hole in average
  TotC += TC;               // fill it
  tempC = TotC >> ShiftSz;  // get new average
  //lcd.setCursor(8, 0);
  //lcd.print(tempC,1);
  tempC = (5.0 * tempC * 100.0)/1024.0;
  tempF = ((tempC*9)/5) + 32;

  lcd.setCursor(0, 0);
  lcd.print(tempC,1);
  lcd.print("'C");

  lcd.setCursor(0, 1);
  lcd.print(tempF,1);
  lcd.print("'F");

  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // create an averaging system
  sum = sum - avg;
  sum = sum + duration;
  avg = sum >> shifter;
  // convert the time into a distance
  inches = microsecondsToInches(avg);
  cm = microsecondsToCentimeters(avg);
    
  if(inches > 200)
  {
    lcd.setCursor(0, 2);
    lcd.print("out of range");
  }
  else
  {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 2);
    lcd.print(inches);
    lcd.print(" in      ");
    lcd.setCursor(0, 3);
    lcd.print(cm);
    lcd.print(" cm      ");
  }
  delay(200);
}
double microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return (microseconds >>1) / 73.746;
}

double microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29.034 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return (microseconds >> 1) / 29.034;
}

