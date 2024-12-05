// RoverTest.ino
// FGCU Computing & Software Engineering
// Prof. Allen
// Test the basic Rover robot interfaces. Uses a set of states
// to run the wheels through each of the basic movements and
// adjusts the head to point in the direction of movement. The
// Serial inteface is used to output direction of movement, along
// with direction of the head and distance measured.
// ------------------------------------------------------------

#include "RoverHead.h"
#include "RoverWheels.h"
#include "StopWatch.h"

enum RoverWheelState {
  Stop,
  Straight,
  ShortStraight,
  CorrectLeft,
  TurnLeft,
  CorrectRight,
  TurnRight,
  Backup
};


fgcu::FourPin motorPinsLeft { A0, A1, A2, A3};
fgcu::FourPin motorPinsRight { 4, 5, 6, 7};
float speed = 600.f;
fgcu::RoverWheels wheels{motorPinsLeft, motorPinsRight, speed};

RoverWheelState wheelStates[] = { Straight, TurnLeft, TurnRight, CorrectLeft, CorrectRight, ShortStraight, Stop };
int wheelStateCount = 9;
int wheelStateIndex = 0;

const byte ServoPin = 9;
const byte EchoPin = A5;
const byte TriggerPin = A4;

fgcu::RoverHead head{EchoPin, TriggerPin, ServoPin};

int leftDistance = 0; 
int rightDistance = 0;

void setup() {

  delay(1000);

  Serial.begin(9600);

  head.turnHead(90);

  bool turning = true;
    do {
      turning = head.run();
    } while (turning);

} // setup


void loop() {

  head.run();
  if (!wheels.run()) { 

    delay(100);
    int frontDistance = getDistance(89);
    Serial.print("center Distance: ");
    Serial.print(frontDistance);
    delay(100);
    

    if (frontDistance < 15){

    leftDistance = getDistance(0); 
    rightDistance = getDistance(180); 

    Serial.print("Left Distance: ");
    Serial.println(leftDistance);
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);

    if (leftDistance > rightDistance) {
      wheelStateIndex = 1;
      moveWheels();
      delay(1000);
      head.turnHead(90);
    } else if (rightDistance > leftDistance) {
      wheelStateIndex = 2;
      moveWheels();
      delay(1000);
      head.turnHead(90);
    }
    
  }
   else {
      Serial.println("Moving forward.");
      wheelStateIndex = 0;
      moveWheels();
      delay(100); 
      leftDistance = getDistance(0); 
      rightDistance = getDistance(180); 
      if (leftDistance < 7){
        wheelStateIndex = 4;
        moveWheels();
        delay(1000);
      }
      if (rightDistance < 7){
        wheelStateIndex = 3;
        moveWheels();
        delay(1000);
      }
      head.turnHead(90);
      wheelStateIndex = 5;
      moveWheels();
    }
 
  }
}


int getDistance(int angle) {
  head.turnHead(angle);
  bool turning = true;
  do {
    turning = head.run();
  } while (turning);
  return head.getDistance();
}


void moveWheels() {

  switch (wheelStates[wheelStateIndex]) {
    case Stop:
      Serial.print("Stop\n");
      wheels.stop();
      break;
    case Straight:
      Serial.print("Straight\n");
      wheels.moveForward();
      head.turnHead(90);
      break;
    case TurnLeft:
      Serial.print("TurnLeft\n");
      wheels.turnLeft();
      break;

    case TurnRight:
      Serial.print("TurnRight\n");
      wheels.turnRight();
      break;
    case CorrectLeft:
      Serial.print("CorrectLeft\n");
      wheels.turnLeft(0.2f);
      head.turnHead(180);
      break;
    case CorrectRight:
      Serial.print("CorrectRight\n");
      wheels.turnRight(0.2f);
      head.turnHead(0);
      break;
    case ShortStraight:
      Serial.print("ShortStraight\n");
      wheels.moveForward(0.5f);
      head.turnHead(90);
      break;
  } // rover state

} // moveWheels