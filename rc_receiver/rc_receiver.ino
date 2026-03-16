#include <SPI.h>
#include <RF24.h>
#include <Servo.h>


struct RCpackage {
  bool buzzer;
  bool leds;
  int16_t direction_x;
  int16_t direction_y;
  int16_t servo_x;
};
const int buzzer_pin = 3;
const int leds = A1;
Servo myservo;

class RC_Car {

  private:
    // engines pins
    int l_engine_H;
    int l_engine_L;
    int r_engine_H;
    int r_engine_L;
  
    // pwm pins (VELOCITY)
    int r_engine_V;
    int l_engine_V;

    // buzzer config
    int freq = 1000;
    bool ascending = true;


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

    void invertRotateLeft() {
      digitalWrite(l_engine_H, LOW);
      digitalWrite(l_engine_L, HIGH);

      digitalWrite(r_engine_H, HIGH);
      digitalWrite(r_engine_L, LOW);
    }

    void invertRotateRight() {
      digitalWrite(r_engine_H, LOW);
      digitalWrite(r_engine_L, HIGH);

      digitalWrite(l_engine_H, HIGH);
      digitalWrite(l_engine_L, LOW);
    }

  public:
    RCpackage package;

    void begin() {

      package.buzzer = false;
      package.leds = false;
      package.servo_x = 0;
      package.direction_x = 0;
      package.direction_y = 0;

      // left
      analogWrite(l_engine_V, 0);
      digitalWrite(l_engine_H, HIGH);
      digitalWrite(l_engine_L, LOW);
      // right
      analogWrite(r_engine_V, 0);
      digitalWrite(r_engine_H, HIGH);
      digitalWrite(r_engine_L, LOW);
    }

    void setLeftEngine(int pinHIGH, int pinLOW, int pinVelocity) {
      // define pinMode
      l_engine_H = pinHIGH;
      l_engine_L = pinLOW;
      l_engine_V = pinVelocity;
    
      pinMode(l_engine_H, OUTPUT);
      pinMode(l_engine_L, OUTPUT);
      pinMode(l_engine_V, OUTPUT);
    }

    void setRightEngine(int pinHIGH, int pinLOW, int pinVelocity) {
      // define pinMode
      r_engine_H = pinHIGH;
      r_engine_L = pinLOW;
      r_engine_V = pinVelocity;
    
      pinMode(r_engine_H, OUTPUT);
      pinMode(r_engine_L, OUTPUT);
      pinMode(r_engine_V, OUTPUT); 
    }

    
    void moveCar() {
      if (package.direction_y > 0) {
        moveToFront();
      } else if (package.direction_y < 0) {
        moveToBack();
      } else {
        analogWrite(r_engine_V, 0);
        analogWrite(l_engine_V, 0);
      }

      if (package.direction_x > 0) {

        int speed = abs(package.direction_y);
        int turn = abs(package.direction_x);
        
        if (package.direction_y == 0) {
          invertRotateRight();
        }
        analogWrite(r_engine_V, constrain(package.direction_y != 0 ? speed - turn : turn, 0, 230));
        analogWrite(l_engine_V, package.direction_y != 0 ? constrain(speed, 0, 230) : 230);

      } else if (package.direction_x < 0) {

        int speed = abs(package.direction_y);
        int turn = abs(package.direction_x);
        if (package.direction_y == 0) {
          invertRotateLeft();
        }
        analogWrite(l_engine_V, constrain(package.direction_y ? speed -turn : turn, 0, 230));
        analogWrite(r_engine_V, package.direction_y != 0? constrain(speed, 0, 230) : 230);

      } else {
        int speed = abs(package.direction_y);
        analogWrite(r_engine_V, constrain(speed, 0, 230));
        analogWrite(l_engine_V, constrain(speed, 0, 230));
      }
    }
  
    void alarm() {
      if (package.buzzer) {
        tone(buzzer_pin, freq);

        if (ascending) {
          freq += 20;
          if (freq >= 1500) ascending = false;
        } else {
          freq -= 20;
          if (freq <= 600) ascending = true;
        }
        
      } else {
        noTone(buzzer_pin);
      }
    }

    void lights() {

      digitalWrite(leds, package.leds ? HIGH : LOW);

    }
    void head() {
      myservo.write(package.servo_x);
    }

};

RF24 radio(9, 10);
RC_Car mycar;

unsigned long lastPacketTime = 0;
const byte ADDRESS[6] = "32802";

void setup() {

  
  pinMode(A0, OUTPUT); // STANDBY PIN 
  digitalWrite(A0, HIGH); // STANDBY PIN (HIGH MOTOR ON)

  // LEDS
  pinMode(leds, OUTPUT);

  // BUZZER 
  pinMode(buzzer_pin, OUTPUT);

  // SERVO  
  myservo.attach(A2);

  // CAR BEGIN 
  mycar.setLeftEngine(8, 7, 6);
  mycar.setRightEngine(4, 2, 5);
  mycar.begin();

  // NRF24 BEGIN 
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
    radio.read(&mycar.package, sizeof(mycar.package));

    Serial.print("x: ");
    Serial.println(mycar.package.direction_x);
    Serial.print("y: ");
    Serial.println(mycar.package.direction_y);
    lastPacketTime = millis();
  }

  if (millis() - lastPacketTime > 500) {
    mycar.package.direction_x = 0;
    mycar.package.direction_y = 0;
    mycar.package.buzzer = false;
    mycar.package.leds = false;
    mycar.package.servo_x = 90;
  }
  mycar.moveCar();
  mycar.lights();
  mycar.alarm();
  mycar.head();
}




