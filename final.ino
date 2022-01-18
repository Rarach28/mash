//millis
int wait1 = 1000;
unsigned long time_now = 0;

//DHT11
#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h; // humidity
float t; //temperature
//DHT11 END

//OLED START
  #include <Arduino.h>
  #include <U8g2lib.h>
  U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
//OLED END

//DISTANCE START
  #define echoPin 5 // attach pin D2 Arduino to pin Echo of HC-SR04
  #define trigPin 6 //attach pin D3 Arduino to pin Trig of HC-SR04
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement
//DISTANCE END

//DALLAS START
  #include <OneWire.h>
  #include <DS18B20.h>
  #define ONE_WIRE_BUS 3

  OneWire oneWire(ONE_WIRE_BUS);
  DS18B20 dallasTemp(&oneWire);
//DALLAS END

//sys
  String a; // serial buffer


void setup() {
  Serial.begin(115200);

  //DISTANCE START
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  //DISTANCE END
  
  //DHT11
    dht.begin();
  //DHT11 END

  //OLED START
    u8g2.begin();  
  //OLED END

  //DALLAS START
    dallasTemp.begin();
  //DALLAS END

}

void loop() {
  //OLED START
    u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_6x12_tf);    // nastaveni fontu
        u8g2.drawStr(0,11,"Arduino Weather station");  // vystup
        u8g2.setCursor(0, 23);
        u8g2.print(F("Humidity: "));
        u8g2.print(h);
        u8g2.print(F("%"));
        u8g2.setCursor(0, 33);
        u8g2.print(("Temperature: "));
        u8g2.print(t);
        u8g2.drawUTF8(110,33,"°C");
        u8g2.setCursor(0, 43);
        u8g2.print("Distance: ");
        u8g2.print(distance);
        u8g2.print(" cm");
        u8g2.setCursor(0, 53);
        u8g2.print("Dallas Temp: ");
        u8g2.print(dallasTemp.getTempC());
        u8g2.drawUTF8(110,53,"°C");
      } while ( u8g2.nextPage() );
  //OLED END
  


  
  //print weather every one sec
  if(millis() >= time_now + wait1){
        time_now += wait1;

        //DHT11 START
          h = dht.readHumidity();
          t = dht.readTemperature(); //kdyz passnu true tak Fahrenheit
          if (isnan(h) || isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
          }
          Serial.print(F("Humidity: "));
          Serial.print(h);
          Serial.print(F("%  Temperature: "));
          Serial.print(t);
          Serial.println(F("°C "));
        //DHT11 END

        //DISTANCE START
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          // Reads the echoPin, returns the sound wave travel time in microseconds
          duration = pulseIn(echoPin, HIGH);
          // Calculating the distance
          distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
          // Displays the distance on the Serial Monitor
          Serial.print("Distance: ");
          Serial.print(distance);
          Serial.println(" cm");
        //DISTANCE END

        //DALLAS START
          dallasTemp.requestTemperatures();

          while (!dallasTemp.isConversionComplete());  // wait until sensor is ready

          Serial.print("Temp: ");
          Serial.println(dallasTemp.getTempC());
        //DALLAS END    
   }
}
