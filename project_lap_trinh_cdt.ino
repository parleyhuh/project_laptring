#include "DHT.h"

const int DHTPIN = 4;      
const int DHTTYPE = DHT11; 

#define GAMMA 0.7
#define RL10 50

class ldrSensor{
private:
int ldrInput;
public:
ldrSensor(int n):ldrInput(n){}


int readSensor(){
  return analogRead(ldrInput);
}
float luxCalculate(){
  int analogValue = readSensor();
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  return lux;
}
};

class mayBom{

private:

  int mayBom_pin;
  double luuLuong;

public:
  mayBom(int m , int l): mayBom_pin(m), luuLuong(l){}
  
  void turnOn(){
    digitalWrite(mayBom_pin,LOW);
  }

  void turnOff(){
    digitalWrite(mayBom_pin,HIGH);
  }

  void On(int v){
    digitalWrite(mayBom_pin,LOW);
    delay(v/luuLuong * 1000);
    digitalWrite(mayBom_pin,HIGH);
  }
};

class leds{
private:

  int led_pin;
  char color;

public:

  leds(int n, char r):led_pin(n),color(r){}

  void led_on(){
    digitalWrite(led_pin,HIGH);
    
  }

  void led_off(){
    digitalWrite(led_pin,LOW);
  }

  void led_write(int brightness){
    digitalWrite(led_pin,brightness);
    return;
  }

  char ledName(){
    return color;
  }

};

class beCa {
  double theTich;
  leds &led1,&led2;
  mayBom &my_mayBom;
  ldrSensor &ldr1;
  DHT &dht;

public:
beCa(int t, leds &l1, leds &l2, mayBom &m): theTich(t), led1(l1), led2(l2), my_mayBom(m){}


  void turnOnleds(){
    led1.led_on();
    led2.led_on();
  }

  void turnOffleds(){
    led1.led_off();
    led2.led_off();
  }
  void mayBomOn(){
    my_mayBom.turnOn();
  }
  void mayBomOff(){
    my_mayBom.turnOff();
  }

  void bomTuDong(double p = 100){
    my_mayBom.On((p/100)*theTich);
  }
 void dieuChinhNhietDo(float t , float t0){
    if(t > t0){
      my_mayBom.turnOn();
      delay(10000);
      my_mayBom.turnOff();
    }
 }

 void dieuchinhAnhsang(float l){
   if(l < 5000 ){
     led1.led_on();
     led2.led_on();
   }
   else{
      led1.led_off();
      led2.led_off();
     
   }
 }
};

mayBom my_mayBom(13,10);
leds led1(2,'r'),led2(8,'b');
beCa my_beCa(100,led1,led2,my_mayBom);
DHT dht(DHTPIN, DHTTYPE);
ldrSensor ldr1(A0);

void setup() {

pinMode(13, OUTPUT);
pinMode(2,OUTPUT);
pinMode(8,OUTPUT);
Serial.begin(9600);
dht.begin();

digitalWrite(13,HIGH);
}

void loop() {
float h = dht.readHumidity();
float t = dht.readTemperature();
int analogValue = ldr1.readSensor();
float result = ldr1.luxCalculate();


if (Serial.available()){
    char cmd = Serial.read();
    int n = Serial.parseInt();
    write_mayBom(cmd,n);
    write_leds(cmd,n);

    if(cmd == 'k')
    {   
 //     my_beCa.dieuChinhNhietDo(t);
      return;
    }

    if(cmd = 'f'){
 //   my_beCa.dieuchinhAnhsang(result);
     return;
    }    
}

Serial.print(t);
Serial.flush();
delay(3000);
}

inline void write_mayBom(char a,int b){
  if(a == 'o'){
    my_beCa.bomTuDong(100);
    return;
  }

  if(a == 'c'){
  my_beCa.bomTuDong(b);
  return;
  }

  if(a == 'm'){
    my_beCa.mayBomOn();
    return;
  }
  
  if(a == 'n' ){
    my_beCa.mayBomOff();
    return;
  }
}

inline void write_leds(char c , int n){
  if(c == 'a'){
   my_beCa.turnOnleds();
    return;
  }
  if(c == 'b'){
    my_beCa.turnOffleds();
    return;
  }

  if(c == led1.ledName()){
    led1.led_write(n);
  }
}