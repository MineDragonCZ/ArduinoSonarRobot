#include <NewPing.h>
/**
 * USER CONFIGURATION
 */
const int SONAR_RECEIVE_PIN = 10;
const int SONAR_TRANSMIT_PIN = 9;
const double MAX_DETECTING_DISTANCE = 40; //cm
const int MOTORS[] = {3, 8, 6, 5};
//const int MOTORS[] = {11, 11, 11, 11};
const int btnPin = 2;

/**
 * VARIABLES
 */

bool isRunning = false;
bool initRunOfRobot = false;

NewPing sonar(SONAR_TRANSMIT_PIN, SONAR_RECEIVE_PIN, 1000);

void setup() {
  Serial.begin(9600); 
  for(int i = 0; i < 4; i++){
    pinMode(MOTORS[i], OUTPUT);
  }
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), startStopRobotByUser, FALLING);
}
int btnIsOkToBePressed = true;
int lastBtnPress = 0;
void loop() {
  
  pinMode(SONAR_TRANSMIT_PIN, OUTPUT);
  pinMode(SONAR_RECEIVE_PIN, INPUT);
  if(!btnIsOkToBePressed && (millis() - lastBtnPress > 800)){
    btnIsOkToBePressed = true;
  }
  if(!isRunning) return;
  if(initRunOfRobot){
    delay(5000);
    if(initRunOfRobot && isRunning)
      startSpining();
    delay(1000);
    if(initRunOfRobot && isRunning)
      stopRobot();
    // second case after delay - just to be sure
    initRunOfRobot = false;
    if(!isRunning) return;
  }
  if(isThereObject()){
    moveForward();
  }
  else {
    startSpining();
  }
  delay(500);
}

void startStopRobotByUser(){
  if(!btnIsOkToBePressed) return;
  btnIsOkToBePressed = false;
  lastBtnPress = millis();
  if(isRunning){
    isRunning = false;
    initRunOfRobot = false;
    stopRobot();
    return;
  }
  initRunOfRobot = true;
  isRunning = true;
}

void stopRobot() {
  for(int i = 0; i < 4; i++){
    digitalWrite(MOTORS[i], LOW);
  }
}

void moveForward() {
  stopRobot();
  for(int i = 0; i < 2; i++){
    digitalWrite(MOTORS[i*2], HIGH);
  }
}

void startSpining() {
  stopRobot();
  for(int i = 0; i < 2; i++){
    digitalWrite(MOTORS[i], HIGH);
  }
}

bool isThereObject() {
  double distance = sonar.ping_cm();
  Serial.print("Distance: ");  
	Serial.println(distance); 
  return (distance <= MAX_DETECTING_DISTANCE);
}
