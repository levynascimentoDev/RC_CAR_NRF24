#include <SPI.h>
#include <RF24.h>

const int l_engine_H = 8;
const int l_engine_L = 9;
const int l_engine_V = 10;
const int r_engine_H = 6;
const int r_engine_L = 7;
const int r_engine_V = 5;
// const int buzzer_pin;
const int leds_pin1 = A1;
const int leds_pin2 = A0;


const int CE_pin = 3;
const int CSN_pin = 2;

RF24 radio(CE_pin, CSN_pin);

struct RCpackage {
  bool buzzer;
  bool leds;
  int16_t direction_x;
  int16_t direction_y;
  int16_t servo_x;
};

unsigned long lastPacketTime = 0;
const unsigned long TIMEOUT = 500;

int freq = 1000;
int step = 20; 
bool ascending = true;


const byte ADDRESS[6] = "32802";

RCpackage package;

void setup() {

  package.buzzer = false;
  package.leds = false;
  package.servo_x = 0;
  package.direction_x = 0;
  package.direction_y = 0;

  pinMode(l_engine_H, OUTPUT);
  pinMode(l_engine_L, OUTPUT);
  pinMode(r_engine_H, OUTPUT);
  pinMode(r_engine_L, OUTPUT);
  pinMode(l_engine_V, OUTPUT);
  pinMode(r_engine_V, OUTPUT);
  // pinMode(buzzer_pin, OUTPUT);
  pinMode(leds_pin1, OUTPUT);
  pinMode(leds_pin2, OUTPUT);
  
  analogWrite(r_engine_V, 0);
  analogWrite(l_engine_V, 0);
  digitalWrite(l_engine_H, HIGH);
  digitalWrite(r_engine_H, HIGH);
  digitalWrite(l_engine_L, LOW);
  digitalWrite(r_engine_L, LOW);

  radio.begin();
  radio.setChannel(100);
  radio.openReadingPipe(0, ADDRESS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  Serial.begin(9600);
  radio.setDataRate(RF24_250KBPS);

}

void loop() {
  
  if (radio.available()) {
    
    radio.read(&package, sizeof(package));

    Serial.print("x: ");
    Serial.println(package.direction_x);
    Serial.print("y: ");
    Serial.println(package.direction_y);
    lastPacketTime = millis();
  }

  if (millis() - lastPacketTime > TIMEOUT) {
    package.direction_x = 0;
    package.direction_y = 0;
    package.buzzer = false;
    package.leds = false;
  }
  moveCAR();
  // alarm();
  lights();
  // delay(500);
}

void lights() {

  digitalWrite(leds_pin1, package.leds ? HIGH : LOW);
  digitalWrite(leds_pin2, package.leds ? HIGH : LOW);

}

void alarm() {
  // if (package.buzzer) {

  //   tone(buzzer, freq);

  //   if (ascending) {
  //     freq += step;
  //     if (freq >= 2000) ascending = false;
  //   } else {
  //     freq -= step;
  //     if (freq <= 1000) ascending = true;
  //   }
    
  // } else {
  //   noTone(buzzer);
  // }

}

void moveToFront() {
  digitalWrite(l_engine_H, HIGH);
  digitalWrite(l_engine_L, LOW);
  digitalWrite(r_engine_H, HIGH);
  digitalWrite(r_engine_L, LOW);
}

void moveToBack() {
  digitalWrite(l_engine_H, LOW);
  digitalWrite(l_engine_L, HIGH);
  digitalWrite(r_engine_H, LOW);
  digitalWrite(r_engine_L, HIGH);
}

void moveCAR() {
  if ((package.direction_y > 0) || (package.direction_x >= 255)) {
    moveToFront();
  } else if ((package.direction_y > 0) || (package.direction_x >= -255)) {
    moveToBack();
  } else {
    analogWrite(r_engine_V, 0);
    analogWrite(l_engine_V, 0);
  }

  if (package.direction_x < 0) {

    int speed = abs(package.direction_y);
    int turn = abs(package.direction_x);
    analogWrite(r_engine_V, constrain(speed - turn, 0, 230));
    analogWrite(l_engine_V, constrain(speed, 0, 230));

  } else if (package.direction_x > 0) {

    int speed = abs(package.direction_y);
    int turn = abs(package.direction_x);
    analogWrite(l_engine_V, constrain(speed - turn, 0, 230));
    analogWrite(r_engine_V, constrain(speed, 0, 230));

  } else {
    uint8_t speed = abs(package.direction_y);
    analogWrite(r_engine_V, constrain(speed, 0, 230));
    analogWrite(l_engine_V, constrain(speed, 0, 230));
  }
}



