#include "Arduino.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Define Flex Sensor Pins (Analog inputs)
const int FLEX_INDEX = A0;


const int FLEX_MIDDLE = A1;
const int FLEX_RING = A2;
const int FLEX_PINKY = A3;

// Software Serial RX/TX configuration for DFPlayer Mini communication
// Arduino Pin 10 connects to DFPlayer TX, Pin 11 connects to DFPlayer RX
SoftwareSerial softwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;
// Variables to store analog readings (0 - 1023)
int valIndex = 0;
int valMiddle = 0;
int valRing = 0;
int valPinky = 0;
// Variables for MPU-6050 Accelerometer
const int MPU_addr = 0x68; // I2C address of the MPU-6050

int16_t AcX, AcY, AcZ; // Raw 16-bit accelerometer readings
float pitchAngle = 0; // Calculated tilt angle along the X-axis
void setup() {
// Initialize standard hardware serial for diagnostic debugging
Serial.begin(115200);
// Initialize software serial bus at 9600 bps for the DFPlayer Mini
softwareSerial.begin(9600);
// Initialize I2C communication interface
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); // Access register 6B (Power Management 1)
Wire.write(0); // Set register to 0 to wake up the MPU-6050
Wire.endTransmission(true);
Serial.println(F("Initializing DFPlayer Mini...
"));
// Verify physical connection and status of the DFPlayer hardware

if (!myDFPlayer.begin(softwareSerial)) {
Serial.println(F("Initialization failed. Please verify hardware wiring.
"));
while(true); // Halt execution if DFPlayer is not detected
}
Serial.println(F("DFPlayer Mini Online. System Ready.
"));
// Set output volume level (ranges from 0 to 30)
myDFPlayer.volume(25);
}
void loop() {
// --- 1. Read Analog Data from Flex Sensors ---
valIndex = analogRead(FLEX_INDEX);
valMiddle = analogRead(FLEX_MIDDLE);
valRing = analogRead(FLEX_RING);
valPinky = analogRead(FLEX_PINKY);

// --- 2. Read Digital Spatial Data from MPU-6050 ---

Wire.beginTransmission(MPU_addr);
Wire.write(0x3B); // Start reading from register 3B (Accelerometer X-axis high byte)
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr, 6, true); // Request 6 registers (X, Y, Z axes)
AcX = Wire.read() << 8 | Wire.read(); // Combine high and low byte for X-axis
AcY = Wire.read() << 8 | Wire.read(); // Combine high and low byte for Y-axis
AcZ = Wire.read() << 8 | Wire.read(); // Combine high and low byte for Z-axis

// --- 3. Physics Angle Deduction (Pitch Angle Calculation) ---
// Calculates hand tilt along the primary longitudinal axis (X-axis) in degrees
pitchAngle = atan2((double)AcX, sqrt((double)AcY*AcY + (double)AcZ*AcZ)) *
180.0 / PI;

// Print raw values to Serial Monitor for diagnostic debugging
Serial.print("INDEX: "); Serial.print(valIndex);
Serial.print(" | MIDDLE: "); Serial.print(valMiddle);
Serial.print(" | RING: "); Serial.print(valRing);
Serial.print(" | PINKY: "); Serial.print(valPinky);
Serial.print(" | PITCH: "); Serial.println(pitchAngle);

// --- 4. Algorithmic Matching Logic (The Decision Matrix) ---

// GESTURE A: "HELLO" (All fingers flat, Hand in vertical/horizontal plane)
if (valIndex > 800 && valMiddle > 800 && valRing > 800 && valPinky > 800 &&
(pitchAngle > -20 && pitchAngle < 20)) {
Serial.println("Gesture Detected: [HELLO] - Triggering MP3 Track 1");
myDFPlayer.play(1);
delay(2500); // Prevent duplicate trigger loops
}

// GESTURE B: "THANK YOU" (All fingers flat, Hand tilted up at approx 45 degrees)
else if (valIndex > 800 && valMiddle > 800 && valRing > 800 && valPinky > 800
&& (pitchAngle > 35 && pitchAngle < 55)) {
Serial.println("Gesture Detected: [THANK YOU] - Triggering MP3 Track 2");
myDFPlayer.play(2);
delay(2500);
}

// GESTURE C: "YES" (Fist formation, all fingers bent, Hand resting flat)
else if (valIndex < 630 && valMiddle < 630 && valRing < 630 && valPinky < 630
&& (pitchAngle > -20 && pitchAngle < 20)) {
Serial.println("Gesture Detected: [YES] - Triggering MP3 Track 3");
myDFPlayer.play(3);
delay(2500);
}

// GESTURE D: "PEACE / VICTORY" (Index & Middle flat, Ring & Pinky bent, Hand
straight up)
else if (valIndex > 800 && valMiddle > 800 && valRing < 630 && valPinky < 630
&& (pitchAngle > 75 && pitchAngle < 95)) {
Serial.println("Gesture Detected: [PEACE] - Triggering MP3 Track 4");
myDFPlayer.play(4);
delay(2500);
}

// GESTURE E: "HELP" (Index flat, Middle, Ring & Pinky completely bent, Hand
resting flat)
else if (valIndex > 800 && valMiddle < 630 && valRing < 630 && valPinky < 630
&& (pitchAngle > -20 && pitchAngle < 20)) {
Serial.println("Gesture Detected: [HELP] - Triggering MP3 Track 5");
myDFPlayer.play(5);
delay(2500);
}
// Small loop delay to allow safe settling of Analog-to-Digital conversion pipelines
delay(100);
}
