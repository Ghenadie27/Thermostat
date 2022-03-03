#include "DHT.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <max6675.h>
#define MAX6675_CS   A3
#define MAX6675_SO   A4
#define MAX6675_SCK  A5
int soPin = A4;// SO=Serial Out
int csPin = A3;// CS = chip select CS pin
int sckPin = A5;// SCK = Serial Clock pin
MAX6675 robojax(sckPin, csPin, soPin);// create instance object of MAX6675

#define DHTPIN 11//temperatura aer
#define DHTTYPE DHT21 // AM2301 
#define DHTPIN1 12//temperatura apa
#define DHTTYPE DHT21 // AM2301 
#define DHTPIN2 13//temperatura fum
#define DHTTYPE DHT21 // AM2301 

DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

int TrigPin = 6;
int EchoPin = 7;
int TrigPin1 = 10;
int EchoPin1 = 9;
int TrigPin2 = 2;
int EchoPin2 = 3;

float pingTime;
float cm;

int ledRed = A2;
int ledGreen = A1;
int relAer = 4;
int relSistem = 5;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  dht1.begin();
  dht2.begin();

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin1, OUTPUT);
  pinMode(EchoPin1, INPUT);
  pinMode(TrigPin2, OUTPUT);
  pinMode(EchoPin2, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(relAer, OUTPUT);
  pinMode(relSistem, OUTPUT);
}

void loop() {
  float tAer = dht.readTemperature();
  float tApa = dht1.readTemperature();
  float tFum = dht2.readTemperature();

  Serial.print("Distanta:");
  Serial.println(cm);
  Serial.print("Temperatura:");
  Serial.println(tAer);
  Serial.println(tApa);
  Serial.println(tFum);
  Serial.print("C = ");
  Serial.print(robojax.readCelsius());

  sonarSensor(TrigPin, EchoPin);
  float dist1 = cm;
  delay(50);
  sonarSensor(TrigPin1, EchoPin1);
  float dist2 = cm;
  delay(50);
  sonarSensor(TrigPin2, EchoPin2);
  float dist3 = cm;
  pompaAer(dist1, dist2, dist3, tAer, tApa, tFum);
  pompaApa(tFum, tApa);
  afisLcd(tAer, tApa);
}

void sonarSensor(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pingTime = pulseIn(echoPin, HIGH);
  cm = pingTime / 58.2;
}

void pompaAer(float dist1, float dist2, float dist3, float tAer, float tApa, float tFum) {
  if (dist1 < 30 || dist2 < 30 || dist3 < 30 || tAer > 25 || tApa > 25 || tFum < 22)
  {
    digitalWrite(relAer, HIGH);
    delay(7000);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
    delay(3000);
  } else {
    delay(200);
    digitalWrite(relAer, LOW);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }
}

void pompaApa(float tFum, float tApa) {

  if (tFum < 22 && tApa < 25) {
    digitalWrite(relSistem, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    lcd.noBacklight();
  } else {
    digitalWrite(relSistem, LOW);
  }
}

void afisLcd (float t, float t1) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("temp Aer = ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("temp Apa = ");
  lcd.setCursor(11, 1);
  lcd.print(t1);
}
