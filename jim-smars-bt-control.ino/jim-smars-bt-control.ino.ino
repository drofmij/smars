#include <AFMotor.h>
#include <Dabble.h>


#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#define ROBOT_MODE 0
#define MANUAL_MODE 1

// pins for echo sensor
#define SONIC_TRIGGER_PIN 10
#define SONIC_ECHO_PIN 9
int duration;
int distance;


// Motor settings
// NOTE: Left Motor is backwards.
#define RM_FORWARD FORWARD
#define LM_FORWARD BACKWARD
#define RM_BACKWARD BACKWARD
#define LM_BACKWARD FORWARD

#define HIGH_SPEED 255
#define LOW_SPEED 100

#define RIGHT_MOTOR_PINS 3
#define LEFT_MOTOR_PINS 4

AF_DCMotor R_motor(RIGHT_MOTOR_PINS);  // defines Right motor connector
AF_DCMotor L_motor(LEFT_MOTOR_PINS);  // defines Left motor connector

int controlMode = MANUAL_MODE;

void setup() {
  Dabble.begin(9600, A4, A5);
  Serial.begin(9600);  // sets up Serial library at 9600 bps

  //changes the following values to make the robot  drive as straight as possible
  L_motor.setSpeed(HIGH_SPEED);  // sets L motor speed
  R_motor.setSpeed(HIGH_SPEED);  // sets R motor speed

  R_motor.run(RELEASE);      //turns L motor on
  L_motor.run(RELEASE);      //turns R motor on
  pinMode(SONIC_TRIGGER_PIN, OUTPUT);  // Sets the trigPin as an Output
  pinMode(SONIC_ECHO_PIN, INPUT);   // Sets the echoPin as an Input
}

void loop() {
  Dabble.processInput();

  if (GamePad.isSelectPressed() && controlMode == MANUAL_MODE) {
    controlMode = ROBOT_MODE;
    Serial.println("CONTROL MODE: ROBOT");
    stop();
    delay(1000);
    forward();
  } else if(GamePad.isStartPressed() && controlMode == ROBOT_MODE) {
    stop();
    delay(1000);
    controlMode = MANUAL_MODE;
    Serial.println("CONTROL MODE: Manual");
  }

  if (GamePad.isSquarePressed()) {
    controlMode = MANUAL_MODE;
    Serial.println("CONTROL MODE: Manual");
    stop();
  }

  if (controlMode == MANUAL_MODE) {
    if (GamePad.isUpPressed()) {
      Serial.println("Pressed Forward");
      forward();
    }
    if (GamePad.isDownPressed()) {
      Serial.println("Pressed Backward");
      backward();
    }
    if (GamePad.isLeftPressed()) {
      Serial.println("Pressed Left");
      turnLeft();
    }
    if (GamePad.isRightPressed()) {
      Serial.println("Pressed Right");
      turnRight();
    }

    if (checkDistanceCM() < 10) {
          Serial.println("Found Obstacle");
          stop();
    }
  } else if (controlMode == ROBOT_MODE) {
    if (checkDistanceCM() < 10) {
      Serial.println("Found Obstacle");
      stop();
      delay(500);
      //go backward
      Serial.println("Move Backwards");
      backward();
      // turn
      // todo: randomize turn direction?
      delay(1000);
      Serial.println("Turn Right");
      turnRight();
      delay(1000);
      Serial.println("Continue");
    } else {
      forward();
    }
  }
}

void forward() {
  L_motor.setSpeed(HIGH_SPEED);  // sets L motor speed
  R_motor.setSpeed(HIGH_SPEED);  // sets R motor speed
  L_motor.run(LM_FORWARD);       //moves motor L Forward -> make better constants for this?
  R_motor.run(RM_FORWARD);
}

void backward() {
  stop();
  L_motor.setSpeed(LOW_SPEED);  // sets L motor speed
  R_motor.setSpeed(LOW_SPEED);  // sets R motor speed
  R_motor.run(RM_BACKWARD);
  L_motor.run(LM_BACKWARD);
}

void stop() {
  R_motor.run(BRAKE);
  L_motor.run(BRAKE);
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
  digitalWrite(SONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the SONIC_TRIGGER_PIN on HIGH state for 10 micro seconds
  digitalWrite(SONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);

  // send ping
  digitalWrite(SONIC_TRIGGER_PIN, LOW);
  //receive ping -> returns the sound wave travel time in microseconds
  duration = pulseIn(SONIC_ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  return distance;
}
