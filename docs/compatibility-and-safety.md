# TextDraw Compatibility, Resolution & Safety Guide

This guide details how **TextDrawFX** achieves pixel-perfect visual fidelity, universal resolution independence, and bulletproof safety protections across GTA San Andreas client environments.

---

## 1. Resolution & Canvas Coordinate Independence

### The Virtual $640 \times 480$ Canvas
In San Andreas Multiplayer, all TextDraw coordinates $(X, Y)$ are mapped by the GTA:SA rendering engine to an internal virtual canvas of $640.0 \times 480.0$ units, regardless of the player's physical monitor resolution:

* $X = 0.0$ is always the extreme left edge of the display.
* $X = 640.0$ is always the extreme right edge of the display.
* $Y = 0.0$ is always the extreme top edge of the display.
* $Y = 480.0$ is always the extreme bottom edge of the display.

When a client runs at 1080p ($1920 \times 1080$), 1440p ($2560 \times 1440$), 4K ($3840 \times 2160$), or on a 21:9 Ultrawide monitor, GTA:SA scales the $640 \times 480$ space across the screen.

---

## 2. 21:9 Letterbox Widescreen Symmetry Calibration

### The Font 1 Box Ascent Phenomenon
In GTA San Andreas, a boxed textdraw (`UseBox = true`) using Font 1 does not start its colored bounding box at coordinate $Y$. Instead, GTA's `CFont::PrintString` adds an internal font ascent top padding proportional to `LetterSize.Y`:

$$\text{Top Padding} \approx 2.0 + (3.42 \cdot \text{LetterSize.Y}) \approx 35.8 \text{ SA-MP units (for LetterSize.Y } = 9.50)$$

If the bottom letterbox bar was positioned naively at $480.0 - \text{barHeight}$ ($415.0$):
* Its visible box would only begin at $415.0 + 35.8 = 450.8$.
* The visible bottom bar would only measure $480.0 - 450.8 = 29.2$ units, while the top bar measured $65.0$ units!

### The TextDrawFX Mathematical Solution
TextDrawFX incorporates the calibrated font ascent offset directly into the easing formula:

$$\text{curBottomY} = 480.0 - ((\text{barHeight} + 35.8) \cdot \text{factor})$$

At full deployment ($\text{factor} = 1.0$), the bottom bar is positioned at $480.0 - \text{barHeight} - 35.8$. The visible top edge of the box begins at exactly $480.0 - \text{barHeight}$ and extends past the bottom edge of the screen, guaranteeing **strictly identical pixel heights for both top and bottom bars on any display resolution**.

### Dynamic Letter Size Resizing
Because visible Font 1 box height in GTA:SA is strictly proportional to `LetterSize.Y` ($\approx \text{LetterSize.Y} \times 9.5$), dynamic scaling is automatically applied to both bars upon invocation:
$$\text{topLetY} = \frac{\text{barHeight}}{9.5}, \quad \text{botLetY} = \frac{\text{barHeight} + 35.8}{9.5}$$
This ensures that whether `barHeight` is set to 30.0, 65.0, 100.0, or any custom value, the visible boxes scale physically to that exact height, completely eliminating gaps at $Y=0$ or cutoffs at $Y=480$ regardless of how the input textdraws were originally constructed.

---

## 3. Proportional Font Kerning & Outline Dilation (`_TDFX_LayoutChars`)

### Why Monospace & Naive Spacing Fails in GTA SA
Fonts 0, 1, 2, and 3 in GTA San Andreas are **proportional fonts**, meaning that each individual glyph possesses its own unique horizontal metric stored in the game client's `fonts.dat` data asset. Furthermore, GTA SA's text engine (`CFont`) adds the active outline thickness directly to character advance spacing:

$$\text{Character Advance Width} = (\text{Width}_{\text{fonts.dat}}[\text{font}][\text{char}] + \text{Outline}) \times \text{letterSizeX}$$

If individual characters in multi-part animations (such as `LetterRain`, `WaveMotion`, and `HopLetter`) are laid out without exact `fonts.dat` metrics and outline dilation:
1. Narrow letters appear unnaturally spaced out, while wide letters collide.
2. The animation letters occupy fewer screen pixels than the consolidated native textdraw.
3. When the effect finishes and restores the original TextDraw, the text visibly snaps or shifts horizontally.

