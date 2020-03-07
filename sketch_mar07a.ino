
#include <Stepper.h>


// array
#define WEIGHT_SENSOR A3

// Stepper
#define STEPS_PER_REVOLUTION 2038
#define STEPPER1 13
#define STEPPER2 12
#define STEPPER3 11
#define STEPPER4 10


// button array
#define COLUMN_1 9
#define COLUMN_2 8
#define COLUMN_3 7
#define COLUMN_4 6
#define ROW_1 5
#define ROW_2 4
#define ROW_3 3
#define ROW_4 2

Stepper stepper(STEPS_PER_REVOLUTION, STEPPER1, STEPPER2, STEPPER3, STEPPER4);

int previous = 0;
int stepVal = 0;


bool is_running = false;
int forceSensor = 0;
bool resetMode = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(WEIGHT_SENSOR, INPUT);
  Serial.begin(9600);
  stepper.setSpeed(16);
}

char recvOneChar(char key) {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    Serial.print(receivedChar);
    Serial.print(" key is ");
    Serial.println(key);
    return receivedChar;
//    if (receivedChar == key) {
//      return true;
//    }
  }
//  return false;
}

void loop() {
  char receivedChar = recvOneChar('r');
  Serial.print("getting reset ");
  Serial.println(receivedChar);
  delay(1000);
//  
  if (receivedChar == 'r'){
    resetMode = true;
    Serial.println("reset mode is on");
    while (resetMode == true){
      stepper.step(2);
      if (recvOneChar('s') == 's') {
        resetMode = false;
      }
    }
  }
    // open halfway if button is pressed
    if (receivedChar == 'o') {
        is_running = true;
        while (is_running == true) {
          Serial.println(is_running);
          Serial.println(forceSensor);
          int openVal = 0;
          forceSensor = analogRead(WEIGHT_SENSOR);
          while(forceSensor < 900) { // value set to a little less than x amount of grams
            forceSensor = analogRead(WEIGHT_SENSOR);
            Serial.println(forceSensor);
            // continually open Stepper to 90 deg
            
            if (openVal < 254){
              stepper.step(2);
              delay(1);
              Serial.print("Opening ");
              Serial.println(openVal); 
              openVal = openVal + 1;
            }
          }
          is_running = false;
          // close stepper
          while (openVal > 0) {
            openVal --;
            stepper.step(-2);
            delay(1);
          }
        }
    }
}
