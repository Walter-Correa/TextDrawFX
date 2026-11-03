/*
================================================================================
    TextDrawFX - Interactive Demonstration Suite (Filterscript)
    A complete in-game interactive testing suite for TextDrawFX visual effects,
    animations, and transitions on SA-MP and Open.MP.

    Author:      Walter Correa
    Version:     2.2.0
    Repository:  https://github.com/Walter-Correa/TextDrawFX
    License:     Mozilla Public License 2.0 (MPL-2.0)

    Dependencies:
    - TextDrawFX (TextDrawFX.inc v2.2.0)
================================================================================
*/

#define FILTERSCRIPT

#if !defined _samp_included && !defined _INC_open_mp
	#if __Pawn >= 0x030A
		#tryinclude <open.mp>
	#endif
	#if !defined _INC_open_mp
		#include <a_samp>
	#endif
#endif

#include "TextDrawFX.inc"

// -----------------------------------------------------------------------------
// Cross-Platform Compatibility Macros
// -----------------------------------------------------------------------------

#if defined _INC_open_mp
	#define TDFX_DEMO_FONT_1       TEXT_DRAW_FONT_1
	#define TDFX_DEMO_FONT_2       TEXT_DRAW_FONT_2
	#define TDFX_DEMO_FONT_3       TEXT_DRAW_FONT_3
	#define TDFX_DEMO_ALIGN_LEFT   TEXT_DRAW_ALIGN_LEFT
	#define TDFX_DEMO_ALIGN_CENTER TEXT_DRAW_ALIGN_CENTER
	#define TDFX_DEMO_DLG_STYLE    DIALOG_STYLE:
#else
	#define TDFX_DEMO_FONT_1       (1)
	#define TDFX_DEMO_FONT_2       (2)
	#define TDFX_DEMO_FONT_3       (3)
	#define TDFX_DEMO_ALIGN_LEFT   (1)
	#define TDFX_DEMO_ALIGN_CENTER (2)
	#define TDFX_DEMO_DLG_STYLE
#endif

#if defined DIALOG_STYLE_TABLIST_HEADERS
	#define TDFX_DLG_TABLIST DIALOG_STYLE_TABLIST_HEADERS
#else
	#define TDFX_DLG_TABLIST (5)
#endif

#define DIALOG_TDFX_MENU           (29500)

// Chat Color Palette
#define COLOR_TDFX_TAG             "{3498DB}[TextDrawFX]{FFFFFF} "
#define COLOR_TDFX_SUCCESS         "{2ECC71}[TextDrawFX]{FFFFFF} "
#define COLOR_TDFX_WARN            "{F39C12}[TextDrawFX]{FFFFFF} "

// Preview Identifiers
enum
{
	PREVIEW_FADE = 0,
	PREVIEW_COLOR,
	PREVIEW_SCALE,
	PREVIEW_PULSE,
	PREVIEW_SLIDE,
	PREVIEW_COUNT,
	PREVIEW_SCRAMBLE,
	PREVIEW_BLINK,
	PREVIEW_TYPEWRITER,
	PREVIEW_BOUNCE,
	PREVIEW_BOUNCE_CIRC,
	PREVIEW_ORBIT,
	PREVIEW_SHAKE,
	PREVIEW_NEON,
	PREVIEW_WAVE,
	PREVIEW_PROGRESS,
	PREVIEW_GLITCH,
	PREVIEW_FIREWORK,
	PREVIEW_SPIN3D,
	PREVIEW_HOVER3D,
	PREVIEW_SHIMMER,
	PREVIEW_RAINBOW,
	PREVIEW_SLOT,
	PREVIEW_PENDULUM,
	PREVIEW_SLAM,
	PREVIEW_VORTEX,
	PREVIEW_LETTERBOX,
	PREVIEW_SONAR,
	PREVIEW_WAVESWEEP,
	PREVIEW_LETTERRAIN,
	PREVIEW_SCANNER,
	PREVIEW_ELECTRIC,
	PREVIEW_SPLITFLAP,
	PREVIEW_HOLOGRAM,
	PREVIEW_SHOCKWAVE,
	PREVIEW_WAVEMOTION,
	PREVIEW_HOPLETTER,
	PREVIEW_GLOBAL,
	PREVIEW_CHAIN,
	PREVIEW_TOTAL
};

// Event IDs for Lifecycle Callback Testing
#define EVENT_PREVIEW_FADE_IN      (101)
#define EVENT_PREVIEW_FADE_OUT     (102)
#define EVENT_PREVIEW_COLOR        (103)
#define EVENT_PREVIEW_SCALE        (104)
#define EVENT_PREVIEW_PULSE        (105)
#define EVENT_PREVIEW_SLIDE        (106)
#define EVENT_PREVIEW_COUNT        (107)
#define EVENT_PREVIEW_SCRAMBLE     (108)
#define EVENT_PREVIEW_BLINK        (109)
#define EVENT_PREVIEW_TYPEWRITER   (110)
#define EVENT_PREVIEW_GLOBAL_IN    (111)
#define EVENT_PREVIEW_GLOBAL_OUT   (112)
#define EVENT_AUTO_CLEANUP         (113)
#define EVENT_PREVIEW_BOUNCE       (114)
#define EVENT_PREVIEW_BOUNCE_CIRC  (115)
#define EVENT_PREVIEW_ORBIT        (116)
#define EVENT_PREVIEW_SHAKE        (117)
#define EVENT_PREVIEW_NEON         (118)
#define EVENT_PREVIEW_WAVE         (119)
#define EVENT_PREVIEW_PROGRESS     (120)
#define EVENT_PREVIEW_GLITCH       (121)
#define EVENT_PREVIEW_FIREWORK     (122)
#define EVENT_PREVIEW_SPIN3D       (123)
#define EVENT_PREVIEW_HOVER3D      (124)
#define EVENT_PREVIEW_SHIMMER      (125)
#define EVENT_PREVIEW_RAINBOW      (126)
#define EVENT_PREVIEW_SLOT         (127)
#define EVENT_PREVIEW_PENDULUM     (128)
#define EVENT_PREVIEW_SLAM         (129)
#define EVENT_PREVIEW_VORTEX       (130)
#define EVENT_PREVIEW_LETTERBOX      (131)
#define EVENT_PREVIEW_LETTERBOX_EXIT (132)
#define EVENT_PREVIEW_SONAR          (133)
#define EVENT_PREVIEW_WAVESWEEP      (134)
#define EVENT_PREVIEW_LETTERRAIN     (135)
#define EVENT_PREVIEW_SCANNER        (136)
#define EVENT_PREVIEW_ELECTRIC       (137)
#define EVENT_PREVIEW_SPLITFLAP      (138)
#define EVENT_PREVIEW_HOLOGRAM       (139)
#define EVENT_PREVIEW_SHOCKWAVE      (140)
#define EVENT_PREVIEW_WAVEMOTION     (141)
#define EVENT_PREVIEW_HOPLETTER      (142)

#define EVENT_CHAIN_SLIDE          (201)
#define EVENT_CHAIN_PULSE          (202)
#define EVENT_CHAIN_COUNT          (203)
#define EVENT_CHAIN_FADE_OUT       (204)

// -----------------------------------------------------------------------------
// State Storage
// -----------------------------------------------------------------------------

// Mutual exclusion: stores the index of the currently active preview (-1 = none)
static stock gActivePreview[MAX_PLAYERS] = {-1, ...};
static stock bool:gGlobalPreviewActive = false;