### Native GTA:SA Metric Table (`TDFX_FontWidths`)
TextDrawFX embeds the exact 95-character proportional width table for all 4 textdraw fonts (0, 1, 2, and 3) from GTA San Andreas. In addition, `_TDFX_LayoutChars` dynamically retrieves the textdraw's configured outline thickness (`SetOutline`) and factors it into character layout:

* **Font 0 (Diploma / Bank Gothic)**: Complete proportional widths for ASCII 32..126.
* **Font 1 (Bank Gothic / Standard)**: Complete proportional widths for ASCII 32..126.
* **Font 2 (Pricedown)**: Complete proportional widths for ASCII 32..126.
* **Font 3 (Condensed Beckett)**: Complete proportional widths for ASCII 32..126.

This achieves **100% pixel-perfect alignment** between animated individual characters and the restored parent TextDraw across any resolution and alignment mode.

### Clean Background Box & Ghost Text Elimination
When multi-character animations run on a textdraw with `UseBox` enabled, simply setting the text color alpha to 0 (`0x00`) still causes GTA SA to render the solid black outline of the stationary original string underneath the moving glyphs.

TextDrawFX solves this cleanly:
1. When the animation starts, the parent textdraw is temporarily set to a single space (`" "`).
2. A space character has zero glyph pixels and zero outline pixels, eliminating all ghost silhouettes.
3. The background box (`UseBox`) remains rendered at full geometry and opacity behind the moving letters.
4. When the animation concludes or is stopped, the full original string and text color are restored instantaneously.

---

## 4. TextDraw Types & Safety Constraints

TextDrawFX validates and adapts its behaviors depending on the target textdraw's internal type:

### 1. Standard Text (Fonts 0 - 3)
* Compatible with all 36 effects.
* Full support for background boxes (`UseBox`).

### 2. Texture Sprites (Font 4)
* Compatible with Opacity Fades (`FadeIn`, `FadeOut`, `FadeColor`), Coordinate Sliding (`Slide`), 2D Bounding Box Bounce (`Bounce`), Circular Bounce (`BounceCircle`), and Screen Shake (`Shake`).
* Incompatible text effects (such as `Typewriter`, `Scramble`, or `LetterRain`) are safely intercepted and rejected to protect sprite stability.

### 3. 3D Preview Models (Font 5)
* Compatible with 3D Turntable Spin (`Spin3D`), Harmonic Floating Hover (`Hover3D`), Coordinate Slide (`Slide`), Alpha Fading (`FadeIn`, `FadeOut`), and Screen Shake (`Shake`).
* In SA-MP, preview models use the `BackgroundColor` property as the model object index/ID. TextDrawFX's color fading hooks preserve the object model ID, preventing preview models from turning into invalid skins or objects.

### 4. Background Box Scaling Safety in 3D Animations
During high-impact scale animations (such as `ShockwaveSlam`), scaling the textdraw down from a $2.4\times$ multiplier causes GTA SA to scale the background box across the entire viewport. TextDrawFX automatically suppresses `UseBox` during the plummet phase and restores it instantaneously upon impact.

---

## 5. GTA SA Formatting Tag Tokenizer

GTA:SA text formatting codes must be parsed atomically to prevent visual corruption:
* Color tags: `~r~`, `~g~`, `~b~`, `~y~`, `~p~`, `~w~`, `~h~`
* Control tags: `~n~` (newline), `~u~` (up arrow), `~d~` (down arrow), `~<~` (left arrow), `~>~` (right arrow)
* Dynamic keybind tags: `~k~~PED_FIREWEAPON~`

TextDrawFX's tokenizer treats multi-character tags as atomic single tokens during Typewriter, Scramble, Glitch, and Shimmer effects, ensuring that tags are never bisected into raw text.

---

## 7. Global TextDraw Visibility & Player Isolation
In SA-MP, Global TextDraws (`Text:textid`) can be shown either to everyone via `TextDrawShowForAll` or to specific players via `TextDrawShowForPlayer`. 

