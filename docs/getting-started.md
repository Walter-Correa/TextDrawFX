# Getting Started with TextDrawFX

This guide walks you through installing **TextDrawFX**, configuring compile-time capacity settings, understanding the internal ALS hook architecture, and writing your first animated visual effect in Pawn.

---

## 1. Prerequisites & Installation

TextDrawFX is distributed as a single, self-contained Pawn include header (`TextDrawFX.inc`). It requires **zero external plugins** and has no dependencies on YSI, Streamer, or custom C++ extensions.

### Step 1: Copy Include File
Copy `TextDrawFX.inc` into your server development environment's include folder:
* **SA-MP / Pawno:** `pawno/include/TextDrawFX.inc`
* **open.mp / Qawno:** `qawno/include/TextDrawFX.inc`
* **sampctl / build pipelines:** place under your project's include path.

### Step 2: Include in Pawn Code
Include `TextDrawFX.inc` in your gamemode or filterscript immediately after `<a_samp>` or `<open.mp>`:

```pawn
#include <open.mp> // or <a_samp>
#include <TextDrawFX>
```

> [!TIP]
> If `<open.mp>` or `<a_samp>` has not yet been included prior to `<TextDrawFX>`, the library will automatically detect your environment and include the correct core header.

---

## 2. Compile-Time Configuration Defines

You can fine-tune memory usage, maximum concurrent animations, and animation frame rates by overriding any of the following preprocessor constants **before** including `<TextDrawFX>`:

| Constant | Default Value | Value Range | Description |
| :--- | :---: | :---: | :--- |
| `TDFX_MAX_FX` | `128` | `1` - `1024` | Maximum concurrent active animations running across the entire server. |
| `TDFX_MAX_TEXT` | `256` | `64` - `1024` | Maximum string buffer size (in cells) allocated for string manipulations (Typewriter, Decrypt, Glitch). |
| `TDFX_MAX_TD` | `MAX_PLAYER_TEXT_DRAWS` (`256`) | `1` - `256` | Maximum PlayerTextDraw IDs cached per connected player for state tracking and style restoration. |
| `TDFX_MAX_GLOBAL_TD` | `MAX_TEXT_DRAWS` (`2048`) | `0` - `2048` | Maximum Global TextDraw IDs tracked. Setting to `0` completely strips global tracking from memory. |
| `TDFX_LETTERBOX_SCREEN_BOTTOM` | `480.0` | Screen Y Units | Screen bottom boundary coordinate used for cinema bars (can be set to `448.0` for classic PS2 4:3 safe canvas). |
| `TDFX_LETTERBOX_ASCENT_OFFSET` | `35.8` | Units | Font 1 vertical ascent padding calibration ensuring identical top and bottom bar heights. |
| `TDFX_MIN_INTERVAL` | `30` | Milliseconds | Absolute minimum timer interval permitted by the engine to safeguard against timer spam. |
| `TDFX_FRAME_INTERVAL` | `40` | Milliseconds | Internal tick rate for continuous physics interpolations (~25 FPS fluid motion). |

```pawn
// Example: Optimizing memory footprint for a lightweight minigame server
#define TDFX_MAX_FX        32
#define TDFX_MAX_GLOBAL_TD 128
#include <TextDrawFX>
```

---

## 3. Creating Your First Animated TextDraw

Let's create a cinematic notification banner that slides onto the screen, pulses like a heartbeat, and fades away when finished.

