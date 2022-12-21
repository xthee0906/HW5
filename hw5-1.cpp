#include "mbed.h"
#include "bbcar.h"

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmOut pin9(D10), pin10(D9);
PwmIn pin11(D12), pin12(D11);
BBCar car(pin9, pin11, pin10, pin12, servo_ticker, servo_feedback_ticker);
BusInOut qti_pin(D4,D5,D6,D7);

BufferedSerial pc(USBTX, USBRX);
DigitalInOut ping(D12);
Timer t;

double ia0 , c , a, pre;

int main() {

   float val;
   pc.set_baud(9600);


   while(1) {

      ping.output();
      ping = 0; wait_us(200);
      ping = 1; wait_us(5);
      ping = 0; wait_us(5);

      ping.input();
      while(ping.read() == 0);
      t.start();
      while(ping.read() == 1);
      val = t.read();
      if ((val-pre) >= 5) {
        car.feedbackWheel();
        ia0 = car.servo0.angle;
      } else if ((pre-val) >= 5) {
            car.feedbackWheel();
            a = abs(car.servo0.angle-ia0);
            printf("W=""%lf\n", 2*val*17150.0f*sin(a));
            car.stop();
            break;
      }

      //printf("Ping = %lf\r\n", val*17700.4f);
      printf("Laser Ping = %lf\r\n", val*17150.0f); //Laser Ping's distance
      car.turn(45, 0.1);
      pre = val*17150.0f;

      t.stop();
      t.reset();

      ThisThread::sleep_for(1s);
   }
}