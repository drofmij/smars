#include <AFMotor.h>
#include <Dabble.h>
// #include <SoftwareSerial.h>


// SoftwareSerial btSerial(A4,A5);


#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

AF_DCMotor R_motor(3);  // defines Right motor connector
AF_DCMotor L_motor(4);  // defines Left motor connector
// declare variables

#define RM_FORWARD FORWARD
#define LM_FORWARD BACKWARD
#define RM_BACKWARD BACKWARD
#define LM_BACKWARD FORWARD

#define HIGH_SPEED 255
#define LOW_SPEED 150

int distancecm = 0;
const int trigPin = 10;
const int echoPin = 9;
// defines variables
long duration;
int distance;

void setup() {


  Dabble.begin(9600, A4, A5);
  Serial.begin(9600);  // sets up Serial library at 9600 bps
  // btSerial.begin(9600);

  //changes the following values to make the robot  drive as straight as possible
  L_motor.setSpeed(HIGH_SPEED);  // sets L motor speed
  R_motor.setSpeed(HIGH_SPEED);  // sets R motor speed

  //R_motor.run(RELEASE);      //turns L motor on
  //L_motor.run(RELEASE);      //turns R motor on
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
}

void loop() {
  // if (btSerial.available()) {
  //   Serial.write(btSerial.read());
  // }

  // if (Serial.available()) {
  //   btSerial.write(Serial.read());
  // }
  Dabble.processInput();
  if (GamePad.isUpPressed()) {
    forward();
  }
  if (GamePad.isDownPressed()) {
    backward();
  }
  if (GamePad.isLeftPressed()) {
    turnLeft();
  }
  if (GamePad.isRightPressed()) {
    turnRight();
  }

  if(GamePad.isSquarePressed()) {
    stop();
  }

  // distancecm = checkDistanceCM();
  // if (distance < 10) {
  //   stop();

  //   delay(500);

  //   //go backward
  //   reverse();

  //   // turn
  //   // todo: randomize turn direction?
  //   delay(1000);
  //   turnRight();
  //   delay(1000);
  // } else {
  //   forward();
  // }
}

void forward() {
  L_motor.setSpeed(HIGH_SPEED);  // sets L motor speed
  R_motor.setSpeed(HIGH_SPEED);  // sets R motor speed
  L_motor.run(LM_FORWARD);       //moves motor L Forward -> make better constants for this?
  R_motor.run(RM_FORWARD);
}

void backward() {
  L_motor.setSpeed(LOW_SPEED);  // sets L motor speed
  R_motor.setSpeed(LOW_SPEED);  // sets R motor speed
  R_motor.run(RM_BACKWARD);
  L_motor.run(LM_BACKWARD);
}

void stop() {
  R_motor.run(RELEASE);
  L_motor.run(RELEASE);
}

void turnRight() {
  R_motor.run(RM_FORWARD);
  L_motor.run(LM_BACKWARD);
}

void turnLeft() {
  R_motor.run(RM_BACKWARD);
  L_motor.run(LM_FORWARD);
}

int checkDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.print("\n");
}