// Player-specific Distinct Showcase TextDraws (Positioned in the Bottom Viewport X=320.0, Y=365.0)
static stock PlayerText:gTitleTD[MAX_PLAYERS]     = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gCounterTD[MAX_PLAYERS]   = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gSubtitleTD[MAX_PLAYERS]  = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gSlideTD[MAX_PLAYERS]     = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gProgressBarTD[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gPreviewModelTD[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gTopBarTD[MAX_PLAYERS]       = {PlayerText:INVALID_TEXT_DRAW, ...};
static stock PlayerText:gBottomBarTD[MAX_PLAYERS]    = {PlayerText:INVALID_TEXT_DRAW, ...};

// Server-wide Global TextDraw (Positioned at X=320.0, Y=365.0)
static stock Text:gBroadcastTD = Text:INVALID_TEXT_DRAW;

// -----------------------------------------------------------------------------
// Internal TextDraw Initialization Helpers
// -----------------------------------------------------------------------------

static stock InitPlayerDemoTextDraws(playerid)
{
	gActivePreview[playerid] = -1;

	// 1. Center Title Banner: X=320.0, Y=365.0 (Font 2, bold title with dark background box)
	// Note on SA-MP Alignment 2 (Center): TextSize parameters are (lineHeight, boxWidth)
	gTitleTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 365.0, "TEXTDRAWFX DEMO");
	PlayerTextDrawAlignment(playerid, gTitleTD[playerid], TDFX_DEMO_ALIGN_CENTER);
	PlayerTextDrawFont(playerid, gTitleTD[playerid], TDFX_DEMO_FONT_2);
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.52, 2.10);
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, gTitleTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, gTitleTD[playerid], 0);
	PlayerTextDrawUseBox(playerid, gTitleTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 24.0, 420.0);

	// 2. Center Cash Counter Badge: X=320.0, Y=365.0 (Font 3, GTA bank style, emerald green)
	gCounterTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 365.0, "$0");
	PlayerTextDrawAlignment(playerid, gCounterTD[playerid], TDFX_DEMO_ALIGN_CENTER);
	PlayerTextDrawFont(playerid, gCounterTD[playerid], TDFX_DEMO_FONT_3);
	PlayerTextDrawLetterSize(playerid, gCounterTD[playerid], 0.50, 2.00);
	PlayerTextDrawColor(playerid, gCounterTD[playerid], 0x2ECC71FF);
	PlayerTextDrawSetOutline(playerid, gCounterTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, gCounterTD[playerid], 0);
	PlayerTextDrawUseBox(playerid, gCounterTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gCounterTD[playerid], 0x000000AA);
	PlayerTextDrawTextSize(playerid, gCounterTD[playerid], 24.0, 280.0);

	// 3. Subtitle Bar: X=320.0, Y=365.0 (Font 1, wide cinematic box for Scramble & Typewriter)
	gSubtitleTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 340.0, "System ready. Select an effect from /tdfx.");
	PlayerTextDrawAlignment(playerid, gSubtitleTD[playerid], TDFX_DEMO_ALIGN_CENTER);
	PlayerTextDrawFont(playerid, gSubtitleTD[playerid], TDFX_DEMO_FONT_1);
	PlayerTextDrawLetterSize(playerid, gSubtitleTD[playerid], 0.32, 1.40);
	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, gSubtitleTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, gSubtitleTD[playerid], 0);
	PlayerTextDrawUseBox(playerid, gSubtitleTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000CC);
	PlayerTextDrawTextSize(playerid, gSubtitleTD[playerid], 20.0, 520.0);

	// 4. Sliding Banner: X=320.0, Y=365.0 (Font 2, framed dark banner for coordinate motion)
	gSlideTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 365.0, "~y~MISSION PASSED!~w~ +RESPECT");
	PlayerTextDrawAlignment(playerid, gSlideTD[playerid], TDFX_DEMO_ALIGN_CENTER);
	PlayerTextDrawFont(playerid, gSlideTD[playerid], TDFX_DEMO_FONT_2);
	PlayerTextDrawLetterSize(playerid, gSlideTD[playerid], 0.45, 1.80);
	PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, gSlideTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, gSlideTD[playerid], 0);
	PlayerTextDrawUseBox(playerid, gSlideTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
	PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 22.0, 380.0);

	// 5. Dedicated Progress Bar: X=180.0, Y=380.0 (Left-aligned solid emerald bar, matching title box)
	gProgressBarTD[playerid] = CreatePlayerTextDraw(playerid, 150.0, 380.0, "_");
	PlayerTextDrawAlignment(playerid, gProgressBarTD[playerid], TDFX_DEMO_ALIGN_LEFT);
	PlayerTextDrawFont(playerid, gProgressBarTD[playerid], TDFX_DEMO_FONT_1);
	PlayerTextDrawLetterSize(playerid, gProgressBarTD[playerid], 0.0, 1.25);
	PlayerTextDrawColor(playerid, gProgressBarTD[playerid], 0x2ECC7100);
	PlayerTextDrawUseBox(playerid, gProgressBarTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gProgressBarTD[playerid], 0x2ECC71EE);
	PlayerTextDrawTextSize(playerid, gProgressBarTD[playerid], 150.0, 14.0);

	// 6. 3D Model Preview: X=260.0, Y=310.0, Size=120.0, 90.0
	gPreviewModelTD[playerid] = CreatePlayerTextDraw(playerid, 260.0, 310.0, "_");
	PlayerTextDrawFont(playerid, gPreviewModelTD[playerid], 5);
	PlayerTextDrawTextSize(playerid, gPreviewModelTD[playerid], 120.0, 90.0);
	PlayerTextDrawSetPreviewModel(playerid, gPreviewModelTD[playerid], 411);
	PlayerTextDrawSetPreviewRot(playerid, gPreviewModelTD[playerid], -15.0, 0.0, -45.0, 1.0);
	PlayerTextDrawBackgroundColor(playerid, gPreviewModelTD[playerid], 0x00000000);

	// 7. Top & Bottom Cinematic Letterbox Bars
	gTopBarTD[playerid] = CreatePlayerTextDraw(playerid, 0.0, -80.0, "_");
	PlayerTextDrawFont(playerid, gTopBarTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, gTopBarTD[playerid], 0.0, 6.84);
	PlayerTextDrawUseBox(playerid, gTopBarTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gTopBarTD[playerid], 0x000000FF);
	PlayerTextDrawTextSize(playerid, gTopBarTD[playerid], 640.0, 0.0);

	gBottomBarTD[playerid] = CreatePlayerTextDraw(playerid, 0.0, 480.0, "_");
	PlayerTextDrawFont(playerid, gBottomBarTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, gBottomBarTD[playerid], 0.0, 9.50);
	PlayerTextDrawUseBox(playerid, gBottomBarTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gBottomBarTD[playerid], 0x000000FF);
	PlayerTextDrawTextSize(playerid, gBottomBarTD[playerid], 640.0, 0.0);
}

static stock DestroyPlayerDemoTextDraws(playerid)
{
	gActivePreview[playerid] = -1;

	if (gTitleTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gTitleTD[playerid]);
		gTitleTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gCounterTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gCounterTD[playerid]);
		gCounterTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gSubtitleTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gSubtitleTD[playerid]);
		gSubtitleTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gSlideTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gSlideTD[playerid]);
		gSlideTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gProgressBarTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gProgressBarTD[playerid]);
		gProgressBarTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gPreviewModelTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gPreviewModelTD[playerid]);
		gPreviewModelTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gTopBarTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gTopBarTD[playerid]);
		gTopBarTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
	if (gBottomBarTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, gBottomBarTD[playerid]);
		gBottomBarTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
}

// -----------------------------------------------------------------------------
// Interactive Dialog Menu
// -----------------------------------------------------------------------------

