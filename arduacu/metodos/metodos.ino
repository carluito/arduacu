#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Stepper.h>
#include <PushButton.h>

const int STEPS=1024;
const int NUMSTEPS= 5120;// Número de pasos que queremos que de
Stepper stepper(STEPS, 8, 9, 10, 11);// Constructor, pasamos STEPS y los pines donde tengamos conectado el motor

RTC_DS3231 rtc;
//Set up Alarmas
const int HAlarma1=10, MAlarma1=0, SAlarma1=0;    
const int HAlarma2=15, MAlarma2=0, SAlarma2=0;
const int HAlarma3=20, MAlarma3=10, SAlarma3=0;    
const int HAlarmacontador=00, MAlarmacontador=10, SAlarmacontador=0;
const int SENSOR_PIN = 2; // Arduino pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature sensors(&oneWire); // pass oneWire to DallasTemperature library
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C address 0x27, 16 column and 2 rows
float tempCelsius;    // temperature in Celsius
float tmax=0;
float tmin=999;

const int BUTTON_PIN = 3; //Configuramos el pin del botón q activa el comedero, reseteo dias y LCD

PushButton boton1(BUTTON_PIN); // Create a button object giving the digital pin as the only input parameter

int contarDias;

const int buzzer=7;

void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button as input with the internal pullup resistor enabled
 
 boton1.setActiveLogic(HIGH); // Set the button logic as active low (the voltage at the input pin is LOW when the button is pressed)

  
  stepper.setSpeed(10); // Asignamos la velocidad en RPM (Revoluciones por Minuto)
  
  sensors.begin();    // initialize the sensor
  

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

     
    pinMode(buzzer, OUTPUT);

}

void loop(){

  
  sensors.requestTemperatures();             // send the command to get temperatures
  tempCelsius = sensors.getTempCByIndex(0);  // read temperature in Celsius
     if(tempCelsius>tmax){
      tmax=tempCelsius;
     }
     if(tempCelsius<tmin){
      tmin=tempCelsius;
     }
  boton1.update(); // Update the button state. This should ALWAYS be done as the first thing in your main loop!

  // Check for a specific button event, eg. isClicked(), isReleased(), isDoubleClicked(), isHeld() or isActive()
  if (boton1.isClicked())
  {
     Lcd();
  }
  else if(boton1.isDoubleClicked())
  {
     BotonComer();
  }
    else if(boton1.isHeld())
  {
    ReseteoContador();
  }
       
  Alarmacontador(HAlarmacontador, MAlarmacontador, SAlarmacontador);
  Alarma1(HAlarma1, MAlarma1, SAlarma1);
  Alarma2(HAlarma2, MAlarma2, SAlarma2);
  Alarma3(HAlarma3, MAlarma3, SAlarma3);
}
//metodo mostrar Lcd al pulsar boton

void Lcd(){
    
    DateTime now = rtc.now(); //obtiene datos del modulo RTC

      
        Serial.println("lcd");
  lcd.init();         // initialize the lcd
  lcd.backlight();    // open the backlight 
  lcd.clear();
  lcd.setCursor(0, 0);       // start to print at the first row
  lcd.print(tempCelsius);    // print the temperature in Celsius
  lcd.print((char)223);      // print ° character
  lcd.print("C");
  lcd.print(" ");
    if(now.hour() < 10){
      lcd.print("0");
     }
  lcd.print(now.hour(), DEC); //imprime hora
  lcd.print(':');
    if(now.minute() < 10){
      lcd.print("0");
    } 
  lcd.print(now.minute(), DEC); //imprime minutos
  lcd.print(':');
    if(now.second() < 10){
      lcd.print("0");
    }
  lcd.print(now.second(), DEC); //imprime segundos
  lcd.setCursor(0,1);
  lcd.scrollDisplayLeft();
    if(now.day() < 10){
      lcd.print("0");
    }
  lcd.print(now.day(), DEC); //imprime dia
  lcd.print('/');
    if(now.month() < 10){
      lcd.print("0");
      }
  lcd.print(now.month(), DEC); //imprime mes
  lcd.print('/');
  lcd.print(now.year(), DEC);  //imprime el año
  lcd.scrollDisplayLeft();
  lcd.print("Tmax");
  lcd.print(" ");
  lcd.print(tmax);
  lcd.print(" ");
  lcd.print("Tmin");
  lcd.print(tmin);
  lcd.print(" ");
  lcd.print(contarDias); 
  lcd.print(" "); 
}

void BotonComer(){
  
      Serial.println("Comer");
      stepper.step(NUMSTEPS);
  
}


  void Alarma1(int Hor, int Minu, int Segu){
 if (Hor==HAlarma1){
   if (Minu==MAlarma1){
     if (Segu==SAlarma1){
            Buzzer2();
            stepper.step(NUMSTEPS);
      }
    }
  }
}
 void Alarma2(int Hor, int Minu, int Segu){
 if (Hor==HAlarma2){
   if (Minu==MAlarma2){
     if (Segu==SAlarma2){
            Buzzer2();
            stepper.step(NUMSTEPS);
      }
    }
  }
}
 void Alarma3(int Hor, int Minu, int Segu){
 if (Hor==HAlarma3){
   if (Minu==MAlarma3){
     if (Segu==SAlarma3){
            Buzzer2();
            stepper.step(NUMSTEPS);
      }
    }
  }
}



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

void ReseteoContador(){


  contarDias=0;

  
}

void Buzzer1(){

  digitalWrite(buzzer, HIGH);   // Ponemos en alto(5V) el pin del buzzer
  delay(200);                     // Esperamos 1 segundo
  digitalWrite(buzzer, LOW);    // Ponemos en bajo(0V) el pin del buzzer
  delay(200);                     // Esperamos 4 segundos
}

void Buzzer2(){

  digitalWrite(buzzer, HIGH);   // Ponemos en alto(5V) el pin del buzzer
  delay(200);                     // Esperamos 1 segundo
  digitalWrite(buzzer, LOW);    // Ponemos en bajo(0V) el pin del buzzer
  delay(200);                     // Esperamos 4 segundos
  digitalWrite(buzzer, HIGH);   // Ponemos en alto(5V) el pin del buzzer
  delay(200);                     // Esperamos 1 segundo
  digitalWrite(buzzer, LOW);    // Ponemos en bajo(0V) el pin del buzzer
  delay(200);                     // Esperamos 4 segundos
}
