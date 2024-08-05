#include <Stepper.h>

// the number of steps per rotation for the stepper motor
const int stepsPerRevolution = 2048;  

// IN1-IN3-IN2-IN4, stepper class instance
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

// oystick pins
const int VRx = A0;
const int VRy = A1;
const int SW = 2;  // joystick button

// DC motor pins
const int motorPin1 = 3;
const int motorPin2 = 4;
const int motorEnablePin = 5;  // PWM pin

// (Red) LED pin for speed level indicator
const int redLEDPin = 7;

// DC motor speed levels --> levels go from 0-3. 
int motorSpeedLevel = 1;  // start with an initial speed level of 1
const int maxSpeedLevel = 3;  // maximum speed level
const int speedIncrement = 85;  // speed increment for each level (255 / maxSpeedLevel)


// track the mode (auto or manual) and state of the button
bool autoMode = false;  // start in manual mode
bool buttonPressed = false;  // track the state of the button

void setup() {
  // init serial port
  Serial.begin(9600);
  Serial.println("Setup complete");

  // init the button pin as input with pull-up resistor
  pinMode(SW, INPUT_PULLUP);
  myStepper.setSpeed(10);  // 10 RPM for stepper motor

  // init DC motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);

  // init LED pin
  pinMode(redLEDPin, OUTPUT);

  // init set LED to off
  digitalWrite(redLEDPin, LOW);
}

void loop() {
  // read joystick values
  int xPosition = analogRead(VRx);
  int yPosition = analogRead(VRy);
  bool currentButtonState = digitalRead(SW) == LOW;

  // use joystick button on DP2 to check for mode changes
  if (currentButtonState && !buttonPressed) {
    autoMode = !autoMode;  // toggle modes
    Serial.print("Mode changed to: ");
    Serial.println(autoMode ? "AUTO" : "MANUAL");
    buttonPressed = true;  // no multiple toggling
  } else if (!currentButtonState) {
    buttonPressed = false;
  }

  // stepper controls based on AUTO/MANUAL mode
  if (autoMode) {
    static bool direction = true;
    static unsigned long lastStepTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastStepTime >= 1000) {  // go opposite direction every 1s
      lastStepTime = currentTime;
      direction = !direction;
    }

    if (direction) {
      Serial.println("Stepping clockwise in auto mode");
      myStepper.step(stepsPerRevolution / 2);  // step clockwise for half a revolution
    } else {
      Serial.println("Stepping counterclockwise in auto mode");
      myStepper.step(-stepsPerRevolution / 2);  // step counterclockwise for half a revolution
    }
  } else {
    // adjust stepper motor based on vertical joystick position (Y axis)
    if (yPosition < 300) {
      Serial.println("Stepping counterclockwise...");
      myStepper.step(stepsPerRevolution / 10);  
    } else if (yPosition > 700) {
      Serial.println("Stepping clockwise...");
      myStepper.step(-stepsPerRevolution / 10);  
    } else {
      Serial.println("Stopping stepper motor...");
      // motor stops naturally if no input on manual mode
    }
  }

  // adjust DC motor speed based on horizontal joystick position (X axis)
  if (xPosition < 300) {
    if (motorSpeedLevel < maxSpeedLevel) {
      motorSpeedLevel++;
      Serial.print("Increasing speed to level: ");
      Serial.println(motorSpeedLevel);
    } else {
      Serial.println("Already at maximum speed level!");
    }
    delay(300);  // debounce 
  } else if (xPosition > 700) {
    if (motorSpeedLevel > 0) {
      motorSpeedLevel--;
      Serial.print("Decreasing speed to level: ");
      Serial.println(motorSpeedLevel);
    } else {
      Serial.println("Already at minimum speed level!");
    }
    delay(300);  // debounce
  }

  // adjust speed of dc motor based on speed level
  int motorSpeed = motorSpeedLevel * speedIncrement;
  analogWrite(motorEnablePin, motorSpeed);

  // LED indicator for lowest speed level (0- off)
  if (motorSpeedLevel == 0) {
    digitalWrite(redLEDPin, HIGH);
  } else {
    digitalWrite(redLEDPin, LOW);
  }

  // determine the direction for fan motor
  if (motorSpeed > 0) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  delay(5);  // delay
}

