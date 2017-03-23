#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VirtualWire.h>
#include <ServoTimer2.h>

ServoTimer2 servo1, // Base giro
            servo2, // Base
            servo3, // Codo
            servo4; // Mano

LiquidCrystal_I2C lcd(0x27, 16, 2);
int n = 0;
const int PLED = 13;

int ang1 = 0,
    ang2 = 0,
    ang3 = 0,
    ang4 = 0,

    topeA1 = 0,
    topeB1 = 180,

    topeA2 = 10,
    topeB2 = 140,

    topeA3 = 0,
    topeB3 = 180,

    topeA4 = 80,
    topeB4 = 160,

    spd = 10
    ;

void setup() {
  Serial.begin(9600); // Debugging only
  lcd.begin();
  lcd.backlight();
  lcd.print("- servoRF v.0 - ");
  lcd.setCursor(0, 1);

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  pinMode(PLED, OUTPUT);
  digitalWrite(PLED, false);

  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(7);

  ang1 = servo1.read();
  ang2 = servo2.read();
  ang3 = servo3.read();
  ang4 = servo4.read();
  
  lcd.print(ang1);
  lcd.print(", ");
  lcd.print(ang2);
  lcd.print(", ");
  lcd.print(ang3);
  lcd.print(", ");
  lcd.print(ang4);
    
  servo1.write(ang1);
  servo2.write(ang2);
  servo3.write(ang3);
  servo4.write(ang4);
}
void loop(){
  // uint8_t buf;
  // uint8_t buflen = 1;
  // if (vw_get_message(&buf, &buflen)) {
  //   lcd.clear();
  //   lcd.print("- servoRF v.0 - ");
  //   lcd.setCursor(0, 1);
  //   switch ((char)buf) {
  //     case 'a':
  //       digitalWrite(13, true);
  //       lcd.print("Giro va");
  //       break;
  //     case 'b':
  //       lcd.print("Base va");
  //       // gira(servo2, 1);
  //       break;
  //     case 'c':
  //       lcd.print("Codo va");
  //       // gira(servo3, 1);
  //       break;
  //     case 'd':
  //       lcd.print("Mano va");
  //       // gira(servo4, 1);
  //       break;
  //     case 'z':
  //       lcd.print("Giro viene");

  //       break;
  //     case 'y':
  //       lcd.print("Base viene");
  //       // gira(servo2, 0);
  //       break;
  //     case 'x':
  //       lcd.print("Codo viene");
  //       // gira(servo3, 0);
  //       break;
  //     case 'w':
  //       lcd.print("Mano viene");
  //       // gira(servo4, 0);
  //       break;
  //   }
  // } else {
    n++;
    if (n == 1000) {
      digitalWrite(PLED, true);
      delay(200);
      digitalWrite(PLED, false);
      n = 200;
    }
  // }
  delay(1);
}