```pawn
#include <a_samp>
#include <TextDrawFX>

new PlayerText:gAlertTD[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

#define EVENT_ALERT_SLIDE 1
#define EVENT_ALERT_FADE  2

public OnPlayerConnect(playerid)
{
    // Create the banner off-screen to the left (X = -200.0)
    gAlertTD[playerid] = CreatePlayerTextDraw(playerid, -200.0, 360.0, "ZONE SECURED: LOS SANTOS");
    PlayerTextDrawAlignment(playerid, gAlertTD[playerid], 2); // Center alignment
    PlayerTextDrawFont(playerid, gAlertTD[playerid], 1);
    PlayerTextDrawLetterSize(playerid, gAlertTD[playerid], 0.45, 1.80);
    PlayerTextDrawColor(playerid, gAlertTD[playerid], 0x2ECC71FF);
    PlayerTextDrawUseBox(playerid, gAlertTD[playerid], true);
    PlayerTextDrawBoxColor(playerid, gAlertTD[playerid], 0x000000BB);
    PlayerTextDrawTextSize(playerid, gAlertTD[playerid], 500.0, 320.0);
    PlayerTextDrawSetOutline(playerid, gAlertTD[playerid], 1);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (gAlertTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, gAlertTD[playerid]);
        gAlertTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }
    return 1;
}

// Command to trigger the animation sequence
public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/testalert", true))
    {
        // 1. Position off-screen and show
        PlayerTextDrawSetPos(playerid, gAlertTD[playerid], -200.0, 360.0);
        PlayerTextDrawShow(playerid, gAlertTD[playerid]);

        // 2. Slide to center screen (X = 320.0) with Ease-Out Quad
        SlidePlayerTextDraw(
            playerid, 
            gAlertTD[playerid], 
            .targetX = 320.0, 
            .targetY = 360.0, 
            .duration = 1200, 
            .easing = TDFX_EASE_OUT_QUAD, 
            .extra_id = EVENT_ALERT_SLIDE
        );
        return 1;
    }
    return 0;
}

// 3. React when the animation concludes
public OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id)
{
    if (textid == gAlertTD[playerid] && extra_id == EVENT_ALERT_SLIDE)
    {
        // Slide concluded! Now trigger a 2-second true alpha fade out with auto-hide
        FadeOutPlayerTextDraw(
            playerid, 
            gAlertTD[playerid], 
            .duration = 1500, 
            .fadeBox = true, 
            .autoHide = true, 
            .extra_id = EVENT_ALERT_FADE
        );
        return 1;
    }
    return 1;
}
```

---

## 4. Lifecycle & Callback System

TextDrawFX features an asynchronous event callback system that informs your gamemode whenever an animation cycle finishes:

### Global Callback Signatures

```pawn
// Fired when a PlayerTextDraw animation cycle ends
forward OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id);

// Fired when a Global TextDraw animation cycle ends
forward OnTextDrawFXFinish(Text:textid, fx_type, extra_id);
```

* **`playerid`:** Player who owns the animated textdraw.
* **`textid`:** Handle of the textdraw whose effect completed.
* **`fx_type`:** Animation identifier (e.g., `TDFX_TYPE_SLIDE`, `TDFX_TYPE_FADE`, `TDFX_TYPE_SHOCKWAVE`).
* **`extra_id`:** Custom user-defined identifier passed when starting the effect (enables clean state machines and chaining).

### Direct Function Callbacks (`callback[]`)
Every effect also supports a direct callback parameter (e.g., `callback[] = "OnMyAnimationEnd"`), allowing you to route completion events directly to custom `public` functions.

### Built-in Completion Modifiers
Most animations accept these optional lifecycle modifiers:
* **`.autoHide = true`:** Automatically hides the textdraw upon completion (`PlayerTextDrawHide` / `TextDrawHideForAll`).
* **`.autoDestroy = true`:** Automatically destroys the textdraw upon completion, preventing memory leaks for temporary notices.
* **`.loop = true`:** Automatically loops the animation continuously until stopped manually via `StopPlayerTextDrawFX` or `StopTextDrawFX`.

---

## 5. Stopping Active Effects

You can safely stop any active effect at any time:

```pawn
// Stops any running effect on a PlayerTextDraw and restores base styles
StopPlayerTextDrawFX(playerid, PlayerText:textid);

// Stops any running effect on a Global TextDraw and restores base styles
StopTextDrawFX(Text:textid);

// Check if an effect is currently running
if (IsPlayerTextDrawFXActive(playerid, textid))
{
    // ...
}
```
