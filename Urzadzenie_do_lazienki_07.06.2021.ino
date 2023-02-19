#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte stopnie[8] =
{
  0b00000,
  0b00110,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};


#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
unsigned long  czas_dzialania_temp = 0;
unsigned long  czas_dzialania_wilg = 0;
unsigned long  czas_dzialania_menu = 0;
unsigned long  czas_dzialania_przycisk1 = 0;
unsigned long  czas_dzialania_przycisk2 = 0;
unsigned long  czas_dzialania_przycisk2_1 = 0;
unsigned long  szybkosc_sciemniania = 0;
short a = 0;
short b = 0;
float h;
float t;
int zmianawilgotnosci();
int pomiar_temperatury();
int pomiar_wilgotnosci();

short y; //zmienna dla petli switch(1-wlaczone,2-sciemnianie,3-wylaczone
short e; //0 mówi, że ledy nie byly sciemniane a 1, że tak
short f; //0 mowi, ze przycisk byl puszczony, 1 ze nie
short g; //zmienna mowiaca czy przycisk zostal juz wcisniety drugi raz. 0 nie, 1 tak.
short k; //jasnosc ledow

void setup()
{
  Serial.begin(9600);
  lcd.createChar(0, stopnie);
  pinMode(6, OUTPUT);
  analogWrite(10, 0);
  e = 0;
  g = 0;
}

void loop()
{
  menu();
  przycisk1();
  przycisk2();
  /* analogWrite(10,255);
    delay(1000);
    analogWrite(10,0);
    delay(1000);*/
}


//__________________________
void menu()
{
  lcd.begin(16, 2);
  short x;
  a = zmianawilgotnosci();
  if (a != b) // zmiania wartosci x zaleznie czy jest przekrecony potencjometr czy nie, po to zeby wyswietlic inne menu przez 2 sekundy
  {
    x = 2;
    czas_dzialania_menu = millis() + 2000;
  }
  if (millis() > czas_dzialania_menu)
  {
    x = 1;
  }
  b = a;
  switch (x)
  {
    case 1:
      lcd.clear();
      lcd.print("Wilgotnosc: ");
      lcd.setCursor(12, 0);
      lcd.print(h);
      lcd.setCursor(15, 0);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Temperatura: ");
      lcd.setCursor(12, 1);
      lcd.print(t);
      lcd.setCursor(15, 1);
      lcd.write(byte(0));
      delay(200);
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Max wilg.: ");
      lcd.setCursor(11, 0);
      lcd.print(zmianawilgotnosci());
      delay(200);
      break;
  }
}

//__________________________
void menu2() // poprawne menu na if-ach
{
  lcd.begin(16, 2);
  short x;
  a = zmianawilgotnosci();

  if (a != b) czas_dzialania_menu = millis() + 5000;
  if ((a == b) & (millis() > czas_dzialania_menu))
  {
    lcd.clear();
    lcd.print("Wilgotnosc: ");
    lcd.setCursor(12, 0);
    lcd.print(h);
    lcd.setCursor(15, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    lcd.setCursor(12, 1);
    lcd.print(t);
    lcd.setCursor(15, 1);
    lcd.write(byte(0));
    delay(200);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Max wilg.: ");
    lcd.setCursor(11, 1);
    lcd.print(zmianawilgotnosci());
    delay(200);
  }
  b = a;
  ;
}

//___________________________
int pomiar_temperatury() //pomiar temperatury co 2 sekundy
{
  if (millis() > czas_dzialania_temp)
  {
    czas_dzialania_temp = millis() + 2000;
    float t = dht.readTemperature();
  }
  return (t);

}
//___________________________
int pomiar_wilgotnosci() //pomiar wilgotnosci co 2 sekundy
{
  if (millis() > czas_dzialania_wilg)
  {
    czas_dzialania_wilg = millis() + 2000;
    float h = dht.readHumidity();
  }
  return (h);

}

//___________________________
int zmianawilgotnosci() //zmiana zalaczania wentylatora w zaleznosci od wilgotnosci, zmiana potencjometrem na urzadzeniu
{
  int odczyt = analogRead(A2);
  odczyt = map(odczyt, 0, 1023, 20, 100);
  return (odczyt);
}

//___________________________
void przycisk1() //uruchamianie wentylatora z przycisku w lazience na 20 sekund
{
  if (analogRead(A5) > 900)
  {
    delay(20);
    if (analogRead(A5) > 900)
    {
      digitalWrite(6, HIGH);
      czas_dzialania_przycisk1 = millis() + 5000;
    }
  }
  if (millis() > czas_dzialania_przycisk1)
  {
    digitalWrite(6, LOW);
  }
}

//___________________________
void przycisk2() //uruchamianie swiatla i sciemniacz(moze petla while przed ifami ze zmienna ktora ja uruchamia?)
{
  /* short y; //zmienna dla petli switch(1-wlaczone,2-sciemnianie,3-wylaczone
    short e; //0 mówi, że ledy nie byly sciemniane a 1, że tak
    short f; //0 mowi, ze przycisk byl puszczony, 1 ze nie
    short g; //zmienna mowiaca czy przycisk zostal juz wcisniety drugi raz. 0 nie, 1 tak.
    short h; //jasnosc ledow*/

  if (analogRead(A6) > 900)
  {
    delay(20);
    if (analogRead(A6) > 900)
    {

      if (e == 0)
      {
        czas_dzialania_przycisk2 = millis() + 1000;
        e = 1;
        f = 1;
      }


      if ((e == 1) & (f == 1))
      {
        if (millis() < czas_dzialania_przycisk2)
        {
          y = 1;//uruchamia ledy na full
        }
        else
        {
          e = 2;
        }
      }

      if ((e == 2) & (f == 1))
      {
        y = 2; //sciemnia ledy, przy zgaszonych ledach wartosc e musza zmienic sie na 0.
      }
    }
  }

  //Sciemnianie albo wylaczanie po ponownym nacisnieciu przycisku
  if ((analogRead(A6) > 900) & (f == 0) & (g == 0))
  {
    delay(20);
    if (analogRead(A6) > 900)
    {
      czas_dzialania_przycisk2_1 = millis() + 1000;
      g = 1;
    }
  }

  if (analogRead((A6) < 900) & (millis() < czas_dzialania_przycisk2_1) & (g == 1))
  {
    y = 3; //wylacza ledy

  }

  if (analogRead((A6) > 900) & (millis() > czas_dzialania_przycisk2_1) & (g == 1))
  {
    y = 2; //sciemnia ledy dalej
  }


  if (analogRead(A6) < 900)
  {
    f = 0; //0 mowi, ze przycisk byl puszczony, 1 ze nie
  }


  switch (y)
  {
    case 1:
      analogWrite(10, 255);
      k = 255;
      break;
    case 2:
      if (millis() > szybkosc_sciemniania)
      {
        szybkosc_sciemniania = millis() + 250;
        k = k - 10;
        analogWrite(10, k);
        if (k <= 0)
        {
          e = 0;
          g = 0;
        }
      }
      break;
    case 3:
      {
        analogWrite(10, 0);
        e = 0;
        g = 0;
      }
      break;
  }
}
