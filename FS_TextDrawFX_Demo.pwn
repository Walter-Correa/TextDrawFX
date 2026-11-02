/*
================================================================================
    TextDrawFX - Interactive Demonstration Suite (Filterscript)
    A complete in-game interactive testing suite for TextDrawFX visual effects,
    animations, and transitions on SA-MP and Open.MP.

    Author:      Walter Correa
    Version:     2.0.0
    Repository:  https://github.com/Walter-Correa/TextDrawFX
    License:     Mozilla Public License 2.0 (MPL-2.0)

    Dependencies:
    - TextDrawFX (TextDrawFX.inc v2.0.0)
    - Pawn.CMD by urShadow (https://github.com/urShadow/Pawn.CMD)
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

#include <Pawn.CMD>
#include "TextDrawFX.inc"

// -----------------------------------------------------------------------------
// Cross-Platform Compatibility Macros
// -----------------------------------------------------------------------------

#if defined _INC_open_mp
	#define TDFX_DEMO_FONT_1       TEXT_DRAW_FONT_1
	#define TDFX_DEMO_FONT_2       TEXT_DRAW_FONT_2
	#define TDFX_DEMO_FONT_3       TEXT_DRAW_FONT_3
	#define TDFX_DEMO_ALIGN_CENTER TEXT_DRAW_ALIGN_CENTER
	#define TDFX_DEMO_DLG_STYLE    DIALOG_STYLE:
#else
	#define TDFX_DEMO_FONT_1       (1)
	#define TDFX_DEMO_FONT_2       (2)
	#define TDFX_DEMO_FONT_3       (3)
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
	gSubtitleTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 365.0, "System ready. Select an effect from /tdfx.");
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
}

// -----------------------------------------------------------------------------
// Interactive Dialog Menu
// -----------------------------------------------------------------------------

stock ShowTDFXMenu(playerid)
{
	new current = gActivePreview[playerid];
	new dialogContent[1400];
	format(dialogContent, sizeof dialogContent,
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
		{FFFFFF}Global Broadcast\tServer-Wide Announcement TD\t%s\n\
		{FFFFFF}Chained Cutscene\tAutomated 4-Stage Sequence\t%s\n\
		{E74C3C}[Stop Active Effect]\tImmediately reset screen\t-",
		(current == PREVIEW_FADE)       ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_COLOR)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SCALE)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_PULSE)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SLIDE)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_COUNT)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_SCRAMBLE)   ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_BLINK)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_TYPEWRITER) ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		gGlobalPreviewActive            ? ("{2ECC71}(Active)") : ("{7F8C8D}-"),
		(current == PREVIEW_CHAIN)      ? ("{2ECC71}(Active)") : ("{7F8C8D}-")
	);

	ShowPlayerDialog(
		playerid, 
		DIALOG_TDFX_MENU, 
		TDFX_DEMO_DLG_STYLE TDFX_DLG_TABLIST, 
		"{3498DB}TextDrawFX {FFFFFF}v2.0 - Demonstration Suite", 
		dialogContent, 
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
	if (current == -1 && !gGlobalPreviewActive) return 0;

	StopPlayerTextDrawFX(playerid, gTitleTD[playerid]);
	StopPlayerTextDrawFX(playerid, gCounterTD[playerid]);
	StopPlayerTextDrawFX(playerid, gSubtitleTD[playerid]);
	StopPlayerTextDrawFX(playerid, gSlideTD[playerid]);

	PlayerTextDrawHide(playerid, gTitleTD[playerid]);
	PlayerTextDrawHide(playerid, gCounterTD[playerid]);
	PlayerTextDrawHide(playerid, gSubtitleTD[playerid]);
	PlayerTextDrawHide(playerid, gSlideTD[playerid]);

	// Restore base colors for future shows
	PlayerTextDrawColor(playerid, gTitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gTitleTD[playerid], 0x000000BB);
	PlayerTextDrawColor(playerid, gCounterTD[playerid], 0x2ECC71FF);
	PlayerTextDrawBoxColor(playerid, gCounterTD[playerid], 0x000000AA);
	PlayerTextDrawColor(playerid, gSubtitleTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSubtitleTD[playerid], 0x000000CC);
	PlayerTextDrawColor(playerid, gSlideTD[playerid], 0xFFFFFFFF);
	PlayerTextDrawBoxColor(playerid, gSlideTD[playerid], 0x111111DD);

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
		"ACCESS GRANTED: TEXTDRAWFX v2.0 SYSTEM CORE INITIALIZED", 
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

	BlinkPlayerTextDraw(
		playerid, 
		gSubtitleTD[playerid], 
		.interval = 50, 
		.duration = 4000, 
		.perChar = true, 
		.colour = 0, 
		.text = "~y~Carl Johnson:~w~ Grove Street... home. At least it was before I messed everything up."
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
	print("  TextDrawFX v2.0 - Demonstration Suite Loaded    ");
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
		}
	#else
		for (new i = 0; i < MAX_PLAYERS; i++)
		{
			if (IsPlayerConnected(i)) InitPlayerDemoTextDraws(i);
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

public OnPlayerConnect(playerid)
{
	InitPlayerDemoTextDraws(playerid);
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

		if (listitem == 11) // [Stop Active Effect]
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
			case PREVIEW_FADE:       Start_Fade(playerid);
			case PREVIEW_COLOR:      Start_Color(playerid);
			case PREVIEW_SCALE:      Start_Scale(playerid);
			case PREVIEW_PULSE:      Start_Pulse(playerid);
			case PREVIEW_SLIDE:      Start_Slide(playerid);
			case PREVIEW_COUNT:      Start_Count(playerid);
			case PREVIEW_SCRAMBLE:   Start_Scramble(playerid);
			case PREVIEW_BLINK:      Start_Blink(playerid);
			case PREVIEW_TYPEWRITER: Start_Typewriter(playerid);
			case PREVIEW_GLOBAL:     Start_Global(playerid);
			case PREVIEW_CHAIN:      Start_Chain(playerid);
		}

		// Reopen menu to immediately reflect the updated (Active) tag
		ShowTDFXMenu(playerid);
		return 1;
	}
	return 0;
}

// -----------------------------------------------------------------------------
// Pawn.CMD Commands
// -----------------------------------------------------------------------------

cmd:tdfx(playerid, params[])
{
	#pragma unused params
	ShowTDFXMenu(playerid);
	return 1;
}
alias:tdfx("txdfx")
