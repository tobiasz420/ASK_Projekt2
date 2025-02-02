const int pirPins[] = {2, 5, 10, 13, A5};
const int buttonPins[] = {4, 6, 8, 11, A4};
const int ledPins[] = {3, 7, 9, 12, A3};
const int numSensors = 5;

const int datapin = A0;
const int latchpin = A1;
const int clockpin = A2;

bool sensorEnabled[numSensors] = {false};
bool motionDetected[numSensors] = {false};
int motionCount = 0;

const byte segmentData[] = {64, 121, 36, 48, 25,18,2,120,0,24,247,254};

void setup() {
  for (int i = 0; i < numSensors; i++) {
    pinMode(pirPins[i], INPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  bool anySensorEnabled = false;

  for (int i = 0; i < numSensors; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      sensorEnabled[i] = !sensorEnabled[i];
      if (sensorEnabled[i]) {
        motionCount = 0;
        for (int j = 0; j < numSensors; j++) {
          motionDetected[j] = false;
        }
      } else {
        if (motionDetected[i]) {
          motionDetected[i] = false;
          motionCount--;
           if (motionCount < 0) motionCount = 0;

        }
      }
      delay(300);
    }
    digitalWrite(ledPins[i], !sensorEnabled[i]);

    anySensorEnabled |= sensorEnabled[i];

    if (sensorEnabled[i]) {
      int pirState = digitalRead(pirPins[i]);

      if (pirState == HIGH && !motionDetected[i]) {
        motionDetected[i] = true;
        motionCount++;
      }
    }
  }
  
   byte displayData;

  if (!anySensorEnabled) {
    displayData = segmentData[10];
  } else if (motionCount == 0) {
    displayData = segmentData[11];
  } else if (motionCount < 10) {
    displayData = segmentData[motionCount];
  } else {
    displayData = segmentData[11];
  }

  digitalWrite(latchpin, LOW);
  shiftOut(datapin, clockpin, MSBFIRST, displayData);
  digitalWrite(latchpin, HIGH);
}