stock ShowTDFXMenu(playerid)
{
	new current = gActivePreview[playerid];
	static sDialogContent[4096];
	sDialogContent[0] = '\0';
	format(sDialogContent, sizeof sDialogContent,
		"Effect\tDescription\tStatus\n\
		{FFFFFF}True Alpha Fade\t0%% -> 100%% Opacity\t%s\n\
		{FFFFFF}Color Cross-Fade\tGreen -> Red Alert Shift\t%s\n\
		{FFFFFF}Dimension Scaling\tLetter Grow with Bounce Easing\t%s\n\
		{FFFFFF}Heartbeat Pulse\tSine Wave Pulsing (~70 BPM)\t%s\n\
		{FFFFFF}Coordinate Slide\tGlide In with Ease-Out Quad\t%s\n\
		{FFFFFF}Animated Cash Counter\tRolling $0 -> $50,000\t%s\n\
		{FFFFFF}Cyberpunk Decrypt\tMatrix Glyph Decryption\t%s\n\
		{FFFFFF}Visibility Blink\tClassical Show / Hide (500ms)\t%s\n\
		{FFFFFF}Cinematic Typewriter\tGTA Tag-Safe Dialogue Pacing\t%s\n\
		{FFFFFF}Retro DVD Box Bounce\t2D Bounding Box Reflection\t%s\n\
		{FFFFFF}Circular Bounce\tElastic Circle Perimeter Bounce\t%s\n\
		{FFFFFF}Orbital Motion\tContinuous Elliptical Path\t%s\n\
		{FFFFFF}Trauma Screen Shake\tMicro-displacement Decay\t%s\n\
		{FFFFFF}Broken Neon Flicker\tStochastic Gas Tube Malfunction\t%s\n\
		{FFFFFF}Waveform Drift\tHarmonic Dual-Sine Floating\t%s\n\
		{FFFFFF}Dynamic Progress Fill\tSmooth Box Bar Fill with Easing\t%s\n\
		{FFFFFF}Cyberpunk Glitch\tReal-time Glyph Disturbance\t%s\n\
		{FFFFFF}Firework Text\tExploding Fragmentation FX\t%s\n\
		{FFFFFF}3D Turntable Spin\tContinuous 360 Rotation\t%s\n\
		{FFFFFF}3D Floating Tilt\tHarmonic 3D Hover & Pitch\t%s\n\
		{FFFFFF}Golden Shimmer\tSweeping Chrome Highlight\t%s\n\
		{FFFFFF}RGB Rainbow Wave\tDynamic 8-Stage Flowing Wave\t%s\n\
		{FFFFFF}Casino Slot Machine\tMechanical Rolling Reel Lock\t%s\n\
		{FFFFFF}Pendulum Swing\tDamped Gravity Arm Swing\t%s\n\
		{FFFFFF}Gravity Drop Slam\tFreefall Acceleration & Impact\t%s\n\
		{FFFFFF}Black Hole Vortex\tSpiral Suction Decay\t%s\n\
		{FFFFFF}Cinematic Letterbox\t21:9 Cinema Black Bars\t%s\n\
		{FFFFFF}Sonar Echo Ping\tSubmarine Radar Acoustic Pulse\t%s\n\
		{FFFFFF}Traveling Wave Sweep\tFlowing Horizontal Wave Crest\t%s\n\
		{FFFFFF}Falling Letter Rain\tGravity Drop Letter Assembly\t%s\n\
		{FFFFFF}Scanner Laser Sweep\tKITT / Cylon Laser Sweep\t%s\n\
		{FFFFFF}Electric Arc Surge\tShort-Circuit Sparks & Jitter\t%s\n\
		{FFFFFF}Split-Flap Airport Board\tMechanical Rolling Solari Board\t%s\n\
		{FFFFFF}Sci-Fi Hologram\tFuturistic Cyan Beam & Scanlines\t%s\n\
		{FFFFFF}Shockwave Impact Slam\t3D Plummet, Trauma & Blast Ring\t%s\n\
		{FFFFFF}Sinusoidal Wave Motion\tPhysical Traveling Arch Wave\t%s\n\
		{FFFFFF}Letter Hopping Cascade\tStaggered Letter Jump & Bounce\t%s\n\
		{FFFFFF}Global Broadcast\tServer-Wide Announcement TD\t%s\n\
		{FFFFFF}Chained Cutscene\tAutomated 4-Stage Sequence\t%s\n\
		{E74C3C}[Stop Active Effect]\tImmediately reset screen\t-",
		(current == PREVIEW_FADE)        ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_COLOR)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SCALE)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_PULSE)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SLIDE)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_COUNT)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SCRAMBLE)    ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_BLINK)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_TYPEWRITER)  ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_BOUNCE)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_BOUNCE_CIRC) ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_ORBIT)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SHAKE)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_NEON)        ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_WAVE)        ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_PROGRESS)    ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_GLITCH)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_FIREWORK)    ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SPIN3D)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_HOVER3D)     ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SHIMMER)     ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_RAINBOW)     ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SLOT)        ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_PENDULUM)    ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SLAM)        ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_VORTEX)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_LETTERBOX)   ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SONAR)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_WAVESWEEP)   ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_LETTERRAIN)  ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SCANNER)     ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_ELECTRIC)    ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SPLITFLAP)   ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_HOLOGRAM)    ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SHOCKWAVE)   ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_WAVEMOTION)  ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_HOPLETTER)   ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		gGlobalPreviewActive             ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_CHAIN)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-")
	);

	ShowPlayerDialog(
		playerid, 
		DIALOG_TDFX_MENU, 
		TDFX_DEMO_DLG_STYLE TDFX_DLG_TABLIST, 
		"{3498DB}TextDrawFX {FFFFFF}v" TEXTDRAWFX_VERSION_STRING " - Demonstration Suite", 
		sDialogContent, 
		"Select", 
		"Close"
	);
	return 1;
}

// -----------------------------------------------------------------------------
// Mutual Exclusion Controller
// -----------------------------------------------------------------------------

stock StopCurrentPreview(playerid, bool:notify = false)
{
	new current = gActivePreview[playerid];

	StopPlayerTextDrawFX(playerid, gTitleTD[playerid]);
	StopPlayerTextDrawFX(playerid, gCounterTD[playerid]);
	StopPlayerTextDrawFX(playerid, gSubtitleTD[playerid]);
	StopPlayerTextDrawFX(playerid, gSlideTD[playerid]);
	StopPlayerTextDrawFX(playerid, gProgressBarTD[playerid]);
	StopPlayerTextDrawFX(playerid, gPreviewModelTD[playerid]);
	StopPlayerTextDrawFX(playerid, gTopBarTD[playerid]);
	StopPlayerTextDrawFX(playerid, gBottomBarTD[playerid]);

	PlayerTextDrawHide(playerid, gTitleTD[playerid]);
	PlayerTextDrawHide(playerid, gCounterTD[playerid]);
	PlayerTextDrawHide(playerid, gSubtitleTD[playerid]);
	PlayerTextDrawHide(playerid, gSlideTD[playerid]);
	PlayerTextDrawHide(playerid, gProgressBarTD[playerid]);
	PlayerTextDrawHide(playerid, gPreviewModelTD[playerid]);
	PlayerTextDrawHide(playerid, gTopBarTD[playerid]);
	PlayerTextDrawHide(playerid, gBottomBarTD[playerid]);

	// Restore default positions and base colors for future shows
	#if defined PlayerTextDrawSetPos
		new Float:px, Float:py;
		if (TDFX_GetPlayerPos(playerid, gTitleTD[playerid], px, py)) PlayerTextDrawSetPos(playerid, gTitleTD[playerid], px, py);
		if (TDFX_GetPlayerPos(playerid, gCounterTD[playerid], px, py)) PlayerTextDrawSetPos(playerid, gCounterTD[playerid], px, py);
		if (TDFX_GetPlayerPos(playerid, gSubtitleTD[playerid], px, py)) PlayerTextDrawSetPos(playerid, gSubtitleTD[playerid], px, py);
		if (TDFX_GetPlayerPos(playerid, gSlideTD[playerid], px, py)) PlayerTextDrawSetPos(playerid, gSlideTD[playerid], px, py);
		PlayerTextDrawSetPos(playerid, gTopBarTD[playerid], 0.0, -80.0);
		PlayerTextDrawSetPos(playerid, gBottomBarTD[playerid], 0.0, 480.0);
	#endif

	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);

	PlayerTextDrawColor(playerid, gCounterTD[playerid], 0x2ECC71FF);
	PlayerTextDrawBoxColor(playerid, gCounterTD[playerid], 0x000000AA);
	PlayerTextDrawTextSize(playerid, gCounterTD[playerid], 24.0, 280.0);

	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000CC);
	PlayerTextDrawTextSize(playerid, gSubtitleTD[playerid], 20.0, 520.0);

	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.52, 2.10);
	PlayerTextDrawLetterSize(playerid, gSlideTD[playerid], 0.45, 1.80);
	PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
	PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 500.0, 380.0);

	PlayerTextDrawColor(playerid, gProgressBarTD[playerid], 0x2ECC7100);
	PlayerTextDrawBoxColor(playerid, gProgressBarTD[playerid], 0x2ECC71EE);
	PlayerTextDrawTextSize(playerid, gProgressBarTD[playerid], 150.0, 14.0);

	if (current == PREVIEW_GLOBAL || gGlobalPreviewActive)
	{
		StopTextDrawFX(gBroadcastTD);
		TextDrawHideForAll(gBroadcastTD);
		TextDrawColor(gBroadcastTD, 0xFFFFFFFF);
		TextDrawBoxColor(gBroadcastTD, 0x1A5276DD);
		gGlobalPreviewActive = false;
	}

	gActivePreview[playerid] = -1;

	if (notify)
	{
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Active preview stopped.");
	}
	return 1;
}

// -----------------------------------------------------------------------------
// Individual Preview Triggers
// -----------------------------------------------------------------------------

stock Start_Fade(playerid)
{
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "TRUE ALPHA FADE");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);

	FadeInPlayerTextDraw(
		playerid, 
		gTitleTD[playerid], 
		.duration = 1400, 
		.fadeBox = true, 
		.targetAlpha = 255, 
		.extra_id = EVENT_PREVIEW_FADE_IN
	);

	gActivePreview[playerid] = PREVIEW_FADE;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "True Alpha Fade In started (1400ms)...");
	return 1;
}

stock Start_Color(playerid)
{
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "COLOR CROSS-FADE");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0x2ECC71FF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x145A32AA);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	FadeColorPlayerTextDraw(
		playerid, 
		gTitleTD[playerid], 
		.fromColor = 0x2ECC71FF, 
		.toColor = 0xE74C3CFF, 
		.duration = 1800, 
		.fadeBox = true, 
		.fromBox = 0x145A32AA, 
		.toBox = 0x78281FAA, 
		.easing = TDFX_EASE_IN_OUT_QUAD, 
		.extra_id = EVENT_PREVIEW_COLOR
	);

	gActivePreview[playerid] = PREVIEW_COLOR;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Color Cross-Fade started (Green -> Red)...");
	return 1;
}

