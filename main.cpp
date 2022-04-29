#include "mbed.h"
#include "rtos.h"
#include "LSM9DS1.h"
#include "Motor.h"
#define PI 3.14159
// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -4.94 // Declination (degrees) in Atlanta,GA.
/**
4180 final project


TODO:
[] Setup PID controller for wheel outputs
    - Listen for globals FORW, BACK, LEFT, RGHT


*/
volatile bool FORW = false;
volatile bool BACK = false;
volatile bool LEFT = false;
volatile bool RGHT = false;
DigitalOut myled(LED1); // flash to show working
DigitalOut ledDir1(LED2);
DigitalOut ledDir2(LED3);
DigitalOut ledStby(LED4);

Motor left(p21, p22, p23); // TODO: fix post schematic pwm, fwd, rev
Motor right(p26, p25, p24); // TODO: fix post schematic

volatile float speed = 0;
volatile float rollAcc = 0; // in deg
volatile float rollGyro = 0; // in deg/s
volatile float inclAngle = 0; // deg
volatile float delta = 0;
volatile float correction = 0;
volatile float destinationTheta = 86.5; // deg, might change if going foward/backward
volatile float ldx = 0;
volatile float rdx = 0;
volatile unsigned int cnt = 1;

LSM9DS1 IMU(p9, p10, 0xD6, 0x3C); // TODO: fix pins

Serial blue(p28,p27); //TODO: fix pins



void balance() {
    while(1) {
        // Reads IMU and updates PID accordingly
        // Checks for direction globals to add/subtract speed from motors accordingly
        if (IMU.gyroAvailable() && IMU.accelAvailable()) {
        //while(!IMU.gyroAvailable());
//            IMU.readGyro();
//        while(!IMU.accelAvailable());
//            IMU.readAccel();
            IMU.readGyro();
            IMU.readAccel();
            rollAcc = -1*atan2(IMU.calcAccel(IMU.ax), IMU.calcAccel(IMU.az))*180/PI; // in deg
            rollGyro = -1 * IMU.calcGyro(IMU.gy) * 0.01; 
            inclAngle = 0.98*(inclAngle + rollGyro)+0.02*rollAcc;
            correction = inclAngle - correction;
            speed = (20*(destinationTheta - inclAngle) + 0.5*correction)/120;
            if (!LEFT && !RGHT && !FORW && !BACK && cnt%20 != 0) {
                left.speed(speed);
                right.speed(speed);
            } else {
                left.speed(speed + ldx);
                right.speed(speed + rdx);
            }
            cnt+=1;
        } else {
            while (1) {
                myled = 0;
                Thread::wait(500);
                myled = 1;
                Thread::wait(500);
            }
        }
    }
}

void listenBluetooth() {
    while(1) {
        // Checks for inputs from app
        // Sets globals
        char bnum;
        char bhit;
        if (blue.getc()=='!') {
            if (blue.getc()=='B') { //button data packet
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                    switch (bnum) {
                        case '5': //button 5 up arrow
                            if (bhit=='1') {
                                FORW = true;
                                ldx = 0.4;
                                rdx = 0.4;
                            } else {
                                FORW = false;
                            }
                            break;
                        case '6': //button 6 down arrow
                            if (bhit=='1') {
                                BACK = true;
                                ldx = -0.4;
                                rdx = -0.4;
                            } else {
                                BACK = false;
                            }
                            break;
                        case '7': //button 7 left arrow
                            if (bhit=='1') {
                                LEFT = true;
                                ldx = 0.3;
                                rdx = -0.3;
                            } else {
                                LEFT = false;
                            }
                            break;
                        case '8': //button 8 right arrow
                            if (bhit=='1') {
                                RGHT = true;
                                ldx = -0.3;
                                rdx = 0.3;
                            } else {
                                RGHT = false;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }   
        // Thread::wait(1);
    }
}

void blink(){
    myled = !myled;
    
    Thread::wait(1);
}

int main() {
//    IMU.begin();
    if (!IMU.begin()) {
        while(1) { // flash quickly to indicate error
            myled = !myled;
            wait(.001);
        }
    }
    IMU.calibrate();
    myled = 1;
    wait(1);

    Thread t2;
    Thread t3;
    
    t2.start(balance);
    t3.start(listenBluetooth);
    while(1) {
        //myled = !myled;
        if (FORW) {
            ledDir1 = 0;
            ledDir2 = 0;
            ledStby = 0;
        } 
        else if (RGHT) {
            ledDir1 = 1;
            ledDir2 = 0;
        } 
        else if (BACK){ 
            ledDir1 = 1;
            ledDir2 = 1;
            ledStby = 0;
        }
        else if (LEFT){ 
            ledDir1 = 0;
            ledDir2 = 1;
            ledStby = 0;
        }
        else {
            ledDir1 = 0;
            ledDir2 = 0;
            ledStby = 1;
        }
        wait(0.1);
    }
}
