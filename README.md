# TextDrawFX

[![GitHub release](https://img.shields.io/github/v/release/Walter-Correa/TextDrawFX?include_prereleases&color=blue)](https://github.com/Walter-Correa/TextDrawFX/releases)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)
[![Platform](https://img.shields.io/badge/platform-SA--MP%20%7C%20open.mp-orange.svg)](https://open.mp)

**TextDrawFX** is a comprehensive, high-performance, lightweight, and plug-and-play visual effects, animation, and textdraw manipulation library engineered specifically for **San Andreas Multiplayer (SA-MP)** and **Open.MP**.

It empowers Pawn developers to bring static GTA interfaces to life with rich visual effects: **2D Physics Bounding Wall Reflection (DVD screensaver)**, **Circular Elastic Perimeter Bounce**, **Continuous Orbital Paths**, **Combat Trauma Screen Shake**, **Stochastic Broken Neon Flickering**, **Waveform Floating Drift**, **Dynamic Native Progress Bar Interpolation**, **Cyberpunk Chromatic Glitch Storms**, **True RGBA & Alpha Fading**, **Dynamic Scaling & Heartbeat Pulsing**, **Penner Easing Motion (Sliding)**, **Animated Numeric Counters**, **Cyberpunk Matrix Glyph Decryption**, **Cinematic Typewriter Reveals**, **Chromatic Alert Shifts**, and classical **Visibility Blinking**—all operating with zero external dependencies and mirrored support for both **PlayerTextDraws** (`PlayerText:textid`) and **Global TextDraws** (`Text:textid`).

---

## Installation

### 1. File Placement

Copy `TextDrawFX.inc` into your server's include directory:
* **SA-MP / Pawno:** `pawno/include/TextDrawFX.inc`
* **open.mp / Qawno:** `qawno/include/TextDrawFX.inc`

### 2. Script Inclusion

Add the include near the top of your gamemode or filterscript, after `<a_samp>` or `<open.mp>`:

```pawn
#include <open.mp> // or <a_samp>
#include <TextDrawFX>
```

> [!TIP]
> If `<open.mp>` or `<a_samp>` has not yet been included when `<TextDrawFX>` is parsed, the include will automatically detect and include the correct header for you.

---

## Configuration & Compile-Time Defines

You can customize the following configuration constants before including `<TextDrawFX>` to tailor memory usage and performance:

| Constant | Default Value | Unit / Constraints | Description |
| :--- | :---: | :---: | :--- |
| `TDFX_MAX_FX` | `64` | Slots ($1$ - $512$) | Maximum concurrent animations active simultaneously across the server. |
| `TDFX_MAX_TEXT` | `256` | Cells (Characters) | Maximum string buffer size allocated for typewriter/scramble/glitch handling. |
| `TDFX_MAX_TD` | `MAX_PLAYER_TEXT_DRAWS` (`256`) | Range ($1$ - $256$) | Maximum PlayerTextDraw IDs cached per player for SA-MP state fallback. |
| `TDFX_MAX_GLOBAL_TD` | `MAX_TEXT_DRAWS` (`2048`) | Range ($1$ - $2048$) | Maximum Global TextDraw IDs cached for SA-MP state fallback. |
| `TDFX_MIN_INTERVAL` | `30` | Milliseconds | Hard lower limit for animation timer intervals (prevents timer spam). |
| `TDFX_FRAME_INTERVAL` | `40` | Milliseconds (25 FPS) | Internal tick rate for continuous interpolations (Fade, Scale, Slide, Bounce, Orbit). |

```pawn
// Example: Tailoring memory footprint for lightweight servers
#define TDFX_MAX_FX 32
#define TDFX_MAX_GLOBAL_TD 128
#include <TextDrawFX>
```

---

## Constants & Enumerations

### Effect Types (`fx_type`)
Passed to `OnPlayerTextDrawFXFinish` and `OnTextDrawFXFinish` to identify which animation mode concluded:

| Identifier | Value | Description |
| :--- | :---: | :--- |
| `TDFX_TYPE_NONE` | `0` | No effect / Idle. |
| `TDFX_TYPE_BLINK` | `1` | Classical visibility blink or chromatic color alert. |
| `TDFX_TYPE_TYPEWRITER` | `2` | Character-by-character typewriter reveal. |
| `TDFX_TYPE_FADE` | `3` | True Alpha or RGBA color cross-fade. |
| `TDFX_TYPE_SCALE` | `4` | Dimension scaling (LetterSize / TextSize). |
| `TDFX_TYPE_PULSE` | `5` | Oscillating heartbeat pulse. |
| `TDFX_TYPE_SLIDE` | `6` | Coordinate motion with easing curves. |
| `TDFX_TYPE_COUNT` | `7` | Numeric counter interpolation. |
| `TDFX_TYPE_SCRAMBLE` | `8` | Cyberpunk glyph decrypt effect. |
| `TDFX_TYPE_BOUNCE` | `9` | 2D rectangular box bounce with velocity reflection and color shift. |
| `TDFX_TYPE_BOUNCE_CIRC` | `10` | Elastic circular arena perimeter bounce. |
| `TDFX_TYPE_ORBIT` | `11` | Continuous circular or elliptical orbital motion path. |
| `TDFX_TYPE_SHAKE` | `12` | Combat trauma screen shake with exponential decay. |
| `TDFX_TYPE_NEON` | `13` | Stochastic broken fluorescent tube malfunction / flicker. |
| `TDFX_TYPE_WAVE` | `14` | Coupled dual-sine harmonic floating wave / drift. |
| `TDFX_TYPE_PROGRESS` | `15` | Native smooth box width progress bar fill with easing. |
| `TDFX_TYPE_GLITCH` | `16` | Cyberpunk chromatic glyph disturbance and micro-alpha glitching. |

### Easing Functions (`easing`)

| Identifier | Value | Motion Characteristic |
| :--- | :---: | :--- |
| `TDFX_EASE_LINEAR` | `0` | Constant velocity from start to finish. |
| `TDFX_EASE_IN_QUAD` | `1` | Accelerates smoothly from zero velocity. |
| `TDFX_EASE_OUT_QUAD` | `2` | Decelerates smoothly to a stop (ideal for UI menus and banners). |
| `TDFX_EASE_IN_OUT_QUAD` | `3` | Accelerates at start and decelerates towards conclusion. |
| `TDFX_EASE_OUT_BOUNCE` | `4` | Bounces at destination before settling (spring/elastic feel). |

---

## Functions

### 1. Alpha & Color Fading

#### `FadeInPlayerTextDraw`
```pawn
stock FadeInPlayerTextDraw(playerid, PlayerText:textid, duration = 1000, bool:fadeBox = true, targetAlpha = 255, extra_id = 0, const callback[] = "", bool:autoHide = false);
```
Smoothly transitions a player textdraw from transparent ($0\%$ opacity) to its target opacity over the specified duration.
* **`playerid`:** The ID of the player who owns the textdraw.
* **`textid`:** The `PlayerText:` handle of the textdraw to fade in.
* **`duration`:** Time in milliseconds for the transition to complete (defaults to `1000ms`).
* **`fadeBox`:**
  * `true` *(default)*: Simultaneously interpolates the alpha channel of the background box along with the text.
  * `false`: Fades only the foreground text/sprite tint, preserving static box opacity.
* **`targetAlpha`:** Maximum opacity level to reach ($0$ to $255$, defaults to `255` = $100\%$).
* **`extra_id`:** Optional user-defined identifier passed to `OnPlayerTextDrawFXFinish`. Passing `0` disables event firing.
* **`callback[]`:** Name of an optional targeted public function to call upon completion.
* **`autoHide`:** Whether to automatically hide the textdraw when the fade completes (`false` by default).
* **Returns:** `1` on success, `0` if player is disconnected, textdraw handle is invalid, or animation pool is full.

#### `FadeInTextDraw`
```pawn
stock FadeInTextDraw(Text:textid, duration = 1000, bool:fadeBox = true, targetAlpha = 255, extra_id = 0, const callback[] = "", bool:autoHide = false);
```
Server-wide mirror of `FadeInPlayerTextDraw` for Global TextDraws (`Text:textid`). Operates for all players simultaneously.
* **`textid`:** The `Text:` handle of the global textdraw to fade in.
* **`duration`:** Time in milliseconds for the transition to complete (defaults to `1000ms`).
* **`fadeBox`:** Whether to fade the background box alpha simultaneously (defaults to `true`).
* **`targetAlpha`:** Target opacity level ($0$ to $255$, defaults to `255`).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public callback name.
* **`autoHide`:** Whether to hide the textdraw for all players on completion (`false` by default).
* **Returns:** `1` on success, `0` on failure.

#### `FadeOutPlayerTextDraw`
```pawn
stock FadeOutPlayerTextDraw(playerid, PlayerText:textid, duration = 1000, bool:fadeBox = true, extra_id = 0, const callback[] = "", bool:autoHide = true);
```
Smoothly fades a player textdraw from its current opacity down to fully transparent ($0\%$ alpha) over the specified duration.
* **`playerid`:** The ID of the player who owns the textdraw.
* **`textid`:** The `PlayerText:` handle to fade out.
* **`duration`:** Time in milliseconds for the fade out (defaults to `1000ms`).
* **`fadeBox`:** Whether to fade the background box alpha simultaneously (defaults to `true`).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **`autoHide`:**
  * `true` *(default)*: Automatically calls `PlayerTextDrawHide` as soon as the fade finishes, removing the transparent element from the player's screen.
  * `false`: Leaves the textdraw shown with zero alpha.
* **Returns:** `1` on success, `0` on failure.

#### `FadeOutTextDraw`
```pawn
stock FadeOutTextDraw(Text:textid, duration = 1000, bool:fadeBox = true, extra_id = 0, const callback[] = "", bool:autoHide = true);
```
Server-wide mirror of `FadeOutPlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw to fade out.
* **`duration`:** Time in milliseconds for the transition (defaults to `1000ms`).
* **`fadeBox`:** Whether to fade the box simultaneously (defaults to `true`).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **`autoHide`:** Automatically calls `TextDrawHideForAll` when finished (defaults to `true`).
* **Returns:** `1` on success, `0` on failure.

#### `FadeColorPlayerTextDraw`
```pawn
stock FadeColorPlayerTextDraw(playerid, PlayerText:textid, fromColor, toColor, duration = 1000, bool:fadeBox = false, fromBox = 0, toBox = 0, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "");
```
Interpolates individual RGBA color channels smoothly from an initial color to a target color over time.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle.
* **`fromColor`:** Starting RGBA color (`0xRRGGBBAA`).
* **`toColor`:** Destination RGBA color (`0xRRGGBBAA`).
* **`duration`:** Time in milliseconds for the transition (defaults to `1000ms`).
* **`fadeBox`:** Whether to interpolate the background box color as well (`false` by default).
* **`fromBox`:** Starting background box RGBA color.
* **`toBox`:** Destination background box RGBA color.
* **`easing`:** Easing formula to govern the transition speed (`TDFX_EASE_LINEAR`, `TDFX_EASE_IN_QUAD`, etc.).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

---

### 2. Dimension Scaling & Pulsing

#### `ScalePlayerTextDraw`
```pawn
stock ScalePlayerTextDraw(playerid, PlayerText:textid, Float:startSizeX, Float:startSizeY, Float:targetSizeX, Float:targetSizeY, duration = 1000, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "");
```
Smoothly scales the font letter size (`PlayerTextDrawLetterSize`) from an initial dimension to a target dimension using physical easing curves.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to scale.
* **`startSizeX`:** Initial letter width.
* **`startSizeY`:** Initial letter height.
* **`targetSizeX`:** Final destination letter width.
* **`targetSizeY`:** Final destination letter height.
* **`duration`:** Time in milliseconds to complete the scaling (defaults to `1000ms`).
* **`easing`:** Easing curve applied to the animation (`TDFX_EASE_LINEAR`, `TDFX_EASE_OUT_BOUNCE`, etc.).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `ScaleTextDraw`
```pawn
stock ScaleTextDraw(Text:textid, Float:startSizeX, Float:startSizeY, Float:targetSizeX, Float:targetSizeY, duration = 1000, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `ScalePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle to scale.
* **`startSizeX`, `startSizeY`:** Starting letter dimensions.
* **`targetSizeX`, `targetSizeY`:** Target letter dimensions.
* **`duration`:** Time in milliseconds.
* **`easing`:** Easing function identifier.
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `PulsePlayerTextDraw`
```pawn
stock PulsePlayerTextDraw(playerid, PlayerText:textid, Float:amplitude = 0.25, interval = 300, cycles = 0, extra_id = 0, const callback[] = "");
```
Rhythmically pulses textdraw dimensions around its base letter size using a smooth harmonic sine wave.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to pulse.
* **`amplitude`:** Scale expansion factor (e.g. `0.25` expands dimensions up to $+25\%$, defaults to `0.25`).
* **`interval`:** Duration of a single pulse cycle in milliseconds (defaults to `300ms`).
* **`cycles`:**
  * `0` *(default)*: Pulses continuously in an infinite loop until stopped manually.
  * `> 0`: Executes the exact number of designated heartbeat cycles, then automatically restores base dimensions and triggers completion events.
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `PulseTextDraw`
```pawn
stock PulseTextDraw(Text:textid, Float:amplitude = 0.25, interval = 300, cycles = 0, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `PulsePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle to pulse.
* **`amplitude`:** Pulse expansion ratio.
* **`interval`:** Time in milliseconds per heartbeat.
* **`cycles`:** Repetition count (`0` for continuous loop).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

---

### 3. Positional Motion & Physics

#### `SlidePlayerTextDraw`
```pawn
stock SlidePlayerTextDraw(playerid, PlayerText:textid, Float:targetX, Float:targetY, duration = 1000, easing = TDFX_EASE_OUT_QUAD, extra_id = 0, const callback[] = "");
```
Smoothly relocates a player textdraw across 2D screen coordinates from its current position to `(targetX, targetY)` using mathematical easing curves.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to glide.
* **`targetX`:** Destination horizontal screen coordinate ($0.0$ to $640.0$).
* **`targetY`:** Destination vertical screen coordinate ($0.0$ to $480.0$).
* **`duration`:** Time in milliseconds to travel to the target (defaults to `1000ms`).
* **`easing`:** Easing curve applied to the motion (defaults to `TDFX_EASE_OUT_QUAD` for natural UI deceleration).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `SlideTextDraw`
```pawn
stock SlideTextDraw(Text:textid, Float:targetX, Float:targetY, duration = 1000, easing = TDFX_EASE_OUT_QUAD, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `SlidePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`targetX`, `targetY`:** Destination screen coordinates.
* **`duration`:** Duration in milliseconds.
* **`easing`:** Easing curve identifier.
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `BouncePlayerTextDraw`
```pawn
stock BouncePlayerTextDraw(playerid, PlayerText:textid, Float:speedX, Float:speedY, Float:minX, Float:minY, Float:maxX, Float:maxY, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "");
```
Simulates the classic retro DVD screensaver: moves the textdraw in 2D space with a velocity vector `(speedX, speedY)` and bounces off rectangular boundary walls with velocity reflection, optional DVD color shifts, and corner hit detection.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to bounce.
* **`speedX`:** Horizontal velocity vector per tick (pixels/frame).
* **`speedY`:** Vertical velocity vector per tick (pixels/frame).
* **`minX`, `minY`:** Top-left boundary box coordinates ($0.0$ to $640.0$).
* **`maxX`, `maxY`:** Bottom-right boundary box coordinates ($0.0$ to $480.0$).
* **`colorShift`:** Whether to alternate text color randomly from the vibrant DVD palette upon each wall collision (`false` by default).
* **`duration`:** Total duration in milliseconds (`0` for indefinite looping until manually stopped).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `BounceTextDraw`
```pawn
stock BounceTextDraw(Text:textid, Float:speedX, Float:speedY, Float:minX, Float:minY, Float:maxX, Float:maxY, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `BouncePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`speedX`, `speedY`:** Velocity vector per tick.
* **`minX`, `minY`, `maxX`, `maxY`:** Bounding box limits.
* **`colorShift`:** Whether to cycle colors on bounce.
* **`duration`:** Total runtime in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `BounceCirclePlayerTextDraw`
```pawn
stock BounceCirclePlayerTextDraw(playerid, PlayerText:textid, Float:speed, Float:angle, Float:centerX, Float:centerY, Float:radius, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "");
```
Confines the textdraw inside a circular arena, reflecting its velocity along the radial collision surface normal upon contact with the circular perimeter.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to bounce.
* **`speed`:** Movement speed scalar (pixels/frame).
* **`angle`:** Initial movement heading in degrees ($0.0^\circ$ to $360.0^\circ$).
* **`centerX`, `centerY`:** Center origin of the circular arena.
* **`radius`:** Radius of the arena boundary in pixels.
* **`colorShift`:** Whether to shift color upon each perimeter collision.
* **`duration`:** Total duration in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `BounceCircleTextDraw`
```pawn
stock BounceCircleTextDraw(Text:textid, Float:speed, Float:angle, Float:centerX, Float:centerY, Float:radius, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `BounceCirclePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`speed`:** Movement speed scalar.
* **`angle`:** Initial direction angle in degrees.
* **`centerX`, `centerY`:** Center origin.
* **`radius`:** Boundary radius.
* **`colorShift`:** Whether to cycle colors on bounce.
* **`duration`:** Duration in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `OrbitPlayerTextDraw`
```pawn
stock OrbitPlayerTextDraw(playerid, PlayerText:textid, Float:centerX, Float:centerY, Float:radiusX, Float:radiusY, Float:angularSpeed = 60.0, Float:startAngle = 0.0, duration = 0, extra_id = 0, const callback[] = "");
```
Guides the textdraw along a continuous, smooth circular or elliptical trigonometric orbital trajectory around an anchor coordinate.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to orbit.
* **`centerX`, `centerY`:** Center origin around which the element rotates.
* **`radiusX`:** Horizontal radius of the orbit ($X$-axis).
* **`radiusY`:** Vertical radius of the orbit ($Y$-axis). Setting `radiusX == radiusY` produces a circular orbit.
* **`angularSpeed`:** Speed of rotation in degrees per second (e.g. `90.0` completes one full rotation every $4$ seconds; negative values rotate counter-clockwise).
* **`startAngle`:** Starting angular offset in degrees ($0.0^\circ$ to $360.0^\circ$, defaults to `0.0`).
* **`duration`:** Total duration in milliseconds (`0` for continuous orbit).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `OrbitTextDraw`
```pawn
stock OrbitTextDraw(Text:textid, Float:centerX, Float:centerY, Float:radiusX, Float:radiusY, Float:angularSpeed = 60.0, Float:startAngle = 0.0, duration = 0, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `OrbitPlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`centerX`, `centerY`:** Pivot origin.
* **`radiusX`, `radiusY`:** Horizontal and vertical radii.
* **`angularSpeed`:** Angular speed in degrees per second.
* **`startAngle`:** Starting phase angle.
* **`duration`:** Duration in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `ShakePlayerTextDraw`
```pawn
stock ShakePlayerTextDraw(playerid, PlayerText:textid, Float:intensity = 6.0, duration = 800, extra_id = 0, const callback[] = "");
```
Applies a high-frequency micro-displacement trauma screen shake with quadratic attenuation decay ($trauma^2 \cdot intensity$), automatically restoring base anchor coordinates upon completion.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to shake.
* **`intensity`:** Maximum pixel displacement offset at maximum trauma (defaults to `6.0` pixels).
* **`duration`:** Total duration of the shockwave impact in milliseconds (defaults to `800ms`).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `ShakeTextDraw`
```pawn
stock ShakeTextDraw(Text:textid, Float:intensity = 6.0, duration = 800, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `ShakePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`intensity`:** Maximum pixel displacement.
* **`duration`:** Impact duration in milliseconds.
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `WaveDriftPlayerTextDraw`
```pawn
stock WaveDriftPlayerTextDraw(playerid, PlayerText:textid, Float:amplitudeX, Float:amplitudeY, speedMs = 1500, duration = 0, extra_id = 0, const callback[] = "");
```
Generates a floating, organic wave hover motion using coupled harmonic dual-sine waves ($X = \sin(\theta), Y = \cos(1.5\theta)$), ideal for floating mission markers, underwater effects, or intoxicated vision.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to drift.
* **`amplitudeX`:** Maximum horizontal oscillation distance in pixels.
* **`amplitudeY`:** Maximum vertical oscillation distance in pixels.
* **`speedMs`:** Time in milliseconds to complete one full sinusoidal cycle (defaults to `1500ms`).
* **`duration`:** Total duration in milliseconds (`0` for continuous hover loop).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `WaveDriftTextDraw`
```pawn
stock WaveDriftTextDraw(Text:textid, Float:amplitudeX, Float:amplitudeY, speedMs = 1500, duration = 0, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `WaveDriftPlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`amplitudeX`, `amplitudeY`:** Oscillation amplitudes.
* **`speedMs`:** Period duration in milliseconds.
* **`duration`:** Total duration in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

---

### 4. Dynamic Text & Number Effects

#### `CountPlayerTextDrawNumber`
```pawn
stock CountPlayerTextDrawNumber(playerid, PlayerText:textid, startValue, targetValue, duration = 1000, const prefix[] = "", const suffix[] = "", bool:thousandsSep = false, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "");
```
Renders a rolling numeric counter on the textdraw, interpolating values from `startValue` to `targetValue` with formatting options.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle.
* **`startValue`:** Starting integer number.
* **`targetValue`:** Ending integer number.
* **`duration`:** Total roll duration in milliseconds (defaults to `1000ms`).
* **`prefix[]`:** Optional string placed immediately before the number (e.g. `"$"` or `"+"`, defaults to `""`).
* **`suffix[]`:** Optional string placed immediately after the number (e.g. `" HP"` or `" / 100"`, defaults to `""`).
* **`thousandsSep`:**
  * `true`: Inserts comma delimiters for thousands formatting (e.g. `$1,250,000`).
  * `false` *(default)*: Formats raw unformatted integers (e.g. `$1250000`).
* **`easing`:** Easing curve applied to the numeric interpolation (e.g. `TDFX_EASE_OUT_QUAD`).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `CountTextDrawNumber`
```pawn
stock CountTextDrawNumber(Text:textid, startValue, targetValue, duration = 1000, const prefix[] = "", const suffix[] = "", bool:thousandsSep = false, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `CountPlayerTextDrawNumber` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`startValue`, `targetValue`:** Starting and ending integers.
* **`duration`:** Rolling duration in milliseconds.
* **`prefix[]`, `suffix[]`:** Prefix and suffix strings.
* **`thousandsSep`:** Whether to insert thousand separators.
* **`easing`:** Easing formula.
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `ScramblePlayerTextDraw`
```pawn
stock ScramblePlayerTextDraw(playerid, PlayerText:textid, const finalText[], duration = 2000, extra_id = 0, const callback[] = "");
```
Produces a Cyberpunk / Matrix decryption animation: displays randomized glyphs that progressively resolve and lock into the final string character-by-character from left to right while safely preserving GTA color codes.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle.
* **`finalText[]`:** The final decoded string to reveal.
* **`duration`:** Total decryption duration in milliseconds (defaults to `2000ms`).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `ScrambleTextDraw`
```pawn
stock ScrambleTextDraw(Text:textid, const finalText[], duration = 2000, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `ScramblePlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`finalText[]`:** The final target string.
* **`duration`:** Decryption duration in milliseconds.
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `FillPlayerTextDrawProgress`
```pawn
stock FillPlayerTextDrawProgress(playerid, PlayerText:textid, Float:startWidth, Float:targetWidth, duration = 2000, easing = TDFX_EASE_OUT_QUAD, Float:lineHeight = 0.0, extra_id = 0, const callback[] = "");
```
Smoothly animates the width dimension of a textdraw background box over time using easing curves, creating native progress bars without requiring external plugins.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle with box enabled (`UseBox(true)`).
* **`startWidth`:** Initial box width in pixels.
* **`targetWidth`:** Final target box width in pixels.
* **`duration`:** Fill duration in milliseconds (defaults to `2000ms`).
* **`easing`:** Easing formula (defaults to `TDFX_EASE_OUT_QUAD`).
* **`lineHeight`:** Optional box height parameter. If set to `0.0`, uses the textdraw's existing cached height.
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `FillTextDrawProgress`
```pawn
stock FillTextDrawProgress(Text:textid, Float:startWidth, Float:targetWidth, duration = 2000, easing = TDFX_EASE_OUT_QUAD, Float:lineHeight = 0.0, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `FillPlayerTextDrawProgress` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`startWidth`, `targetWidth`:** Initial and target box width dimensions.
* **`duration`:** Fill duration in milliseconds.
* **`easing`:** Easing formula.
* **`lineHeight`:** Optional box height.
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

#### `GlitchPlayerTextDraw`
```pawn
stock GlitchPlayerTextDraw(playerid, PlayerText:textid, const text[] = "", intensity = 2, duration = 3000, extra_id = 0, const callback[] = "");
```
Applies real-time Cyberpunk chromatic disturbance to a visible textdraw: sporadically corrupts random characters into cybernetic symbols and fluctuates opacity, restoring pristine text upon completion.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to glitch.
* **`text[]`:** Optional explicit text string. If omitted, automatically reads the active text from open.mp or memory cache.
* **`intensity`:** Number of simultaneous characters glitched per burst ($1$ to $5$, defaults to `2`).
* **`duration`:** Glitch disturbance duration in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `GlitchTextDraw`
```pawn
stock GlitchTextDraw(Text:textid, const text[] = "", intensity = 2, duration = 3000, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `GlitchPlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`text[]`:** Optional text string.
* **`intensity`:** Glitch intensity ($1$ to $5$).
* **`duration`:** Duration in milliseconds (`0` for continuous).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

---

### 5. Fluorescent & Atmospheric FX

#### `NeonFlickerPlayerTextDraw`
```pawn
stock NeonFlickerPlayerTextDraw(playerid, PlayerText:textid, duration = 3000, brokenRatio = 40, extra_id = 0, const callback[] = "");
```
Simulates the stochastic flickering of a malfunctioning neon sign or fluorescent gas tube: rapid micro-sparks, temporary dropouts, and settling into a stable solid glow.
* **`playerid`:** The ID of the player.
* **`textid`:** The `PlayerText:` handle to flicker.
* **`duration`:** Total duration of the malfunction effect in milliseconds (defaults to `3000ms`).
* **`brokenRatio`:** Percentage of instability and spark dropouts ($0\%$ to $100\%$, defaults to `40%`).
* **`extra_id`:** Optional identifier passed to `OnPlayerTextDrawFXFinish`.
* **`callback[]`:** Optional targeted public function name.
* **Returns:** `1` on success, `0` on failure.

#### `NeonFlickerTextDraw`
```pawn
stock NeonFlickerTextDraw(Text:textid, duration = 3000, brokenRatio = 40, extra_id = 0, const callback[] = "");
```
Server-wide mirror of `NeonFlickerPlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`duration`:** Total duration in milliseconds.
* **`brokenRatio`:** Instability ratio ($0\%$ to $100\%$).
* **`extra_id`:** Optional identifier passed to `OnTextDrawFXFinish`.
* **`callback[]`:** Optional targeted callback name.
* **Returns:** `1` on success, `0` on failure.

---

### 6. Visual Blinking & Typewriter

#### `BlinkPlayerTextDraw`
```pawn
stock BlinkPlayerTextDraw(playerid, PlayerText:textid, interval = 500, duration = 0, bool:perChar = false, colour = 0, const text[] = "", baseColour = 0);
```
Activates a visual blinking, chromatic alert flash, or cinematic typewriter animation on a player textdraw. Calling this function on a textdraw that is already animating safely replaces the previous animation.
* **`playerid`:** The ID of the player who owns the PlayerTextDraw.
* **`textid`:** The PlayerTextDraw handle to animate.
* **`interval`:** Time in milliseconds between each animation toggle or character typing step (minimum `30ms`, defaults to `500ms`).
* **`duration`:** Total duration in milliseconds for the animation. Passing `0` keeps the effect running indefinitely until stopped manually.
* **`perChar`:**
  * `false` *(default)*: Blinks the entire textdraw on/off or shifts color.
  * `true`: Enables typewriter mode, revealing text character by character.
* **`colour`:**
  * `0` *(default)*: Visibility blinking (textdraw toggles between shown and hidden).
  * `0xRRGGBBAA`: Chromatic alert shift (textdraw stays visible and alternates between this color and `baseColour`). In `perChar` mode, text is rendered in this color while typing.
* **`text[]`:** Explicit text string to use for `perChar` mode. If omitted or empty (`""`), the include automatically retrieves the current text from open.mp native getters or memory cache.
* **`baseColour`:** Original base color to restore when unlit or when the effect ends. If omitted (`0`), the include retrieves the color automatically.
* **Returns:** `1` on success, `0` if player is disconnected, textdraw handle is invalid, or the animation pool is full.

#### `BlinkTextDraw`
```pawn
stock BlinkTextDraw(Text:textid, interval = 500, duration = 0, bool:perChar = false, colour = 0, const text[] = "", baseColour = 0);
```
Server-wide mirror of `BlinkPlayerTextDraw` for Global TextDraws (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw.
* **`interval`:** Interval in milliseconds (minimum `30ms`, defaults to `500ms`).
* **`duration`:** Total duration in milliseconds (`0` for continuous).
* **`perChar`:** Enables character-by-character typewriter mode.
* **`colour`:** Flash accent color (`0` for visibility show/hide toggle).
* **`text[]`:** Target text string for typewriter mode.
* **`baseColour`:** Base restoration color.
* **Returns:** `1` on success, `0` on failure.

---

### 7. Animation Control & Queries

#### `StopPlayerTextDrawFX`
```pawn
stock StopPlayerTextDrawFX(playerid, PlayerText:textid);
```
Immediately stops any active animation running on the specified player textdraw. Terminates active timers, restores original base visual properties (colors, dimensions, strings, coordinates), and leaves the textdraw in a clean, stable state.
* **`playerid`:** The ID of the player.
* **`textid`:** The PlayerTextDraw handle to stop.
* **Returns:** `1` on success, `0` if no effect was active on the textdraw.

#### `StopTextDrawFX`
```pawn
stock StopTextDrawFX(Text:textid);
```
Immediately stops any active animation running on the specified Global TextDraw (`Text:textid`).
* **`textid`:** The `Text:` handle of the global textdraw to stop.
* **Returns:** `1` on success, `0` if no effect was active.

#### `bool:IsPlayerTextDrawFXActive`
```pawn
stock bool:IsPlayerTextDrawFXActive(playerid, PlayerText:textid);
```
Checks whether a player textdraw is currently executing any active visual effect or animation.
* **`playerid`:** The ID of the player.
* **`textid`:** The PlayerTextDraw handle to query.
* **Returns:** `true` if an animation is currently running, `false` otherwise.

#### `bool:IsTextDrawFXActive`
```pawn
stock bool:IsTextDrawFXActive(Text:textid);
```
Checks whether a Global TextDraw is currently executing any active animation.
* **`textid`:** The `Text:` handle to query.
* **Returns:** `true` if an animation is running, `false` otherwise.

---

### 8. Lifecycle Callbacks

#### `OnPlayerTextDrawFXFinish`
```pawn
forward OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id);
```
Called when a PlayerTextDraw animation completes. Triggered only when the animation was started with `extra_id != 0`, guaranteeing zero overhead for unmonitored animations.
* **`playerid`:** The ID of the player who owns the textdraw.
* **`textid`:** The `PlayerText:` handle that finished animating.
* **`fx_type`:** The type of effect that concluded (`TDFX_TYPE_FADE`, `TDFX_TYPE_BOUNCE`, `TDFX_TYPE_PROGRESS`, etc.).
* **`extra_id`:** The user-defined identifier supplied when initiating the animation.

#### `OnTextDrawFXFinish`
```pawn
forward OnTextDrawFXFinish(Text:textid, fx_type, extra_id);
```
Called when a Global TextDraw animation completes with `extra_id != 0`.
* **`textid`:** The `Text:` handle that finished animating.
* **`fx_type`:** The type of effect that concluded.
* **`extra_id`:** The user-defined identifier.

#### Targeted Callback (`callback[]`)
Any animation function can receive the name of an optional public function to invoke directly upon completion:
* **For PlayerTextDraws:** `forward CallbackName(playerid, PlayerText:textid, extra_id);`
* **For Global TextDraws:** `forward CallbackName(Text:textid, extra_id);`

---

## Interactive Demonstration Suite (Filterscript)

A turnkey interactive demonstration filterscript ([`FS_TextDrawFX_Demo.pwn`](FS_TextDrawFX_Demo.pwn)) is included to test and showcase every single feature and effect in-game:

### Running the Demo
1. Compile or copy `FS_TextDrawFX_Demo.amx` into your server's `filterscripts/` directory.
2. Add `FS_TextDrawFX_Demo` to your `server.cfg` (SA-MP) or `config.json` (open.mp) filterscripts list.
3. In-game, use **/tdfx** (or alias **/txdfx**) to open the single-click interactive control panel:
   * **One-Click Toggle:** Click any preview item to start it; clicking an active preview stops and dismisses it.
   * **Single-Active Mutex:** Activating a new preview immediately cancels and cleans up any previously running animation.
   * **Live Status Feedback:** Active animations display a green `{2ECC71}(Active)` tag dynamically in the menu list.
   * **Auto-Cleanup:** Every preview finishes its sequence, fades out, and hides itself automatically upon completion.
   * **Reset Option:** Dedicated `[Stop Active Effect]` button to reset all preview elements instantly.

| Command / Alias | Action |
| :--- | :--- |
| `/tdfx` or `/txdfx` | Opens the interactive demonstration menu with live `(Active)` status tracking. |

---

## Version History

<details>
<summary>Show the version history</summary>

### 2.1.0 - 02/11/2026
* Feature-packed expansion introducing 2D runtime physics, orbital geometry, and atmospheric FX:
  * Added **Retro DVD / CRT Box Bounce** (`BouncePlayerTextDraw`, `BounceTextDraw`): 2D elastic velocity reflection inside rectangular bounding boxes with optional classic DVD color shifting and corner hit detection.
  * Added **Circular Perimeter Bounce** (`BounceCirclePlayerTextDraw`, `BounceCircleTextDraw`): 2D radial elastic boundary reflection using dynamic surface normal vectors.
  * Added **Orbital Motion Path** (`OrbitPlayerTextDraw`, `OrbitTextDraw`): Continuous trigonometric circular or elliptical paths around a configurable pivot center with angular velocity.
  * Added **Trauma Screen Shake** (`ShakePlayerTextDraw`, `ShakeTextDraw`): Realistic combat/explosion feedback with quadratic trauma decay and automatic anchor restoration.
  * Added **Broken Neon Flicker** (`NeonFlickerPlayerTextDraw`, `NeonFlickerTextDraw`): Stochastic atmospheric flickering simulating malfunctioning fluorescent gas discharge lamps with micro-sparks, blackouts, and steady settling.
  * Added **Waveform Floating Drift** (`WaveDriftPlayerTextDraw`, `WaveDriftTextDraw`): Coupled dual-sine harmonic oscillation for gentle UI floating, underwater, or intoxicated vision effects.
  * Added **Dynamic Native Progress Fill** (`FillPlayerTextDrawProgress`, `FillTextDrawProgress`): Smooth interpolation of box width using Penner easing curves without requiring external progress bar plugins.
  * Added **Cyberpunk Chromatic Glitch** (`GlitchPlayerTextDraw`, `GlitchTextDraw`): Real-time stochastic glyph corruption and micro-alpha fluctuations while preserving original text integrity.
  * Full mirrored global support across all new functions for server-wide TextDraws (`Text:textid`).
  * Expanded demonstration suite filterscript (`FS_TextDrawFX_Demo.pwn`) with interactive testing for all 17 distinct visual animation modes.

### 2.0.0 - 02/11/2026
* Major release with full animation engine overhaul:
  * Added True RGBA & Alpha Channel Fading (`FadeIn`, `FadeOut`, `FadeColor`) with smooth linear and quadratic transitions.
  * Added Dimension Scaling (`Scale`) with support for Penner Easing curves (`Linear`, `Ease-In Quad`, `Ease-Out Quad`, `Ease-In-Out Quad`, `Ease-Out Bounce`).
  * Added Heartbeat Sine Wave Pulsing (`Pulse`) with automatic base size restoration.
  * Added Coordinate Sliding Motion (`Slide`) with Penner Easing curves across screen coordinates.
  * Added Animated Numeric Counter FX (`Count...Number`) supporting prefix, suffix, and thousands separators.
  * Added Cyberpunk Matrix Decrypt FX (`Scramble`) with randomized character decoding and GTA format tag preservation.
  * Full mirrored support for Global TextDraws (`Text:textid`) across all animation functions.
  * Added Lifecycle & Event Callback System: `OnPlayerTextDrawFXFinish`, `OnTextDrawFXFinish`, and targeted function callbacks (`callback[]`).
  * Built-in completion auto-actions: `.autoHide`, `.autoDestroy`, and `.loop`.
  * Added interactive demonstration test suite filterscript (`FS_TextDrawFX_Demo.pwn`).

### 1.0.0 - 01/11/2026
* Initial public release of **TextDrawFX**:
  * 100% original, authorial implementation.
  * Three distinct animation modes: Visibility Blink (`colour = 0`), Chromatic Color Flash (`colour != 0`), and Typewriter Animation (`perChar = true`).
  * Full support for text fonts (`0` - `3`), sprite textures (Font `4`), 3D preview models (Font `5`), and boxed textdraws.
  * Smart GTA formatting tag and whitespace tokenizer (`TDFX_SkipTag` & `TDFX_Cut`) supporting `~r~`, `~k~~KEY~`, `~n~`, spaces, and underscores.
  * Dynamic auto-pacing duration compensation for typewriter animations.
  * Universal cross-compatibility between SA-MP and open.mp without `#define SAMP_COMPAT` or `#define MIXED_SPELLINGS`.
  * Multi-tag hooks for `PlayerTextDrawFont` and `PlayerTextDrawUseBox` eliminating `warning 213`.
  * Complete ALS hook suite for `PlayerTextDrawColor`/`Colour`, `PlayerTextDrawBoxColor`/`BoxColour`, `PlayerTextDrawBackgroundColor`/`BackgroundColour`, `PlayerTextDrawFont`, `PlayerTextDrawUseBox`, `PlayerTextDrawHide`, `PlayerTextDrawDestroy`, `PlayerTextDrawSetString`, and `OnPlayerDisconnect`.
  * Clean, comment-free include file structure with modular function grouping.
  * Automatic header detection (`#tryinclude <open.mp>` / `#include <a_samp>`) and include guards.
  * Released under the **Mozilla Public License 2.0 (MPL-2.0)**.

</details>

---

## Author & Credits

* **Walter Corrêa** ([@Walter-Correa](https://github.com/Walter-Correa)) - Creator & Maintainer of **TextDrawFX**.

---

## License

This project is licensed under the **Mozilla Public License 2.0 (MPL-2.0)**. See the [LICENSE](LICENSE) file for complete legal terms.
