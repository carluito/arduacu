#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Stepper.h>
#include <PushButton.h>

//Servo myservo;  // crea el objeto servo
//int pos = 0;    // posicion del servo

#define STEPS 1024
#define NUMSTEPS 5120// Número de pasos que queremos que de
Stepper stepper(STEPS, 8, 9, 10, 11);// Constructor, pasamos STEPS y los pines donde tengamos conectado el motor
 
RTC_DS3231 rtc;
/*Set up Alarmas
const int HAlarma1=10, MAlarma1=0, SAlarma1=0;    
const int HAlarma2=15, MAlarma2=0, SAlarma2=0;
const int HAlarma3=20, MAlarma3=10, SAlarma3=0;  
const int HAlarmacontador=19, MAlarmacontador=25, SAlarmacontador=0;
*/  
const int SENSOR_PIN = 2; // Arduino pin connected to DS18B20 sensor's DQ pin

 OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
 DallasTemperature sensors(&oneWire); // pass oneWire to DallasTemperature library
 LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C address 0x27, 16 column and 2 rows

float tempCelsius;    // temperature in Celsius
float tmax=0;
float tmin=999;

const int BUTTON_PIN = 3; //Configuramos el pin del botón q activa el comedero, reseteo dias y LCD
PushButton boton1(BUTTON_PIN); // Create a button object giving the digital pin as the only input parameter
const int BUTTON_PIN2 = 4; 
PushButton boton2(BUTTON_PIN2); // Create a button object giving the digital pin as the only input parameter  
int contarDias;

const int pinBuzzer = 7;
void setup()
{
 
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button as input with the internal pullup resistor enabled
   pinMode(BUTTON_PIN2, INPUT_PULLUP);
  //pinMode(buzzer, OUTPUT); // Set buzzer as output 
  
 boton1.setActiveLogic(LOW); // Set the button logic as active low (the voltage at the input pin is LOW when the button is pressed)
 boton2.setActiveLogic(LOW); 
   // myservo.attach(7);  // vincula el servo al pin digital 7
  //  myservo.write(0);
   // Asignamos la velocidad en RPM (Revoluciones por Minuto)
  stepper.setSpeed(10); 
  
  sensors.begin();    // initialize the sensor
  lcd.init();         // initialize the lcd
  //lcd.backlight();    // open the backlight 

//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

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
  if(tempCelsius>tmax){
      tmax=tempCelsius;
     }
     if(tempCelsius<tmin){
      tmin=tempCelsius;
     }

DateTime now = rtc.now(); //obtiene datos del modulo RTC

 lcd.clear();
 lcd.setCursor(0,0);       // start to print at the first row
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
//lcd.print('/');
//lcd.print(now.year(), DEC);  //imprime el año
//lcd.setCursor(0,9); 
//lcd.scrollDisplayLeft();
lcd.print("T");
lcd.print(tmax,1);
lcd.print("t");
lcd.print(tmin,1);
  /*lcd.print(" ");
  lcd.print(contarDias); 
  lcd.print(" "); 
*/

boton1.update(); // Update the button state. This should ALWAYS be done as the first thing in your main loop!
boton2.update(); 
 if (boton1.isClicked())
  {   
     Serial.println("boton1");
    tone(pinBuzzer, 440); 
    delay(400);
    Serial.println("Tono");
  noTone(pinBuzzer);
   delay(400);
   tone(pinBuzzer, 440); 
    delay(400);
  noTone(pinBuzzer);
   delay(400);
    lcd.backlight();
    stepper.step(NUMSTEPS);
   digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);   
  }
/* else if(boton1.isDoubleClicked())
  {
     BotonComer();
  }
    else if(boton1.isHeld())
  {
    Buzzer1();
     lcd.backlight();
  }*/
else{
    lcd.noBacklight();
  }
  
 if (boton2.isClicked())
  { 
    Serial.println("boton2");
    lcd.backlight();
    delay(4000);
  }
    else{
    lcd.noBacklight();
  }
 
if(now.hour() == 12 && now.minute() ==00&&now.second()==00){
   tone(pinBuzzer, 440); 
    delay(400);
    Serial.println("Tono");
  noTone(pinBuzzer);
   delay(400);
   tone(pinBuzzer, 440); 
    delay(400);
  noTone(pinBuzzer);
   delay(400);
  stepper.step(NUMSTEPS);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
 }
 else if(now.hour() == 15 && now.minute() ==00&&now.second()==00){
   tone(pinBuzzer, 440); 
    delay(400);
    Serial.println("Tono");
  noTone(pinBuzzer);
   delay(400);
   tone(pinBuzzer, 440); 
    delay(400);
  noTone(pinBuzzer);
   delay(400);
  stepper.step(NUMSTEPS);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
 }
 else if(now.hour() == 20 && now.minute() ==00&&now.second()==00){
   tone(pinBuzzer, 440); 
    delay(400);
    Serial.println("Tono");
  noTone(pinBuzzer);
   delay(400);
   tone(pinBuzzer, 440); 
    delay(400);
  noTone(pinBuzzer);
   delay(400);
  stepper.step(NUMSTEPS);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
 }
 

  delay(500);
}
/*
void Alarmacontador(int Hor, int Minu, int Segu)
 {
 if (Hor==HAlarmacontador){
   if (Minu==MAlarmacontador){
     if (Segu==SAlarmacontador){

           Buzzer1();
           contarDias++;
      }
    }
  }
}
void Buzzer1(){
  Serial.println("suenooooooo"); 
  digitalWrite(buzzer, HIGH);   // Ponemos en alto(5V) el pin del buzzer
  delay(200);                     // Esperamos 1 segundo
  digitalWrite(buzzer, LOW);    // Ponemos en bajo(0V) el pin del buzzer
  delay(200);                     // Esperamos 4 segundos
}*/
