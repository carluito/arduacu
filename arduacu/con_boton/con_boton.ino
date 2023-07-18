#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Stepper.h>
//Servo myservo;  // crea el objeto servo

//int pos = 0;    // posicion del servo

#define STEPS 1024
#define NUMSTEPS 5120// Número de pasos que queremos que de
Stepper stepper(STEPS, 8, 9, 10, 11);// Constructor, pasamos STEPS y los pines donde tengamos conectado el motor



 


 
RTC_DS3231 rtc;

const int SENSOR_PIN = 2; // Arduino pin connected to DS18B20 sensor's DQ pin

 OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
 DallasTemperature sensors(&oneWire); // pass oneWire to DallasTemperature library
 LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C address 0x27, 16 column and 2 rows

float tempCelsius;    // temperature in Celsius
 
 //Configuramos el pin del botón como entrada de pull-up
  const int boton = 3;
  

void setup()
{
 
  Serial.begin(9600);
  
 // Ponemos el pin como una entrada, puesto que vamos a leer
  // un botón. Habilito la resistencia de PULLUP.
  pinMode(boton,INPUT_PULLUP);
 
   // myservo.attach(7);  // vincula el servo al pin digital 7
  //  myservo.write(0);
   // Asignamos la velocidad en RPM (Revoluciones por Minuto)
  stepper.setSpeed(10); 
  
  sensors.begin();    // initialize the sensor
  lcd.init();         // initialize the lcd
  lcd.backlight();    // open the backlight 

// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

   if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }
   // Si se ha perdido la corriente, fijar fecha y hora
   if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
       //rtc.adjust(DateTime(2021, 10, 16, 00, 32, 0));
}
}
void loop()
{
  sensors.requestTemperatures();             // send the command to get temperatures
  tempCelsius = sensors.getTempCByIndex(0);  // read temperature in Celsius
  

DateTime now = rtc.now(); //obtiene datos del modulo RTC
lcd.clear();
lcd.setCursor(0,0);
 lcd.clear();
 lcd.setCursor(0, 0);       // start to print at the first row
 lcd.print(tempCelsius);    // print the temperature in Celsius
 lcd.print((char)223);      // print ° character
 lcd.print("C");
 lcd.print(" ");
if(now.hour() < 10)
{
lcd.print("0");
}
lcd.print(now.hour(), DEC); //imprime hora
lcd.print(':');
if(now.minute() < 10)
{
lcd.print("0");
}
lcd.print(now.minute(), DEC); //imprime minutos
lcd.print(':');
if(now.second() < 10)
{
lcd.print("0");
}
lcd.print(now.second(), DEC); //imprime segundos
lcd.setCursor(0,1);
if(now.day() < 10)
{
lcd.print("0");
}
lcd.print(now.day(), DEC); //imprime dia
lcd.print('/');
if(now.month() < 10)
{
lcd.print("0");
}
lcd.print(now.month(), DEC); //imprime mes
lcd.print('/');
lcd.print(now.year(), DEC);  //imprime el año

if( digitalRead(boton)==LOW ) {
   Serial.println("boton polsado");
   stepper.step(NUMSTEPS);
   
  delay(500);
 }

else if(now.hour() == 12 && now.minute() ==00){
 stepper.step(NUMSTEPS);
delay(500);
 }
 else if(now.hour() == 15 && now.minute() ==00){
 stepper.step(NUMSTEPS);
 delay(500);

 }
 else if(now.hour() == 20 && now.minute() ==00){
 stepper.step(NUMSTEPS);
 delay(500);

 }
 

  delay(500);
}