stock Start_Scale(playerid)
{
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "BOUNCE SCALE FX");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xF1C40FFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000CC);

	ScalePlayerTextDraw(
		playerid, 
		gTitleTD[playerid], 
		.startSizeX = 0.20, 
		.startSizeY = 0.80, 
		.targetSizeX = 0.52, 
		.targetSizeY = 2.10, 
		.duration = 1500, 
		.easing = TDFX_EASE_OUT_BOUNCE, 
		.extra_id = EVENT_PREVIEW_SCALE
	);

	gActivePreview[playerid] = PREVIEW_SCALE;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Letter scaling with Bounce Easing started...");
	return 1;
}

stock Start_Pulse(playerid)
{
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "HEARTBEAT PULSE");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xE74C3CFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x2C0B0ECC);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	// Realistic human heartbeat pulse (~70 BPM, 850ms per rhythmic beat)
	PulsePlayerTextDraw(
		playerid, 
		gTitleTD[playerid], 
		.amplitude = 0.22, 
		.interval = 850, 
		.cycles = 5, 
		.extra_id = EVENT_PREVIEW_PULSE
	);

	gActivePreview[playerid] = PREVIEW_PULSE;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Heartbeat Pulse started (~70 BPM, 5 beats)...");
	return 1;
}

stock Start_Slide(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Coordinate slide motion requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 140.0, 365.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~y~SLIDE MOTION~w~ (EASE OUT QUAD)");
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xFFFFFFFF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		SlidePlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.targetX = 320.0, 
			.targetY = 365.0, 
			.duration = 1300, 
			.easing = TDFX_EASE_OUT_QUAD, 
			.extra_id = EVENT_PREVIEW_SLIDE
		);

		gActivePreview[playerid] = PREVIEW_SLIDE;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Coordinate Slide started (Ease-Out Quad)...");
	#endif
	return 1;
}

stock Start_Count(playerid)
{
	PlayerTextDrawSetString(playerid, gCounterTD[playerid], "$0");
	PlayerTextDrawColor(playerid, gCounterTD[playerid], 0x2ECC71FF);
	PlayerTextDrawBoxColor(playerid, gCounterTD[playerid], 0x000000AA);
	PlayerTextDrawShow(playerid, gCounterTD[playerid]);

	CountPlayerTextDrawNumber(
		playerid, 
		gCounterTD[playerid], 
		.startValue = 0, 
		.targetValue = 50000, 
		.duration = 2400, 
		.prefix = "$", 
		.suffix = "", 
		.thousandsSep = true, 
		.easing = TDFX_EASE_OUT_QUAD, 
		.extra_id = EVENT_PREVIEW_COUNT
	);

	gActivePreview[playerid] = PREVIEW_COUNT;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Animated Cash Counter started ($0 -> $50,000)...");
	return 1;
}

stock Start_Scramble(playerid)
{
	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000CC);
	PlayerTextDrawShow(playerid, gSubtitleTD[playerid]);

	ScramblePlayerTextDraw(
		playerid, 
		gSubtitleTD[playerid], 
		"ACCESS GRANTED: TEXTDRAWFX v" TEXTDRAWFX_VERSION_STRING " SYSTEM CORE INITIALIZED", 
		.duration = 2800, 
		.extra_id = EVENT_PREVIEW_SCRAMBLE
	);

	gActivePreview[playerid] = PREVIEW_SCRAMBLE;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Cyberpunk Decrypt started...");
	return 1;
}

stock Start_Blink(playerid)
{
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "VISIBILITY BLINK (500ms)");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);

	BlinkPlayerTextDraw(
		playerid, 
		gTitleTD[playerid], 
		.interval = 500, 
		.duration = 4500
	);

	SetTimerEx("TDFX_OnBlinkFinish", 4500, false, "i", playerid);

	gActivePreview[playerid] = PREVIEW_BLINK;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Visibility Blink started (500ms intervals)...");
	return 1;
}

forward TDFX_OnBlinkFinish(playerid);
public TDFX_OnBlinkFinish(playerid)
{
	if (gActivePreview[playerid] == PREVIEW_BLINK)
	{
		PlayerTextDrawHide(playerid, gTitleTD[playerid]);
		gActivePreview[playerid] = -1;
		SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Visibility Blink animation completed.");
	}
	return 1;
}

stock Start_Typewriter(playerid)
{
	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000CC);
	PlayerTextDrawShow(playerid, gSubtitleTD[playerid]);

	TypewriterPlayerTextDraw(
		playerid, 
		gSubtitleTD[playerid], 
		"~y~Carl Johnson:~w~ Grove Street... home. At least it was before I messed everything up.",
		.interval = 50, 
		.duration = 4000, 
		.extra_id = EVENT_PREVIEW_TYPEWRITER
	);

	SetTimerEx("TDFX_OnTypewriterFinish", 4500, false, "i", playerid);

	gActivePreview[playerid] = PREVIEW_TYPEWRITER;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Cinematic Typewriter started...");
	return 1;
}

forward TDFX_OnTypewriterFinish(playerid);
public TDFX_OnTypewriterFinish(playerid)
{
	if (gActivePreview[playerid] == PREVIEW_TYPEWRITER)
	{
		FadeOutPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
	}
	return 1;
}

stock Start_Bounce(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "2D Physics Bounce requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 320.0, 365.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~y~[DVD]~w~ TEXTDRAWFX");
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xF1C40FFF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
		PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 20.0, 160.0);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		// Bounces within the bottom viewport area (X: 140..500, Y: 355..410) with DVD color shifts!
		BouncePlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.speedX = 2.4, 
			.speedY = 1.6, 
			.minX = 140.0, 
			.minY = 355.0, 
			.maxX = 500.0, 
			.maxY = 410.0, 
			.colorShift = true, 
			.duration = 12000, 
			.extra_id = EVENT_PREVIEW_BOUNCE
		);

		gActivePreview[playerid] = PREVIEW_BOUNCE;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Retro DVD Box Bounce started (Wall reflections with color shifts)...");
	#endif
	return 1;
}

stock Start_BounceCircle(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Circular Perimeter Bounce requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 345.0, 370.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~r~[O]~w~ BOUNCE");
		PlayerTextDrawLetterSize(playerid, gSlideTD[playerid], 0.38, 1.50);
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xE74C3CFF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111EE);
		PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 18.0, 95.0);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		// Bounces dynamically inside circular perimeter arena (Center: 320, 385 | Radius: 65.0)
		BounceCirclePlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.speed = 3.0, 
			.angle = 52.0, 
			.centerX = 320.0, 
			.centerY = 385.0, 
			.radius = 65.0, 
			.colorShift = true, 
			.duration = 14000, 
			.extra_id = EVENT_PREVIEW_BOUNCE_CIRC
		);

		gActivePreview[playerid] = PREVIEW_BOUNCE_CIRC;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Circular Perimeter Bounce started (Multi-point 2D reflection)...");
	#endif
	return 1;
}

stock Start_Orbit(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Orbital Motion requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 320.0, 385.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~b~* ~w~ORBITAL PATH~b~ *");
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0x3498DBFF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		// Continuous trigonometric orbit (rx = 45.0, ry = 16.0, 90 deg/sec)
		OrbitPlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.centerX = 320.0, 
			.centerY = 385.0, 
			.radiusX = 45.0, 
			.radiusY = 16.0, 
			.angularSpeed = 90.0, 
			.startAngle = 0.0, 
			.duration = 10000, 
			.extra_id = EVENT_PREVIEW_ORBIT
		);

		gActivePreview[playerid] = PREVIEW_ORBIT;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Orbital Motion started (Continuous 90 deg/s rotation)...");
	#endif
	return 1;
}

stock Start_Shake(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Trauma Screen Shake requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 365.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "~r~CRITICAL IMPACT: TRAUMA SHAKE");
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xE74C3CFF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x2C0B0ECC);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		// Violent screen shake with quadratic trauma decay
		ShakePlayerTextDraw(
			playerid, 
			gTitleTD[playerid], 
			.intensity = 8.5, 
			.duration = 1800, 
			.extra_id = EVENT_PREVIEW_SHAKE
		);

		gActivePreview[playerid] = PREVIEW_SHAKE;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Trauma Screen Shake started (Nonlinear impact decay)...");
	#endif
	return 1;
}

stock Start_Neon(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 365.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "~p~> NEON NIGHTCLUB <~w~");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xBB33FFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x1B0033DD);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	// Stochastic fluorescent lamp malfunction (sparks, dropouts, settling glow)
	NeonFlickerPlayerTextDraw(
		playerid, 
		gTitleTD[playerid], 
		.duration = 4500, 
		.brokenRatio = 45, 
		.extra_id = EVENT_PREVIEW_NEON
	);

	gActivePreview[playerid] = PREVIEW_NEON;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Broken Neon Flicker started (Stochastic tube malfunction)...");
	return 1;
}

