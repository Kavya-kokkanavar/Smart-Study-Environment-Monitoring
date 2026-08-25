#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN D4
#define DHTTYPE DHT11
#define TRIG D5
#define ECHO D6
#define LED D7
#define LDR A0
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
float temp = 0, hum = 0, dist = -1;
int light = 0, score = 0, breaks = 0;
bool person = false;
unsigned long studyTime=0, breakTime=0, absentTime=0;
unsigned long lastSec=0, lastRead=0, lastLCD=0;
int page=0;
void readSensors(){
  float t=dht.readTemperature(), h=dht.readHumidity();
  if(!isnan(t)) temp=t;
  if(!isnan(h)) hum=h;
  light=analogRead(LDR);
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  long duration=pulseIn(ECHO,HIGH,30000);
  if(duration>0) dist=duration*0.0343/2;
  else dist=-1;
  person=(dist>=2 && dist<=100);
  if(light<150) digitalWrite(LED,HIGH);
  else if(light>250) digitalWrite(LED,LOW);
  int ts;
  if(temp>=20 && temp<=28) ts=25;
  else if((temp>=18 && temp<20)||(temp>28 && temp<=30)) ts=20;
  else if((temp>=15 && temp<18)||(temp>30 && temp<=33)) ts=10;
  else ts=5;
  int hs;
  if(hum>=40 && hum<=60) hs=25;
  else if((hum>=30 && hum<40)||(hum>60 && hum<=70)) hs=20;
  else if((hum>=20 && hum<30)||(hum>70 && hum<=80)) hs=10;
  else hs=5;
  int ls=(light>=500)?25:((light>=200)?18:8);
  score=ts+hs+ls+(person?25:0);
}
void updateTimer(){
  if(millis()-lastSec>=1000){
    lastSec=millis();
    if(person){ studyTime++; absentTime=0; }
    else { breakTime++; absentTime++; }
    if(absentTime==300) breaks++;
  }
}
String status(){
  if(person) return "STUDYING";
  if(absentTime>=300) return "BREAK";
  return "PAUSED";
}
String recommendation(){
  if(!person) return "PAUSED";
  if(studyTime>=2700) return "TAKE BREAK";
  if(score<60) return "IMPROVE ENV";
  return "KEEP STUDYING";
}
String efficiency(){
  int s=score;
  if(studyTime>=1800) s+=10;
  if(breaks>3) s-=10;
  if(s>=80) return "EXCELLENT";
  if(s>=65) return "GOOD";
  if(s>=50) return "AVERAGE";
  return "LOW";
}
void printTime(unsigned long sec){
  lcd.print(sec/60);
  lcd.print(":");
  if(sec%60<10) lcd.print("0");
  lcd.print(sec%60);
}
void showLCD(){
  lcd.clear();
  if(page==0){
    lcd.setCursor(0,0);
    lcd.print("T:"); lcd.print(temp,1); lcd.print("C H:"); lcd.print(hum,0);
    lcd.setCursor(0,1); lcd.print("Light:"); lcd.print(light);
  }
  else if(page==1){
    lcd.setCursor(0,0); lcd.print("Distance:");
    if(dist>=0) lcd.print(dist,0); else lcd.print("--");
    lcd.print("cm");
    lcd.setCursor(0,1); lcd.print("Person:"); lcd.print(person?"YES":"NO");
  }
  else if(page==2){
    lcd.setCursor(0,0); lcd.print("Study Time");
    lcd.setCursor(0,1); printTime(studyTime);
  }
  else if(page==3){
    lcd.setCursor(0,0); lcd.print("Score:"); lcd.print(score); lcd.print("/100");
    lcd.setCursor(0,1); lcd.print(light<200?"LIGHT:LOW":"LIGHT:GOOD");
  }
  else if(page==4){
    lcd.setCursor(0,0); lcd.print(status());
    lcd.setCursor(0,1); lcd.print(recommendation());
  }
  else{
    lcd.setCursor(0,0); lcd.print("Efficiency");
    lcd.setCursor(0,1); lcd.print(efficiency());
  }
}
void report(){
  Serial.println("\n----------------------");
  Serial.print("Temperature: "); Serial.print(temp); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");
  Serial.print("Light: "); Serial.println(light);
  Serial.print("Distance: ");
  if(dist>=0){Serial.print(dist);Serial.println(" cm");}
  else Serial.println("NO ECHO");
  Serial.print("Person: "); Serial.println(person?"YES":"NO");
  Serial.print("Study Time: "); Serial.print(studyTime/60);
Serial.print(" min "); Serial.print(studyTime%60); Serial.println(" sec");
  Serial.print("Break Time: "); Serial.print(breakTime/60);
  Serial.print(" min "); Serial.print(breakTime%60); Serial.println(" sec");
  Serial.print("Breaks: "); Serial.println(breaks);
  Serial.print("Environment Score: "); Serial.print(score); Serial.println("/100");
  Serial.print("Status: "); Serial.println(status());
  Serial.print("Recommendation: "); Serial.println(recommendation());
  Serial.print("Efficiency: "); Serial.println(efficiency());
  Serial.println("----------------------");
}
void setup(){
  Serial.begin(9600);
  dht.begin();
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  Wire.begin(D2,D1);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("SMART STUDY");
  lcd.setCursor(0,1); lcd.print("SYSTEM READY");
  delay(2000);
  readSensors();
  showLCD();
  lastSec=millis();
  lastRead=millis();
  lastLCD=millis();
}
void loop(){
  unsigned long now=millis();
  if(now-lastRead>=2000){
    lastRead=now;
    readSensors();
    report();
  }
  updateTimer();
  if(now-lastLCD>=3000){
    lastLCD=now;
    page++;
    if(page>5) page=0;
    showLCD();
  }
}