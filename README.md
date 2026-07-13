# Cardinal Epic Device

Arduino Uno project: a 2-axis analog joystick drives **four LEDs in a diamond
(cardinal-direction) layout**. Point the joystick and the matching LED lights up;
point diagonally and the two neighboring LEDs light; center the stick and all LEDs
go dark.

## Behavior

- **Up / Down / Left / Right** → the one LED in that direction lights.
- **Diagonal** (e.g. up-left) → the **two** neighboring LEDs light.
- **Centered** → all LEDs off.
- **Hold semantics:** LEDs are lit only while you hold the direction; release to
  center turns them off (no toggling/latching).
- **Full brightness** on/off only.
- **(Optional)** a separate pushbutton turns **all four LEDs on** while held — see note below.

## Hardware

| Qty | Component |
|-----|-----------|
| 1 | Arduino Uno |
| 1 | Parallax 2-Axis Joystick **#27800** (dual 10 kΩ pots, self-centering, **no button**) |
| 4 | LEDs (4 colors) + 4 × ~220–330 Ω resistors |
| 1 | Breadboard + jumper wires |
| (opt) | 1 momentary pushbutton (for the "all LEDs on" feature) |

## Wiring

### LEDs (each: `pin → resistor → LED anode → LED cathode → GND`)

| Direction | Diamond position | Arduino pin |
|-----------|-----------------|-------------|
| Up    | Top    | D2 |
| Right | Right  | D3 |
| Down  | Bottom | D4 |
| Left  | Left   | D5 |

### Joystick (Parallax #27800)

| Joystick pin | Arduino pin |
|--------------|-------------|
| `L/R+`        | 5V   |
| `L/R` (wiper) | A0   |
| `U/D+`        | 5V   |
| `U/D` (wiper) | A1   |
| `GND`         | GND  |

If an axis reads reversed, swap that pot's `+` and `GND` wires (or invert in code).

### Optional pushbutton
`D6 → button → GND`, read with `INPUT_PULLUP` (pressed = LOW).

> ⚠️ The Parallax #27800 has **no built-in pushbutton**. The "all LEDs on" feature
> requires adding a separate tactile button on D6, or it can be dropped. See `PRD.md`.

## Build & Upload

1. Open `firmware/cardinal_epic_device/cardinal_epic_device.ino` in the Arduino IDE.
2. **Tools → Board → Arduino Uno**, then select the correct **Port**.
3. **Upload**.

## Repo layout

```
cardinal-epic-device/
├── README.md
├── PRD.md                # canonical product-requirements doc
├── .gitignore
├── docs/
│   └── wiring.md         # detailed wiring notes
└── firmware/
    └── cardinal_epic_device/
        └── cardinal_epic_device.ino
```

## Status

We are 100% done with this project and have finished implementation entirely!
