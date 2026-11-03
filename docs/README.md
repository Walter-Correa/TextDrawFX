# TextDrawFX — Documentation Hub

Welcome to the technical documentation hub for **TextDrawFX** (SA-MP & open.mp). This library provides a high-performance, lightweight, and production-tested suite of 36+ visual effects, cinematics, and animation algorithms engineered specifically for San Andreas Multiplayer.

---

## Documentation Sections

| Section | Description |
| :--- | :--- |
| **[Getting Started](getting-started.md)** | Installation, zero-dependency setup, compiler configuration, automatic header detection, and your first animated textdraw. |
| **[Visual Effects Catalog](effects-catalog.md)** | Comprehensive gallery and mathematical breakdown of all 36+ animation and cinematic modes across 5 specialized categories. |
| **[Complete API Reference](api-reference.md)** | Exhaustive technical dictionary covering all PlayerTextDraw & Global TextDraw functions, parameters, return values, callbacks, and enumerations. |
| **[Compatibility, Resolution & Safety](compatibility-and-safety.md)** | Detailed guide on font kerning, box geometry, 21:9 & 16:9 widescreen correction, 3D preview model safety locks, and GTA color-tag tokenization. |

---

## Quick Navigation Matrix

Find immediate guidance based on your development goal:

* **I am installing TextDrawFX on a new or existing server:**
  * Follow the [Installation & Setup Guide](getting-started.md#1-prerequisites--installation).
  * Check [Compile-Time Configuration Defines](getting-started.md#3-compile-time-configuration-defines) to tune slot limits.
* **I want to create smooth screen transitions or subtitles:**
  * Read [True Alpha Fading & Cross-Fade](effects-catalog.md#1-true-alpha-fade-in--fade-out).
  * Read [Cinematic 21:9 Letterbox Widescreen Bars](effects-catalog.md#26-cinematic-219-letterbox).
* **I want physical text movement across the screen:**
  * Read [Sinusoidal Wave Motion Arch](effects-catalog.md#35-sinusoidal-wave-motion).
  * Read [Letter Hopping Cascade](effects-catalog.md#36-letter-hopping-cascade).
  * Read [Coordinate Sliding with Penner Easing](effects-catalog.md#6-coordinate-sliding-motion).
  * Read [2D Physics Bounding Wall Reflection (DVD screensaver)](effects-catalog.md#9-2d-bounding-box-reflection-dvd-screensaver).
* **I want dramatic dialogue or cyberpunk reveal effects:**
  * Read [Cyberpunk Matrix Decrypt (Scramble)](effects-catalog.md#8-cyberpunk-matrix-decrypt).
  * Read [Cinematic Typewriter](effects-catalog.md#2-cinematic-typewriter-reveal).
  * Read [Falling Letter Rain Assembly](effects-catalog.md#29-falling-letter-rain).
  * Read [Split-Flap Mechanical Airport Board](effects-catalog.md#32-split-flap-mechanical-airport-board).
* **I want combat feedback or celebratory impacts:**
  * Read [3D Shockwave Impact Slam](effects-catalog.md#34-3d-shockwave-impact-slam).
  * Read [Trauma Screen Shake](effects-catalog.md#12-combat-trauma-screen-shake).
  * Read [Firework Fragmentation Text Explosion](effects-catalog.md#17-firework--exploding-fragmentation-text).
* **I am animating a 3D skin, vehicle, or weapon preview model (Font 5):**
  * Read [3D Turntable Spin & Floating Hover](effects-catalog.md#category-5-3d-preview-model-animations-font-5).
  * Review [Model Preview Safety Constraints](compatibility-and-safety.md#3-3d-preview-models-font-5).

---

## Architectural Highlights & Performance Guarantees

* **Low Stack Footprint & Static Allocation:** TextDrawFX operates strictly within Pawn's default 16 KB stack boundary. All internal particle pools, string buffers, and mathematical state machines reside in `.data` memory as static allocations, consuming fewer than ~412 stack cells during execution.
* **Universal Dual API Parity:** Every single effect is mirrored with 100% feature parity for both **PlayerTextDraws** (`PlayerText:textid`) and **Global TextDraws** (`Text:textid`).
* **Zero External Dependencies:** Completely standalone Pawn library. No third-party plugins, streamers, or external dynamic libraries are required.
* **Aspect-Ratio & Multi-Resolution Independence:** Full mathematical compensation for SA-MP's virtual $640 \times 480$ canvas, ensuring that letterbox bars, particle bursts, and font kerning render identically across standard 4:3, 16:9 widescreen, and 21:9 ultrawide monitors.
* **Full Open.MP & SA-MP Cross-Compatibility:** Automatic compile-time environment detection with support for both legacy SA-MP (`0.3.7-R1` through `0.3.7-R5`) and modern open.mp native repositioning and color getters.

---

> [!NOTE]
> All code examples in this documentation suite are verified production-ready and compile with **0 errors and 0 warnings**.
