# Cardinal Epic Device — Product Requirements Document (PRD)

**Status:** Scaffolded (pre-implementation)
**Last updated:** 2026-07-12
**Owner:** Caleb Terrill

---

## 1. Overview

**Cardinal Epic Device** is an Arduino Uno project that maps the direction of an
analog joystick to four LEDs arranged in a diamond (cardinal-direction) layout.
Pointing the joystick toward a cardinal direction lights the corresponding LED;
pointing diagonally lights the two neighboring LEDs; centering the joystick turns
all LEDs off.

This is the first hardware project in a broader effort to integrate local models
with Arduino components. It is intentionally simple (single sketch, no PC/LLM in
the loop) to validate the toolchain, wiring workflow, and GitHub process before
tackling the larger local-model projects.

---

## 2. Goals & Non-Goals

### Goals
- Read a 2-axis analog joystick on the Arduino Uno.
- Light 4 LEDs in a diamond based on joystick direction (8-way: 4 cardinal + 4 diagonal).
- All LEDs off when the joystick is centered (spring-return neutral).
- **Hold semantics:** LEDs are lit only while the direction is held; releasing to
  center turns them off. (No latching/toggling.)
- Full brightness on/off only (no PWM dimming).
- Ship as a single Arduino sketch in a clean, public GitHub repo.

### Non-Goals (for this project)
- No PC connection, serial protocol, or local LLM.
- No LCD, motors, GPS, IR, or RF — those belong to later projects.
- No brightness/PWM control.
- No latching or toggle behavior.

---

## 3. Hardware — Bill of Materials

| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | Arduino Uno | Primary microcontroller |
| 1 | Parallax 2-Axis Joystick **#27800** (Rev B) | Dual 10 kΩ pots, **spring-return self-centering**, **no pushbutton** |
| 4 | LEDs (4 different colors, same model) | Diamond layout |
| 4 | Resistors ~220–330 Ω | One per LED, current limiting |
| 1 | Breadboard | |
| — | Jumper wires | |
| (opt) | 1 momentary tactile pushbutton | **Only if** the "all LEDs on" feature is wanted — see §6 |

---

## 4. Pin Assignments

### LEDs (digital output, diamond layout)

| Direction | Diamond position | Arduino pin | LED color |
|-----------|-----------------|-------------|-----------|
| Up    | Top    | D2 | _TBD_ |
| Right | Right  | D3 | _TBD_ |
| Down  | Bottom | D4 | _TBD_ |
| Left  | Left   | D5 | _TBD_ |

> Color→position is cosmetic and does not affect the code. Fill in the colors once
> the LEDs are placed.

### Joystick (analog input — Parallax #27800)

| Joystick pin | Arduino pin | Purpose |
|--------------|-------------|---------|
| `L/R+`       | 5V   | Power for L/R (X) potentiometer |
| `L/R` (wiper)| A0   | X-axis signal |
| `U/D+`       | 5V   | Power for U/D (Y) potentiometer |
| `U/D` (wiper)| A1   | Y-axis signal |
| `GND`        | GND  | Common ground (either/both GND pins) |

> If an axis reads reversed during testing, swap that potentiometer's `+` and `GND`
> wires (or invert it in software).

### Optional pushbutton
| Component | Arduino pin | Wiring |
|-----------|-------------|--------|
| Pushbutton | D6 | Pin → button → GND, using `INPUT_PULLUP` (pressed = LOW) |

---

## 5. Behavior Specification

Let `x = analogRead(A0)` and `y = analogRead(A1)`, each in the range `0..1023`,
centered near `512`.

- **Deadzone:** a tolerance band around center (default `±150`, i.e. `DEADZONE = 150`)
  within which an axis is considered "neutral." This absorbs spring-return drift and
  prevents flicker.
- **Axis states:**
  - X: `LEFT` if `x < 512 - DEADZONE`, `RIGHT` if `x > 512 + DEADZONE`, else `NEUTRAL`.
  - Y: `UP`/`DOWN` similarly on `y` (exact sign confirmed at test time).
- **LED rules (hold semantics — recomputed every loop):**

  | X state | Y state | LEDs lit |
  |---------|---------|----------|
  | NEUTRAL | NEUTRAL | none (all off) |
  | NEUTRAL | UP      | Top |
  | NEUTRAL | DOWN    | Bottom |
  | LEFT    | NEUTRAL | Left |
  | RIGHT   | NEUTRAL | Right |
  | LEFT    | UP      | Left + Top |
  | RIGHT   | UP      | Right + Top |
  | LEFT    | DOWN    | Left + Bottom |
  | RIGHT   | DOWN    | Right + Bottom |

- **Optional button:** while the pushbutton (D6) is pressed, **all four LEDs on**,
  overriding the joystick. (Only if the button is installed.)

---

## 6. Open Items / Decisions

1. **Pushbutton for "all LEDs on":** The Parallax #27800 has **no built-in button**.
   To keep the "press to turn all on" feature, add a separate momentary pushbutton
   on D6. **Decision pending:** add a button (A) or drop the feature (B). The code is
   scaffolded to support an optional button so either path works.
2. **Axis direction sign:** Up/down and left/right polarity confirmed empirically on
   first upload; adjust `DEADZONE` comparisons or swap pot wiring as needed.
3. **DEADZONE tuning:** Default `150`; tune after testing for feel.
4. **LED colors:** Assign colors to positions in §4 once placed.

---

## 7. Software Architecture

- Single Arduino sketch: `firmware/cardinal_epic_device/cardinal_epic_device.ino`.
- No external libraries required (core `analogRead`/`digitalWrite` only).
- Structure:
  - Pin + threshold constants.
  - `setup()`: configure LED pins as `OUTPUT`, optional button as `INPUT_PULLUP`.
  - `loop()`: read axes → derive X/Y states → set LED outputs (with button override).
  - Small helpers for readability (e.g., `setLeds(...)`).

---

## 8. Build & Upload

1. Open `firmware/cardinal_epic_device/cardinal_epic_device.ino` in the Arduino IDE.
2. Select **Tools → Board → Arduino Uno**.
3. Select the correct **Tools → Port** (COM port for the Uno).
4. Click **Upload**.
5. Open **Serial Monitor** (optional, if debug prints are added) to observe raw axis values.

---

## 9. Future Work

Once validated, this repo pattern extends to the larger local-model + Arduino
projects (LLM-over-serial desk assistant, robot car, IR remote, GPS narrator, etc.).
Those live in separate repos.
