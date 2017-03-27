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
    topeB1 = 3600,
    topeA2 = 900,
    topeB2 = 2000,
    topeA3 = 1000,
    topeB3 = 2000,

    topeA4 = 1400,
    topeB4 = 2200,
    spd = 20,
    stp = 10;
int s1 = 0,
    s2 = 0,
    s3 = 0,
    s4 = 0;

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
  // Inicializa lo servos y los posiciona en su punto medio
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(7);
  ang1 = (topeA1 + topeB1) / 2;
  ang2 = (topeA2 + topeB2) / 2;
  ang3 = (topeA3 + topeB3) / 2;
  ang4 = (topeA4 + topeB4) / 2;
  lcd.print("1:");
  lcd.print(ang1);
  lcd.print(" 2:");
  lcd.print(ang2);
  lcd.print(" 3:");
  lcd.print(ang3);
  lcd.print(" 4:");
  lcd.print(ang4);
  servo1.write(ang1);
  servo2.write(ang2);
  servo3.write(ang3);
  servo4.write(ang4);
}
void loop() {
  n++;
  if (n == 1000) {
    digitalWrite(PLED, true);
    delay(200);
    digitalWrite(PLED, false);
    n = 200;
  }
  // Lee instrucciones remotas
  uint8_t buf;
  uint8_t buflen = 1;
  if (vw_get_message(&buf, &buflen)) {
    lcd.clear();
    lcd.print("- servoRF v.0 - ");
    lcd.setCursor(0, 1);
    switch ((char)buf) {
      case 'a':
        s1 = 1;
        lcd.print("Giro va - ");
        lcd.print(s1);
        break;
      case 'z':
        s1 = 0;
        lcd.print("Giro viene - ");
        lcd.print(s1);
        break;
      case 'b':
        s2 = 1;
        lcd.print("Base va - ");
        break;
      case 'y':
        s2 = 0;
        lcd.print("Base viene - ");
        break;
      case 'c':
        s3 = 1;
        lcd.print("Codo va - ");
        break;
      case 'x':
        s3 = 0;
        lcd.print("Codo viene - ");
        break;
      case 'd':
        s4 = 1;
        lcd.print("Mano va - ");
        break;
      case 'w':
        s4 = 0;
        lcd.print("Mano viene - ");
        break;
    }
  }
  // Aplica reglas paraavanzar y regresar
  if (s1 == 0) {
    if (ang1 > topeA1) {
      ang1 -= spd;
    }
  } else if (s1 == 1) {
    if (ang1 < topeB1) {
      ang1 += spd;
    }
  }
  if (s2 == 0) {
    if (ang2 > topeA2) {
      ang2 -= spd;
    }
  } else if (s2 == 1) {
    if (ang2 < topeB2) {
      ang2 += spd;
    }
  }
  if (s3 == 0) {
    if (ang3 > topeA3) {
      ang3 -= spd;
    }
  } else if (s3 == 1) {
    if (ang3 < topeB3) {
      ang3 += spd;
    }
  }
  if (s4 == 0) {
    if (ang4 > topeA4) {
      ang4 -= spd;
    }
  } else if (s4 == 1) {
    if (ang4 < topeB4) {
      ang4 += spd;
    }
  }
  // Escribe valores en los servos
  servo1.write(ang1);
  servo2.write(ang2);
  servo3.write(ang3);
  servo4.write(ang4);

  delay(1);
}