stock Start_Wave(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Waveform Floating Drift requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 365.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "~b~HARMONIC WAVE DRIFT");
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0x3498DBFF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x0B2545DD);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		// Smooth coupled dual-sine floating wave
		WaveDriftPlayerTextDraw(
			playerid, 
			gTitleTD[playerid], 
			.amplitudeX = 14.0, 
			.amplitudeY = 7.0, 
			.speedMs = 1600, 
			.duration = 8000, 
			.extra_id = EVENT_PREVIEW_WAVE
		);

		gActivePreview[playerid] = PREVIEW_WAVE;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Waveform Floating Drift started (Harmonic hover oscillation)...");
	#endif
	return 1;
}

stock Start_Progress(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 356.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "DOWNLOADING RESOURCES (0%)");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.36, 1.50);
	// In SA-MP Alignment 2 (Center): param 1 is line wrap width (500.0 prevents wrapping!), param 2 is box width (340.0)
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	// Synchronized numeric counter with safe parenthesis typography
	CountPlayerTextDrawNumber(
		playerid, 
		gTitleTD[playerid], 
		.startValue = 0, 
		.targetValue = 100, 
		.duration = 3200, 
		.prefix = "DOWNLOADING RESOURCES (", 
		.suffix = "%)"
	);

	// Dedicated progress bar (starts at X=150.0, expands to X=490.0, perfectly matching 340px title box)
	PlayerTextDrawTextSize(playerid, gProgressBarTD[playerid], 150.0, 14.0);
	PlayerTextDrawShow(playerid, gProgressBarTD[playerid]);

	// Native smooth progress bar expansion from X=150.0 to X=490.0 (width 340px)
	FillPlayerTextDrawProgress(
		playerid, 
		gProgressBarTD[playerid], 
		.startWidth = 150.0, 
		.targetWidth = 490.0, 
		.duration = 3200, 
		.easing = TDFX_EASE_OUT_QUAD, 
		.lineHeight = 14.0, 
		.extra_id = EVENT_PREVIEW_PROGRESS
	);

	gActivePreview[playerid] = PREVIEW_PROGRESS;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Dynamic Progress Fill started (Synchronized bar fill & counter)...");
	return 1;
}

stock Start_Glitch(playerid)
{
	#if defined PlayerTextDrawSetPos
		new Float:gx, Float:gy;
		if (TDFX_GetPlayerPos(playerid, gSubtitleTD[playerid], gx, gy))
		{
			PlayerTextDrawSetPos(playerid, gSubtitleTD[playerid], gx, gy);
		}
	#endif
	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0x00FF88FF);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x002211DD);
	PlayerTextDrawShow(playerid, gSubtitleTD[playerid]);

	// Real-time cyberpunk glyph distortion and micro-alpha fluctuations
	GlitchPlayerTextDraw(
		playerid, 
		gSubtitleTD[playerid], 
		"ACCESS GRANTED: TEXTDRAWFX SYSTEM CORE COMPROMISED (0x7F)", 
		.intensity = 3, 
		.duration = 4500, 
		.extra_id = EVENT_PREVIEW_GLITCH
	);

	gActivePreview[playerid] = PREVIEW_GLITCH;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Cyberpunk Chromatic Glitch started (Real-time glyph disturbance)...");
	return 1;
}

static stock gFwPattern[MAX_PLAYERS] = {0, ...};

stock Start_Firework(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Firework Text requires open.mp or the YSF plugin.");
	#else
		new currentPattern = gFwPattern[playerid];
		// Cycle patterns for successive tests: 0=Sphere, 1=Fan, 2=Rocket, 3=Ring, 4=Fountain
		gFwPattern[playerid] = (gFwPattern[playerid] + 1) % 5;

		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 320.0, 375.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~y~* ~r~DETONATING... ~y~*");
		PlayerTextDrawLetterSize(playerid, gSlideTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xE74C3CFF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 500.0, 260.0);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		FireworkPlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.pattern = currentPattern, 
			.fuseDuration = 1500, 
			.explodeDuration = 1300, 
			.shakeIntensity = 6.5, 
			.burstSpeed = 6.0, 
			.particleCount = 12, 
			.extra_id = EVENT_PREVIEW_FIREWORK
		);

		gActivePreview[playerid] = PREVIEW_FIREWORK;

		static const sFwNames[][] = {
			"Spherical 360 Radial Blast",
			"Upward Pyrotechnic Fan Spray",
			"Ascending Rocket Tail Blast",
			"Horizontal Shockwave Ring",
			"Volcano Fountain Cascade"
		};
		new msg[128];
		format(msg, sizeof msg, COLOR_TDFX_TAG "Firework Text started [Pattern: %s]...", sFwNames[currentPattern]);
		SendClientMessage(playerid, -1, msg);
	#endif
	return 1;
}


stock Start_Spin3D(playerid)
{
	PlayerTextDrawSetPreviewModel(playerid, gPreviewModelTD[playerid], 411);
	PlayerTextDrawSetPreviewRot(playerid, gPreviewModelTD[playerid], -15.0, 0.0, -45.0, 1.0);
	PlayerTextDrawShow(playerid, gPreviewModelTD[playerid]);
	Spin3DPlayerTextDraw(playerid, gPreviewModelTD[playerid], .speedZ = 50.0, .zoom = 1.0, .duration = 0, .extra_id = EVENT_PREVIEW_SPIN3D);
	gActivePreview[playerid] = PREVIEW_SPIN3D;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "3D Turntable Spin started (Smooth continuous 360 rotation)...");
	return 1;
}

stock Start_Hover3D(playerid)
{
	PlayerTextDrawSetPreviewModel(playerid, gPreviewModelTD[playerid], 1212);
	PlayerTextDrawSetPreviewRot(playerid, gPreviewModelTD[playerid], -10.0, 0.0, 0.0, 1.1);
	PlayerTextDrawShow(playerid, gPreviewModelTD[playerid]);
	Hover3DPlayerTextDraw(playerid, gPreviewModelTD[playerid], .tiltX = 8.0, .tiltZ = 12.0, .zoomAmp = 0.08, .speedMs = 2200, .duration = 0, .extra_id = EVENT_PREVIEW_HOVER3D);
	gActivePreview[playerid] = PREVIEW_HOVER3D;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "3D Floating Tilt started (Harmonic 3D hover & pitch)...");
	return 1;
}

stock Start_Shimmer(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "VIP PLATINUM MEMBER");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xF1C40FFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	ShimmerPlayerTextDraw(playerid, gTitleTD[playerid], "VIP PLATINUM MEMBER", "~y~", "~w~", .sweepDuration = 1100, .pauseDuration = 1400, .duration = 0, .extra_id = EVENT_PREVIEW_SHIMMER);
	gActivePreview[playerid] = PREVIEW_SHIMMER;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Golden Shimmer started (Sweeping chrome highlight ray)...");
	return 1;
}

stock Start_Rainbow(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "TEXTDRAWFX RAINBOW");
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	RainbowPlayerTextDraw(playerid, gTitleTD[playerid], "TEXTDRAWFX RAINBOW", .speedMs = 120, .duration = 6000, .extra_id = EVENT_PREVIEW_RAINBOW);
	gActivePreview[playerid] = PREVIEW_RAINBOW;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "RGB Rainbow Wave started (Flowing GTA chromatic spectrum)...");
	return 1;
}

stock Start_Slot(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "SPINNING REELS...");
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xF1C40FFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	SlotMachinePlayerTextDraw(playerid, gTitleTD[playerid], "$ 7 7 7 JACKPOT $", .spinDuration = 2400, .extra_id = EVENT_PREVIEW_SLOT);
	gActivePreview[playerid] = PREVIEW_SLOT;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Casino Slot Machine started (Mechanical rolling reel lock)...");
	return 1;
}

stock Start_Pendulum(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Pendulum Swing requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 320.0, 260.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "PENDULUM PHYSICS");
		PlayerTextDrawLetterSize(playerid, gSlideTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xF1C40FFF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
		PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 500.0, 260.0);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		PendulumPlayerTextDraw(playerid, gSlideTD[playerid], 320.0, 260.0, .armLength = 95.0, .maxAngle = 45.0, .damping = 0.55, .duration = 4200, .extra_id = EVENT_PREVIEW_PENDULUM);
		gActivePreview[playerid] = PREVIEW_PENDULUM;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Pendulum Swing started (Damped harmonic gravity swing)...");
	#endif
	return 1;
}

stock Start_Slam(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Gravity Drop requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 40.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "HEAVY SLAM IMPACT!");
		PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.48, 1.90);
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xE74C3CFF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 320.0);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		GravityDropPlayerTextDraw(playerid, gTitleTD[playerid], 40.0, 360.0, .elasticity = 0.40, .extra_id = EVENT_PREVIEW_SLAM);
		gActivePreview[playerid] = PREVIEW_SLAM;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Gravity Drop started (Freefall acceleration & elastic impact)...");
	#endif
	return 1;
}

