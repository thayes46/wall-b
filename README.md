# wall-b
Final project for ECE 4180 Spring 2022

Group: Todd Hayes and Kevin Xiao

## Description
This project is to design and create a 2-wheeled self balancing robot that can be directed by a bluetooth app to go forwards, backwards, or rotate. Building a balancing robot on two wheels is essentially trying to control an inverted pendulum, which is unstable in all except a perfectly balanced upright position, and it is further an underactuated system, increasing difficulty. Any erronious input, or even the user's inputs may occasionally cause it to tip too far in one direction, causing it to either fall over or run away attempting to correct it's balance. Because of this the system must respond with low latency to any disturbances in IMU readings to remain balanced.

## Parts list
- Custom 3d printed chassis + counterweights
- [Mbed](https://www.sparkfun.com/products/9564)
- [Slide switch](https://www.sparkfun.com/products/9609)
- [IMU](https://os.mbed.com/components/LSM9DS1-IMU)
- [Bluetooth module](https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-)
- [Battery pack and batteries](https://www.sparkfun.com/products/9835)
- [Barrel Jack](https://www.sparkfun.com/products/10811)
- [Motors + Gearboxes](https://www.sparkfun.com/products/13302)
- [Wheels](https://www.sparkfun.com/products/13259)
- [Dual H-Bridge](https://www.sparkfun.com/products/14451)


## Schematic
![2022-04-29 16 44 05 docs google com 511927e96488](https://user-images.githubusercontent.com/55037136/166066730-9dda018b-f966-41b9-b5d9-93892dc0d661.jpg)

## Code
[Main code](main.cpp)

## Demo video
