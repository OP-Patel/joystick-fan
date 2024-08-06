#include <Stepper.h>


const int stepsPerRevolution = 2048; 

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);


const int VRx = A0;
const int VRy = A1;
const int SW = 2;  


const int motorPin1 = 3;
const int motorPin2 = 4;
const int motorEnablePin = 5;  


const int redLEDPin = 7;


int motorSpeedLevel = 1;  
const int maxSpeedLevel = 3; 
const int speedIncrement = 85;  


bool autoMode = false;  // Start in manual mode
bool buttonPressed = false;  

void setup() {
 
  Serial.begin(9600);
  Serial.println("Setup complete");

  // Initialize the button pin as input with pull-up resistor
  pinMode(SW, INPUT_PULLUP);

  // Set initial speed of the stepper motor
  myStepper.setSpeed(10);  // 10 RPM

  // Initialize DC motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);

  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
}

void loop() {
  // Read joystick values
  int xPosition = analogRead(VRx);
  int yPosition = analogRead(VRy);
  bool currentButtonState = digitalRead(SW) == LOW;

  // Check for button press to toggle mode
  if (currentButtonState && !buttonPressed) {
    autoMode = !autoMode;  
    Serial.print("Mode changed to: ");
    Serial.println(autoMode ? "AUTO" : "MANUAL");
    buttonPressed = true; 
  } else if (!currentButtonState) {
    buttonPressed = false;
  }

  // Control stepper motor based on mode
  if (autoMode) {
    static bool direction = true;
    static unsigned long lastStepTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastStepTime >= 250) {  // Change direction every 250ms
      lastStepTime = currentTime;
      direction = !direction;
    }

    if (direction) {
      Serial.println("Stepping clockwise in auto mode");
      myStepper.step(stepsPerRevolution / 7);  // Step clockwise for 1/5 a revolution
    } else {
      Serial.println("Stepping counterclockwise in auto mode");
      myStepper.step(-stepsPerRevolution / 7);  // Step counterclockwise for 1/5 a revolution
    }
  } else {
    // Control stepper motor based on vertical joystick position (Y axis)
    if (yPosition < 300) {
      Serial.println("Stepping counterclockwise...");
      myStepper.step(stepsPerRevolution / 10);  
    } else if (yPosition > 700) {
      Serial.println("Stepping clockwise...");
      myStepper.step(-stepsPerRevolution / 10);  /
    } else {
      Serial.println("Stopping stepper motor...");
      // No steps taken, the motor stops naturally
    }
  }

  // Adjust DC motor speed based on horizontal joystick position (X axis)
  if (xPosition < 300) {
    if (motorSpeedLevel < maxSpeedLevel) {
      motorSpeedLevel++;
      Serial.print("Increasing speed to level: ");
      Serial.println(motorSpeedLevel);
    } else {
      Serial.println("Already at maximum speed level!");
    }
    delay(300);  // Debounce 
  } else if (xPosition > 700) {
    if (motorSpeedLevel > 0) {
      motorSpeedLevel--;
      Serial.print("Decreasing speed to level: ");
      Serial.println(motorSpeedLevel);
    } else {
      Serial.println("Already at minimum speed level!");
    }
    delay(300);  // Debounce 
  }

  // Set the speed of the DC motor based on the speed level
  int motorSpeed = motorSpeedLevel * speedIncrement;
  analogWrite(motorEnablePin, motorSpeed);

  // Update LED indicator 
  if (motorSpeedLevel == 0) {
    digitalWrite(redLEDPin, HIGH);
  } else {
    digitalWrite(redLEDPin, LOW);
  }

  if (motorSpeed > 0) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  delay(5); 
}

