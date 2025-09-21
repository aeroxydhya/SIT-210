// Task 4.2C - Multiple Interrupts (Button + Ultrasonic Sensor)

const int buttonPin = 2;   // Push button
const int trigPin   = 3;   // Ultrasonic TRIG
const int echoPin   = 4;   // Ultrasonic ECHO
const int led1      = 8;   // LED controlled by button
const int led2      = 9;   // LED controlled by sensor

volatile bool led1State = LOW;
volatile bool led2State = LOW;

volatile unsigned long startMicro = 0;
volatile unsigned long durationMicro = 0;
volatile bool newReading = false;

// ISR for button press
void buttonISR() {
  led1State = !led1State;
  digitalWrite(led1, led1State);
  Serial.println("Button interrupt: LED1 toggled");
}

// ISR for ultrasonic echo
void echoISR() {
  if (digitalRead(echoPin) == HIGH) {
    startMicro = micros();   // rising edge
  } else {
    durationMicro = micros() - startMicro;  // falling edge
    newReading = true;
  }
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(echoPin), echoISR, CHANGE);

  Serial.println("System ready - press button or trigger sensor...");
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  delay(50);  // short wait

  if (newReading) {
    noInterrupts();
    unsigned long d = durationMicro;
    newReading = false;
    interrupts();

    float distance_cm = d / 58.0; // convert pulse length to cm

    if (distance_cm > 0 && distance_cm < 20) {  // object detected < 20cm
      led2State = !led2State;
      digitalWrite(led2, led2State);
      Serial.print("Sensor interrupt: LED2 toggled, distance = ");
      Serial.println(distance_cm);
    }
  }

  delay(100);
}