stock Start_Vortex(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Black Hole Vortex requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 440.0, 360.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "BLACK HOLE VORTEX");
		PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0x9B59B6FF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 300.0);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		VortexPlayerTextDraw(playerid, gTitleTD[playerid], 320.0, 360.0, .startRadius = 150.0, .spins = 1.6, .duration = 4500, .extra_id = EVENT_PREVIEW_VORTEX);
		gActivePreview[playerid] = PREVIEW_VORTEX;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Black Hole Vortex started (Spiral suction & letter scale decay)...");
	#endif
	return 1;
}

stock Start_Letterbox(playerid)
{
	LetterboxPlayerTextDraw(playerid, gTopBarTD[playerid], gBottomBarTD[playerid], .barHeight = 65.0, .duration = 1500, .reverse = false, .extra_id = EVENT_PREVIEW_LETTERBOX);
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gSubtitleTD[playerid], 320.0, 340.0);
	#endif
	PlayerTextDrawSetString(playerid, gSubtitleTD[playerid], "CHAPTER 1: THE EVOLUTION OF TEXTDRAWFX");
	PlayerTextDrawLetterSize(playerid, gSubtitleTD[playerid], 0.32, 1.40);
	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, gSubtitleTD[playerid], true);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000BB);
	PlayerTextDrawTextSize(playerid, gSubtitleTD[playerid], 20.0, 520.0);
	PlayerTextDrawSetOutline(playerid, gSubtitleTD[playerid], 1);
	PlayerTextDrawShow(playerid, gSubtitleTD[playerid]);
	FadeInPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1500, .fadeBox = true, .targetAlpha = 255);

	gActivePreview[playerid] = PREVIEW_LETTERBOX;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Cinematic Letterbox started (21:9 Widescreen bars sliding in)...");
	return 1;
}

forward OnDemoCinematicHoldEnd(playerid);
public OnDemoCinematicHoldEnd(playerid)
{
	if (!IsPlayerConnected(playerid) || gActivePreview[playerid] != PREVIEW_LETTERBOX) return 0;
	FadeOutPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1200, .autoHide = true);
	LetterboxPlayerTextDraw(playerid, gTopBarTD[playerid], gBottomBarTD[playerid], .barHeight = 65.0, .duration = 1500, .reverse = true, .extra_id = EVENT_PREVIEW_LETTERBOX_EXIT);
	return 1;
}

stock Start_Sonar(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Sonar Echo Ping requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 320.0, 360.0);
		PlayerTextDrawSetString(playerid, gSlideTD[playerid], "(( RADAR PING ))");
		PlayerTextDrawLetterSize(playerid, gSlideTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gSlideTD[playerid], 0x2ECC71FF);
		PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gSlideTD[playerid], 500.0, 260.0);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);

		SonarPingPlayerTextDraw(playerid, gSlideTD[playerid], 320.0, 360.0, .maxRadius = 140.0, .pulseCount = 3, .pulseDuration = 1300, .extra_id = EVENT_PREVIEW_SONAR);
		gActivePreview[playerid] = PREVIEW_SONAR;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Sonar Echo Ping started (Submarine acoustic pulse & wave decay)...");
	#endif
	return 1;
}

stock Start_WaveSweep(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "TEXTDRAWFX WAVE CREST");
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0x3498DBFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	WaveSweepPlayerTextDraw(playerid, gTitleTD[playerid], "TEXTDRAWFX WAVE CREST", .crestTag = "~w~", .baseTag = "~b~", .crestWidth = 3, .direction = TDFX_DIR_LEFT_TO_RIGHT, .speedMs = 70, .duration = 6000, .extra_id = EVENT_PREVIEW_WAVESWEEP);
	gActivePreview[playerid] = PREVIEW_WAVESWEEP;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Traveling Wave Sweep started (Left -> Right wave ripple across letters)...");
	return 1;
}

stock Start_LetterRain(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Letter Rain requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "VICTORY ROYALE");
		PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xF1C40FFF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 320.0);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		LetterRainPlayerTextDraw(playerid, gTitleTD[playerid], "VICTORY ROYALE", 320.0, 360.0, .letterSpacing = 0.0, .duration = 3200, .extra_id = EVENT_PREVIEW_LETTERRAIN);
		gActivePreview[playerid] = PREVIEW_LETTERRAIN;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Falling Letter Rain started (Letters falling with gravity & landing bounce)...");
	#endif
	return 1;
}

stock Start_Scanner(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "CYLON SCANNER ONLINE");
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.42, 1.75);
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xE74C3CFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000CC);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	ScannerSweepPlayerTextDraw(playerid, gTitleTD[playerid], "CYLON SCANNER ONLINE", .laserTag = "~r~", .trailTag = "~y~", .speedMs = 65, .duration = 6000, .extra_id = EVENT_PREVIEW_SCANNER);
	gActivePreview[playerid] = PREVIEW_SCANNER;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Scanner Laser Sweep started (Knight Rider continuous ping-pong laser beam)...");
	return 1;
}

stock Start_Electric(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Electric Arc requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSubtitleTD[playerid], 320.0, 340.0);
		PlayerTextDrawSetString(playerid, gSubtitleTD[playerid], "HIGH VOLTAGE GRID OVERLOAD");
		PlayerTextDrawLetterSize(playerid, gSubtitleTD[playerid], 0.35, 1.45);
		PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0x00FFFFFF);
		PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x002233DD);
		PlayerTextDrawShow(playerid, gSubtitleTD[playerid]);

		ElectricArcPlayerTextDraw(playerid, gSubtitleTD[playerid], "HIGH VOLTAGE GRID OVERLOAD", .surgeDuration = 3200, .extra_id = EVENT_PREVIEW_ELECTRIC);
		gActivePreview[playerid] = PREVIEW_ELECTRIC;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Electric Arc Surge started (Sparks, voltage jitter & circuit settling)...");
	#endif
	return 1;
}

stock Start_SplitFlap(playerid)
{
	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
	#endif
	PlayerTextDrawSetString(playerid, gTitleTD[playerid], "FLIGHT TO LOS SANTOS");
	PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.40, 1.65);
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x111111EE);
	PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 350.0);
	PlayerTextDrawShow(playerid, gTitleTD[playerid]);

	SplitFlapPlayerTextDraw(playerid, gTitleTD[playerid], "FLIGHT TO LOS SANTOS", .charRollMs = 70, .lockIntervalMs = 150, .extra_id = EVENT_PREVIEW_SPLITFLAP);
	gActivePreview[playerid] = PREVIEW_SPLITFLAP;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Split-Flap Airport Board started (Vintage Solari mechanical letter flip)...");
	return 1;
}

stock Start_Hologram(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Hologram requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gSubtitleTD[playerid], 320.0, 340.0);
		PlayerTextDrawSetString(playerid, gSubtitleTD[playerid], "HOLO-COMMS: TRANSMISSION INCOMING");
		PlayerTextDrawLetterSize(playerid, gSubtitleTD[playerid], 0.35, 1.45);
		PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0x00FFFFFF);
		PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000BB);
		PlayerTextDrawShow(playerid, gSubtitleTD[playerid]);

		HologramPlayerTextDraw(playerid, gSubtitleTD[playerid], "HOLO-COMMS: TRANSMISSION INCOMING", .duration = 5500, .extra_id = EVENT_PREVIEW_HOLOGRAM);
		gActivePreview[playerid] = PREVIEW_HOLOGRAM;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Sci-Fi Hologram started (Cyan alpha modulation, scanlines & beam jitter)...");
	#endif
	return 1;
}

stock Start_Shockwave(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Shockwave Slam requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "IMPACT SLAM");
		PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.50, 2.0);
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xE74C3CFF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 280.0);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		ShockwaveSlamPlayerTextDraw(playerid, gTitleTD[playerid], 320.0, 360.0, .duration = 2000, .extra_id = EVENT_PREVIEW_SHOCKWAVE);
		gActivePreview[playerid] = PREVIEW_SHOCKWAVE;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Shockwave Impact Slam started (3D scale plummet, impact trauma & blast ring)...");
	#endif
	return 1;
}

stock Start_WaveMotion(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Wave Motion requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "OCEAN WAVE MOTION");
		PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0x3498DBFF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 340.0);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		WaveMotionPlayerTextDraw(playerid, gTitleTD[playerid], "OCEAN WAVE MOTION", 320.0, 360.0, .crestHeight = 15.0, .crestWidth = 2.8, .direction = TDFX_DIR_LEFT_TO_RIGHT, .duration = 3200, .extra_id = EVENT_PREVIEW_WAVEMOTION);
		gActivePreview[playerid] = PREVIEW_WAVEMOTION;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Sinusoidal Wave Motion started (Physical traveling arch crest across letters)...");
	#endif
	return 1;
}

