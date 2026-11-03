# Complete API Reference

Exhaustive technical dictionary for all functions, compile-time macros, callback signatures, and enumerations provided by **TextDrawFX**.

---

## Table of Contents

* [1. Control & Lifecycle Functions](#1-control--lifecycle-functions)
* [2. Opacity & Color Functions](#2-opacity--color-functions)
* [3. Physical Motion & Kinematics Functions](#3-physical-motion--kinematics-functions)
* [4. Text Reveal & Decryption Functions](#4-text-reveal--decryption-functions)
* [5. Combat, Cinematics & HUD Functions](#5-combat-cinematics--hud-functions)
* [6. 3D Preview Model Functions (Font 5)](#6-3d-preview-model-functions-font-5)
* [7. Global TextDraw API (Mirrored)](#7-global-textdraw-api-mirrored)
* [8. Event Callbacks](#8-event-callbacks)
* [9. Enumerations & Constants](#9-enumerations--constants)
* [10. Version Identification & Configuration Constants](#10-version-identification--configuration-constants)

---

## 1. Control & Lifecycle Functions

### `StopPlayerTextDrawFX`
```pawn
stock StopPlayerTextDrawFX(playerid, PlayerText:textid);
```
Terminates any active animation running on the specified PlayerTextDraw, frees internal particle instances, and restores base colors and coordinates.
* **`playerid`:** Connected player ID.
* **`textid`:** Valid PlayerTextDraw handle.
* **Returns:** `1` if an active effect was stopped, `0` otherwise.

### `StopTextDrawFX`
```pawn
stock StopTextDrawFX(Text:textid);
```
Terminates any active animation running on the specified Global TextDraw.
* **`textid`:** Valid Global TextDraw handle.
* **Returns:** `1` if stopped, `0` otherwise.

### `IsPlayerTextDrawFXActive`
```pawn
stock bool:IsPlayerTextDrawFXActive(playerid, PlayerText:textid);
```
Checks if an animation is currently processing on the PlayerTextDraw.
* **Returns:** `true` if active, `false` otherwise.

### `IsTextDrawFXActive`
```pawn
stock bool:IsTextDrawFXActive(Text:textid);
```
Checks if an animation is currently processing on the Global TextDraw.
* **Returns:** `true` if active, `false` otherwise.

---

## 2. Opacity & Color Functions

### `FadeInPlayerTextDraw`
```pawn
stock FadeInPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    duration = 1000, 
    bool:fadeBox = true, 
    targetAlpha = 255, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = "", 
    bool:autoHide = false
);
```
Smoothly fades in opacity from 0 to `targetAlpha`.
* **`duration`:** Transition time in milliseconds.
* **`targetAlpha`:** Maximum opacity level ($0$ - $255$).
* **`fadeBox`:** If `true`, simultaneously interpolates the background box opacity.

### `FadeOutPlayerTextDraw`
```pawn
stock FadeOutPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    duration = 1000, 
    bool:fadeBox = true, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = "", 
    bool:autoHide = true
);
```
Fades opacity from current level down to 0.
* **`autoHide`:** Automatically calls `PlayerTextDrawHide` upon reaching zero opacity.

### `FadeColorPlayerTextDraw`
```pawn
stock FadeColorPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    fromColor, 
    toColor, 
    duration = 1000, 
    bool:fadeBox = false, 
    fromBox = 0, 
    toBox = 0, 
    easing = TDFX_EASE_LINEAR, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Morphs RGBA color values with Penner easing support.

### `NeonFlickerPlayerTextDraw`
```pawn
stock NeonFlickerPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    duration = 2000, 
    brokenRatio = 75, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `ShimmerPlayerTextDraw`
```pawn
stock ShimmerPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    const baseTag[] = "~y~", 
    const shimmerTag[] = "~w~", 
    sweepDuration = 1000, 
    pauseDuration = 1500, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `RainbowPlayerTextDraw`
```pawn
stock RainbowPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    speedMs = 100, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `BlinkPlayerTextDraw`
```pawn
stock BlinkPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    interval = 500, 
    duration = 0, 
    bool:perChar = false, 
    colour = 0, 
    const text[] = "", 
    baseColour = 0, 
    bool:loop = false
);
```

---

## 3. Physical Motion & Kinematics Functions

### `SlidePlayerTextDraw`
```pawn
stock SlidePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:targetX, 
    Float:targetY, 
    duration = 1000, 
    easing = TDFX_EASE_OUT_QUAD, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Translates textdraw coordinates using Penner easing curves (`TDFX_EASE_*`).

### `BouncePlayerTextDraw`
```pawn
stock BouncePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:speedX = 3.2, 
    Float:speedY = 2.4, 
    Float:minX = 40.0, 
    Float:minY = 60.0, 
    Float:maxX = 600.0, 
    Float:maxY = 420.0, 
    bool:colorShift = false, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = "",
    Float:textWidth = 0.0,
    Float:textHeight = 0.0
);
```
2D physical reflection simulation inside bounding coordinates.
* **Automatic Dimension Compensation:** When `textWidth` and `textHeight` are `0.0` (default), TextDrawFX dynamically computes exact text dimensions and visual offsets (`leftOff, rightOff, topOff, bottomOff`) via GTA font tables, outline dilation, and `UseBox` / `TextSize` metrics across all alignments (Left, Center, Right). This clamps effective reflection boundaries (`effMinX = minX + leftOff`, `effMaxX = maxX - rightOff`, `effMinY = minY + topOff`, `effMaxY = maxY - bottomOff`), guaranteeing that the visual glyphs or background box will kiss the perimeter boundary without clipping outside the screen.
* **`textWidth`:** Optional manual override for text bounding width. If set $> 0.0$, replaces automatic measurement.
* **`textHeight`:** Optional manual override for text bounding height. If set $> 0.0$, replaces automatic measurement.

### `BounceCirclePlayerTextDraw`
```pawn
stock BounceCirclePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:speed = 2.8, 
    Float:angle = 45.0, 
    Float:centerX = 320.0, 
    Float:centerY = 240.0, 
    Float:radius = 120.0, 
    bool:colorShift = false, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = "",
    Float:textWidth = 0.0,
    Float:textHeight = 0.0
);
```
Elastic perimeter reflection within a circular arena.
* **Automatic Radial Padding:** Automatically offsets the reflection radius ($R_{	ext{eff}} = 	ext{radius} - \sqrt{	ext{halfW}^2 + 	ext{halfH}^2}$) using measured glyph or box dimensions so no edge of the TextDraw penetrates outside the circular perimeter.
* **`textWidth`:** Optional manual override for text bounding width.
* **`textHeight`:** Optional manual override for text bounding height.

### `OrbitPlayerTextDraw`
```pawn
stock OrbitPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:centerX, 
    Float:centerY, 
    Float:radiusX = 120.0, 
    Float:radiusY = 60.0, 
    Float:angularSpeed = 2.0, 
    Float:startAngle = 0.0, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `WaveDriftPlayerTextDraw`
```pawn
stock WaveDriftPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:amplitudeX, 
    Float:amplitudeY, 
    speedMs = 1500, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `PendulumPlayerTextDraw`
```pawn
stock PendulumPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:pivotX, Float:pivotY, 
    Float:armLength = 95.0, 
    Float:maxAngle = 45.0, 
    Float:damping = 0.55, 
    duration = 4200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `VortexPlayerTextDraw`
```pawn
stock VortexPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:centerX, 
    Float:centerY, 
    Float:startRadius = 120.0, 
    Float:spins = 2.5, 
    duration = 2000, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `WaveMotionPlayerTextDraw`
```pawn
stock WaveMotionPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    Float:centerX, Float:centerY, 
    Float:crestHeight = 15.0, 
    Float:crestWidth = 2.8, 
    direction = TDFX_DIR_LEFT_TO_RIGHT, 
    duration = 3200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Physical traveling sinusoidal wave arch across proportional font glyphs.

### `HopLetterPlayerTextDraw`
```pawn
stock HopLetterPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    Float:centerX, Float:centerY, 
    Float:hopHeight = 16.0, 
    staggerMs = 90, 
    hopDurationMs = 340, 
    direction = TDFX_DIR_LEFT_TO_RIGHT, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Cascading character leap with landing bounce.

---

## 4. Text Reveal & Decryption Functions

### `TypewriterPlayerTextDraw`
```pawn
stock TypewriterPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    interval = 50, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `ScramblePlayerTextDraw`
```pawn
stock ScramblePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const finalText[], 
    duration = 1500, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `GlitchPlayerTextDraw`
```pawn
stock GlitchPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[] = "", 
    intensity = 2, 
    duration = 3000, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `SlotMachinePlayerTextDraw`
```pawn
stock SlotMachinePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const finalText[], 
    spinDuration = 2200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `SplitFlapPlayerTextDraw`
```pawn
stock SplitFlapPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const targetText[], 
    charRollMs = 80, 
    lockIntervalMs = 200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `ScannerSweepPlayerTextDraw`
```pawn
stock ScannerSweepPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    const laserTag[] = "~r~", 
    const trailTag[] = "~y~", 
    speedMs = 60, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `LetterRainPlayerTextDraw`
```pawn
stock LetterRainPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    Float:targetX, Float:targetY, 
    Float:letterSpacing = 0.0, 
    duration = 3200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Pass `letterSpacing = 0.0` for automatic proportional font kerning.

---

## 5. Combat, Cinematics & HUD Functions

### `ShockwaveSlamPlayerTextDraw`
```pawn
stock ShockwaveSlamPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:centerX, 
    Float:centerY, 
    duration = 1800, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `ShakePlayerTextDraw`
```pawn
stock ShakePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:intensity = 5.0, 
    duration = 1000, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `FireworkPlayerTextDraw`
```pawn
stock FireworkPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    pattern = TDFX_FW_BURST_SPHERE, 
    fuseDuration = 1500, 
    explodeDuration = 1200, 
    Float:shakeIntensity = 6.0, 
    Float:burstSpeed = 6.0, 
    particleCount = 10, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `LetterboxPlayerTextDraw`
```pawn
stock LetterboxPlayerTextDraw(
    playerid, 
    PlayerText:topBarTD, 
    PlayerText:bottomBarTD, 
    Float:barHeight = 65.0, 
    duration = 1500, 
    bool:reverse = false, 
    easing = TDFX_EASE_OUT_QUAD, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Smoothly slides 21:9 cinematic black bars from the top ($Y=0.0$) and bottom ($Y=480.0$) screen boundaries to create widescreen letterboxing.
* **`topBarTD`:** PlayerTextDraw handle for the upper cinema bar.
* **`bottomBarTD`:** PlayerTextDraw handle for the lower cinema bar (`PlayerText:0xFFFF` to omit).
* **`barHeight`:** Desired visible pixel height for each cinema bar (defaults to 65.0 units).
* **`duration`:** Animation slide duration in milliseconds.
* **`reverse`:** If `false`, slides bars in (cinematic intro); if `true`, slides bars out (cinematic outro).
* **`easing`:** Easing formula (default `TDFX_EASE_OUT_QUAD`).
* **`loop`:** If `true`, repeats slide cycle indefinitely until stopped.

> [!NOTE]
> **Dynamic Letter Size Scaling:** The function automatically calculates and applies dynamic letter sizes (`topLetY = barHeight / 9.5` and `botLetY = (barHeight + 35.8) / 9.5`) to both bars. This guarantees that the visible boxes dynamically scale to the exact requested `barHeight` in pixels without top gaps or bottom clipping, for any custom height.

### `SonarPingPlayerTextDraw`
```pawn
stock SonarPingPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:centerX, Float:centerY, 
    Float:maxRadius = 140.0, 
    pulseCount = 3, 
    pulseDuration = 1200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `HologramPlayerTextDraw`
```pawn
stock HologramPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    duration = 5500, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `ElectricArcPlayerTextDraw`
```pawn
stock ElectricArcPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    const text[], 
    surgeDuration = 3200, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `PulsePlayerTextDraw`
```pawn
stock PulsePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:amplitude = 0.25, 
    interval = 300, 
    cycles = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `ScalePlayerTextDraw`
```pawn
stock ScalePlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:startSizeX, Float:startSizeY, 
    Float:targetSizeX, Float:targetSizeY, 
    duration = 1000, 
    easing = TDFX_EASE_LINEAR, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `CountPlayerTextDrawNumber`
```pawn
stock CountPlayerTextDrawNumber(
    playerid, 
    PlayerText:textid, 
    startValue, 
    targetValue, 
    duration = 1000, 
    const prefix[] = "", 
    const suffix[] = "", 
    bool:thousandsSep = false, 
    easing = TDFX_EASE_LINEAR, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `FillPlayerTextDrawProgress`
```pawn
stock FillPlayerTextDrawProgress(
    playerid, 
    PlayerText:textid, 
    Float:startWidth, 
    Float:targetWidth, 
    duration = 2000, 
    easing = TDFX_EASE_OUT_QUAD, 
    Float:lineHeight = 0.0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```
Smoothly interpolates a native textdraw background box to represent loading/progress bars using custom easing curves without external plugins.
* **`startWidth`:** Starting box width or boundary coordinate.
* **`targetWidth`:** Final target box width or boundary coordinate.
* **`duration`:** Fill duration in milliseconds.
* **`easing`:** Easing interpolation algorithm (default `TDFX_EASE_OUT_QUAD`).
* **`lineHeight`:** Secondary dimension parameter (defaults to existing dimension if `<= 0.0`).
* **`loop`:** If `true`, repeats progress fill indefinitely until stopped.

> [!IMPORTANT]
> **SA-MP Box Geometry & Alignment Semantics:**
> * **Left Alignment (`Alignment 1`):** In SA-MP's native box rendering for left-aligned textdraws, `TextDrawTextSize(textid, x, y)` treats `x` as the **absolute horizontal coordinate on screen** where the box ends (not relative width), while `y` defines clickable height. The visual height of the box itself is determined strictly by `LetterSize Y`. Consequently, for left alignment, `startWidth` and `targetWidth` represent the absolute screen X end coordinates.
> * **Center Alignment (`Alignment 2`):** In center-aligned textdraws, parameter `x` defines the text wrapping boundary, while parameter `y` controls the actual box width. `TextDrawFX` automatically detects alignment 2: when `lineHeight <= 0.0`, it reads line wrap width from `TextSize X` and smoothly animates `TextSize Y` from `startWidth` to `targetWidth` (which represents the true box width).

---

## 6. 3D Preview Model Functions (Font 5)

### `Spin3DPlayerTextDraw`
```pawn
stock Spin3DPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:speedX = 0.0, 
    Float:speedY = 0.0, 
    Float:speedZ = 45.0, 
    Float:zoom = 1.0, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

### `Hover3DPlayerTextDraw`
```pawn
stock Hover3DPlayerTextDraw(
    playerid, 
    PlayerText:textid, 
    Float:tiltX = 8.0, 
    Float:tiltZ = 12.0, 
    Float:zoomAmp = 0.06, 
    speedMs = 2400, 
    duration = 0, 
    bool:loop = false,
    extra_id = 0, 
    const callback[] = ""
);
```

---

## 7. Global TextDraw API (Mirrored)

All functions documented above have identical global counterparts that operate on `Text:textid` across all connected players:

```pawn
// Examples of Global TextDraw equivalents:
FadeInTextDraw(Text:textid, duration = 1000, ...);
FadeOutTextDraw(Text:textid, duration = 1000, ...);
SlideTextDraw(Text:textid, Float:targetX, Float:targetY, duration = 1000, ...);
WaveMotionTextDraw(Text:textid, const text[], Float:centerX, Float:centerY, ...);
HopLetterTextDraw(Text:textid, const text[], Float:centerX, Float:centerY, ...);
ShockwaveSlamTextDraw(Text:textid, Float:centerX, Float:centerY, ...);
FireworkTextDraw(Text:textid, const text[], ...);
LetterboxTextDraw(Text:topBarTD, Text:bottomBarTD, Float:barHeight = 65.0, ...);
StopTextDrawFX(Text:textid);
```

> [!NOTE]
> Standard native hooks are active: calling `TextDrawHideForAll(textid)` or `TextDrawDestroy(textid)` automatically stops any running animation and cleans up internal resources, maintaining complete parity with `PlayerTextDrawHide` and `PlayerTextDrawDestroy`. If `TextDrawHideForPlayer` is called on the last viewing player of an active animation, the animation timer is automatically stopped. Furthermore, starting a new global animation on a previously hidden textdraw automatically wakes it up for its target audience (all connected players if shown globally, or the specific player group if shown per-player), preventing repeated animations from running invisibly while strictly avoiding visibility leaks to other players.

---

## 8. Event Callbacks

Global event callbacks invoked automatically when an animation reaches its duration or finishes its execution cycle (unconditionally triggered regardless of whether `extra_id` is set):

### `OnPlayerTextDrawFXFinish`
Called when an effect completes on a player-specific TextDraw:
```pawn
public OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id)
{
    return 1;
}
```

### `OnTextDrawFXFinish`
Called when an effect completes on a global TextDraw:
```pawn
public OnTextDrawFXFinish(Text:textid, fx_type, extra_id)
{
    return 1;
}
```

---

## 9. Enumerations & Constants

### Effect Identifiers (`fx_type`)
```pawn
#define TDFX_TYPE_NONE          (0)
#define TDFX_TYPE_BLINK         (1)
#define TDFX_TYPE_TYPEWRITER    (2)
#define TDFX_TYPE_FADE          (3)
#define TDFX_TYPE_SCALE         (4)
#define TDFX_TYPE_PULSE         (5)
#define TDFX_TYPE_SLIDE         (6)
#define TDFX_TYPE_COUNT         (7)
#define TDFX_TYPE_SCRAMBLE      (8)
#define TDFX_TYPE_BOUNCE        (9)
#define TDFX_TYPE_BOUNCE_CIRC   (10)
#define TDFX_TYPE_ORBIT         (11)
#define TDFX_TYPE_SHAKE         (12)
#define TDFX_TYPE_NEON          (13)
#define TDFX_TYPE_WAVE          (14)
#define TDFX_TYPE_PROGRESS      (15)
#define TDFX_TYPE_GLITCH        (16)
#define TDFX_TYPE_FIREWORK      (17)
#define TDFX_TYPE_SPIN3D        (18)
#define TDFX_TYPE_HOVER3D       (19)
#define TDFX_TYPE_SHIMMER       (20)
#define TDFX_TYPE_RAINBOW       (21)
#define TDFX_TYPE_SLOT          (22)
#define TDFX_TYPE_PENDULUM      (23)
#define TDFX_TYPE_SLAM          (24)
#define TDFX_TYPE_VORTEX        (25)
#define TDFX_TYPE_LETTERBOX     (26)
#define TDFX_TYPE_SONAR         (27)
#define TDFX_TYPE_WAVESWEEP     (28)
#define TDFX_TYPE_LETTERRAIN    (29)
#define TDFX_TYPE_SCANNER       (30)
#define TDFX_TYPE_ELECTRIC      (31)
#define TDFX_TYPE_SPLITFLAP     (32)
#define TDFX_TYPE_HOLOGRAM      (33)
#define TDFX_TYPE_SHOCKWAVE     (34)
#define TDFX_TYPE_WAVEMOTION    (35)
#define TDFX_TYPE_HOPLETTER     (36)
```

### Direction Constants
```pawn
#define TDFX_DIR_LEFT_TO_RIGHT  (0)
#define TDFX_DIR_RIGHT_TO_LEFT  (1)
#define TDFX_DIR_BOUNCE         (2)
```

### Easing Curve Modes
```pawn
#define TDFX_EASE_LINEAR        (0)
#define TDFX_EASE_IN_QUAD       (1)
#define TDFX_EASE_OUT_QUAD      (2)
#define TDFX_EASE_IN_OUT_QUAD   (3)
#define TDFX_EASE_OUT_BOUNCE    (4)
```

### Firework Burst Patterns
```pawn
#define TDFX_FW_SPHERE          (0)
#define TDFX_FW_FAN             (1)
#define TDFX_FW_ROCKET          (2)
#define TDFX_FW_RING            (3)
#define TDFX_FW_FOUNTAIN        (4)
```

---

## 10. Version Identification & Configuration Constants

### Semantic Version Constants
```pawn
#define TEXTDRAWFX_VERSION_MAJOR              2
#define TEXTDRAWFX_VERSION_MINOR              2
#define TEXTDRAWFX_VERSION_PATCH              0
#define TEXTDRAWFX_VERSION_STRING             "2.2.0"
```

| Macro | Type | Description |
| :--- | :---: | :--- |
| `TEXTDRAWFX_VERSION_MAJOR` | `cell` | Major version number (breaking API changes). |
| `TEXTDRAWFX_VERSION_MINOR` | `cell` | Minor version number (new backward-compatible features). |
| `TEXTDRAWFX_VERSION_PATCH` | `cell` | Patch / maintenance revision number (bug fixes and optimizations). |
| `TEXTDRAWFX_VERSION_STRING`| `string` | Complete semantic version string (`"2.2.0"`). |

### Compile-Time Configuration Constants

| Macro | Default | Purpose |
| :--- | :---: | :--- |
| `TDFX_MAX_FX` | `128` | Maximum active concurrent animation instances across the server. |
| `TDFX_MAX_CHAR_INSTANCES` | `16` | Maximum concurrent multi-character animation instances. |
| `TDFX_MAX_CHAR_PARTS` | `32` | Maximum letters animated individually per character instance. |
| `TDFX_MAX_FW_INSTANCES` | `16` | Maximum concurrent particle burst instances (Fireworks / Shockwaves). |
| `TDFX_FW_PARTICLES` | `12` | Particles per firework or shockwave explosion instance. |
| `TDFX_MAX_TEXT` | `256` | Maximum character capacity for dynamic string buffers. |
| `TDFX_MAX_TD` | `256` | Maximum PlayerTextDraw ID tracking capacity per player. |
| `TDFX_MAX_GLOBAL_TD` | `2048` | Maximum Global TextDraw ID tracking capacity (`0` = disabled). |
| `TDFX_LETTERBOX_SCREEN_BOTTOM` | `480.0` | Screen bottom boundary coordinate for 21:9 letterbox black bars. |
| `TDFX_LETTERBOX_ASCENT_OFFSET` | `35.8` | Font 1 vertical ascent padding calibration constant. |
| `TDFX_FRAME_INTERVAL`| `40` | Internal physics interpolation tick frequency in milliseconds. |
| `TDFX_MIN_INTERVAL`  | `30` | Minimum allowed timer tick rate protecting server performance. |

---

## 10. Position & Metric Getters

### `TDFX_GetPlayerPos`
```pawn
stock bool:TDFX_GetPlayerPos(playerid, PlayerText:textid, &Float:x, &Float:y);
```
Retrieves the exact spatial screen coordinates of a PlayerTextDraw.
* Uses native open.mp `PlayerTextDrawGetPos` when available.
* Seamlessly falls back to the internal `TextDrawFX` memory coordinate cache (`TDFX_RemPosX`/`TDFX_RemPosY`) on standard SA-MP servers.
* **Returns:** `true` if valid coordinates were resolved, `false` otherwise.

### `TDFX_GetGlobalPos`
```pawn
stock bool:TDFX_GetGlobalPos(Text:textid, &Float:x, &Float:y);
```
Retrieves the exact spatial screen coordinates of a Global TextDraw.
* Uses native open.mp `TextDrawGetPos` when available.
* Seamlessly falls back to the internal `TextDrawFX` memory coordinate cache on standard SA-MP servers.
* **Returns:** `true` if valid coordinates were resolved, `false` otherwise.

### `TDFX_GetFreePlayerTDCount`
```pawn
stock TDFX_GetFreePlayerTDCount(playerid);
```
Calculates how many PlayerTextDraw allocation slots remain available for the player in real time.
* In open.mp, directly probes `IsValidPlayerTextDraw` across `0 .. MAX_PLAYER_TEXT_DRAWS-1`.
* In SA-MP 0.3.7, uses a safe dynamic memory probe (allocating server structures without network RPCs and freeing immediately) to discover true headroom in real time with zero cache delay, preventing slot exhaustion before spawning multi-character or firework bursts.
* **Returns:** Count of remaining free PlayerTextDraw slots.

### `TDFX_GetFreeGlobalTDCount`
```pawn
stock TDFX_GetFreeGlobalTDCount();
```
Calculates how many Global TextDraw allocation slots remain available on the server in real time.
* In open.mp, directly probes `IsValidTextDraw` across `0 .. MAX_TEXT_DRAWS-1`.
* In SA-MP 0.3.7, utilizes safe dynamic server-side structure probing without network overhead.
* **Returns:** Count of remaining free Global TextDraw slots.

### `TDFX_Exit`
```pawn
stock TDFX_Exit();
```
Cleans up all active animation slots across the entire server, stopping timers and cleanly restoring base text strings, colors, letter sizes, and spatial coordinates without triggering completion callbacks. Automatically called on `OnFilterScriptExit` and `OnGameModeExit` via ALS v7 hooks.
