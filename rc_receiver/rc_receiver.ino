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


class RC_Car {

  private:
    // engines pins
    int l_engine_H;
    int l_engine_L;
    int r_engine_H;
    int r_engine_L;
  
    // pwm pins (SPEED)
    int r_engine_V;
    int l_engine_V;
    
    // led pin
    int LED_pin;

    // buzzer pin 
    int BUZZER_pin;

    // buzzer config
    int freq = 1000;
    bool ascending = true;
    unsigned long lastToneChange = 0;

    //servo class instancied
    Servo myservo;


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
      package.servo_x = 90;
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

      // SERVO 
      myservo.write(90);
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

    void setLed(int ledPin) {
      LED_pin = ledPin;
      pinMode(LED_pin, OUTPUT);
    }

    void setBuzzer(int buzzerPin) {
      BUZZER_pin = buzzerPin;
      pinMode(BUZZER_pin, OUTPUT);
    }

    void setServo(int servoPin) {
      myservo.attach(servoPin);
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
        // TURN TO RIGHT
        int speed = abs(package.direction_y);
        int turn = abs(package.direction_x);

        if (package.direction_y == 0) {
          invertRotateRight();
        }
        // IF DIRECTION Y = 0 SET ONLY TURN ELSE SET SPEED - TURN 
        analogWrite(r_engine_V, constrain(package.direction_y ? speed -turn : turn, 0, 230)); // RIGHT MOTOR 
        // IF DIRECTION Y = 0 SET MAX_SPEED ELSE SET SPEED_JOISTICK Y   
        analogWrite(l_engine_V, package.direction_y != 0 ? constrain(speed, 0, 230) : 230); // LEFT MOTOR 

      } else if (package.direction_x < 0) {

        // TURN TO LEFT   
        int speed = abs(package.direction_y);
        int turn = abs(package.direction_x);

        if (package.direction_y == 0) {
          invertRotateLeft();
        }
        
        // IF DIRECTION Y = 0 SET ONLY TURN ELSE SET SPEED - TURN 
        analogWrite(l_engine_V, constrain(package.direction_y ? speed -turn : turn, 0, 230)); // LEFT MOTOR 
        // IF DIRECTION Y = 0 SET MAX_SPEED ELSE SET SPEED_JOISTICK Y
        analogWrite(r_engine_V, package.direction_y != 0? constrain(speed, 0, 230) : 230); // RIGHT MOTOR 

      } else {
        // MOVE TO FORWARD
        int speed = abs(package.direction_y);
        analogWrite(r_engine_V, constrain(speed, 0, 230));
        analogWrite(l_engine_V, constrain(speed, 0, 230));
      }
    }
  
    void alarm() {
      
      if (package.buzzer) {
        tone(BUZZER_pin, freq);

        if (millis() - lastToneChange > 30) {
          lastToneChange = millis();

          if (ascending) {
            freq += 20;
            if (freq >= 1500) ascending = false;
          } else {
            freq -= 20;
            if (freq <= 600) ascending = true;
          }
        }

      } else {
        noTone(BUZZER_pin);
      }
    }

    void lights() {
      digitalWrite(LED_pin, package.leds? HIGH : LOW);
    }
    void head() {
      myservo.write(package.servo_x);
    }

};

// INITAL CONFIGS 
RF24 radio(9, 10); // SPI CONFIG (SCK = 13; MISO = 12; MOSI = 11;) 
RC_Car mycar;

// LOGICAL MILLIS VARIABLES
unsigned long lastPacketTime = 0;
const byte ADDRESS[6] = "32802";

void setup() {

  // STANDBY PIN ON VCC 

  // CAR BEGIN 
  mycar.setLeftEngine(8, 7, 6);
  mycar.setRightEngine(4, 2, 5);
  mycar.setBuzzer(3);
  mycar.setServo(A1);
  mycar.setLed(A0);
  mycar.begin();

  // NRF24 BEGIN 
  if (!radio.begin()) {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    while (1);
  }
  radio.setChannel(100);
  radio.openReadingPipe(0, ADDRESS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.startListening();
}

void loop() {
  
  if (radio.available()) {
    radio.read(&mycar.package, sizeof(mycar.package));
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




