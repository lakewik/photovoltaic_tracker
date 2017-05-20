char command;
String string;
boolean ledon = false;

#define NUM_SAMPLES 10

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculate
int TermometrPin = 7;
int GreenLEDPin = 13;
int RedLEDPin = 12;
int YellowLEDPin = 11;
int voltageMeasurmentPin = 6;
int servoPin = 9;

  void setup()
  {
    Serial.begin(9600);
    pinMode(GreenLEDPin, OUTPUT);
  }

  void loop()
  {

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

    readTemperatureSensor();
    sendVoltage();
    sendSunPower();

    //// GŁÓWNY PROCES TRACKERA ////
    /// TODO
    
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

    
void searchSun()
   {
     //// funkcja szukania miejsca o najlepszym natężeniu światła
     /// TODO
      delay(10);
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
 

    
