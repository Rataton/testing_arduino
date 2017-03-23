// this sketch cycles three servos at different rates 

#include <VirtualWire.h>
#include <ServoTimer2.h>  // the servo library

// define the pins for the servos
#define s1 4
#define s2 5
#define s3 6

ServoTimer2 servo1;    // declare variables for up to eight servos
ServoTimer2 servo2; 
ServoTimer2 servo3; 
 
void setup() {
  servo1.attach(s1);     // attach a pin to the servos and they will start pulsing
  servo2.attach(s2); 
  servo3.attach(s3); 
}


// this function just increments a value until it reaches a maximum 
int incPulse(int val, int inc){
   if( val + inc  > 2000 )
      return 1000 ;
   else
       return val + inc;  
}

void loop()
{ 
 int val;
  
   val = incPulse( servo1.read(), 1);
   servo1.write(val);

   val =  incPulse( servo2.read(), 2);
   servo2.write(val);
   
   val = incPulse(servo3.read(), 4);
   servo3.write(val);
   
   delay(10);   
}