stock Start_HopLetter(playerid)
{
	#if !defined PlayerTextDrawSetPos
		SendClientMessage(playerid, -1, COLOR_TDFX_WARN "Hop Letter requires open.mp or the YSF plugin.");
	#else
		PlayerTextDrawSetPos(playerid, gTitleTD[playerid], 320.0, 360.0);
		PlayerTextDrawSetString(playerid, gTitleTD[playerid], "BOUNCING HOP LETTERS");
		PlayerTextDrawLetterSize(playerid, gTitleTD[playerid], 0.45, 1.80);
		PlayerTextDrawColor(playerid, gTitleTD[playerid], 0x2ECC71FF);
		PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
		PlayerTextDrawTextSize(playerid, gTitleTD[playerid], 500.0, 360.0);
		PlayerTextDrawShow(playerid, gTitleTD[playerid]);

		HopLetterPlayerTextDraw(playerid, gTitleTD[playerid], "BOUNCING HOP LETTERS", 320.0, 360.0, .hopHeight = 16.0, .staggerMs = 90, .hopDurationMs = 340, .direction = TDFX_DIR_LEFT_TO_RIGHT, .extra_id = EVENT_PREVIEW_HOPLETTER);
		gActivePreview[playerid] = PREVIEW_HOPLETTER;
		SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Letter Hopping Cascade started (Staggered letter jump with elastic settle)...");
	#endif
	return 1;
}

stock Start_Global(playerid)
{
	TextDrawSetString(gBroadcastTD, "SERVER BROADCAST: TEXTDRAWFX GLOBAL ANIMATION TEST");
	TextDrawColor(gBroadcastTD, 0xFFFFFFFF);
	TextDrawBoxColor(gBroadcastTD, 0x1A5276DD);

	FadeInTextDraw(
		gBroadcastTD, 
		.duration = 1200, 
		.fadeBox = true, 
		.targetAlpha = 255, 
		.extra_id = EVENT_PREVIEW_GLOBAL_IN
	);

	gGlobalPreviewActive = true;
	gActivePreview[playerid] = PREVIEW_GLOBAL;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Global TextDraw broadcast started for all players!");
	return 1;
}

stock Start_Chain(playerid)
{
	PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~g~STAGE 1:~w~ MISSION INITIALIZED");
	PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);

	#if defined PlayerTextDrawSetPos
		PlayerTextDrawSetPos(playerid, gSlideTD[playerid], 140.0, 365.0);
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);
		SlidePlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.targetX = 320.0, 
			.targetY = 365.0, 
			.duration = 1000, 
			.easing = TDFX_EASE_OUT_QUAD, 
			.extra_id = EVENT_CHAIN_SLIDE
		);
	#else
		PlayerTextDrawShow(playerid, gSlideTD[playerid]);
		PulsePlayerTextDraw(
			playerid, 
			gSlideTD[playerid], 
			.amplitude = 0.20, 
			.interval = 600, 
			.cycles = 3, 
			.extra_id = EVENT_CHAIN_PULSE
		);
	#endif

	gActivePreview[playerid] = PREVIEW_CHAIN;
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Chained cutscene sequence started...");
	return 1;
}

// -----------------------------------------------------------------------------
// Lifecycle Callbacks Implementation
// -----------------------------------------------------------------------------

public OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id)
{
	switch (extra_id)
	{
		case EVENT_PREVIEW_FADE_IN:
		{
			PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
			FadeOutPlayerTextDraw(
				playerid, 
				gTitleTD[playerid], 
				.duration = 1200, 
				.extra_id = EVENT_PREVIEW_FADE_OUT, 
				.autoHide = true
			);
		}

		case EVENT_PREVIEW_FADE_OUT:
		{
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "True Alpha Fade animation completed.");
		}

		case EVENT_PREVIEW_COLOR:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_SCALE:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_PULSE:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_SLIDE:
		{
			FadeOutPlayerTextDraw(playerid, gSlideTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_COUNT:
		{
			FadeOutPlayerTextDraw(playerid, gCounterTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_SCRAMBLE:
		{
			FadeOutPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_BOUNCE:
		{
			FadeOutPlayerTextDraw(playerid, gSlideTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_BOUNCE_CIRC:
		{
			FadeOutPlayerTextDraw(playerid, gSlideTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_ORBIT:
		{
			FadeOutPlayerTextDraw(playerid, gSlideTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_SHAKE:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_NEON:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_WAVE:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_PROGRESS:
		{
			PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
			PlayerTextDrawSetString(playerid, gTitleTD[playerid], "~g~DOWNLOAD COMPLETE! (100%)");
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			FadeOutPlayerTextDraw(playerid, gProgressBarTD[playerid], .duration = 1200, .extra_id = 0, .autoHide = true);
		}

		case EVENT_PREVIEW_GLITCH:
		{
			FadeOutPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1000, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
		}

		case EVENT_PREVIEW_FIREWORK:
		{
			PlayerTextDrawHide(playerid, gSlideTD[playerid]);
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Firework explosion preview completed.");
		}

		case EVENT_PREVIEW_RAINBOW:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "RGB Rainbow Wave preview completed.");
		}

		case EVENT_PREVIEW_SLOT:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Casino Slot Machine jackpot lock completed!");
		}

		case EVENT_PREVIEW_PENDULUM:
		{
			FadeOutPlayerTextDraw(playerid, gSlideTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Pendulum swing settled at rest position.");
		}

		case EVENT_PREVIEW_SLAM:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Gravity Drop slam impact and bounces completed.");
		}

		case EVENT_PREVIEW_VORTEX:
		{
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Black Hole Vortex suction sequence completed.");
		}

		case EVENT_PREVIEW_LETTERBOX:
		{
			SetTimerEx("OnDemoCinematicHoldEnd", 4500, false, "i", playerid);
			SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Cinematic scene active (Displaying for 4.5s)...");
		}

		case EVENT_PREVIEW_LETTERBOX_EXIT:
		{
			PlayerTextDrawHide(playerid, gTopBarTD[playerid]);
			PlayerTextDrawHide(playerid, gBottomBarTD[playerid]);
			PlayerTextDrawHide(playerid, gSubtitleTD[playerid]);
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Cinematic Letterbox preview completed.");
		}

		case EVENT_PREVIEW_SONAR:
		{
			PlayerTextDrawHide(playerid, gSlideTD[playerid]);
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Sonar Echo Ping 3-wave cycle completed.");
		}

		case EVENT_PREVIEW_WAVESWEEP:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Traveling Wave Sweep preview completed.");
		}

		case EVENT_PREVIEW_LETTERRAIN:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1400, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Falling Letter Rain preview completed (Word assembled).");
		}

		case EVENT_PREVIEW_SCANNER:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Scanner Laser Sweep preview completed.");
		}

		case EVENT_PREVIEW_ELECTRIC:
		{
			FadeOutPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Electric Arc Surge preview completed.");
		}

		case EVENT_PREVIEW_SPLITFLAP:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1400, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Split-Flap mechanical board preview completed.");
		}

		case EVENT_PREVIEW_HOLOGRAM:
		{
			FadeOutPlayerTextDraw(playerid, gSubtitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Sci-Fi Hologram projection preview completed.");
		}

		case EVENT_PREVIEW_SHOCKWAVE:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Shockwave Impact Slam preview completed.");
		}

		case EVENT_PREVIEW_WAVEMOTION:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Sinusoidal Wave Motion preview completed.");
		}

		case EVENT_PREVIEW_HOPLETTER:
		{
			FadeOutPlayerTextDraw(playerid, gTitleTD[playerid], .duration = 1200, .extra_id = EVENT_AUTO_CLEANUP, .autoHide = true);
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Letter Hopping Cascade preview completed.");
		}

		case EVENT_AUTO_CLEANUP:
		{
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Preview completed and hidden.");
		}

		case EVENT_CHAIN_SLIDE:
		{
			PlayerTextDrawSetString(playerid, gSlideTD[playerid], "~y~STAGE 2:~w~ TARGET LOCKED!");
			PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);

			// Rhythmic lock-on pulse (650ms per pulse, 3 cycles = ~2 seconds readable display)
			PulsePlayerTextDraw(
				playerid, 
				gSlideTD[playerid], 
				.amplitude = 0.18, 
				.interval = 650, 
				.cycles = 3, 
				.extra_id = EVENT_CHAIN_PULSE
			);
		}

		case EVENT_CHAIN_PULSE:
		{
			PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);
			PlayerTextDrawSetString(playerid, gSlideTD[playerid], "$0 REWARD");
			PlayerTextDrawColor(playerid, gSlideTD[playerid], 0x2ECC71FF);
			PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);
			PlayerTextDrawShow(playerid, gSlideTD[playerid]);

			CountPlayerTextDrawNumber(
				playerid, 
				gSlideTD[playerid], 
				.startValue = 0, 
				.targetValue = 75000, 
				.duration = 1800, 
				.prefix = "$", 
				.suffix = " REWARD", 
				.thousandsSep = true, 
				.easing = TDFX_EASE_OUT_QUAD, 
				.extra_id = EVENT_CHAIN_COUNT
			);
		}

		case EVENT_CHAIN_COUNT:
		{
			PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

			FadeOutPlayerTextDraw(
				playerid, 
				gSlideTD[playerid], 
				.duration = 1500, 
				.extra_id = EVENT_CHAIN_FADE_OUT, 
				.autoHide = true
			);
		}

		case EVENT_CHAIN_FADE_OUT:
		{
			gActivePreview[playerid] = -1;
			SendClientMessage(playerid, -1, COLOR_TDFX_SUCCESS "Chained cutscene sequence completed successfully!");
		}
	}
	return 1;
}

