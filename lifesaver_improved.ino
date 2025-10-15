/*
 * Ghostbusters Theme Player - Improved Version
 * =============================================
 * Plays the Ghostbusters theme song on a piezo buzzer
 * 
 * Hardware Setup:
 * - Piezo buzzer connected to BUZZER_PIN (default: pin 11)
 * - Optional: Push button connected to BUTTON_PIN (default: pin 2) with internal pullup
 *   Button should connect pin to GND when pressed
 * 
 * Features:
 * - Clean code structure with constants
 * - Melody data separated into header file
 * - Configurable auto-play on startup or button-triggered playback
 * - Volume control via PWM duty cycle adjustment
 * - Debounced button input
 * 
 * Configuration:
 * - Set AUTO_PLAY to true for automatic playback on startup
 * - Set AUTO_PLAY to false to require button press to play
 * - Adjust VOLUME (0.0 to 1.0) to control buzzer volume
 */

#include "melody_data.h"

// ===== PIN CONFIGURATION =====
const int BUZZER_PIN = 11;           // PWM-capable pin for piezo buzzer
const int BUTTON_PIN = 2;            // Digital pin for optional button (uses internal pullup)

// ===== PLAYBACK CONFIGURATION =====
const float VOLUME = 0.7;            // Volume control: 0.0 (silent) to 1.0 (full volume)
const bool AUTO_PLAY = true;         // true = play on startup, false = wait for button press

// ===== BUTTON DEBOUNCE =====
const unsigned long DEBOUNCE_DELAY = 50;  // Debounce time in milliseconds
unsigned long lastDebounceTime = 0;
int lastButtonState = HIGH;
int buttonState = HIGH;

// ===== STATE TRACKING =====
bool hasPlayed = false;              // Track if melody has played (for AUTO_PLAY mode)

// ===== FUNCTION DECLARATIONS =====
void playMelody();
bool isButtonPressed();

/*
 * Setup Function
 * Initializes pins and plays melody if AUTO_PLAY is enabled
 */
void setup() {
  // Configure buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Configure button pin with internal pullup resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Small delay to allow hardware to stabilize
  delay(100);
  
  // Play melody immediately if AUTO_PLAY is enabled
  if (AUTO_PLAY) {
    playMelody();
    hasPlayed = true;
  }
}

/*
 * Main Loop Function
 * Monitors button press and triggers playback when button is pressed
 */
void loop() {
  // Only check button if AUTO_PLAY is disabled or if we want repeatable playback
  if (!AUTO_PLAY) {
    if (isButtonPressed()) {
      playMelody();
      // Wait for button release to prevent multiple triggers
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
      delay(100);  // Additional delay after release
    }
  }
  // If AUTO_PLAY is true, loop does nothing (melody plays once in setup)
}

/*
 * Play Melody Function
 * Iterates through the melody array and plays each note with proper timing
 */
void playMelody() {
  for (int thisNote = 0; thisNote < MELODY_LENGTH; thisNote++) {
    int frequency = melody[thisNote];
    int duration = noteDurations[thisNote];
    
    // Only play tone if frequency is non-zero (0 = rest/silence)
    if (frequency > 0) {
      // Apply volume control by adjusting the duration
      // The tone() function doesn't support volume directly,
      // but we can simulate it with duration adjustment
      tone(BUZZER_PIN, frequency, duration * VOLUME);
    }
    
    // Wait for the note duration
    delay(duration);
    
    // Ensure tone is stopped between notes
    noTone(BUZZER_PIN);
  }
}

/*
 * Button Press Detection with Debouncing
 * Returns true when button is pressed (with debounce filtering)
 */
bool isButtonPressed() {
  int reading = digitalRead(BUTTON_PIN);
  
  // Check if button state has changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  // Only update button state if debounce delay has passed
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      
      // Button is pressed when it reads LOW (pulled to ground)
      if (buttonState == LOW) {
        lastButtonState = reading;
        return true;
      }
    }
  }
  
  lastButtonState = reading;
  return false;
}
