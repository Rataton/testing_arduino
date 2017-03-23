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
    ang2 = 20,
    ang3 = 0,
    ang4 = 0,
    // Límites de los movimientos de las articulaciones
    topeA1 = 0,
    topeA2 = 20,
    topeA3 = 0,
    topeA4 = 0,
    topeB1 = 0,
    topeB2 = 170,
    topeB3 = 0,
    topeB4 = 0;
int spd = 100;

void setup() {
  Serial.begin(9600); // Debugging only
  lcd.begin();
  lcd.backlight();
  lcd.print("-prueboServos v.0-");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando desu");

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  pinMode(PLED, OUTPUT);
  digitalWrite(PLED, false);

  servo1.attach(4);
  // servo2.attach(8);
  // servo3.attach(9);
  // servo4.attach(10);
  servo1.write(ang1);
}
void loop() {
 for (ang1 = 0; ang1 <= 180; ang1 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(ang1);              // tell servo to go to ang1ition in variable 'ang1'
    delay(15);                       // waits 15ms for the servo to reach the ang1ition
  }
  for (ang1 = 180; ang1 >= 0; ang1 -= 1) { // goes from 180 degrees to 0 degrees
    servo1.write(ang1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
