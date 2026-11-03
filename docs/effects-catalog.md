# Visual Effects Catalog

TextDrawFX features **36+ distinct visual animation and cinematic modes**. This document provides mathematical breakdowns, visual explanations, parameter references, and copy-paste examples for every effect in the library.

---

> [!NOTE]
> **Universal Looping & Color Tag Compatibility:**
> * All 38 effect functions support the universal `bool:loop = false` parameter. Once activated with `loop = true`, effects cycle continuously until manually deactivated with `StopPlayerTextDrawFX` or `StopTextDrawFX`.
> * All visual and multi-character text animations fully support embedded SA-MP color tags (`~r~`, `~y~`, `~w~`, `~b~`, `~g~`, `~p~`, etc.) without stripping, corrupting, or losing tags during or after animation completion.


## Catalog Index by Category

* [Category 1: Opacity, Color & Chromatic FX](#category-1-opacity-color--chromatic-fx)
  * [1. True Alpha Fade In / Fade Out](#1-true-alpha-fade-in--fade-out)
  * [2. RGBA Color Cross-Fade](#2-rgba-color-cross-fade)
  * [3. Broken Neon Tube Flicker](#3-broken-neon-tube-flicker)
  * [4. Golden Chrome Shimmer](#4-golden-chrome-shimmer)
  * [5. 8-Stage RGB Rainbow Wave](#5-8-stage-rgb-rainbow-wave)
  * [6. Classic Visibility Blink](#6-classic-visibility-blink)
* [Category 2: Physical Motion & Kinematics](#category-2-physical-motion--kinematics)
  * [7. Coordinate Slide with Penner Easing](#7-coordinate-slide-with-penner-easing)
  * [8. 2D Bounding Box Reflection (DVD Screensaver)](#8-2d-bounding-box-reflection-dvd-screensaver)
  * [9. Elastic Circular Arena Bounce](#9-elastic-circular-arena-bounce)
  * [10. Continuous Orbital Elliptical Motion](#10-continuous-orbital-elliptical-motion)
  * [11. Harmonic Dual-Sine Waveform Drift](#11-harmonic-dual-sine-waveform-drift)
  * [12. Damped Gravity Pendulum Swing](#12-damped-gravity-pendulum-swing)
  * [13. Black Hole Spiral Vortex Suction](#13-black-hole-spiral-vortex-suction)
  * [14. Sinusoidal Traveling Wave Motion](#14-sinusoidal-traveling-wave-motion)
  * [15. Letter Hopping Cascade](#15-letter-hopping-cascade)
* [Category 3: Text Reveal, Decryption & Glyphs](#category-3-text-reveal-decryption--glyphs)
  * [16. Cinematic Typewriter](#16-cinematic-typewriter)
  * [17. Cyberpunk Matrix Glyph Decrypt](#17-cyberpunk-matrix-glyph-decrypt)
  * [18. Real-Time Glyph Glitch Distortion](#18-real-time-glyph-glitch-distortion)
  * [19. Casino Mechanical Slot Machine](#19-casino-mechanical-slot-machine)
  * [20. Split-Flap Mechanical Airport Board](#20-split-flap-mechanical-airport-board)
  * [21. Scanner Laser Sweep (KITT / Cylon)](#21-scanner-laser-sweep-kitt--cylon)
  * [22. Falling Letter Rain Assembly](#22-falling-letter-rain-assembly)
  * [23. Traveling Wave Tag Sweep](#23-traveling-wave-tag-sweep)
* [Category 4: Combat, Cinematics & HUD FX](#category-4-combat-cinematics--hud-fx)
  * [24. 3D Shockwave Impact Slam](#24-3d-shockwave-impact-slam)
  * [25. Combat Trauma Screen Shake](#25-combat-trauma-screen-shake)
  * [26. Firework Fragmentation Text Explosion](#26-firework-fragmentation-text-explosion)
  * [27. 21:9 Widescreen Cinematic Letterbox](#27-219-widescreen-cinematic-letterbox)
  * [28. Submarine Sonar Acoustic Echo Ping](#28-submarine-sonar-acoustic-echo-ping)
  * [29. Sci-Fi Hologram Projection Beam](#29-sci-fi-hologram-projection-beam)
  * [30. Short-Circuit Electric Arc Surge](#30-short-circuit-electric-arc-surge)
  * [31. Heartbeat Sine Wave Pulse](#31-heartbeat-sine-wave-pulse)
  * [32. Elastic Dimension Scaling](#32-elastic-dimension-scaling)
  * [33. Animated Numeric Counter](#33-animated-numeric-counter)
  * [34. Dynamic Native Box Progress Fill](#34-dynamic-native-box-progress-fill)
  * [35. Freefall Gravity Drop Slam](#35-freefall-gravity-drop-slam)
* [Category 5: 3D Preview Model Animations (Font 5)](#category-5-3d-preview-model-animations-font-5)
  * [36. 3D Turntable Spin](#36-3d-turntable-spin)
  * [37. Harmonic 3D Hover & Pitch Tilt](#37-harmonic-3d-hover--pitch-tilt)

---

## Category 1: Opacity, Color & Chromatic FX

### 1. True Alpha Fade In / Fade Out
Smooth linear or quadratic interpolation across the alpha channel ($0$ to $255$), with synchronized background box transparency fading.
* **Functions:** `FadeInPlayerTextDraw`, `FadeOutPlayerTextDraw`, `FadeInTextDraw`, `FadeOutTextDraw`
* **Features:** Optional `.fadeBox = true` interpolates both the text glyphs and background box color simultaneously.
```pawn
// Fade in from 0% to 100% opacity over 1.4 seconds with box fading
FadeInPlayerTextDraw(playerid, textid, .duration = 1400, .fadeBox = true);
```

### 2. RGBA Color Cross-Fade
Simultaneous 4-channel linear interpolation across Red, Green, Blue, and Alpha channels ($C(t) = C_{\text{start}} + t \cdot (C_{\text{end}} - C_{\text{start}})$). Supports Penner easing curves (`TDFX_EASE_IN_OUT_QUAD`, etc.).
* **Functions:** `FadeColorPlayerTextDraw`, `FadeColorTextDraw`
```pawn
// Smoothly morph color from Green to Red over 1.8 seconds with In-Out Quad easing
FadeColorPlayerTextDraw(playerid, textid, 0x2ECC71FF, 0xE74C3CFF, .duration = 1800, .easing = TDFX_EASE_IN_OUT_QUAD);
```

### 3. Broken Neon Tube Flicker
Stochastic simulation of a malfunctioning gas discharge lamp. Generates micro-strobe sparks, intermittent blackouts, and low-voltage settling.
* **Functions:** `NeonFlickerPlayerTextDraw`, `NeonFlickerTextDraw`
* **Parameters:** `duration` (settling duration in ms), `brokenRatio` (malfunction intensity 0-100%).
```pawn
NeonFlickerPlayerTextDraw(playerid, textid, .duration = 2400, .brokenRatio = 75);
```

### 4. Golden Chrome Shimmer
Sweeps a luminous specular highlight beam across the text characters, creating an authentic luxury reflection.
* **Functions:** `ShimmerPlayerTextDraw`, `ShimmerTextDraw`
* **Parameters:** `baseTag` (default `~y~`), `shimmerTag` (default `~w~`), `sweepDuration` (sweep transit time), `pauseDuration` (pause between sweeps).
```pawn
ShimmerPlayerTextDraw(playerid, textid, "VIP MEMBER STATUS", .sweepDuration = 600, .pauseDuration = 1800);
```

### 5. 8-Stage RGB Rainbow Wave
Flows an 8-stage dynamic chromatic hue spectrum across the string characters, creating vibrant cycling waves.
* **Functions:** `RainbowPlayerTextDraw`, `RainbowTextDraw`
```pawn
RainbowPlayerTextDraw(playerid, textid, "LEGENDARY LEVEL REACHED", .speedMs = 60, .duration = 4000);
```

### 6. Classic Visibility Blink
Periodically toggles visibility between visible and invisible, or between two alternating chromatic color states.
* **Functions:** `BlinkPlayerTextDraw`, `BlinkTextDraw`
```pawn
BlinkPlayerTextDraw(playerid, textid, .interval = 400, .duration = 3200);
```

---

## Category 2: Physical Motion & Kinematics

### 7. Coordinate Slide with Penner Easing
Moves a textdraw smoothly between screen coordinates $(X_1, Y_1) \to (X_2, Y_2)$ using Robert Penner's mathematical easing curves (`Linear`, `Ease-In Quad`, `Ease-Out Quad`, `Ease-In-Out Quad`, `Ease-Out Bounce`).
* **Functions:** `SlidePlayerTextDraw`, `SlideTextDraw`
```pawn
SlidePlayerTextDraw(playerid, textid, .targetX = 320.0, .targetY = 360.0, .duration = 1000, .easing = TDFX_EASE_OUT_BOUNCE);
```

### 8. 2D Bounding Box Reflection (DVD Screensaver)
Authentic 2D kinematic physics simulation of an object gliding inside screen boundaries with randomized velocity vectors, reflection upon wall impacts, dynamic trajectory variation, and corner hit detection:
* **Ergodic Coverage:** Transverse velocity perturbations on every wall collision ($0.94$ - $1.07$) prevent closed cyclic loops, guaranteeing the TextDraw traverses every region of the screen over time.
* **Automatic Dimension Compensation:** Exact visual bounds are measured automatically based on font character tables, outline dilation, box sizing (`UseBox`), and alignment (Left, Center, Right). Effective reflection boundaries are clamped (`effMinX = minX + leftOff`, `effMaxX = maxX - rightOff`, `effMinY = minY + topOff`, `effMaxY = maxY - bottomOff`), ensuring that the visual text never clips beyond screen or arena limits on any edge.
* **Corner Hit Detection:** Proximity calculations (`cornerTol = (|vx| + |vy|) * 1.5`) detect when the TextDraw reaches one of the 4 exact screen corners. Upon a corner strike, it reflects both axes simultaneously, triggers celebratory audio chime `1057`, and shifts to the next vibrant DVD color palette entry.
* **Functions:** `BouncePlayerTextDraw`, `BounceTextDraw`
* **Parameters:** `speedX`, `speedY` (initial directional velocity vectors; randomized if zero), `minX`, `minY`, `maxX`, `maxY` (bounding perimeter), `colorShift` (DVD palette cycling), `loop` (continuous repetition), `textWidth` (optional manual width override), `textHeight` (optional manual height override).
```pawn
BouncePlayerTextDraw(playerid, textid, .speedX = 3.2, .speedY = 2.4, .minX = 40.0, .minY = 60.0, .maxX = 600.0, .maxY = 420.0, .colorShift = true, .loop = true);
```

### 9. Elastic Circular Arena Bounce
Confines a textdraw inside a circular perimeter. When the textdraw's distance from center exceeds the boundary radius ($R$), the velocity reflects along the normal collision vector ($\vec{V}' = \vec{V} - 2(\vec{V} \cdot \vec{n})\vec{n}$). Measured text bounding dimensions automatically adjust the effective reflection radius ($R_{\text{eff}} = R - \sqrt{\text{halfW}^2 + \text{halfH}^2}$) so no part of the TextDraw escapes outside the circle.
* **Functions:** `BounceCirclePlayerTextDraw`, `BounceCircleTextDraw`
* **Parameters:** `speed`, `angle`, `centerX`, `centerY`, `radius`, `colorShift`, `duration`, `loop`, `extra_id`, `callback`, `textWidth`, `textHeight`.
```pawn
BounceCirclePlayerTextDraw(playerid, textid, .centerX = 320.0, .centerY = 240.0, .radius = 120.0);
```

### 10. Continuous Orbital Elliptical Motion
Trigonometric orbital movement along a circular or elliptical path with angular velocity ($\omega$):
$$X(t) = X_{\text{center}} + R_x \cdot \cos(\omega t), \quad Y(t) = Y_{\text{center}} + R_y \cdot \sin(\omega t)$$
* **Functions:** `OrbitPlayerTextDraw`, `OrbitTextDraw`
```pawn
OrbitPlayerTextDraw(playerid, textid, .centerX = 320.0, .centerY = 240.0, .radiusX = 140.0, .radiusY = 70.0, .angularSpeed = 2.5);
```

### 11. Harmonic Dual-Sine Waveform Drift
Superimposes two sine waves with continuous phase synchronization to produce organic, buttery-smooth floating drift:
$$Y(t) = Y_0 + A_1 \cdot \sin(\omega_1 t) + A_2 \cdot \cos(\omega_1 t)$$
* **Seamless Cyclic Phase:** Frequency-synchronized $360^\circ$ continuous harmonic phase alignment eliminates discontinuous vertical jumps or sudden downward resets at cycle boundaries.
* **Functions:** `WaveDriftPlayerTextDraw`, `WaveDriftTextDraw`
```pawn
WaveDriftPlayerTextDraw(playerid, textid, .amplitudeX = 6.0, .amplitudeY = 3.0, .speedMs = 1500, .loop = true);
```

### 12. Damped Gravity Pendulum Swing
Physically accurate harmonic pendulum motion under gravity with exponential drag decay:
$$\theta(t) = \theta_{\text{max}} \cdot e^{-\lambda t} \cdot \cos(\omega t)$$
* **Functions:** `PendulumPlayerTextDraw`, `PendulumTextDraw`
```pawn
PendulumPlayerTextDraw(playerid, textid, .pivotX = 320.0, .pivotY = 240.0, .armLength = 95.0, .maxAngle = 45.0, .damping = 0.55);
```

### 13. Black Hole Spiral Vortex Suction
Spirals the textdraw inward toward a singularity while simultaneously shrinking its dimension until absorption:
$$R(t) = R_0 \cdot (1 - t), \quad \theta(t) = \text{spins} \cdot 360^\circ \cdot t$$
* **Functions:** `VortexPlayerTextDraw`, `VortexTextDraw`
```pawn
VortexPlayerTextDraw(playerid, textid, .centerX = 320.0, .centerY = 240.0, .startRadius = 150.0, .spins = 1.6, .duration = 4500);
```

### 14. Sinusoidal Traveling Wave Motion
Physical arch wave traveling across individual characters. Letters rise up along a raised-cosine curve ($\Delta Y = -h \cdot \cos^2\left(\frac{90^\circ \cdot d}{w}\right)$) and settle back into their original baseline positions as the wave crest passes.
* **Functions:** `WaveMotionPlayerTextDraw`, `WaveMotionTextDraw`
* **Features:** Automatic proportional kerning with zero character compression.
```pawn
WaveMotionPlayerTextDraw(playerid, textid, "OCEAN WAVE MOTION", .centerX = 320.0, .centerY = 360.0, .crestHeight = 15.0, .crestWidth = 2.8, .direction = TDFX_DIR_LEFT_TO_RIGHT, .duration = 3200);
```

### 15. Letter Hopping Cascade
Individual characters jump into the air in rapid succession with a staggered delay. Trajectory follows an accelerated parabolic leap with an elastic settle bounce upon landing.
* **Functions:** `HopLetterPlayerTextDraw`, `HopLetterTextDraw`
```pawn
HopLetterPlayerTextDraw(playerid, textid, "BOUNCING HOP LETTERS", .centerX = 320.0, .centerY = 360.0, .hopHeight = 16.0, .staggerMs = 90, .hopDurationMs = 340);
```

---

## Category 3: Text Reveal, Decryption & Glyphs

### 16. Cinematic Typewriter
Progressively reveals characters one by one with tag-aware parsing (respecting `~r~`, `~w~`, etc.) and authentic dialogue cadence.
* **Functions:** `TypewriterPlayerTextDraw`, `TypewriterTextDraw`
```pawn
TypewriterPlayerTextDraw(playerid, textid, "Welcome to Los Santos, Carl.", .interval = 65);
```

### 17. Cyberpunk Matrix Glyph Decrypt
Randomized glyph scrambling that resolves into the plaintext from left to right, mimicking sci-fi cryptanalysis terminals.
* **Functions:** `ScramblePlayerTextDraw`, `ScrambleTextDraw`
```pawn
ScramblePlayerTextDraw(playerid, textid, "ACCESS GRANTED: LEVEL 5", .duration = 2400);
```

### 18. Real-Time Glyph Glitch Distortion
Maintains text legibility while randomly corrupting arbitrary glyphs into glitch characters (`#`, `@`, `%`, `&`, `?`) with chromatic flash tags.
* **Functions:** `GlitchPlayerTextDraw`, `GlitchTextDraw`
```pawn
GlitchPlayerTextDraw(playerid, textid, "SYSTEM MALFUNCTION", .intensity = 3, .duration = 3000);
```

### 19. Casino Mechanical Slot Machine
Rolls characters through randomized alphanumeric glyphs and locks each column sequentially from left to right with mechanical audio clicks.
* **Functions:** `SlotMachinePlayerTextDraw`, `SlotMachineTextDraw`
```pawn
SlotMachinePlayerTextDraw(playerid, textid, "$ 7 7 7 JACKPOT $", .spinDuration = 2400);
```

### 20. Split-Flap Mechanical Airport Board
Recreates vintage electromechanical split-flap departures boards (Solari di Udine), cycling characters through airport font symbols before locking.
* **Functions:** `SplitFlapPlayerTextDraw`, `SplitFlapTextDraw`
```pawn
SplitFlapPlayerTextDraw(playerid, textid, "FLIGHT 702: ON TIME", .lockIntervalMs = 180);
```

### 21. Scanner Laser Sweep (KITT / Cylon)
Sweeps a concentrated laser crest back and forth across characters with trailing fade tags (`~r~` core, `~y~` trail).
* **Functions:** `ScannerSweepPlayerTextDraw`, `ScannerSweepTextDraw`
```pawn
ScannerSweepPlayerTextDraw(playerid, textid, "AUTONOMOUS PATROL ACTIVE", .speedMs = 60);
```

### 22. Falling Letter Rain Assembly
Rains characters down from above the screen under simulated gravity, landing in proportional kerning alignment with an elastic ground bounce.
* **Functions:** `LetterRainPlayerTextDraw`, `LetterRainTextDraw`
```pawn
LetterRainPlayerTextDraw(playerid, textid, "VICTORY ROYALE", .targetX = 320.0, .targetY = 360.0, .letterSpacing = 0.0, .duration = 3200);
```

### 23. Traveling Wave Tag Sweep
Sweeps a color tag highlight across the string characters without moving the text physically.
* **Functions:** `WaveSweepPlayerTextDraw`, `WaveSweepTextDraw`
```pawn
WaveSweepPlayerTextDraw(playerid, textid, "TEXTDRAWFX WAVE CREST", .speedMs = 65);
```

---

## Category 4: Combat, Cinematics & HUD FX

### 24. 3D Shockwave Impact Slam
Text plummets from a massive 3D scale down to normal scale with rapid acceleration ($t^2$). Upon impact, it plays authentic GTA explosion audio (ID `1159`), triggers camera trauma shake, and bursts a 360° ring of shockwave sparks.
* **Functions:** `ShockwaveSlamPlayerTextDraw`, `ShockwaveSlamTextDraw`
```pawn
ShockwaveSlamPlayerTextDraw(playerid, textid, .centerX = 320.0, .centerY = 360.0, .duration = 2000);
```

### 25. Combat Trauma Screen Shake
Applies quadratic decaying random micro-displacements ($\Delta X, \Delta Y \propto \text{Trauma}^2$), perfectly simulating heavy artillery or explosive concussions.
* **Functions:** `ShakePlayerTextDraw`, `ShakeTextDraw`
```pawn
ShakePlayerTextDraw(playerid, textid, .intensity = 6.0, .duration = 1200);
```

### 26. Firework Fragmentation Text Explosion
Pre-detonation escalating tremor and color strobe followed by explosive fragmentation debris bursting across 5 distinct trajectory patterns (`TDFX_FW_SPHERE`, `TDFX_FW_FAN`, `TDFX_FW_ROCKET`, `TDFX_FW_RING`, `TDFX_FW_FOUNTAIN`).
* **Functions:** `FireworkPlayerTextDraw`, `FireworkTextDraw`
```pawn
FireworkPlayerTextDraw(playerid, textid, .pattern = TDFX_FW_BURST_SPHERE, .particleCount = 14);
```

### 27. 21:9 Widescreen Cinematic Letterbox
Smoothly slides letterbox cinema black bars from top and bottom. Mathematically calibrated with SA-MP Font 1 vertical ascent metrics ($35.8$ offset correction), ensuring **exact pixel-matched bar heights across all resolutions**.
* **Fixed Screen Boundaries:** Upper and lower black bars slide directly to/from screen borders ($Y=0.0$ and $Y=480.0$) independently of any text position, ensuring cinema borders always frame the display perfectly while dialogue/subtitle textdraws animate exactly at their created coordinates.
* **Dynamic Letter Size Scaling:** Automatically resizes font letter size on both bars (`topLetY = barHeight / 9.5` and `botLetY = (barHeight + 35.8) / 9.5`), guaranteeing that top and bottom bars render at the exact requested `barHeight` in physical pixels without top gaps or bottom clipping, for any custom height value.
* **Functions:** `LetterboxPlayerTextDraw`, `LetterboxTextDraw`
```pawn
LetterboxPlayerTextDraw(playerid, topBar, bottomBar, .barHeight = 65.0, .duration = 1500, .loop = false);
```

### 28. Submarine Sonar Acoustic Echo Ping
Emits concentric acoustic radar ping rings expanding outward with radial deceleration and quadratic alpha decay, accompanied by submarine sonar audio.
* **Functions:** `SonarPingPlayerTextDraw`, `SonarPingTextDraw`
```pawn
SonarPingPlayerTextDraw(playerid, textid, .centerX = 320.0, .centerY = 360.0, .maxRadius = 140.0, .pulseCount = 3);
```

### 29. Sci-Fi Hologram Projection Beam
Modulates cyan alpha channels with coupled high-frequency jitter and random scanline beam dropouts.
* **Functions:** `HologramPlayerTextDraw`, `HologramTextDraw`
```pawn
HologramPlayerTextDraw(playerid, textid, "TRANSMISSION INCOMING", .duration = 5500);
```

### 30. Short-Circuit Electric Arc Surge
Simulates high-voltage electrical arcing with rapid erratic coordinate jitter, random spark glyphs (`*`, `^`, `#`), and arc discharge audio.
* **Functions:** `ElectricArcPlayerTextDraw`, `ElectricArcTextDraw`
```pawn
ElectricArcPlayerTextDraw(playerid, textid, "HIGH VOLTAGE GRID OVERLOAD", .surgeDuration = 3200);
```

### 31. Heartbeat Sine Wave Pulse
Rhythmically pulses text dimensions along a sine wave ($\sim 70$ BPM):
$$\text{Scale}(t) = \text{Base} \cdot (1.0 + \text{depth} \cdot |\sin(\omega t)|)$$
* **Functions:** `PulsePlayerTextDraw`, `PulseTextDraw`
```pawn
PulsePlayerTextDraw(playerid, textid, .amplitude = 0.20, .interval = 300);
```

### 32. Elastic Dimension Scaling
Scales text letter size from $(S_{x1}, S_{y1}) \to (S_{x2}, S_{y2})$ using Penner easing curves (ideal for level-up popups and mission titles).
* **Functions:** `ScalePlayerTextDraw`, `ScaleTextDraw`
```pawn
ScalePlayerTextDraw(playerid, textid, .startSizeX = 0.2, .startSizeY = 0.8, .targetSizeX = 0.5, .targetSizeY = 2.0, .duration = 1000, .easing = TDFX_EASE_OUT_BOUNCE);
```

### 33. Animated Numeric Counter
Rolls a numeric integer from $V_1 \to V_2$ with optional currency prefixes (`$`), suffixes (`XP`), and thousands comma formatting (`1,000,000`).
* **Functions:** `CountPlayerTextDrawNumber`, `CountTextDrawNumber`
```pawn
CountPlayerTextDrawNumber(playerid, textid, .startValue = 0, .targetValue = 50000, .duration = 2000, .prefix = "$");
```

### 34. Dynamic Native Box Progress Fill
Smoothly interpolates a textdraw box width to represent progress bars without external plugins, supporting custom easing curves.
* **Alignment 1 (Left):** `startWidth` and `targetWidth` represent the absolute screen X coordinates where the box ends. Visual box height is governed by `LetterSize Y`.
* **Alignment 2 (Center):** `startWidth` and `targetWidth` represent the true horizontal width of the box (`TextSize Y` in SA-MP). The horizontal line wrap width (`TextSize X`) is preserved automatically.
* **Functions:** `FillPlayerTextDrawProgress`, `FillTextDrawProgress`
```pawn
FillPlayerTextDrawProgress(playerid, textid, .startWidth = 0.0, .targetWidth = 150.0, .duration = 2200, .easing = TDFX_EASE_OUT_QUAD);
```

### 35. Freefall Gravity Drop Slam
Drops text vertically under simulated gravity ($y(t) = y_0 + \frac{1}{2}gt^2$) with an elastic ground bounce impact and camera vibration.
* **Functions:** `GravityDropPlayerTextDraw`, `GravityDropTextDraw`
```pawn
GravityDropPlayerTextDraw(playerid, textid, .startY = 140.0, .targetY = 360.0, .elasticity = 0.38);
```

---

## Category 5: 3D Preview Model Animations (Font 5)

### 36. 3D Turntable Spin
Smoothly rotates a 3D skin, vehicle, or weapon preview model continuous 360° along its vertical Z-axis.
* **Functions:** `Spin3DPlayerTextDraw`, `Spin3DTextDraw`
```pawn
Spin3DPlayerTextDraw(playerid, textid, .speedZ = 45.0);
```

### 37. Harmonic 3D Hover & Pitch Tilt
Combines a gentle vertical hovering sine wave with harmonic pitch rocking ($X$-axis tilt), creating dynamic floating showroom pedestals.
* **Functions:** `Hover3DPlayerTextDraw`, `Hover3DTextDraw`
```pawn
Hover3DPlayerTextDraw(playerid, textid, .tiltX = 8.0, .tiltZ = 12.0, .zoomAmp = 0.06);
```
