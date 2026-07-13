/*
 * Cardinal Epic Device
 * ---------------------
 * Arduino Uno sketch: a 2-axis analog joystick (Parallax #27800) drives four LEDs
 * arranged in a diamond. Point the joystick and the matching LED lights; point
 * diagonally and the two neighboring LEDs light; center it and all LEDs go off.
 *
 * Hold semantics: LEDs reflect the CURRENT joystick position every loop (no latch).
 *
 * See PRD.md and docs/wiring.md for the full spec and wiring.
 *
 * STATUS: SCAFFOLD. Pin map, constants, and structure are in place. The direction
 * logic is stubbed with TODOs for the implementation step.
 */

// ---------------------------------------------------------------------------
// Pin assignments
// ---------------------------------------------------------------------------
const uint8_t LED_UP    = 2;   // Top    LED (diamond)
const uint8_t LED_RIGHT = 3;   // Right  LED
const uint8_t LED_DOWN  = 4;   // Bottom LED
const uint8_t LED_LEFT  = 5;   // Left   LED

const uint8_t PIN_X = A0;      // Joystick L/R wiper (X axis)
const uint8_t PIN_Y = A1;      // Joystick U/D wiper (Y axis)

// Optional pushbutton for "all LEDs on" (Parallax #27800 has no built-in button).
// Wire: D6 -> button -> GND, read with INPUT_PULLUP (pressed = LOW).
const uint8_t PIN_BUTTON  = 6;
const bool    USE_BUTTON  = false;  // set true once a physical button is added

// ---------------------------------------------------------------------------
// Tuning constants
// ---------------------------------------------------------------------------
const int AXIS_CENTER = 512;   // ~midpoint of a 0..1023 analogRead
const int DEADZONE    = 150;   // neutral band around center (tune after testing)

// Set these to match observed polarity on first upload (see docs/wiring.md).
// If "up" decreases the Y reading, flip INVERT_Y; same idea for X.
const bool INVERT_X = false;
const bool INVERT_Y = false;

const unsigned long LOOP_DELAY_MS = 20;  // small debounce/settle delay

// ---------------------------------------------------------------------------
// Axis state helper
// ---------------------------------------------------------------------------
enum AxisState { NEG, NEUTRAL, POS };
// For X: NEG = LEFT,  POS = RIGHT
// For Y: NEG = DOWN,  POS = UP   (adjust with INVERT_Y after testing)

void setup() {
  pinMode(LED_UP, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_DOWN, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);

  if (USE_BUTTON) {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
  }

  Serial.begin(9600);  // debug: print raw axis values while tuning
}

void loop() {
  int rawX = analogRead(PIN_X);
  int rawY = analogRead(PIN_Y);

  // Optional button override: all LEDs on while pressed.
  if (USE_BUTTON && digitalRead(PIN_BUTTON) == LOW) {
    setLeds(true, true, true, true);
    delay(LOOP_DELAY_MS);
    return;
  }

  AxisState xState = classifyAxis(rawX, INVERT_X);
  AxisState yState = classifyAxis(rawY, INVERT_Y);

  // TODO(implementation): translate (xState, yState) into the four LED booleans
  // per the truth table in PRD.md section 5. Diagonals light TWO neighbors;
  // center lights none. Placeholder below keeps everything OFF so the scaffold
  // compiles and is safe to upload.
  bool up = false, right = false, down = false, left = false;

  setLeds(up, right, down, left);
  delay(LOOP_DELAY_MS);
}

// Classify a raw 0..1023 axis reading into NEG / NEUTRAL / POS around center,
// honoring the deadzone and an optional inversion.
AxisState classifyAxis(int raw, bool invert) {
  int delta = raw - AXIS_CENTER;
  if (invert) delta = -delta;
  if (delta > DEADZONE)  return POS;
  if (delta < -DEADZONE) return NEG;
  return NEUTRAL;
}

// Drive the four LEDs. true = full brightness on.
void setLeds(bool up, bool right, bool down, bool left) {
  digitalWrite(LED_UP,    up    ? HIGH : LOW);
  digitalWrite(LED_RIGHT, right ? HIGH : LOW);
  digitalWrite(LED_DOWN,  down  ? HIGH : LOW);
  digitalWrite(LED_LEFT,  left  ? HIGH : LOW);
}
