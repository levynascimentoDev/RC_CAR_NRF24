#include <SPI.h>
#include <RF24.h>

// LEFT JOYSTICK 
const int l_stk_x = A0;
const int l_stk_y = A1;
const int l_stk_b = A2;

// RIGHT JOYSTICK 
const int r_stk_x = A3;
const int r_stk_y = A4;
const int r_stk_b = A5;

// PINS NRF24 
const int CE_pin = 3;
const int CSN_pin = 4;

// BUTTONS PIN
const int button_left = 7; 
const int button_right = 9; 
const int button_up = 8; 

unsigned long buttonTimeout = 0;

struct RCpackage {
  bool buzzer;
  bool leds;
  int16_t direction_x;
  int16_t direction_y;
  int16_t servo_x;
};
 
RCpackage package = {
  false,
  false,
  0,
  0,
  90
};

RF24 radio(CE_pin, CSN_pin);


const byte ADDRESS[6] = "32802";

void setup() {

  // DEFINE LEFT JOYSTICK 
  pinMode(l_stk_x, INPUT);
  pinMode(l_stk_y, INPUT);
  pinMode(l_stk_b, INPUT_PULLUP);

  // DEFINE RIGHT JOYSTICK 
  pinMode(r_stk_x, INPUT);
  pinMode(r_stk_y, INPUT);
  pinMode(r_stk_b, INPUT_PULLUP);

  // NRF24 INITALIZED AND CONFIGURED
  radio.begin();
  radio.setChannel(100);
  radio.openWritingPipe(ADDRESS);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  radio.setAutoAck(false);
}

void loop() {
  // GET VALUES PACKAGE
  package.direction_x = map(analogRead(r_stk_x), 0, 1024, -255, 255);
  package.direction_y = map(analogRead(l_stk_y), 0, 1024, -255, 255);
  package.servo_x = map(analogRead(l_stk_x), 0, 1024, 0, 180);

  // BUTTON LEFT (CLICK)
  if (!digitalRead(l_stk_b)) {
    if (millis() - buttonTimeout > 500) {
      package.leds = !package.leds;
      buttonTimeout = millis();

    }
  }

  // BUTTON RIGHT (CLICK)
  if (!digitalRead(r_stk_b)){
    if (millis() - buttonTimeout > 500) {
      package.buzzer = !package.buzzer;
      buttonTimeout = millis();
    }
  }
    
  // PREVENT FLOATIING VALUES (JOISTICK) 
  if (abs(package.direction_x) < 20) package.direction_x = 0;
  if (abs(package.direction_y) < 20) package.direction_y = 0;
  if (abs(package.servo_x-90) < 50) package.servo_x = 90;

  // SEND PACKAGE TO RECEIVER 
  radio.write(&package, sizeof(package));
    
}
