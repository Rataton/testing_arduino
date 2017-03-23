#include <Servo.h>

Servo servo1, // Base giro
      servo2, // Base
      servo3, // Codo
      servo4; // Mano

int ang1 = 0,
    ang2 = 90,
    ang3 = 90,
    ang4 = 90,

    topeA1 = 0,
    topeA2 = 20,
    topeA3 = 0,

    topeB1 = 180,
    topeB2 = 160,
    topeB3 = 0,

    topeA4 = 80,
    topeB4 = 160,
    spd = 10
          ;

void setup() {
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(7);

  servo1.write(ang1);
  servo2.write(ang2);
  servo3.write(ang3);
  servo4.write(ang4);
}

void loop() {
/*
  for (ang1 = topeA1; ang1 <= topeB1; ang1 += 1) {
    servo1.write(ang1);
    delay(spd);
  }
  for (ang1 = topeB1; ang1 >= topeA1; ang1 -= 1) {
    servo1.write(ang1);
    delay(spd);
  }
  */
  for (ang3 = topeA3; ang3 <= topeB3; ang3 += 1) {
    servo3.write(ang3);
    delay(spd);
  }
  for (ang3 = topeB3; ang3 >= topeA3; ang3 -= 1) {
    servo3.write(ang3);
    delay(spd);
  }
  
  /*
  for (ang4 = topeA4; ang4 <= topeB4; ang4 += 1) {
    servo4.write(ang4);
    delay(spd);
  }
  for (ang4 = topeB4; ang4 >= topeA4; ang4 -= 1) {
    servo4.write(ang4);
    delay(spd);
  }*/
}