public OnTextDrawFXFinish(Text:textid, fx_type, extra_id)
{
	if (extra_id == EVENT_PREVIEW_GLOBAL_IN)
	{
		FadeOutTextDraw(gBroadcastTD, .duration = 1500, .extra_id = EVENT_PREVIEW_GLOBAL_OUT, .autoHide = true);
	}
	else if (extra_id == EVENT_PREVIEW_GLOBAL_OUT)
	{
		gGlobalPreviewActive = false;
	}
	return 1;
}

// -----------------------------------------------------------------------------
// SA-MP / Open.MP Standard Callbacks
// -----------------------------------------------------------------------------

public OnFilterScriptInit()
{
	print("\n==================================================");
	print("  TextDrawFX v" TEXTDRAWFX_VERSION_STRING " - Demonstration Suite Loaded    ");
	print("  Use /tdfx or /txdfx to open interactive menu    ");
	print("==================================================\n");

	// Server-wide broadcast banner (Font 2, sapphire blue box) positioned at X=320.0, Y=365.0
	gBroadcastTD = TextDrawCreate(320.0, 365.0, "SERVER BROADCAST DEMO");
	TextDrawAlignment(gBroadcastTD, TDFX_DEMO_ALIGN_CENTER);
	TextDrawFont(gBroadcastTD, TDFX_DEMO_FONT_2);
	TextDrawLetterSize(gBroadcastTD, 0.45, 1.80);
	TextDrawColor(gBroadcastTD, 0xFFFFFFFF);
	TextDrawSetOutline(gBroadcastTD, 1);
	TextDrawSetShadow(gBroadcastTD, 0);
	TextDrawUseBox(gBroadcastTD, true);
	TextDrawBoxColor(gBroadcastTD, 0x1A5276DD);
	TextDrawTextSize(gBroadcastTD, 22.0, 500.0);

	#if defined foreach
		foreach (new i : Player)
		{
			InitPlayerDemoTextDraws(i);
			SendWelcomeMessage(i);
		}
	#else
		for (new i = 0; i < MAX_PLAYERS; i++)
		{
			if (IsPlayerConnected(i))
			{
				InitPlayerDemoTextDraws(i);
				SendWelcomeMessage(i);
			}
		}
	#endif
	return 1;
}

public OnFilterScriptExit()
{
	if (gBroadcastTD != Text:INVALID_TEXT_DRAW)
	{
		TextDrawDestroy(gBroadcastTD);
		gBroadcastTD = Text:INVALID_TEXT_DRAW;
	}

	#if defined foreach
		foreach (new i : Player)
		{
			DestroyPlayerDemoTextDraws(i);
		}
	#else
		for (new i = 0; i < MAX_PLAYERS; i++)
		{
			if (IsPlayerConnected(i)) DestroyPlayerDemoTextDraws(i);
		}
	#endif

	print("[TextDrawFX] Demonstration suite unloaded safely.");
	return 1;
}

stock SendWelcomeMessage(playerid)
{
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Welcome to {3498DB}TextDrawFX v" TEXTDRAWFX_VERSION_STRING "{FFFFFF}!");
	SendClientMessage(playerid, -1, COLOR_TDFX_TAG "Type {F1C40F}/tdfx{FFFFFF} or {F1C40F}/txdfx{FFFFFF} to open the visual animations showcase menu.");
	return 1;
}

public OnPlayerConnect(playerid)
{
	InitPlayerDemoTextDraws(playerid);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	SendWelcomeMessage(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	DestroyPlayerDemoTextDraws(playerid);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == DIALOG_TDFX_MENU)
	{
		if (!response) return 1;

		if (listitem == PREVIEW_TOTAL) // [Stop Active Effect]
		{
			StopCurrentPreview(playerid, true);
			if (gGlobalPreviewActive)
			{
				StopTextDrawFX(gBroadcastTD);
				TextDrawHideForAll(gBroadcastTD);
				gGlobalPreviewActive = false;
			}
			ShowTDFXMenu(playerid);
			return 1;
		}

		// If clicking the currently active preview -> Toggle off
		if (gActivePreview[playerid] == listitem)
		{
			StopCurrentPreview(playerid, true);
			ShowTDFXMenu(playerid);
			return 1;
		}

		// If activating a new preview while another is running -> Stop the previous one first
		StopCurrentPreview(playerid, false);

		// Start the selected preview
		switch (listitem)
		{
			case PREVIEW_FADE:        Start_Fade(playerid);
			case PREVIEW_COLOR:       Start_Color(playerid);
			case PREVIEW_SCALE:       Start_Scale(playerid);
			case PREVIEW_PULSE:       Start_Pulse(playerid);
			case PREVIEW_SLIDE:       Start_Slide(playerid);
			case PREVIEW_COUNT:       Start_Count(playerid);
			case PREVIEW_SCRAMBLE:    Start_Scramble(playerid);
			case PREVIEW_BLINK:       Start_Blink(playerid);
			case PREVIEW_TYPEWRITER:  Start_Typewriter(playerid);
			case PREVIEW_BOUNCE:      Start_Bounce(playerid);
			case PREVIEW_BOUNCE_CIRC: Start_BounceCircle(playerid);
			case PREVIEW_ORBIT:       Start_Orbit(playerid);
			case PREVIEW_SHAKE:       Start_Shake(playerid);
			case PREVIEW_NEON:        Start_Neon(playerid);
			case PREVIEW_WAVE:        Start_Wave(playerid);
			case PREVIEW_PROGRESS:    Start_Progress(playerid);
			case PREVIEW_GLITCH:      Start_Glitch(playerid);
			case PREVIEW_FIREWORK:    Start_Firework(playerid);
			case PREVIEW_SPIN3D:      Start_Spin3D(playerid);
			case PREVIEW_HOVER3D:     Start_Hover3D(playerid);
			case PREVIEW_SHIMMER:     Start_Shimmer(playerid);
			case PREVIEW_RAINBOW:     Start_Rainbow(playerid);
			case PREVIEW_SLOT:        Start_Slot(playerid);
			case PREVIEW_PENDULUM:    Start_Pendulum(playerid);
			case PREVIEW_SLAM:        Start_Slam(playerid);
			case PREVIEW_VORTEX:      Start_Vortex(playerid);
			case PREVIEW_LETTERBOX:   Start_Letterbox(playerid);
			case PREVIEW_SONAR:       Start_Sonar(playerid);
			case PREVIEW_WAVESWEEP:   Start_WaveSweep(playerid);
			case PREVIEW_LETTERRAIN:  Start_LetterRain(playerid);
			case PREVIEW_SCANNER:     Start_Scanner(playerid);
			case PREVIEW_ELECTRIC:    Start_Electric(playerid);
			case PREVIEW_SPLITFLAP:   Start_SplitFlap(playerid);
			case PREVIEW_HOLOGRAM:    Start_Hologram(playerid);
			case PREVIEW_SHOCKWAVE:   Start_Shockwave(playerid);
			case PREVIEW_WAVEMOTION:  Start_WaveMotion(playerid);
			case PREVIEW_HOPLETTER:   Start_HopLetter(playerid);
			case PREVIEW_GLOBAL:      Start_Global(playerid);
			case PREVIEW_CHAIN:       Start_Chain(playerid);
		}

		// Reopen menu to immediately reflect the updated (Active) tag
		ShowTDFXMenu(playerid);
		return 1;
	}
	return 0;
}

// -----------------------------------------------------------------------------
// Standard SA-MP Command Handling
// -----------------------------------------------------------------------------

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (!strcmp(cmdtext, "/tdfx", true) || !strcmp(cmdtext, "/txdfx", true))
	{
		ShowTDFXMenu(playerid);
		return 1;
	}
	return 0;
}

