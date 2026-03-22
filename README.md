# 🏎️ RC CAR WITH NRF24L01
IDE (ARDUINO IDE)

## 🎮 RADIO RC (PROTOTYPE)

![Radio RC Image](.//assets/imgs/rc_car.jpg)

### 📦ITEMS
- 1x NRF24L01
- 3x Push Buttons 
- 2x JoySticks
- 1x Arduino nano 
- 2x 18650 (7.4v)
- 1x Step Down mp1584 "TO 5v"
- 1x AMS1117 Module "3.3v"
- 1x Electrolytic Capacitor 470µF (for nrf24)
- 1x Ceramic Capacitor 0.1µF/100nf (for nrf24)
- 1x Universal fiberglass PCB (10x10 cm)

### PINOUT (ARDUINO NANO)
- SPI(SCK = 13; MISO = 12; MOSI = 11;)
- NRF24(CE = 3; CSN = 4;)
- LEFT JOYSTICK KY-023
- - VRX = A0;
- - VRY = A1;
- - SW(BUTTON) = A2;
- RIGHT JOYSTICK KY-023
- - VRX = A3;
- - VRY = A4;
- - SW(BUTTON) = A5;
- button_left = 7;
- button_right = 9;
- button_up = 8;




## 🚗 Car RC (PROTOTYPE)
![Radio RC Image](.//assets/imgs/rc_controller.jpg)
## 🚗 Car RC 1.0
<p>
  <img src="./assets/imgs/RC_car_1.0.jpeg.png" width="45%">
  <img src="./assets/imgs/circuit.jpeg.png" width="45%">
</p>

### 📦 ITEMS
- 1x Arduino Pro Mini 
- 1x Driver TB6612FNG
- 1x NRF24L01
- 2x 18650 (7.4V)
- 4x DC Motors 3-6v
- 2x Flash leds 5mm
- 1x Passive Buzzer
- 1x AMS1117 Module "3.3v"
- 1x Step Down mp1584 "TO 5v"
- 1x Electrolytic Capacitor 470µF (for 18650)
- 1x Electrolytic Capacitor 1000µF (for VM_pin TB6612 and GND)
- 1x Electrolytic Capacitor 100µF (for NRF24)
- 1x Ceramic Capacitor 0,1µF/100nf (for NRF24)
- 1x Electrolytic Capacitor 1470µF (for CHANNELS/SERVO)
- 1X Transistor MOSFET IRFZ44N (SWITCH LEDS)
- 1x Resistor 10k PULLDOWN (GATE MOSFET  and GND)
- 1x Universal fiberglass PCB (9x7 cm)


### PINTOUT (ARDUINO PRO MINI) 
- SPI(SCK = 13; MISO = 12; MOSI = 11;)
- NRF24(CE = 9; CSN = 10;)
- LEDS(MOSFET) = A0;
- BUZZER = 3;
- SERVO(CAMERA) = A2;
- TB6612FNG:
- - STANDBY = VCC;
- - LEFT MOTOR:  
- - - AIN1 = 4;
- - - AIN2 = 2;
- - - PWMA = 5;
- - RIGHT MOTOR:
- - - BIN1 = 7;
- - - BIN2 = 8;
- - - PWMB = 6;
- SERVO(FRONT) = A1;
- CHANNELS REMAINING (A2, A3);








