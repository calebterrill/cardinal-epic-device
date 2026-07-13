# Wiring Notes

Detailed wiring reference for **Cardinal Epic Device**.

## Diamond LED layout

Physically arrange the four LEDs in a diamond so their positions match the joystick
directions:

```
            (D2) Top / UP
                  ( )
                   |
 (D5) Left ( ) --- + --- ( ) Right (D3)
                   |
                  ( )
          (D4) Bottom / DOWN
```

Each LED is wired: **Arduino pin → ~220–330 Ω resistor → LED anode (long leg) →
LED cathode (short leg) → GND rail**.

| Direction | Position | Pin | Color |
|-----------|----------|-----|-------|
| Up    | Top    | D2 | _TBD_ |
| Right | Right  | D3 | _TBD_ |
| Down  | Bottom | D4 | _TBD_ |
| Left  | Left   | D5 | _TBD_ |

## Joystick — Parallax #27800

The #27800 exposes two 10 kΩ potentiometers (one per axis) with a common ground.
Wire each pot as a voltage divider: `+` to 5V, `GND` to ground, wiper to an analog pin.

| Joystick pin | Arduino pin | Role |
|--------------|-------------|------|
| `L/R+`        | 5V   | X (left/right) pot high side |
| `L/R` (wiper) | A0   | X-axis analog signal |
| `U/D+`        | 5V   | Y (up/down) pot high side |
| `U/D` (wiper) | A1   | Y-axis analog signal |
| `GND`         | GND  | Common ground (either/both GND pins) |

### Verifying axis polarity
On first upload, open the Serial Monitor (if debug prints are enabled) and observe:
- Centered joystick → both axes ≈ **512**.
- If moving "up" *decreases* the Y reading (or the wrong LED lights), either swap the
  `U/D+` and `GND` wires on that pot, or flip the comparison in software.
- Same procedure for the X axis.

## Optional pushbutton (for "all LEDs on")

The Parallax #27800 has **no built-in switch**. To support the "press to light all
four LEDs" feature, add a separate momentary tactile pushbutton:

```
D6 ---- [ button ] ---- GND
```

Configured with `INPUT_PULLUP`, so **pressed = LOW**. If the button is omitted,
leave D6 unconnected and disable the feature in firmware.

## Power notes
- All grounds (Arduino GND, LED cathode rail, joystick GND, button GND) share a
  common ground rail on the breadboard.
- This project draws little current; USB power from the PC is sufficient. The 9V
  batteries are **not** needed here (they're for the motor projects).
