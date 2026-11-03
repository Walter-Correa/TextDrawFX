# TextDrawFX

[![GitHub release](https://img.shields.io/github/v/release/Walter-Correa/TextDrawFX?include_prereleases&color=blue)](https://github.com/Walter-Correa/TextDrawFX/releases)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)
[![Platform](https://img.shields.io/badge/platform-SA--MP%20%7C%20open.mp-orange.svg)](https://open.mp)

**TextDrawFX** is a comprehensive, high-performance, lightweight, and plug-and-play visual effects, animation, and cinematics library engineered specifically for **San Andreas Multiplayer (SA-MP)** and **open.mp**.

It empowers Pawn developers to bring static GTA interfaces to life with **36+ distinct visual effects**: **2D Physics Bounding Wall Reflection (DVD screensaver)**, **Circular Elastic Perimeter Bounce**, **Continuous Orbital Paths**, **Combat Trauma Screen Shake**, **3D Shockwave Impact Slam**, **Sinusoidal Traveling Wave Motion**, **Letter Hopping Cascade**, **Falling Letter Rain**, **Stochastic Broken Neon Flickering**, **Waveform Floating Drift**, **Dynamic Native Progress Bar Interpolation**, **Cyberpunk Chromatic Glitch Storms**, **True RGBA & Alpha Fading**, **Dynamic Scaling & Heartbeat Pulsing**, **Penner Easing Motion (Sliding)**, **Animated Numeric Counters**, **Cyberpunk Matrix Glyph Decryption**, **Cinematic Typewriter Reveals**, **21:9 Widescreen Letterbox**, **Submarine Sonar Radar Echo**, **Split-Flap Airport Boards**, **Scanner Laser Sweeps**, **Firework Fragmentation Text**, and **3D Model Preview Rotations**—all operating with **zero external dependencies** and mirrored support for both **PlayerTextDraws** (`PlayerText:textid`) and **Global TextDraws** (`Text:textid`).

---

## Technical Highlights & Guarantees

| Capability | TextDrawFX Specification |
| :--- | :--- |
| **Memory Architecture** | Static memory pool architecture with minimal stack consumption (~412 cells). All particle matrices, character buffers, and coordinate tables reside in `.data` memory. |
| **Dual API Parity** | **100% Mirrored Parity** across all 36+ effects for both **PlayerTextDraws** (`PlayerText:textid`) and **Global TextDraws** (`Text:textid`). |
| **Aspect Ratio & Resolution** | Universal compensation for SA-MP's virtual $640 \times 480$ canvas across standard 4:3, 16:9 widescreen, and 21:9 ultrawide monitor displays. |
| **Letterbox Symmetry** | Calibrated Font 1 vertical ascent offset ($35.8$ units) guaranteeing mathematically identical top and bottom cinema bars on any client screen resolution. |
| **Font Kerning Engine** | Built-in proportional font metrics (`_TDFX_GetCharWidth`) ensuring multi-letter animations assemble with zero visual jumping or letter compression. |
| **Cross-Platform Engine** | Automatic environment detection for open.mp (`_INC_open_mp`) and standard SA-MP (`0.3.7-R1` through `0.3.7-R5`). |
| **Tag-Safe Tokenizer** | Full embedded GTA color tag compatibility (`~r~`, `~y~`, `~w~`, etc.) across all 38 effects, preserving active formatting without tag splitting or corruption. |
| **Global Player Isolation** | Intelligent per-player visibility tracking for Global TextDraws; animating a global textdraw shown to select players never bleeds onto others, and `TextDrawHideForPlayer` is fully respected across ticks. |
| **Capacity Safeguards** | Built-in free slot validation (`TDFX_GetFreePlayerTDCount`) preventing crashes or partial glyph leaks when textdraw quotas are constrained. |
| **Clean Script Unload** | Automatic lifecycle recovery via ALS v7 `OnFilterScriptExit`/`OnGameModeExit`, stopping timers and restoring coordinates across server reload. |
| **Universal Looping** | Universal `bool:loop = false` parameter across all 38 Player and Global effect functions for continuous repetition. |
| **DVD Bounce Physics** | Authentic screensaver physics with randomized trajectories, wall deflection jitter (`0.94` - `1.07`), 4-corner strike detection (`cornerTol`), chime audio (`1057`), and DVD color palette cycling. |
| **Cinematic Letterbox** | Screen-anchored 21:9 black bars ($Y=0.0$ and $Y=480.0$) operating independently of text coordinates, allowing dialogue text to animate wherever created. |
| **External Dependencies** | **Zero external dependencies**. Does not require YSI, Streamer, or custom C++ plugins. |

---

## Documentation Hub

Comprehensive, modular guides and technical references are available in the [`docs/`](docs/) directory:

| Guide | Description |
| :--- | :--- |
| **[📖 Documentation Hub](docs/README.md)** | Technical navigation matrix, quick reference, and architectural overview. |
| **[🚀 Getting Started](docs/getting-started.md)** | Step-by-step setup, compiler configurations, ALS hook system, and your first animation. |
| **[✨ Visual Effects Catalog](docs/effects-catalog.md)** | Detailed mathematical breakdown, easing curves, and code snippets for all 36+ effects. |
| **[📚 Complete API Reference](docs/api-reference.md)** | Exhaustive dictionary covering all PlayerTextDraw & TextDraw functions, parameters, and callbacks. |
| **[🛡️ Compatibility, Resolution & Safety](docs/compatibility-and-safety.md)** | Font kerning, 21:9 letterbox math, 3D model safety locks, and GTA color-tag parsing. |

---

## Quick Start & Installation

### 1. File Placement

Copy `TextDrawFX.inc` into your server development environment's include directory:
* **SA-MP / Pawno:** `pawno/include/TextDrawFX.inc`
* **open.mp / Qawno:** `qawno/include/TextDrawFX.inc`

### 2. Include in Your Script

Include `TextDrawFX.inc` after `<a_samp>` or `<open.mp>`:

```pawn
#include <open.mp> // or <a_samp>
#include <TextDrawFX>
```

### 3. Basic Example

Create an animated notification banner that slides onto the screen, pulses, and fades out:

```pawn
new PlayerText:gAlertTD[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

#define EVENT_ALERT_SLIDE 1
#define EVENT_ALERT_FADE  2

public OnPlayerConnect(playerid)
{
    gAlertTD[playerid] = CreatePlayerTextDraw(playerid, -200.0, 360.0, "ZONE SECURED: LOS SANTOS");
    PlayerTextDrawAlignment(playerid, gAlertTD[playerid], 2);
    PlayerTextDrawFont(playerid, gAlertTD[playerid], 1);
    PlayerTextDrawLetterSize(playerid, gAlertTD[playerid], 0.45, 1.80);
    PlayerTextDrawColor(playerid, gAlertTD[playerid], 0x2ECC71FF);
    PlayerTextDrawUseBox(playerid, gAlertTD[playerid], true);
    PlayerTextDrawBoxColor(playerid, gAlertTD[playerid], 0x000000BB);
    PlayerTextDrawTextSize(playerid, gAlertTD[playerid], 500.0, 320.0);
    PlayerTextDrawSetOutline(playerid, gAlertTD[playerid], 1);
    return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    // Slide into screen with Ease-Out Bounce
    PlayerTextDrawSetPos(playerid, gAlertTD[playerid], -200.0, 360.0);
    PlayerTextDrawShow(playerid, gAlertTD[playerid]);

    SlidePlayerTextDraw(
        playerid, 
        gAlertTD[playerid], 
        .targetX = 320.0, 
        .targetY = 360.0, 
        .duration = 1000, 
        .easing = TDFX_EASE_OUT_BOUNCE, 
        .extra_id = EVENT_ALERT_SLIDE
    );
    return 1;
}

public OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id)
{
    if (textid == gAlertTD[playerid] && extra_id == EVENT_ALERT_SLIDE)
    {
        // Slide finished! Fade out with automatic hiding
        FadeOutPlayerTextDraw(playerid, gAlertTD[playerid], .duration = 1500, .fadeBox = true, .autoHide = true);
        return 1;
    }
    return 1;
}
```

---

## Interactive In-Game Demonstration Suite

TextDrawFX comes bundled with a comprehensive interactive demonstration filterscript: **`FS_TextDrawFX_Demo.pwn`**.

It provides an in-game GUI menu accessible via `/tdfx` or `/txdfx` allowing live testing of all 36 visual modes in real-time:

```
[Effect]                         [Description]                          [Status]
True Alpha Fade                  0% -> 100% Opacity                     -
Color Cross-Fade                 Green -> Red Alert Shift               -
Dimension Scaling                Letter Grow with Bounce Easing         -
Heartbeat Pulse                  Sine Wave Pulsing (~70 BPM)            -
Coordinate Slide                 Glide In with Ease-Out Quad            -
Animated Cash Counter            Rolling $0 -> $50,000                  -
Cyberpunk Decrypt                Matrix Glyph Decryption                -
Visibility Blink                 Classical Show / Hide (500ms)          -
Cinematic Typewriter             GTA Tag-Safe Dialogue Pacing           -
Retro DVD Box Bounce             2D Bounding Box Reflection             -
Circular Bounce                  Elastic Circle Perimeter Bounce        -
Orbital Motion                   Continuous Elliptical Path             -
Trauma Screen Shake              Micro-displacement Decay               -
Broken Neon Flicker              Stochastic Gas Tube Malfunction        -
Waveform Drift                   Harmonic Dual-Sine Floating            -
Dynamic Progress Fill            Smooth Box Bar Fill with Easing        -
Cyberpunk Glitch                 Real-time Glyph Disturbance            -
Firework Text                    Exploding Fragmentation FX             -
3D Turntable Spin                Continuous 360 Rotation                -
3D Floating Tilt                 Harmonic 3D Hover & Pitch              -
Golden Shimmer                   Sweeping Chrome Highlight              -
RGB Rainbow Wave                 Dynamic 8-Stage Flowing Wave           -
Casino Slot Machine              Mechanical Rolling Reel Lock           -
Pendulum Swing                   Damped Gravity Arm Swing               -
Gravity Drop Slam                Freefall Acceleration & Impact         -
Black Hole Vortex                Spiral Suction Decay                   -
Cinematic Letterbox              21:9 Cinema Black Bars                 -
Sonar Echo Ping                  Submarine Radar Acoustic Pulse         -
Traveling Wave Sweep             Flowing Horizontal Wave Crest          -
Falling Letter Rain              Gravity Drop Letter Assembly           -
Scanner Laser Sweep              KITT / Cylon Laser Sweep               -
Electric Arc Surge               Short-Circuit Sparks & Jitter          -
Split-Flap Airport Board         Mechanical Rolling Solari Board        -
Sci-Fi Hologram                  Futuristic Cyan Beam & Scanlines       -
Shockwave Impact Slam            3D Plummet, Trauma & Blast Ring        -
Sinusoidal Wave Motion           Physical Traveling Arch Wave           -
Letter Hopping Cascade           Staggered Letter Jump & Bounce         -
Global Broadcast                 Server-Wide Announcement TD            -
Chained Cutscene                 Automated 4-Stage Sequence             -
[Stop Active Effect]             Immediately reset screen               -
```

---

---

## Version History

<details>
<summary>Show the version history</summary>

### 2.2.0 - 03/11/2026
* **Massive Visual Effects Expansion (36+ Total FX):**
  * Added **Exploding Firework Text** (`FireworkPlayerTextDraw`, `FireworkTextDraw`): Multi-particle outward velocity fragmentation, gravity arc physics, and alpha fadeout across 5 burst patterns (Sphere, Fan, Rocket, Ring, Fountain).
  * Added **3D Model Turntable & Floating Tilt** (`Spin3DPlayerTextDraw`, `Spin3DTextDraw`, `Hover3DPlayerTextDraw`, `Hover3DTextDraw`): Continuous 360-degree rotation and dual-axis sinusoidal pitching for Font 5 vehicle/skin/object previews.
  * Added **Golden Shimmer & Chrome Sweep** (`ShimmerPlayerTextDraw`, `ShimmerTextDraw`): Flowing horizontal specular highlight ray across text.
  * Added **RGB Rainbow Wave** (`RainbowPlayerTextDraw`, `RainbowTextDraw`): Continuous multi-stage chromatic phase shifting.
  * Added **Casino Slot Machine Rolling Reel** (`SlotMachinePlayerTextDraw`, `SlotMachineTextDraw`): Mechanical tumbling character reels with staggered column locks and click feedback.
  * Added **Damped Pendulum Swing** (`PendulumPlayerTextDraw`, `PendulumTextDraw`): Realistic gravitational harmonic arm swinging with angular damping decay.
  * Added **Gravity Drop Slam** (`GravityDropPlayerTextDraw`, `GravityDropTextDraw`): Accelerated vertical freefall plummet with ground impact rebound and alpha settle.
  * Added **Black Hole Spiral Vortex** (`VortexPlayerTextDraw`, `VortexTextDraw`): Logarithmic angular spin and centripetal suction decay.
  * Added **21:9 Widescreen Cinematic Letterbox** (`LetterboxPlayerTextDraw`, `LetterboxTextDraw`): Smooth slide-in of widescreen cinema bars with calibrated font ascent offset (35.8 units) guaranteeing identical physical pixel heights across any display resolution.
  * Added **Submarine Sonar Radar Echo** (`SonarPingPlayerTextDraw`, `SonarPingTextDraw`): Expanding concentric acoustic pulse ping with distance attenuation and radar ping feedback.
  * Added **Traveling Wave Motion** (`WaveMotionPlayerTextDraw`, `WaveMotionTextDraw`): Physical sinusoidal arch wave traveling across characters in selectable direction with proportional font kerning.
  * Added **Letter Hopping Cascade** (`HopLetterPlayerTextDraw`, `HopLetterTextDraw`): Sequential parabolic jumping and landing bounce across glyphs.
  * Added **Falling Letter Rain Assembly** (`LetterRainPlayerTextDraw`, `LetterRainTextDraw`): Matrix-style staggered freefall letter drop assembling perfectly into original text position and geometry.
  * Added **KITT / Cylon Scanner Laser Sweep** (`ScannerSweepPlayerTextDraw`, `ScannerSweepTextDraw`): Reciprocating luminous beam scanning across glyphs.
  * Added **Electric Arc Surge** (`ElectricArcPlayerTextDraw`, `ElectricArcTextDraw`): High-voltage jitter, stochastic blackouts, and electrical hum feedback.
  * Added **Split-Flap Airport Board** (`SplitFlapPlayerTextDraw`, `SplitFlapTextDraw`): Mechanical rolling alphanumeric Solari split-flap departure board simulation.
  * Added **Sci-Fi Hologram Emitter** (`HologramPlayerTextDraw`, `HologramTextDraw`): Cyan laser beam projection, horizontal scanline oscillation, and emitter flickering.
  * Added **3D Shockwave Impact Slam** (`ShockwaveSlamPlayerTextDraw`, `ShockwaveSlamTextDraw`): High-altitude freefall slam, ground trauma shake, 3D world explosion sound (1159) played directly at player coordinates, and expanding circular shockwave blast ring.
* **Pixel-Perfect Font Kerning & Outline Dilation Engine:**
  * Embedded exact 95-character proportional kerning tables from GTA San Andreas' native `fonts.dat` asset across all 4 text fonts (Font 0, 1, 2, 3).
  * Dynamic outline dilation tracking via ALS hooks (`PlayerTextDrawSetOutline` and `TextDrawSetOutline`), factoring font outline thickness directly into character advance spacing `(Width + outline) * letterSizeX` for 100% pixel-perfect alignment with original TextDraws.
  * Ghost outline silhouette elimination during multi-character boxed animations (`UseBox`) by temporarily rendering `" "` on the parent TextDraw, preserving clean background boxes without static dark text showing behind moving letters.
  * Universal compensation for SA-MP virtual 640x480 canvas across standard 4:3, 16:9 widescreen, and 21:9 ultrawide monitor displays.
* **Semantic Versioning Architecture:**
  * Added standardized version defines (`TEXTDRAWFX_VERSION_MAJOR`, `TEXTDRAWFX_VERSION_MINOR`, `TEXTDRAWFX_VERSION_PATCH`, `TEXTDRAWFX_VERSION_STRING`) following the GroundAndreas standard for compile-time and runtime checks.
  * Updated CI/CD release workflow (`.github/workflows/release.yml`) to automatically parse `TEXTDRAWFX_VERSION_STRING` and extract matching release notes.
* **Comprehensive Documentation Suite (`docs/`):**
  * Created complete modular documentation hub with 5 dedicated guides: Getting Started, Visual Effects Catalog, Complete API Reference, and Compatibility, Resolution & Safety Guide.
* **Interactive In-Game Demonstration Suite (`FS_TextDrawFX_Demo.pwn`):**
  * Real-time in-game testing suite supporting live interactive previews of all 36 visual animation modes with automatic cleanup and status tracking.

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

* [@Walter-Correa](https://github.com/Walter-Correa) — Concept and development of **TextDrawFX**, with the assistance of **Artificial Intelligence**.

---

## License

This project is licensed under the **Mozilla Public License 2.0 (MPL-2.0)**. See the [LICENSE](LICENSE) file for complete terms.
