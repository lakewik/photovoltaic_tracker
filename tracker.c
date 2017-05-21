
#include <limits.h>
#include <Servo.h>

char command;
String string;
boolean ledon = false;

#define NUM_SAMPLES 10

 int light_power_array[181];

// SERVO VARIABLES //
Servo servo;
int servoAngle = 0;
const int defaultServoAngle = 0;
const int servoAngleIncreaseRate = 1;
// TEMPERATURE(?) VARIABLES //
int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculate
// PIN VARIABLES //
int TermometrPin = 7;
int GreenLEDPin = 13;
int RedLEDPin = 12;
int YellowLEDPin = 11;
int voltageMeasurmentPin = 6;
int servoPin = 9;
int lightSensorPin = 5; //light sensor's pin
int light = 0; //variable to analogRead of Light Sensor
const unsigned long fiveMinutes = 5 * 60 * 1000UL;
static unsigned long lastScan = 0 - fiveMinutes; //initialization: make it scan due first loop();
int loop_count = 0;

  void setup()
  {
    Serial.begin(9600);
    pinMode(GreenLEDPin, OUTPUT);
    pinMode(YellowLEDPin, OUTPUT);
    //set the servo to initial position
    servo.attach(servoPin);
 // servo.write(defaultServoAngle);
 analogWrite(YellowLEDPin, 126);
  //initial scan if initialization of "lastScan" var fails lul
  //searchSun();
  }

  void loop()
  {

    //analogWrite(YellowLEDPin, loop_count*3);
    analogWrite(YellowLEDPin, 0);

  loop_count++;
  if (loop_count == 10) {
    int sun_power_array[181];
     searchSun();
     int max_v = INT_MIN;
      int max_i = 0;
 
      for ( int i = 0; i < sizeof(light_power_array)/sizeof(light_power_array[0]); i++ )
        {
          if ( light_power_array[i] > max_v )
            {
              max_v = light_power_array[i];
              max_i = i;
            }
        }
 
  //printf( "The max value (%d) is at index %d.", max_v, max_i);

    servo.write(max_i);
    analogWrite(YellowLEDPin, 255);
  
     loop_count = 0;
  }

  delay(1000);
  analogWrite(YellowLEDPin, 0);

    

    /// CZYTANIE BLUETOOTH Z SERIAL PORT ///
    
    if (Serial.available() > 0) 
    {string = "";}
    
    while(Serial.available() > 0)
    {
      command = ((byte)Serial.read());
      
      if(command == ':')
      {
        break;
      }
      
      else
      {
        string += command;
      }
      
      delay(1);
    }

    ///// OBSŁUGA KOMEND BLUETOOTH ////

    if(string == "TOBEGINNINGCELLPOSITION") // przekręć panel do pozycji początkowej
    {

    }

    if(string == "RESTART") // zrestartuj moduł
    {

    }

    if(string == "SUNSCAN") // uruchom proces automatycznego szukania słońca
    {
        searchSun();
    }
    
    if(string == "ENABLE") 
    {
        ledon = true;
    }
    
    if(string =="DISABLE")
    {
        ledon = false;
    }

    /// WYSYŁANIE DANYCH Z CZUJNIKÓW ///

    //readTemperatureSensor();
    //sendVoltage();
    //sendSunPower();

    //// GŁÓWNY PROCES TRACKERA ////
    /// WIP

  //unsigned long timeNow = millis(); //millis to make it NOT stop other tasks
  //if (timeNow - lastScan >= fiveMinutes) {
   // lastScan += fiveMinutes;
    //searchSun();
  //}

  
    
 }

void readTemperatureSensor() {
 int sum = 0;
  for(int i = 0; i < 8; i++)  
  {
    sum += analogRead(TermometrPin);
    delay(1000);
  }

  // do all the math at once
  float tempc = sum * 0.061035156;    // temperatura w stopniach Celcjusza


  Serial.print(tempc, 2);
  Serial.print("temperature, ");
}

void sendVoltage()
   {
    /// funkcja odczytujaca i wysyłająca napięcie przez Bluetooth

      // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        sum += analogRead(voltageMeasurmentPin);
        sample_count++;
        delay(10);
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1024.0;
    // send voltage for display on Serial Monitor
    // voltage multiplied by 11 when using voltage divider that
    // divides by 11. 11.132 is the calibrated voltage divide
    // value

      Serial.print(voltage * 11.132);
      Serial.print("voltage");

    sample_count = 0;
    sum = 0;
    


    }

void sendSunPower()
   {

    /// funkcja odczytujaca i wysyłająca natężenie światła przez Bluetooth
    int sunPower = 0;
    sunPower = analogRead(sunPower); 

      Serial.print(sunPower);
      Serial.print("sun_power");


    }

void getTimeParametrsFromApp()
   {
     //// opcjonalna funkcja odczytująca parametry dotyczace czasu (aktualny czas i strefa czasowa) z aplikacji
     /// TODO
      delay(10);
    }

    
int searchSun()
   {
  //1023 / 5.68(3) = 180

  
  servo.write(defaultServoAngle); /// set initial servo position

   int current_angle = 0;
   int current_light = 0;
   
   while (current_angle < 180) {
        servo.write(current_angle); 
        current_light = analogRead(lightSensorPin);
        light_power_array[current_angle] = current_light;
        current_angle++;
        delay(100);
    }

   ///light = analogRead(lightSensorPin);

  //if the scan last scan was less than 80% of 1023 it tries to move the servo by one degree then scans again to check per centage
  //if (light <= 818.4) { 
   // servoAngle += servoAngleIncreaseRate;
   // servo.write(servoAngle);
    //Servo::refresh();
   // light = analogRead(lightSensorPin);
  //}

     //// funkcja szukania miejsca o najlepszym natężeniu światła
     /// WIP
     
      delay(10);
      //return light_power_array;
    }
 
void yellowStatusLedOn()
   {
      analogWrite(YellowLEDPin, 255);
      delay(10);
    }
 
 void yellowStatusLedOff()
 {
      analogWrite(YellowLEDPin, 0);
      delay(10);
 }

 void criticalErrorLedOn()
 {
      analogWrite(RedLEDPin, 255);
      delay(10);
 }
