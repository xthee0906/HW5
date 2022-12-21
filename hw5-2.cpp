#include "mbed.h"
#include "bbcar.h"

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmOut pin9(D10), pin10(D9);
PwmIn pin11(D12), pin12(D11);
BBCar car(pin9, pin11, pin10, pin12, servo_ticker, servo_feedback_ticker);
BusInOut qti_pin(D4,D5,D6,D7);

int main() {

   parallax_qti qti1(qti_pin);
   int pattern;
   int a = 0;
   double d0,d1,ia0,ia1;

   car.goStraight(50);
   car.feedbackWheel();
   ia0 = car.servo0.angle;
   ia1 = car.servo1.angle;
   while(1) {
      pattern = (int)qti1;
      //printf("%d\n",pattern);

      switch (pattern) {
         case 0b1000: car.turn(50, 0.1); a=0; break;
         case 0b1100: car.turn(50, 0.5); a=0; break;
         case 0b0100: car.turn(50, 0.7); a=0; break;
         case 0b0110: car.goStraight(50); a=0; break;
         case 0b0010: car.turn(50, -0.7); a=0; break;
         case 0b0011: car.turn(50, -0.5); a=0; break;
         case 0b0001: car.turn(50, -0.1); a=0; break;
         case 0b1111: 
            if (a==0) {
                car.stop(); 
                a++;
                ThisThread::sleep_for(1s);
                car.feedbackWheel();
                d0 = 2*3.14*3*(abs(car.servo0.angle-ia0)/360);
                d1 = 2*3.14*3*(abs(car.servo1.angle-ia1)/360);
                printf("distance = %lf\n", (d0+d1)/2);
                break;
            } else {
                car.goStraight(45);
                ThisThread::sleep_for(1s);
                car.feedbackWheel();
                ia0 = car.servo0.angle;
                ia1 = car.servo1.angle;
                break;
            }
         default: car.goStraight(50);
      }
      ThisThread::sleep_for(10ms);
   }
}