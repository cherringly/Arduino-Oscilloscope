// Arduino → Web-Serial Oscilloscope Interface
// Reads A0–A5, converts to voltage, prints as CSV lines at ~100 Hz.
// Also toggles pin 8 on and off at 20 Hz (25 ms on/off cycle)

const uint8_t NUM_CHANNELS = 6;
const uint8_t channelPins[NUM_CHANNELS] = {A0, A1, A2, A3, A4, A5};

// ADC parameters
const float VCC = 5.0;         // Arduino UNO reference voltage
const float ADC_MAX = 1023.0;  // 10-bit ADC

// Sampling timing
const unsigned long SAMPLE_INTERVAL_MS = 10; // 100 Hz
unsigned long lastSampleTime = 0;

// Pin 8 toggle at 20 Hz
const uint8_t TOGGLE_PIN = 8;
const unsigned long TOGGLE_INTERVAL_MS = 25; // 20 Hz (on/off cycle)
unsigned long lastToggleTime = 0;
bool pinState = LOW;

void setup() {
  Serial.begin(115200);
  while (!Serial) { /* wait for Serial port */ }
  pinMode(TOGGLE_PIN, OUTPUT);
  digitalWrite(TOGGLE_PIN, pinState);
}

void loop() {
  unsigned long now = millis();

  // Toggle pin 8 at 20 Hz (every 25 ms)
  if (now - lastToggleTime >= TOGGLE_INTERVAL_MS) {
    lastToggleTime = now;
    pinState = !pinState;
    digitalWrite(TOGGLE_PIN, pinState);
  }

  // Read and print analog channels at 100 Hz
  if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    lastSampleTime = now;

    for (uint8_t ch = 0; ch < NUM_CHANNELS; ch++) {
      int raw = analogRead(channelPins[ch]);
      float volts = raw * VCC / ADC_MAX;
      Serial.print(volts, 3);
      if (ch < NUM_CHANNELS - 1) Serial.print(',');
    }
    Serial.println();
  }
}