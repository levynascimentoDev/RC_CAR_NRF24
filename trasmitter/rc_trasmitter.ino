#include <SPI.h>
#include <RF24.h>

const int l_stk_x = A0;
// const int r_stk_x ;
const int l_stk_y = A1;
// const int r_stk_y;
const int sw_leds = 9;
// const int button_buzzer;

const int CE_pin = 3;
const int CSN_pin = 4;

unsigned long buttonTimeout = 0;

struct RCpackage {
  bool buzzer;
  bool leds;
  int16_t direction_x;
  int16_t direction_y;
  int16_t servo_x;
};

RCpackage package;
RF24 radio(CE_pin, CSN_pin);

const byte ADDRESS[6] = "32802";

void setup() {
  pinMode(l_stk_x, INPUT);
  // pinMode(r_stk_x, INPUT);
  pinMode(l_stk_y, INPUT);
  // pinMode(r_stk_y, INPUT);
  pinMode(sw_leds, INPUT_PULLUP);
  // pinMode(button_buzzer, INPUT);

  // package.buzzer = digitalRead(sw_leds) ? true : false;

  package.servo_x =0;
  package.buzzer = false;
  package.leds = false;

  radio.begin();
  radio.setChannel(100);
  radio.openWritingPipe(ADDRESS);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  Serial.begin(9600);

}

void loop() {
  
  package.direction_x = map(analogRead(l_stk_x), 0, 1024, -255, 255);
  package.direction_y = map(analogRead(l_stk_y), 0, 1024, -255, 255);
  // package.servo_x = map(analogRead(r_stk_x), 0, 1024, -255, 255);
  // package.buzzer = digitalRead(button_buzzer) ? true : false;
  if (!digitalRead(sw_leds)) {
    if (millis() - buttonTimeout > 500) {

      package.leds = !package.leds;
      buttonTimeout = millis();
    }
  }
  Serial.println(package.leds);

  if (abs(package.direction_x) < 20) package.direction_x = 0;
  if (abs(package.direction_y) < 20) package.direction_y = 0;
  if (abs(package.servo_x) < 20) package.servo_x = 0;
  
  radio.write(&package, sizeof(package));
  
}
