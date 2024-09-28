// Include necessary libraries
#include <Wire.h>         // Library for I2C communication
#include <MPU6050.h>      // Library for MPU-6050 accelerometer
#include <Arduino.h>      // Main Arduino library

// Create an instance of the MPU6050 class
MPU6050 accelgyro;

// Define the pin connected to the buzzer
const int buzzerPin = 11;  // Adjust if using a different pin

// Variables for motion detection
long lastMotionTime = 0;             // Timestamp of the last detected motion
const long motionThreshold = 1000;   // Threshold to detect motion (adjust as needed)
const long alarmDelay = 30000;       // Time in milliseconds before the alarm activates (30 seconds)
bool alarmActive = false;            // Flag to indicate if the alarm is currently active

// Variables for the non-blocking alarm sound
unsigned long previousAlarmMillis = 0;  // Stores the last time the alarm tone switched
const long alarmInterval = 500;         // Interval for switching tones (500 ms)
bool isHighTone = true;                 // Flag to toggle between high and low tones

void setup() {
  // Initialize I2C communication
  Wire.begin();

  // Initialize serial communication for debugging (optional)
  Serial.begin(9600);

  // Initialize the MPU-6050 accelerometer
  accelgyro.initialize();

  // Verify the connection to the MPU-6050
  if (!accelgyro.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);  // Halt execution if connection fails
  }

  // Set the initial last motion time to the current time
  lastMotionTime = millis();
}

void loop() {
  if (alarmActive) {
    // If the alarm is active, play the alarm sound without blocking
    playAlarmNonBlocking();
  } else {
    // Check for motion when the alarm is not active
    checkForMotion();

    // Determine if the device has been stationary long enough to trigger the alarm
    if ((millis() - lastMotionTime >= alarmDelay) && !alarmActive) {
      alarmActive = true;  // Activate the alarm

      // Play the initial variation at the start of the alarm
      tone(buzzerPin, 1200, 200);  // Play a 1200 Hz tone for 200 ms
      delay(200);                  // Wait for 200 ms
      tone(buzzerPin, 1400, 200);  // Play a 1400 Hz tone for 200 ms
      delay(200);                  // Wait for 200 ms
      noTone(buzzerPin);           // Stop any tone that's playing

      // Reset the timing for the alarm tones
      previousAlarmMillis = millis();
    }
  }

  // Short delay to prevent overwhelming the sensor with readings
  delay(50);
}

// Function to check for motion
void checkForMotion() {
  int16_t ax, ay, az;  // Variables to store acceleration data

  // Read acceleration values from the MPU-6050
  accelgyro.getAcceleration(&ax, &ay, &az);

  // Calculate the magnitude of the acceleration vector
  float acceleration = sqrt(ax * ax + ay * ay + az * az);

  // Debugging: Print acceleration value (optional)
  // Serial.println(acceleration);

  // Check if the acceleration exceeds the motion threshold
  if (abs(acceleration - 16384) > motionThreshold) {
    lastMotionTime = millis();  // Update the last motion time to the current time
  }
}

// Function to play the alarm sound without blocking the main loop
void playAlarmNonBlocking() {
  unsigned long currentMillis = millis();

  // Check if it's time to switch the tone
  if (currentMillis - previousAlarmMillis >= alarmInterval) {
    previousAlarmMillis = currentMillis;  // Update the timing

    if (isHighTone) {
      tone(buzzerPin, 1000);  // Play the high tone (1000 Hz)
    } else {
      tone(buzzerPin, 800);   // Play the low tone (800 Hz)
    }
    isHighTone = !isHighTone;  // Toggle the tone for the next interval
  }

  // Continuously check for motion to stop the alarm
  int16_t ax, ay, az;
  accelgyro.getAcceleration(&ax, &ay, &az);
  float acceleration = sqrt(ax * ax + ay * ay + az * az);

  // If motion is detected, deactivate the alarm
  if (abs(acceleration - 16384) > motionThreshold) {
    alarmActive = false;       // Deactivate the alarm
    noTone(buzzerPin);         // Stop any tone that's playing
    lastMotionTime = millis(); // Reset the last motion time
  }
}