To prevent global animations from improperly forcing textdraws onto players who should not see them, `TextDrawFX` uses an explicit 6-state visibility machine:
1. **`TDFX_GSTATE_UNTRACKED` (0):** Used for textdraws created before TextDrawFX hook was active. If `TextDrawShowForPlayer` or `TextDrawHideForPlayer` is called while untracked, connected players are snapshotted into the mask, transitioning cleanly to `TDFX_GSTATE_PER_PLAYER` without freezing textdraws or hiding them from everyone.
2. **`TDFX_GSTATE_SHOWN_ALL` (1):** Explicitly shown to all currently connected players via `TextDrawShowForAll`. Animation frames update via `TextDrawShowForPlayer` exclusively for players who were connected when the textdraw was shown, guaranteeing that late joiners who connect mid-animation do not see unrequested textdraws.
3. **`TDFX_GSTATE_PER_PLAYER` (2):** Shown to specific players via `TextDrawShowForPlayer` or hidden from specific players via `TextDrawHideForPlayer`. Animation frames update solely for players whose bit in `TDFX_GShownMask` is active. If the last viewing player is hidden mid-animation, any running effect timer is immediately freed to conserve server resources.
4. **`TDFX_GSTATE_HIDDEN_ALL` (3):** TextDraws created via `TextDrawCreate` start in this state (as newly created textdraws are not shown to anyone). Also entered when explicitly hidden from all players (via `TextDrawHideForAll` or when all viewing players are hidden via `TextDrawHideForPlayer`). When a new animation is triggered on a TextDraw in `TDFX_GSTATE_HIDDEN_ALL`, TextDrawFX automatically wakes the textdraw up to `TDFX_GSTATE_SHOWN_ALL` across currently connected players, allowing repeated global effects to run visibly without requiring manual re-showing.
5. **`TDFX_GSTATE_HIDDEN_PER_PLAYER` (4):** When an effect finishes with auto-hide on a TextDraw that was shown to specific players (e.g. a team or faction), it enters this state. The viewer bitmask is preserved in memory. Starting a new animation re-activates exclusively for that group, while an explicit `TextDrawShowForPlayer` while hidden cleanly clears the previous group before registering the new player.
6. **`TDFX_GSTATE_HIDDEN_SHOWN_ALL` (5):** When an effect finishes with auto-hide on a TextDraw that was shown to all connected players, it enters this state. The audience bitmask is preserved in memory. Repeating the animation shows only to players who were originally viewing (excluding late joiners), strictly honoring native SA-MP visibility. If all players from the original audience have disconnected, starting a new animation automatically refreshes the audience to all currently connected players.
7. **Connection & Teardown Synchrony:** `OnPlayerConnect(playerid)` automatically zeroes the player's bit in `TDFX_GShownMask` across all global textdraws, and natural effect completion or auto-hide uses `_TDFX_HideGlobalTD(textid, true)` to synchronize state and bitmasks without disturbing external view states.

---

## 8. Dynamic Particle & Letter Allocation Safeguards
Multi-element effects (`LetterRain`, `WaveMotion`, `HopLetter`, `Firework`, and `ShockwaveSlam`) dynamically allocate child textdraws:
* **Slot Capacity Validation:** Prior to spawning up to 32 individual characters, 12 firework sparks, or 12 shockwave impact fragments (`TDFX_FW_PARTICLES`), the engine verifies available slots via `TDFX_GetFreePlayerTDCount(playerid)` or `TDFX_GetFreeGlobalTDCount()`. The probe runs exactly once per verification check (`new freeTD = ...`), avoiding duplicate probing overhead.
* **Cross-Environment Reliability:**
  * **open.mp:** Directly interrogates `IsValidPlayerTextDraw` / `IsValidTextDraw`.
  * **SA-MP 0.3.7:** Employs zero-network-traffic real-time dynamic safe probing (creating server-side structures without `Show` and destroying immediately), discovering true allocation headroom without stale 1-second cache delays and independent of include order.
* If insufficient slots remain, the animation safely aborts and logs a descriptive server warning (`printf`), preventing partial glyph leaks.

---

## 9. Automatic Script Unload Resource Recovery
`TextDrawFX` hooks `OnFilterScriptExit` and `OnGameModeExit` via ALS v7:
* When a filterscript or gamemode is reloaded or stopped (`/rcon unloadfs` or server mode change), `TDFX_Exit()` is invoked automatically.
* All running animation timers are killed and all textdraw properties (colors, dimensions, text, positions) are restored cleanly to their base states.
* **Callback Suppression:** Completion callbacks (`OnPlayerTextDrawFXFinish`, `OnTextDrawFXFinish`, and custom callback strings) are **strictly suppressed** (`triggerFinish = false`) during shutdown, ensuring game scripts do not inadvertently trigger new animations or execute logic while unloading.
