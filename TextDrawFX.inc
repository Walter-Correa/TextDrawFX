/*
================================================================================
    TextDrawFX - A high-performance textdraw visual effects, animation, and
    blinking library for SA-MP and Open.MP.

    Author:      Walter Corrêa
    Version:     2.1.0
    Repository:  https://github.com/Walter-Correa/TextDrawFX
    License:     Mozilla Public License 2.0 (MPL-2.0)
================================================================================
*/

#if defined _textdrawfx_included
	#endinput
#endif
#define _textdrawfx_included

#if !defined _samp_included && !defined _INC_open_mp
	#if __Pawn >= 0x030A
		#tryinclude <open.mp>
	#endif
	#if !defined _INC_open_mp
		#include <a_samp>
	#endif
#endif

// -----------------------------------------------------------------------------
// Constants & Configuration
// -----------------------------------------------------------------------------

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

#define TDFX_EASE_LINEAR        (0)
#define TDFX_EASE_IN_QUAD       (1)
#define TDFX_EASE_OUT_QUAD      (2)
#define TDFX_EASE_IN_OUT_QUAD   (3)
#define TDFX_EASE_OUT_BOUNCE    (4)

#if !defined TDFX_MAX_FX
	#define TDFX_MAX_FX (64)
#endif

#if !defined TDFX_MAX_BLINKS
	#define TDFX_MAX_BLINKS TDFX_MAX_FX
#endif

#if !defined TDFX_MAX_TEXT
	#define TDFX_MAX_TEXT (256)
#endif

#if !defined TDFX_MAX_TD
	#if defined MAX_PLAYER_TEXT_DRAWS
		#define TDFX_MAX_TD (_:MAX_PLAYER_TEXT_DRAWS)
	#else
		#define TDFX_MAX_TD (256)
	#endif
#endif

#if !defined TDFX_MAX_GLOBAL_TD
	#if defined MAX_TEXT_DRAWS
		#define TDFX_MAX_GLOBAL_TD (_:MAX_TEXT_DRAWS)
	#else
		#define TDFX_MAX_GLOBAL_TD (2048)
	#endif
#endif

#if !defined TDFX_MIN_INTERVAL
	#define TDFX_MIN_INTERVAL (30)
#endif

#if !defined TDFX_FRAME_INTERVAL
	#define TDFX_FRAME_INTERVAL (40)
#endif

// Non-deprecated canonical mappings for open.mp vs SA-MP
#if defined PlayerTextDrawColour
	#define _TDFX_PTDColour PlayerTextDrawColour
#else
	#define _TDFX_PTDColour PlayerTextDrawColor
#endif

#if defined PlayerTextDrawBoxColour
	#define _TDFX_PTDBoxColour PlayerTextDrawBoxColour
#else
	#define _TDFX_PTDBoxColour PlayerTextDrawBoxColor
#endif

#if defined PlayerTextDrawBackgroundColour
	#define _TDFX_PTDBackColour PlayerTextDrawBackgroundColour
#else
	#define _TDFX_PTDBackColour PlayerTextDrawBackgroundColor
#endif

#if defined PlayerTextDrawGetColour
	#define _TDFX_PTDGetColour PlayerTextDrawGetColour
#elseif defined PlayerTextDrawGetColor
	#define _TDFX_PTDGetColour PlayerTextDrawGetColor
#endif

#if defined PlayerTextDrawGetBoxColour
	#define _TDFX_PTDGetBoxColour PlayerTextDrawGetBoxColour
#elseif defined PlayerTextDrawGetBoxColor
	#define _TDFX_PTDGetBoxColour PlayerTextDrawGetBoxColor
#endif

#if defined PlayerTextDrawGetBackgroundColour
	#define _TDFX_PTDGetBackColour PlayerTextDrawGetBackgroundColour
#elseif defined PlayerTextDrawGetBackgroundColor
	#define _TDFX_PTDGetBackColour PlayerTextDrawGetBackgroundColor
#endif

#if defined TextDrawColour
	#define _TDFX_TDColour TextDrawColour
#else
	#define _TDFX_TDColour TextDrawColor
#endif

#if defined TextDrawBoxColour
	#define _TDFX_TDBoxColour TextDrawBoxColour
#else
	#define _TDFX_TDBoxColour TextDrawBoxColor
#endif

#if defined TextDrawBackgroundColour
	#define _TDFX_TDBackColour TextDrawBackgroundColour
#else
	#define _TDFX_TDBackColour TextDrawBackgroundColor
#endif

#if defined TextDrawGetColour
	#define _TDFX_TDGetColour TextDrawGetColour
#elseif defined TextDrawGetColor
	#define _TDFX_TDGetColour TextDrawGetColor
#endif

#if defined TextDrawGetBoxColour
	#define _TDFX_TDGetBoxColour TextDrawGetBoxColour
#elseif defined TextDrawGetBoxColor
	#define _TDFX_TDGetBoxColour TextDrawGetBoxColor
#endif

#if defined TextDrawGetBackgroundColour
	#define _TDFX_TDGetBackColour TextDrawGetBackgroundColour
#elseif defined TextDrawGetBackgroundColor
	#define _TDFX_TDGetBackColour TextDrawGetBackgroundColor
#endif

// -----------------------------------------------------------------------------
// State Storage
// -----------------------------------------------------------------------------

// PlayerTextDraw fallback caches for SA-MP 0.3.7
static stock TDFX_Info[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemText[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemBox[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemBack[MAX_PLAYERS][TDFX_MAX_TD];
static stock Float:TDFX_RemLetX[MAX_PLAYERS][TDFX_MAX_TD];
static stock Float:TDFX_RemLetY[MAX_PLAYERS][TDFX_MAX_TD];
static stock Float:TDFX_RemPosX[MAX_PLAYERS][TDFX_MAX_TD];
static stock Float:TDFX_RemPosY[MAX_PLAYERS][TDFX_MAX_TD];
static stock Float:TDFX_RemSizeX[MAX_PLAYERS][TDFX_MAX_TD];
static stock Float:TDFX_RemSizeY[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemAlign[MAX_PLAYERS][TDFX_MAX_TD];

// Global TextDraw fallback caches for SA-MP 0.3.7
#if TDFX_MAX_GLOBAL_TD > 0
static stock TDFX_GInfo[TDFX_MAX_GLOBAL_TD];
static stock TDFX_GRemText[TDFX_MAX_GLOBAL_TD];
static stock TDFX_GRemBox[TDFX_MAX_GLOBAL_TD];
static stock TDFX_GRemBack[TDFX_MAX_GLOBAL_TD];
static stock Float:TDFX_GRemLetX[TDFX_MAX_GLOBAL_TD];
static stock Float:TDFX_GRemLetY[TDFX_MAX_GLOBAL_TD];
static stock Float:TDFX_GRemPosX[TDFX_MAX_GLOBAL_TD];
static stock Float:TDFX_GRemPosY[TDFX_MAX_GLOBAL_TD];
static stock Float:TDFX_GRemSizeX[TDFX_MAX_GLOBAL_TD];
static stock Float:TDFX_GRemSizeY[TDFX_MAX_GLOBAL_TD];
static stock TDFX_GRemAlign[TDFX_MAX_GLOBAL_TD];
#endif

// Slot pool for active animations
static stock bool:TDFX_Used[TDFX_MAX_FX];
static stock bool:TDFX_IsGlobal[TDFX_MAX_FX];
static stock TDFX_Type[TDFX_MAX_FX];
static stock TDFX_Player[TDFX_MAX_FX];
static stock TDFX_Td[TDFX_MAX_FX];
static stock TDFX_Timer[TDFX_MAX_FX];
static stock TDFX_Start[TDFX_MAX_FX];
static stock TDFX_Duration[TDFX_MAX_FX];
static stock TDFX_Step[TDFX_MAX_FX];
static stock TDFX_Count[TDFX_MAX_FX];
static stock TDFX_Easing[TDFX_MAX_FX];
static stock TDFX_ExtraId[TDFX_MAX_FX];
static stock TDFX_Callback[TDFX_MAX_FX][32];
static stock bool:TDFX_AutoHide[TDFX_MAX_FX];
static stock bool:TDFX_AutoDestroy[TDFX_MAX_FX];
static stock bool:TDFX_Loop[TDFX_MAX_FX];
static stock bool:TDFX_Preview[TDFX_MAX_FX];

// Color & Alpha parameters
static stock TDFX_Colour[TDFX_MAX_FX];
static stock TDFX_Base[TDFX_MAX_FX];
static stock TDFX_BoxBase[TDFX_MAX_FX];
static stock TDFX_ColFrom[TDFX_MAX_FX];
static stock TDFX_ColTo[TDFX_MAX_FX];
static stock TDFX_BoxColFrom[TDFX_MAX_FX];
static stock TDFX_BoxColTo[TDFX_MAX_FX];
static stock bool:TDFX_FadeBox[TDFX_MAX_FX];

// Geometry parameters (Scale, Pulse, Slide, Orbit, Bounce)
static stock Float:TDFX_FloatA[TDFX_MAX_FX];
static stock Float:TDFX_FloatB[TDFX_MAX_FX];
static stock Float:TDFX_FloatC[TDFX_MAX_FX];
static stock Float:TDFX_FloatD[TDFX_MAX_FX];

// Physical & Arena Boundary parameters (Bounce, Orbit)
static stock Float:TDFX_BoundMinX[TDFX_MAX_FX];
static stock Float:TDFX_BoundMinY[TDFX_MAX_FX];
static stock Float:TDFX_BoundMaxX[TDFX_MAX_FX];
static stock Float:TDFX_BoundMaxY[TDFX_MAX_FX];
static stock bool:TDFX_ColorShift[TDFX_MAX_FX];

// Classic DVD screensaver color palette
static stock const TDFX_DvdPalette[] = {
	0xFF3333FF, // Red
	0x33FF33FF, // Green
	0x3388FFFF, // Light Blue
	0xFFFF33FF, // Yellow
	0xFF33FFFF, // Magenta
	0x33FFFFFF, // Cyan
	0xFF8833FF, // Orange
	0xAA33FFFF  // Purple
};

// Text parameters (Typewriter, Scramble, Count)
static stock bool:TDFX_PerChar[TDFX_MAX_FX];
static stock TDFX_Text[TDFX_MAX_FX][TDFX_MAX_TEXT];
static stock TDFX_Prefix[TDFX_MAX_FX][16];
static stock TDFX_Suffix[TDFX_MAX_FX][16];
static stock bool:TDFX_ThousandSep[TDFX_MAX_FX];

// -----------------------------------------------------------------------------
// Math & Helper Functions
// -----------------------------------------------------------------------------

static stock Float:TDFX_ApplyEasing(Float:t, easing)
{
	if (t <= 0.0) return 0.0;
	if (t >= 1.0) return 1.0;
	switch (easing)
	{
		case TDFX_EASE_IN_QUAD:
			return t * t;
		case TDFX_EASE_OUT_QUAD:
			return t * (2.0 - t);
		case TDFX_EASE_IN_OUT_QUAD:
			return (t < 0.5) ? (2.0 * t * t) : (-1.0 + (4.0 - 2.0 * t) * t);
		case TDFX_EASE_OUT_BOUNCE:
		{
			if (t < (1.0 / 2.75))
				return 7.5625 * t * t;
			else if (t < (2.0 / 2.75))
			{
				t -= (1.5 / 2.75);
				return 7.5625 * t * t + 0.75;
			}
			else if (t < (2.5 / 2.75))
			{
				t -= (2.25 / 2.75);
				return 7.5625 * t * t + 0.9375;
			}
			else
			{
				t -= (2.625 / 2.75);
				return 7.5625 * t * t + 0.984375;
			}
		}
	}
	return t;
}

static stock TDFX_LerpColor(c1, c2, Float:t)
{
	if (t <= 0.0) return c1;
	if (t >= 1.0) return c2;
	new r1 = (c1 >>> 24) & 0xFF, g1 = (c1 >>> 16) & 0xFF, b1 = (c1 >>> 8) & 0xFF, a1 = c1 & 0xFF;
	new r2 = (c2 >>> 24) & 0xFF, g2 = (c2 >>> 16) & 0xFF, b2 = (c2 >>> 8) & 0xFF, a2 = c2 & 0xFF;
	new r = r1 + floatround(float(r2 - r1) * t);
	new g = g1 + floatround(float(g2 - g1) * t);
	new b = b1 + floatround(float(b2 - b1) * t);
	new a = a1 + floatround(float(a2 - a1) * t);
	return (r << 24) | (g << 16) | (b << 8) | a;
}

static stock TDFX_FormatNumber(dest[], maxlen, val, const prefix[], const suffix[], bool:sep)
{
	new raw[32], numStr[48];
	valstr(raw, val < 0 ? -val : val);
	if (sep)
	{
		new len = strlen(raw), outIdx = 0;
		if (val < 0) numStr[outIdx++] = '-';
		for (new i = 0; i < len; i++)
		{
			if (i > 0 && ((len - i) % 3) == 0) numStr[outIdx++] = ',';
			numStr[outIdx++] = raw[i];
		}
		numStr[outIdx] = 0;
	}
	else
	{
		format(numStr, sizeof numStr, "%d", val);
	}
	format(dest, maxlen, "%s%s%s", prefix, numStr, suffix);
}

static stock TDFX_SkipTag(const text[], i)
{
	new j = i + 1;
	while (text[j] && text[j] != '~') j++;
	if (text[j] != '~') return j;
	new bool:key = (j == i + 2 && (text[i + 1] == 'k' || text[i + 1] == 'K'));
	j++;
	if (key && text[j] == '~')
	{
		j++;
		while (text[j] && text[j] != '~') j++;
		if (text[j] == '~') j++;
	}
	return j;
}

static stock TDFX_CountChars(const text[])
{
	new i, seen;
	while (text[i])
	{
		if (text[i] == '~')
		{
			i = TDFX_SkipTag(text, i);
			continue;
		}
		if (text[i] == ' ' || text[i] == '_')
		{
			i++;
			continue;
		}
		i++;
		seen++;
	}
	return seen;
}

static stock TDFX_Cut(const text[], chars)
{
	if (chars <= 0) return 0;
	new i, seen;
	while (text[i])
	{
		if (text[i] == '~')
		{
			i = TDFX_SkipTag(text, i);
			continue;
		}
		if (text[i] == ' ' || text[i] == '_')
		{
			i++;
			continue;
		}
		i++;
		seen++;
		if (seen == chars) return i;
	}
	return i;
}

// -----------------------------------------------------------------------------
// Slot Management
// -----------------------------------------------------------------------------

static stock TDFX_FindPlayer(playerid, PlayerText:textid)
{
	for (new i = 0; i < TDFX_MAX_FX; i++)
	{
		if (TDFX_Used[i] && !TDFX_IsGlobal[i] && TDFX_Player[i] == playerid && TDFX_Td[i] == _:textid) return i;
	}
	return -1;
}

static stock TDFX_FindGlobal(Text:textid)
{
	for (new i = 0; i < TDFX_MAX_FX; i++)
	{
		if (TDFX_Used[i] && TDFX_IsGlobal[i] && TDFX_Td[i] == _:textid) return i;
	}
	return -1;
}

static stock TDFX_Alloc()
{
	for (new i = 0; i < TDFX_MAX_FX; i++)
	{
		if (!TDFX_Used[i]) return i;
	}
	return -1;
}

static stock bool:TDFX_ValidPlayer(playerid, PlayerText:textid)
{
	return (0 <= playerid < MAX_PLAYERS && 0 <= _:textid < TDFX_MAX_TD);
}

static stock bool:TDFX_ValidGlobal(Text:textid)
{
	return (0 <= _:textid < TDFX_MAX_GLOBAL_TD);
}

static stock bool:TDFX_GetPlayerPos(playerid, PlayerText:textid, &Float:x, &Float:y)
{
	x = 0.0;
	y = 0.0;
	#if defined PlayerTextDrawGetPos
		PlayerTextDrawGetPos(playerid, textid, x, y);
		if (x != 0.0 || y != 0.0) return true;
	#endif
	if (TDFX_ValidPlayer(playerid, textid))
	{
		x = TDFX_RemPosX[playerid][_:textid];
		y = TDFX_RemPosY[playerid][_:textid];
		return true;
	}
	return false;
}

static stock bool:TDFX_GetGlobalPos(Text:textid, &Float:x, &Float:y)
{
	x = 0.0;
	y = 0.0;
	#if defined TextDrawGetPos
		TextDrawGetPos(textid, x, y);
		if (x != 0.0 || y != 0.0) return true;
	#endif
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			x = TDFX_GRemPosX[_:textid];
			y = TDFX_GRemPosY[_:textid];
			return true;
		}
	#endif
	return false;
}

static stock TDFX_Free(slot, bool:restore, bool:triggerFinish = false)
{
	if (slot < 0 || slot >= TDFX_MAX_FX || !TDFX_Used[slot]) return;
	KillTimer(TDFX_Timer[slot]);
	TDFX_Used[slot] = false;

	new bool:isGlobal = TDFX_IsGlobal[slot];
	new playerid = TDFX_Player[slot];
	new tdId = TDFX_Td[slot];
	new fxType = TDFX_Type[slot];
	new extraId = TDFX_ExtraId[slot];
	new cb[32];
	strmid(cb, TDFX_Callback[slot], 0, 32, 32);

	if (restore)
	{
		if (isGlobal)
		{
			if (TDFX_AutoHide[slot])
			{
				if ((fxType == TDFX_TYPE_FADE || fxType == TDFX_TYPE_NEON) && TDFX_Base[slot])
				{
					_TDFX_TDColour(Text:tdId, TDFX_Base[slot]);
					if (TDFX_BoxBase[slot]) _TDFX_TDBoxColour(Text:tdId, TDFX_BoxBase[slot]);
				}
				else if (fxType == TDFX_TYPE_SCALE || fxType == TDFX_TYPE_PULSE)
				{
					TextDrawLetterSize(Text:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
				}
				#if defined TextDrawSetPos
				else if (fxType == TDFX_TYPE_SHAKE || fxType == TDFX_TYPE_WAVE)
				{
					TextDrawSetPos(Text:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
				}
				#endif
				else if (fxType == TDFX_TYPE_GLITCH && TDFX_Base[slot])
				{
					_TDFX_TDColour(Text:tdId, TDFX_Base[slot]);
					TextDrawSetString(Text:tdId, TDFX_Text[slot]);
				}
				TextDrawHideForAll(Text:tdId);
			}
			else if (TDFX_AutoDestroy[slot]) TextDrawDestroy(Text:tdId);
			else
			{
				if (fxType == TDFX_TYPE_SCALE || fxType == TDFX_TYPE_PULSE)
				{
					TextDrawLetterSize(Text:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
					TextDrawShowForAll(Text:tdId);
				}
				else if ((fxType == TDFX_TYPE_FADE || fxType == TDFX_TYPE_NEON) && TDFX_Base[slot])
				{
					_TDFX_TDColour(Text:tdId, TDFX_Base[slot]);
					if (TDFX_BoxBase[slot]) _TDFX_TDBoxColour(Text:tdId, TDFX_BoxBase[slot]);
					TextDrawShowForAll(Text:tdId);
				}
				else if (fxType == TDFX_TYPE_BLINK && TDFX_Colour[slot])
				{
					_TDFX_TDColour(Text:tdId, TDFX_Base[slot]);
					if (TDFX_BoxBase[slot]) _TDFX_TDBoxColour(Text:tdId, TDFX_BoxBase[slot]);
					TextDrawShowForAll(Text:tdId);
				}
				else if (fxType == TDFX_TYPE_TYPEWRITER || fxType == TDFX_TYPE_SCRAMBLE || fxType == TDFX_TYPE_GLITCH)
				{
					if (fxType == TDFX_TYPE_GLITCH && TDFX_Base[slot]) _TDFX_TDColour(Text:tdId, TDFX_Base[slot]);
					TextDrawSetString(Text:tdId, TDFX_Text[slot]);
					TextDrawShowForAll(Text:tdId);
				}
				#if defined TextDrawSetPos
				else if (fxType == TDFX_TYPE_SHAKE || fxType == TDFX_TYPE_WAVE)
				{
					TextDrawSetPos(Text:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
					TextDrawShowForAll(Text:tdId);
				}
				#endif
				else if (fxType == TDFX_TYPE_PROGRESS)
				{
					new gAlign = 1;
					#if TDFX_MAX_GLOBAL_TD > 0
						if (TDFX_ValidGlobal(Text:tdId)) gAlign = TDFX_GRemAlign[tdId];
					#endif
					#if defined TextDrawGetAlignment
						gAlign = _:TextDrawGetAlignment(Text:tdId);
					#endif
					if (gAlign == 2) TextDrawTextSize(Text:tdId, TDFX_FloatB[slot], TDFX_FloatC[slot]);
					else TextDrawTextSize(Text:tdId, TDFX_FloatC[slot], TDFX_FloatB[slot]);
					TextDrawShowForAll(Text:tdId);
				}
				else if ((fxType == TDFX_TYPE_BOUNCE || fxType == TDFX_TYPE_BOUNCE_CIRC) && TDFX_ColorShift[slot] && TDFX_Base[slot])
				{
					_TDFX_TDColour(Text:tdId, TDFX_Base[slot]);
					TextDrawShowForAll(Text:tdId);
				}
			}
		}
		else if (IsPlayerConnected(playerid))
		{
			if (TDFX_AutoHide[slot])
			{
				if ((fxType == TDFX_TYPE_FADE || fxType == TDFX_TYPE_NEON) && TDFX_Base[slot])
				{
					if (TDFX_Preview[slot]) _TDFX_PTDBackColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
					else
					{
						_TDFX_PTDColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
						if (TDFX_BoxBase[slot]) _TDFX_PTDBoxColour(playerid, PlayerText:tdId, TDFX_BoxBase[slot]);
					}
				}
				else if (fxType == TDFX_TYPE_SCALE || fxType == TDFX_TYPE_PULSE)
				{
					PlayerTextDrawLetterSize(playerid, PlayerText:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
				}
				#if defined PlayerTextDrawSetPos
				else if (fxType == TDFX_TYPE_SHAKE || fxType == TDFX_TYPE_WAVE)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
				}
				#endif
				else if (fxType == TDFX_TYPE_GLITCH && TDFX_Base[slot])
				{
					_TDFX_PTDColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
					PlayerTextDrawSetString(playerid, PlayerText:tdId, TDFX_Text[slot]);
				}
				PlayerTextDrawHide(playerid, PlayerText:tdId);
			}
			else if (TDFX_AutoDestroy[slot]) PlayerTextDrawDestroy(playerid, PlayerText:tdId);
			else
			{
				if (fxType == TDFX_TYPE_SCALE || fxType == TDFX_TYPE_PULSE)
				{
					PlayerTextDrawLetterSize(playerid, PlayerText:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
				else if ((fxType == TDFX_TYPE_FADE || fxType == TDFX_TYPE_NEON) && TDFX_Base[slot])
				{
					if (TDFX_Preview[slot]) _TDFX_PTDBackColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
					else
					{
						_TDFX_PTDColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
						if (TDFX_BoxBase[slot]) _TDFX_PTDBoxColour(playerid, PlayerText:tdId, TDFX_BoxBase[slot]);
					}
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
				else if (fxType == TDFX_TYPE_BLINK && TDFX_Colour[slot])
				{
					if (TDFX_Preview[slot]) _TDFX_PTDBackColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
					else
					{
						_TDFX_PTDColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
						if (TDFX_BoxBase[slot]) _TDFX_PTDBoxColour(playerid, PlayerText:tdId, TDFX_BoxBase[slot]);
					}
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
				else if (fxType == TDFX_TYPE_TYPEWRITER || fxType == TDFX_TYPE_SCRAMBLE || fxType == TDFX_TYPE_GLITCH)
				{
					if (fxType == TDFX_TYPE_GLITCH && TDFX_Base[slot]) _TDFX_PTDColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
					PlayerTextDrawSetString(playerid, PlayerText:tdId, TDFX_Text[slot]);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
				#if defined PlayerTextDrawSetPos
				else if (fxType == TDFX_TYPE_SHAKE || fxType == TDFX_TYPE_WAVE)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, TDFX_FloatA[slot], TDFX_FloatB[slot]);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
				#endif
				else if (fxType == TDFX_TYPE_PROGRESS)
				{
					new align = 1;
					if (TDFX_ValidPlayer(playerid, PlayerText:tdId)) align = TDFX_RemAlign[playerid][tdId];
					#if defined PlayerTextDrawGetAlignment
						align = _:PlayerTextDrawGetAlignment(playerid, PlayerText:tdId);
					#endif
					if (align == 2) PlayerTextDrawTextSize(playerid, PlayerText:tdId, TDFX_FloatB[slot], TDFX_FloatC[slot]);
					else PlayerTextDrawTextSize(playerid, PlayerText:tdId, TDFX_FloatC[slot], TDFX_FloatB[slot]);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
				else if ((fxType == TDFX_TYPE_BOUNCE || fxType == TDFX_TYPE_BOUNCE_CIRC) && TDFX_ColorShift[slot] && TDFX_Base[slot])
				{
					_TDFX_PTDColour(playerid, PlayerText:tdId, TDFX_Base[slot]);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			}
		}
	}
	if (triggerFinish)
	{
		if (cb[0])
		{
			if (isGlobal) CallLocalFunction(cb, "ii", tdId, extraId);
			else CallLocalFunction(cb, "iii", playerid, tdId, extraId);
		}
		if (extraId != 0)
		{
			if (isGlobal)
			{
				CallLocalFunction("OnTextDrawFXFinish", "iii", tdId, fxType, extraId);
			}
			else if (IsPlayerConnected(playerid))
			{
				CallLocalFunction("OnPlayerTextDrawFXFinish", "iiii", playerid, tdId, fxType, extraId);
			}
		}
	}
}

// -----------------------------------------------------------------------------
// Tick Engine
// -----------------------------------------------------------------------------

forward TDFX_Tick(slot);
public TDFX_Tick(slot)
{
	if (slot < 0 || slot >= TDFX_MAX_FX || !TDFX_Used[slot]) return 0;
	new bool:isGlobal = TDFX_IsGlobal[slot];
	new playerid = TDFX_Player[slot];
	new tdId = TDFX_Td[slot];

	if (!isGlobal && !IsPlayerConnected(playerid))
	{
		TDFX_Free(slot, false, false);
		return 0;
	}

	new elapsed = GetTickCount() - TDFX_Start[slot];
	new duration = TDFX_Duration[slot];
	new fxType = TDFX_Type[slot];

	// Handle Duration-based Finish
	if (duration > 0 && elapsed >= duration)
	{
		if (TDFX_Loop[slot])
		{
			TDFX_Start[slot] = GetTickCount();
			TDFX_Step[slot] = 0;
		}
		else
		{
			TDFX_Free(slot, true, true);
			return 1;
		}
	}

	new Float:progress = 1.0;
	if (duration > 0)
	{
		progress = float(elapsed) / float(duration);
		if (progress > 1.0) progress = 1.0;
		if (progress < 0.0) progress = 0.0;
	}

	switch (fxType)
	{
		case TDFX_TYPE_BLINK:
		{
			if (TDFX_PerChar[slot])
			{
				TDFX_Step[slot]++;
				if (TDFX_Step[slot] > TDFX_Count[slot]) TDFX_Step[slot] = 0;
				new part[TDFX_MAX_TEXT], cut = TDFX_Cut(TDFX_Text[slot], TDFX_Step[slot]);
				if (TDFX_Step[slot] >= TDFX_Count[slot]) cut = strlen(TDFX_Text[slot]);
				if (cut > 0) strmid(part, TDFX_Text[slot], 0, cut, sizeof part);
				else { part[0] = ' '; part[1] = 0; }
				if (isGlobal) { TextDrawSetString(Text:tdId, part); TextDrawShowForAll(Text:tdId); }
				else { PlayerTextDrawSetString(playerid, PlayerText:tdId, part); PlayerTextDrawShow(playerid, PlayerText:tdId); }
			}
			else
			{
				TDFX_Step[slot] = TDFX_Step[slot] ? 0 : 1;
				new bool:lit = (TDFX_Step[slot] != 0);
				if (TDFX_Colour[slot])
				{
					new c = lit ? TDFX_Colour[slot] : TDFX_Base[slot];
					if (isGlobal)
					{
						_TDFX_TDColour(Text:tdId, c);
						if (TDFX_BoxBase[slot]) _TDFX_TDBoxColour(Text:tdId, lit ? TDFX_Colour[slot] : TDFX_BoxBase[slot]);
						TextDrawShowForAll(Text:tdId);
					}
					else if (TDFX_Preview[slot])
					{
						_TDFX_PTDBackColour(playerid, PlayerText:tdId, c);
						PlayerTextDrawShow(playerid, PlayerText:tdId);
					}
					else
					{
						_TDFX_PTDColour(playerid, PlayerText:tdId, c);
						if (TDFX_BoxBase[slot]) _TDFX_PTDBoxColour(playerid, PlayerText:tdId, lit ? TDFX_Colour[slot] : TDFX_BoxBase[slot]);
						PlayerTextDrawShow(playerid, PlayerText:tdId);
					}
				}
				else
				{
					if (isGlobal) { if (lit) TextDrawShowForAll(Text:tdId); else TextDrawHideForAll(Text:tdId); }
					else { if (lit) PlayerTextDrawShow(playerid, PlayerText:tdId); else PlayerTextDrawHide(playerid, PlayerText:tdId); }
				}
			}
		}

		case TDFX_TYPE_FADE:
		{
			new Float:factor = TDFX_ApplyEasing(progress, TDFX_Easing[slot]);
			new curCol = TDFX_LerpColor(TDFX_ColFrom[slot], TDFX_ColTo[slot], factor);
			new curBox = TDFX_LerpColor(TDFX_BoxColFrom[slot], TDFX_BoxColTo[slot], factor);

			if (isGlobal)
			{
				_TDFX_TDColour(Text:tdId, curCol);
				if (TDFX_FadeBox[slot]) _TDFX_TDBoxColour(Text:tdId, curBox);
				TextDrawShowForAll(Text:tdId);
			}
			else if (TDFX_Preview[slot])
			{
				_TDFX_PTDBackColour(playerid, PlayerText:tdId, curCol);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
			else
			{
				_TDFX_PTDColour(playerid, PlayerText:tdId, curCol);
				if (TDFX_FadeBox[slot]) _TDFX_PTDBoxColour(playerid, PlayerText:tdId, curBox);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_SCALE:
		{
			new Float:factor = TDFX_ApplyEasing(progress, TDFX_Easing[slot]);
			new Float:curX = TDFX_FloatA[slot] + (TDFX_FloatC[slot] - TDFX_FloatA[slot]) * factor;
			new Float:curY = TDFX_FloatB[slot] + (TDFX_FloatD[slot] - TDFX_FloatB[slot]) * factor;

			if (isGlobal)
			{
				TextDrawLetterSize(Text:tdId, curX, curY);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				PlayerTextDrawLetterSize(playerid, PlayerText:tdId, curX, curY);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_PULSE:
		{
			new Float:cycles = TDFX_FloatD[slot];
			new Float:wave;
			if (cycles > 0.0)
				wave = floatsin(progress * 360.0 * cycles, degrees);
			else
				wave = floatsin(float(elapsed % 1000) * 0.36, degrees);

			new Float:amp = TDFX_FloatC[slot];
			new Float:curX = TDFX_FloatA[slot] * (1.0 + amp * wave);
			new Float:curY = TDFX_FloatB[slot] * (1.0 + amp * wave);

			if (isGlobal)
			{
				TextDrawLetterSize(Text:tdId, curX, curY);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				PlayerTextDrawLetterSize(playerid, PlayerText:tdId, curX, curY);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_SLIDE:
		{
			new Float:factor = TDFX_ApplyEasing(progress, TDFX_Easing[slot]);
			new Float:curX = TDFX_FloatA[slot] + (TDFX_FloatC[slot] - TDFX_FloatA[slot]) * factor;
			new Float:curY = TDFX_FloatB[slot] + (TDFX_FloatD[slot] - TDFX_FloatB[slot]) * factor;

			#if defined PlayerTextDrawSetPos
				if (!isGlobal)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, curX, curY);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			#endif
			#if defined TextDrawSetPos
				if (isGlobal)
				{
					TextDrawSetPos(Text:tdId, curX, curY);
					TextDrawShowForAll(Text:tdId);
				}
			#endif
		}

		case TDFX_TYPE_COUNT:
		{
			new Float:factor = TDFX_ApplyEasing(progress, TDFX_Easing[slot]);
			new fromVal = TDFX_Step[slot], toVal = TDFX_Count[slot];
			new curVal = fromVal + floatround(float(toVal - fromVal) * factor);
			new str[TDFX_MAX_TEXT];
			TDFX_FormatNumber(str, sizeof str, curVal, TDFX_Prefix[slot], TDFX_Suffix[slot], TDFX_ThousandSep[slot]);

			if (isGlobal)
			{
				TextDrawSetString(Text:tdId, str);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				PlayerTextDrawSetString(playerid, PlayerText:tdId, str);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_SCRAMBLE:
		{
			new totalLen = strlen(TDFX_Text[slot]);
			new solved = floatround(float(totalLen) * progress);
			new buf[TDFX_MAX_TEXT];
			new const charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789#%*&@$!";
			new csLen = sizeof(charset) - 1;

			for (new i = 0; i < totalLen && i < sizeof(buf) - 1; i++)
			{
				if (TDFX_Text[slot][i] == '~')
				{
					new nextTag = TDFX_SkipTag(TDFX_Text[slot], i);
					while (i < nextTag && i < sizeof(buf) - 1)
					{
						buf[i] = TDFX_Text[slot][i];
						i++;
					}
					i--;
					continue;
				}
				if (TDFX_Text[slot][i] == ' ' || TDFX_Text[slot][i] == '_')
				{
					buf[i] = TDFX_Text[slot][i];
					continue;
				}
				if (i < solved)
				{
					buf[i] = TDFX_Text[slot][i];
				}
				else
				{
					buf[i] = charset[random(csLen)];
				}
			}
			buf[totalLen] = 0;

			if (isGlobal)
			{
				TextDrawSetString(Text:tdId, buf);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				PlayerTextDrawSetString(playerid, PlayerText:tdId, buf);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_BOUNCE:
		{
			new Float:curX = TDFX_FloatA[slot] + TDFX_FloatC[slot];
			new Float:curY = TDFX_FloatB[slot] + TDFX_FloatD[slot];
			new bool:bounced = false, bool:hitX = false, bool:hitY = false;

			if (curX <= TDFX_BoundMinX[slot])
			{
				curX = TDFX_BoundMinX[slot];
				TDFX_FloatC[slot] = -TDFX_FloatC[slot];
				bounced = true;
				hitX = true;
			}
			else if (curX >= TDFX_BoundMaxX[slot])
			{
				curX = TDFX_BoundMaxX[slot];
				TDFX_FloatC[slot] = -TDFX_FloatC[slot];
				bounced = true;
				hitX = true;
			}

			if (curY <= TDFX_BoundMinY[slot])
			{
				curY = TDFX_BoundMinY[slot];
				TDFX_FloatD[slot] = -TDFX_FloatD[slot];
				bounced = true;
				hitY = true;
			}
			else if (curY >= TDFX_BoundMaxY[slot])
			{
				curY = TDFX_BoundMaxY[slot];
				TDFX_FloatD[slot] = -TDFX_FloatD[slot];
				bounced = true;
				hitY = true;
			}

			TDFX_FloatA[slot] = curX;
			TDFX_FloatB[slot] = curY;

			if (bounced && TDFX_ColorShift[slot])
			{
				new nextCol = TDFX_DvdPalette[random(sizeof(TDFX_DvdPalette))];
				TDFX_Base[slot] = nextCol;
				if (isGlobal) _TDFX_TDColour(Text:tdId, nextCol);
				else _TDFX_PTDColour(playerid, PlayerText:tdId, nextCol);
			}

			if (hitX && hitY)
			{
				if (!isGlobal && IsPlayerConnected(playerid))
				{
					PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
				}
			}

			#if defined PlayerTextDrawSetPos
				if (!isGlobal)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, curX, curY);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			#endif
			#if defined TextDrawSetPos
				if (isGlobal)
				{
					TextDrawSetPos(Text:tdId, curX, curY);
					TextDrawShowForAll(Text:tdId);
				}
			#endif
		}

		case TDFX_TYPE_BOUNCE_CIRC:
		{
			new Float:curX = TDFX_FloatA[slot] + TDFX_FloatC[slot];
			new Float:curY = TDFX_FloatB[slot] + TDFX_FloatD[slot];
			new Float:dx = curX - TDFX_BoundMinX[slot];
			new Float:dy = curY - TDFX_BoundMinY[slot];
			new Float:dist = floatsqroot(dx * dx + dy * dy);

			if (dist >= TDFX_BoundMaxX[slot] && dist > 0.0001)
			{
				new Float:nx = dx / dist;
				new Float:ny = dy / dist;
				new Float:dot = TDFX_FloatC[slot] * nx + TDFX_FloatD[slot] * ny;
				if (dot > 0.0)
				{
					new Float:vx = TDFX_FloatC[slot] - 2.0 * dot * nx;
					new Float:vy = TDFX_FloatD[slot] - 2.0 * dot * ny;

					// Deflect if hitting near head-on to guarantee continuous 2D polygon bounce around circle
					new Float:speed = floatsqroot(vx * vx + vy * vy);
					if (dot >= speed * 0.85)
					{
						vx += -ny * (speed * 0.40);
						vy += nx * (speed * 0.40);
						new Float:ns = floatsqroot(vx * vx + vy * vy);
						if (ns > 0.001)
						{
							vx = (vx / ns) * speed;
							vy = (vy / ns) * speed;
						}
					}

					TDFX_FloatC[slot] = vx;
					TDFX_FloatD[slot] = vy;
					curX = TDFX_BoundMinX[slot] + nx * (TDFX_BoundMaxX[slot] - 0.5);
					curY = TDFX_BoundMinY[slot] + ny * (TDFX_BoundMaxX[slot] - 0.5);

					if (TDFX_ColorShift[slot])
					{
						new nextCol = TDFX_DvdPalette[random(sizeof(TDFX_DvdPalette))];
						TDFX_Base[slot] = nextCol;
						if (isGlobal) _TDFX_TDColour(Text:tdId, nextCol);
						else _TDFX_PTDColour(playerid, PlayerText:tdId, nextCol);
					}
				}
			}

			TDFX_FloatA[slot] = curX;
			TDFX_FloatB[slot] = curY;

			#if defined PlayerTextDrawSetPos
				if (!isGlobal)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, curX, curY);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			#endif
			#if defined TextDrawSetPos
				if (isGlobal)
				{
					TextDrawSetPos(Text:tdId, curX, curY);
					TextDrawShowForAll(Text:tdId);
				}
			#endif
		}

		case TDFX_TYPE_ORBIT:
		{
			new Float:curAngle = TDFX_BoundMinY[slot] + (TDFX_BoundMinX[slot] * float(elapsed) / 1000.0);
			new Float:curX = TDFX_FloatA[slot] + TDFX_FloatC[slot] * floatcos(curAngle, degrees);
			new Float:curY = TDFX_FloatB[slot] + TDFX_FloatD[slot] * floatsin(curAngle, degrees);

			#if defined PlayerTextDrawSetPos
				if (!isGlobal)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, curX, curY);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			#endif
			#if defined TextDrawSetPos
				if (isGlobal)
				{
					TextDrawSetPos(Text:tdId, curX, curY);
					TextDrawShowForAll(Text:tdId);
				}
			#endif
		}

		case TDFX_TYPE_SHAKE:
		{
			new Float:trauma = 1.0 - progress;
			new Float:shake = trauma * trauma * TDFX_FloatC[slot];
			new Float:rndAngle = float(random(360));
			new Float:dist = (float(random(1001)) / 1000.0) * shake;
			new Float:curX = TDFX_FloatA[slot] + dist * floatsin(rndAngle, degrees);
			new Float:curY = TDFX_FloatB[slot] + dist * floatcos(rndAngle, degrees);

			#if defined PlayerTextDrawSetPos
				if (!isGlobal)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, curX, curY);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			#endif
			#if defined TextDrawSetPos
				if (isGlobal)
				{
					TextDrawSetPos(Text:tdId, curX, curY);
					TextDrawShowForAll(Text:tdId);
				}
			#endif
		}

		case TDFX_TYPE_NEON:
		{
			new alpha = 255;
			if (progress < 0.85)
			{
				new roll = random(100);
				if (roll < TDFX_Step[slot])
				{
					new sub = random(100);
					if (sub < 25) alpha = 30 + random(50);
					else if (sub < 55) alpha = 0;
					else if (sub < 80) alpha = 120 + random(60);
					else alpha = 255;
				}
			}

			new curCol = (TDFX_Base[slot] & 0xFFFFFF00) | alpha;
			new curBox = 0;
			if (TDFX_BoxBase[slot])
			{
				new origAlpha = TDFX_BoxBase[slot] & 0xFF;
				new boxAlpha = (alpha * origAlpha) / 255;
				curBox = (TDFX_BoxBase[slot] & 0xFFFFFF00) | boxAlpha;
			}

			if (isGlobal)
			{
				_TDFX_TDColour(Text:tdId, curCol);
				if (TDFX_BoxBase[slot]) _TDFX_TDBoxColour(Text:tdId, curBox);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				_TDFX_PTDColour(playerid, PlayerText:tdId, curCol);
				if (TDFX_BoxBase[slot]) _TDFX_PTDBoxColour(playerid, PlayerText:tdId, curBox);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_WAVE:
		{
			new speedMs = TDFX_Step[slot];
			if (speedMs < 100) speedMs = 1500;
			new Float:phase = float(elapsed % speedMs) / float(speedMs);
			new Float:angle = phase * 360.0;
			new Float:curX = TDFX_FloatA[slot] + TDFX_FloatC[slot] * floatsin(angle, degrees);
			new Float:curY = TDFX_FloatB[slot] + TDFX_FloatD[slot] * floatcos(angle * 1.5, degrees);

			#if defined PlayerTextDrawSetPos
				if (!isGlobal)
				{
					PlayerTextDrawSetPos(playerid, PlayerText:tdId, curX, curY);
					PlayerTextDrawShow(playerid, PlayerText:tdId);
				}
			#endif
			#if defined TextDrawSetPos
				if (isGlobal)
				{
					TextDrawSetPos(Text:tdId, curX, curY);
					TextDrawShowForAll(Text:tdId);
				}
			#endif
		}

		case TDFX_TYPE_PROGRESS:
		{
			new Float:factor = TDFX_ApplyEasing(progress, TDFX_Easing[slot]);
			new Float:curWidth = TDFX_FloatA[slot] + (TDFX_FloatC[slot] - TDFX_FloatA[slot]) * factor;

			if (isGlobal)
			{
				new gAlign = 1;
				#if TDFX_MAX_GLOBAL_TD > 0
					if (TDFX_ValidGlobal(Text:tdId)) gAlign = TDFX_GRemAlign[tdId];
				#endif
				#if defined TextDrawGetAlignment
					gAlign = _:TextDrawGetAlignment(Text:tdId);
				#endif

				if (gAlign == 2) TextDrawTextSize(Text:tdId, TDFX_FloatB[slot], curWidth);
				else TextDrawTextSize(Text:tdId, curWidth, TDFX_FloatB[slot]);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				new align = 1;
				if (TDFX_ValidPlayer(playerid, PlayerText:tdId)) align = TDFX_RemAlign[playerid][tdId];
				#if defined PlayerTextDrawGetAlignment
					align = _:PlayerTextDrawGetAlignment(playerid, PlayerText:tdId);
				#endif

				if (align == 2) PlayerTextDrawTextSize(playerid, PlayerText:tdId, TDFX_FloatB[slot], curWidth);
				else PlayerTextDrawTextSize(playerid, PlayerText:tdId, curWidth, TDFX_FloatB[slot]);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}

		case TDFX_TYPE_GLITCH:
		{
			new buf[TDFX_MAX_TEXT];
			strmid(buf, TDFX_Text[slot], 0, strlen(TDFX_Text[slot]), sizeof buf);
			new totalLen = strlen(buf);
			new alpha = 255;

			if (random(100) < 35 && totalLen > 0)
			{
				alpha = 180 + random(76);
				new const glitchChars[] = "!@#$%^&*<>[]{}01/+=?";
				new gcLen = sizeof(glitchChars) - 1;
				new glitches = TDFX_Step[slot];
				if (glitches < 1) glitches = 1;
				if (glitches > 5) glitches = 5;

				for (new g = 0; g < glitches; g++)
				{
					new idx = random(totalLen);
					new bool:inTag = false, j = 0;
					while (j <= idx && buf[j])
					{
						if (buf[j] == '~')
						{
							new nextTag = TDFX_SkipTag(buf, j);
							if (idx >= j && idx < nextTag) { inTag = true; break; }
							j = nextTag;
							continue;
						}
						j++;
					}
					if (!inTag && buf[idx] != ' ' && buf[idx] != '_')
					{
						buf[idx] = glitchChars[random(gcLen)];
					}
				}
			}

			new curCol = (TDFX_Base[slot] & 0xFFFFFF00) | alpha;

			if (isGlobal)
			{
				_TDFX_TDColour(Text:tdId, curCol);
				TextDrawSetString(Text:tdId, buf);
				TextDrawShowForAll(Text:tdId);
			}
			else
			{
				_TDFX_PTDColour(playerid, PlayerText:tdId, curCol);
				PlayerTextDrawSetString(playerid, PlayerText:tdId, buf);
				PlayerTextDrawShow(playerid, PlayerText:tdId);
			}
		}
	}
	return 1;
}

// -----------------------------------------------------------------------------
// Public API - Player TextDraws
// -----------------------------------------------------------------------------

stock BlinkPlayerTextDraw(playerid, PlayerText:textid, interval = 500, duration = 0, bool:perChar = false, colour = 0, const text[] = "", baseColour = 0)
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	if (duration < 0) duration = 0;
	new bool:known = TDFX_ValidPlayer(playerid, textid), font = 1, bool:box = false;
	if (known)
	{
		font = TDFX_Info[playerid][_:textid] & 7;
		box = (TDFX_Info[playerid][_:textid] & 8) != 0;
	}
	#if defined PlayerTextDrawGetFont
		font = _:PlayerTextDrawGetFont(playerid, textid);
	#endif
	#if defined PlayerTextDrawIsBox
		box = PlayerTextDrawIsBox(playerid, textid);
	#endif
	new bool:preview = (font == 5);
	if (font == 4 || font == 5) perChar = false;
	new boxBase = 0;
	if (colour)
	{
		if (preview)
		{
			#if defined _TDFX_PTDGetBackColour
				if (!baseColour) baseColour = _TDFX_PTDGetBackColour(playerid, textid);
			#endif
			if (!baseColour && known) baseColour = TDFX_RemBack[playerid][_:textid];
		}
		else
		{
			#if defined _TDFX_PTDGetColour
				if (!baseColour) baseColour = _TDFX_PTDGetColour(playerid, textid);
			#endif
			if (!baseColour && known) baseColour = TDFX_RemText[playerid][_:textid];
			if (box)
			{
				#if defined _TDFX_PTDGetBoxColour
					boxBase = _TDFX_PTDGetBoxColour(playerid, textid);
				#endif
				if (!boxBase && known) boxBase = TDFX_RemBox[playerid][_:textid];
			}
		}
		if (!baseColour) return 0;
	}
	TDFX_Text[slot][0] = 0;
	if (perChar)
	{
		if (text[0]) strmid(TDFX_Text[slot], text, 0, strlen(text), TDFX_MAX_TEXT);
		#if defined PlayerTextDrawGetString
		else PlayerTextDrawGetString(playerid, textid, TDFX_Text[slot], TDFX_MAX_TEXT);
		#endif
		TDFX_Count[slot] = TDFX_CountChars(TDFX_Text[slot]);
		if (TDFX_Count[slot] == 0) perChar = false;
	}
	if (perChar && duration > 0 && TDFX_Count[slot] * interval > duration)
	{
		interval = duration / TDFX_Count[slot];
		if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	}
	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_BLINK;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_PerChar[slot] = perChar;
	TDFX_Preview[slot] = preview;
	TDFX_Duration[slot] = duration;
	TDFX_Colour[slot] = colour;
	TDFX_Base[slot] = baseColour;
	TDFX_BoxBase[slot] = boxBase;
	TDFX_ExtraId[slot] = 0;
	TDFX_Callback[slot][0] = 0;
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	if (perChar)
	{
		TDFX_Step[slot] = 0;
		new part[2]; part[0] = ' '; part[1] = 0;
		PlayerTextDrawSetString(playerid, textid, part);
	}
	else TDFX_Step[slot] = 1;

	if (colour)
	{
		if (preview) _TDFX_PTDBackColour(playerid, textid, colour);
		else
		{
			_TDFX_PTDColour(playerid, textid, colour);
			if (boxBase) _TDFX_PTDBoxColour(playerid, textid, colour);
		}
	}
	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", interval, true, "i", slot);
	return 1;
}

stock FadeInPlayerTextDraw(playerid, PlayerText:textid, duration = 1000, bool:fadeBox = true, targetAlpha = 255, extra_id = 0, const callback[] = "", bool:autoHide = false)
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	new bool:known = TDFX_ValidPlayer(playerid, textid), baseCol = 0, boxCol = 0, font = 1;
	if (known)
	{
		font = TDFX_Info[playerid][_:textid] & 7;
		baseCol = TDFX_RemText[playerid][_:textid];
		boxCol = TDFX_RemBox[playerid][_:textid];
	}
	#if defined PlayerTextDrawGetFont
		font = _:PlayerTextDrawGetFont(playerid, textid);
	#endif
	new bool:preview = (font == 5);
	if (preview)
	{
		#if defined _TDFX_PTDGetBackColour
			baseCol = _TDFX_PTDGetBackColour(playerid, textid);
		#endif
		if (!baseCol && known) baseCol = TDFX_RemBack[playerid][_:textid];
	}
	else
	{
		#if defined _TDFX_PTDGetColour
			baseCol = _TDFX_PTDGetColour(playerid, textid);
		#endif
		#if defined _TDFX_PTDGetBoxColour
			boxCol = _TDFX_PTDGetBoxColour(playerid, textid);
		#endif
	}
	if (!baseCol) baseCol = 0xFFFFFFFF;
	if (!boxCol) boxCol = 0x000000AA;

	new startCol = (baseCol & 0xFFFFFF00);
	new endCol = (baseCol & 0xFFFFFF00) | (targetAlpha & 0xFF);
	new startBox = (boxCol & 0xFFFFFF00);
	new endBox = (boxCol & 0xFFFFFF00) | (targetAlpha & 0xFF);

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_FADE;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Preview[slot] = preview;
	TDFX_Duration[slot] = duration;
	TDFX_Base[slot] = endCol;
	TDFX_BoxBase[slot] = endBox;
	TDFX_ColFrom[slot] = startCol;
	TDFX_ColTo[slot] = endCol;
	TDFX_BoxColFrom[slot] = startBox;
	TDFX_BoxColTo[slot] = endBox;
	TDFX_FadeBox[slot] = fadeBox;
	TDFX_Easing[slot] = TDFX_EASE_LINEAR;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = autoHide;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	if (preview) _TDFX_PTDBackColour(playerid, textid, startCol);
	else
	{
		_TDFX_PTDColour(playerid, textid, startCol);
		if (fadeBox) _TDFX_PTDBoxColour(playerid, textid, startBox);
	}
	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock FadeOutPlayerTextDraw(playerid, PlayerText:textid, duration = 1000, bool:fadeBox = true, extra_id = 0, const callback[] = "", bool:autoHide = true)
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	new bool:known = TDFX_ValidPlayer(playerid, textid), baseCol = 0, boxCol = 0, font = 1;
	if (known)
	{
		font = TDFX_Info[playerid][_:textid] & 7;
		baseCol = TDFX_RemText[playerid][_:textid];
		boxCol = TDFX_RemBox[playerid][_:textid];
	}
	#if defined PlayerTextDrawGetFont
		font = _:PlayerTextDrawGetFont(playerid, textid);
	#endif
	new bool:preview = (font == 5);
	if (preview)
	{
		#if defined _TDFX_PTDGetBackColour
			baseCol = _TDFX_PTDGetBackColour(playerid, textid);
		#endif
		if (!baseCol && known) baseCol = TDFX_RemBack[playerid][_:textid];
	}
	else
	{
		#if defined _TDFX_PTDGetColour
			baseCol = _TDFX_PTDGetColour(playerid, textid);
		#endif
		#if defined _TDFX_PTDGetBoxColour
			boxCol = _TDFX_PTDGetBoxColour(playerid, textid);
		#endif
	}
	if (!baseCol) baseCol = 0xFFFFFFFF;
	if (!boxCol) boxCol = 0x000000AA;

	new startCol = baseCol;
	new endCol = (baseCol & 0xFFFFFF00);
	new startBox = boxCol;
	new endBox = (boxCol & 0xFFFFFF00);

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_FADE;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Preview[slot] = preview;
	TDFX_Duration[slot] = duration;
	TDFX_Base[slot] = baseCol;
	TDFX_BoxBase[slot] = boxCol;
	TDFX_ColFrom[slot] = startCol;
	TDFX_ColTo[slot] = endCol;
	TDFX_BoxColFrom[slot] = startBox;
	TDFX_BoxColTo[slot] = endBox;
	TDFX_FadeBox[slot] = fadeBox;
	TDFX_Easing[slot] = TDFX_EASE_LINEAR;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = autoHide;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock FadeColorPlayerTextDraw(playerid, PlayerText:textid, fromColor, toColor, duration = 1000, bool:fadeBox = false, fromBox = 0, toBox = 0, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	new bool:preview = false;
	#if defined PlayerTextDrawGetFont
		preview = (_:PlayerTextDrawGetFont(playerid, textid) == 5);
	#endif

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_FADE;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Preview[slot] = preview;
	TDFX_Duration[slot] = duration;
	TDFX_Base[slot] = toColor;
	TDFX_BoxBase[slot] = toBox;
	TDFX_ColFrom[slot] = fromColor;
	TDFX_ColTo[slot] = toColor;
	TDFX_BoxColFrom[slot] = fromBox;
	TDFX_BoxColTo[slot] = toBox;
	TDFX_FadeBox[slot] = fadeBox;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock ScalePlayerTextDraw(playerid, PlayerText:textid, Float:startSizeX, Float:startSizeY, Float:targetSizeX, Float:targetSizeY, duration = 1000, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_SCALE;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_FloatA[slot] = startSizeX;
	TDFX_FloatB[slot] = startSizeY;
	TDFX_FloatC[slot] = targetSizeX;
	TDFX_FloatD[slot] = targetSizeY;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	PlayerTextDrawLetterSize(playerid, textid, startSizeX, startSizeY);
	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock PulsePlayerTextDraw(playerid, PlayerText:textid, Float:amplitude = 0.25, interval = 300, cycles = 0, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	new Float:baseX = 0.48, Float:baseY = 1.12;
	if (TDFX_ValidPlayer(playerid, textid))
	{
		if (TDFX_RemLetX[playerid][_:textid] > 0.0) baseX = TDFX_RemLetX[playerid][_:textid];
		if (TDFX_RemLetY[playerid][_:textid] > 0.0) baseY = TDFX_RemLetY[playerid][_:textid];
	}
	#if defined PlayerTextDrawGetLetterSize
		PlayerTextDrawGetLetterSize(playerid, textid, baseX, baseY);
	#endif

	new duration = (cycles > 0) ? (cycles * interval) : 0;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_PULSE;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_FloatA[slot] = baseX;
	TDFX_FloatB[slot] = baseY;
	TDFX_FloatC[slot] = amplitude;
	TDFX_FloatD[slot] = float(cycles);
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = (cycles == 0);
	TDFX_Start[slot] = GetTickCount();

	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock SlidePlayerTextDraw(playerid, PlayerText:textid, Float:targetX, Float:targetY, duration = 1000, easing = TDFX_EASE_OUT_QUAD, extra_id = 0, const callback[] = "")
{
	#if !defined PlayerTextDrawSetPos
		#pragma unused playerid, textid, targetX, targetY, duration, easing, extra_id, callback
		return 0;
	#else
		if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindPlayer(playerid, textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
		new Float:startX = 0.0, Float:startY = 0.0;
		#if defined PlayerTextDrawGetPos
			PlayerTextDrawGetPos(playerid, textid, startX, startY);
		#endif

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = false;
		TDFX_Type[slot] = TDFX_TYPE_SLIDE;
		TDFX_Player[slot] = playerid;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = startX;
		TDFX_FloatB[slot] = startY;
		TDFX_FloatC[slot] = targetX;
		TDFX_FloatD[slot] = targetY;
		TDFX_Easing[slot] = easing;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = false;
		TDFX_Start[slot] = GetTickCount();

		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock CountPlayerTextDrawNumber(playerid, PlayerText:textid, startValue, targetValue, duration = 1000, const prefix[] = "", const suffix[] = "", bool:thousandsSep = false, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_COUNT;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_Step[slot] = startValue;
	TDFX_Count[slot] = targetValue;
	strmid(TDFX_Prefix[slot], prefix, 0, strlen(prefix), 16);
	strmid(TDFX_Suffix[slot], suffix, 0, strlen(suffix), 16);
	TDFX_ThousandSep[slot] = thousandsSep;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	new initial[TDFX_MAX_TEXT];
	TDFX_FormatNumber(initial, sizeof initial, startValue, prefix, suffix, thousandsSep);
	PlayerTextDrawSetString(playerid, textid, initial);
	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock ScramblePlayerTextDraw(playerid, PlayerText:textid, const finalText[], duration = 2000, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_SCRAMBLE;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	strmid(TDFX_Text[slot], finalText, 0, strlen(finalText), TDFX_MAX_TEXT);
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock BouncePlayerTextDraw(playerid, PlayerText:textid, Float:speedX, Float:speedY, Float:minX, Float:minY, Float:maxX, Float:maxY, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined PlayerTextDrawSetPos
		#pragma unused playerid, textid, speedX, speedY, minX, minY, maxX, maxY, colorShift, duration, extra_id, callback
		return 0;
	#else
		if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindPlayer(playerid, textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;
		new Float:curX = 0.0, Float:curY = 0.0;
		TDFX_GetPlayerPos(playerid, textid, curX, curY);

		new baseCol = 0xFFFFFFFF;
		if (TDFX_ValidPlayer(playerid, textid) && TDFX_RemText[playerid][_:textid])
			baseCol = TDFX_RemText[playerid][_:textid];
		#if defined _TDFX_PTDGetColour
			baseCol = _TDFX_PTDGetColour(playerid, textid);
		#endif

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = false;
		TDFX_Type[slot] = TDFX_TYPE_BOUNCE;
		TDFX_Player[slot] = playerid;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = curX;
		TDFX_FloatB[slot] = curY;
		TDFX_FloatC[slot] = speedX;
		TDFX_FloatD[slot] = speedY;
		TDFX_BoundMinX[slot] = minX;
		TDFX_BoundMinY[slot] = minY;
		TDFX_BoundMaxX[slot] = maxX;
		TDFX_BoundMaxY[slot] = maxY;
		TDFX_ColorShift[slot] = colorShift;
		TDFX_Base[slot] = baseCol;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		PlayerTextDrawShow(playerid, textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock BounceCirclePlayerTextDraw(playerid, PlayerText:textid, Float:speed, Float:angle, Float:centerX, Float:centerY, Float:radius, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined PlayerTextDrawSetPos
		#pragma unused playerid, textid, speed, angle, centerX, centerY, radius, colorShift, duration, extra_id, callback
		return 0;
	#else
		if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindPlayer(playerid, textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;
		new Float:curX = 0.0, Float:curY = 0.0;
		TDFX_GetPlayerPos(playerid, textid, curX, curY);

		if (floatabs(curX - centerX) < 0.001 && floatabs(curY - centerY) < 0.001)
		{
			curX = centerX + radius * 0.40;
			curY = centerY - radius * 0.25;
			PlayerTextDrawSetPos(playerid, textid, curX, curY);
		}

		new baseCol = 0xFFFFFFFF;
		if (TDFX_ValidPlayer(playerid, textid) && TDFX_RemText[playerid][_:textid])
			baseCol = TDFX_RemText[playerid][_:textid];
		#if defined _TDFX_PTDGetColour
			baseCol = _TDFX_PTDGetColour(playerid, textid);
		#endif

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = false;
		TDFX_Type[slot] = TDFX_TYPE_BOUNCE_CIRC;
		TDFX_Player[slot] = playerid;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = curX;
		TDFX_FloatB[slot] = curY;
		TDFX_FloatC[slot] = speed * floatcos(angle, degrees);
		TDFX_FloatD[slot] = speed * floatsin(angle, degrees);
		TDFX_BoundMinX[slot] = centerX;
		TDFX_BoundMinY[slot] = centerY;
		TDFX_BoundMaxX[slot] = radius;
		TDFX_BoundMaxY[slot] = 0.0;
		TDFX_ColorShift[slot] = colorShift;
		TDFX_Base[slot] = baseCol;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		PlayerTextDrawShow(playerid, textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock OrbitPlayerTextDraw(playerid, PlayerText:textid, Float:centerX, Float:centerY, Float:radiusX, Float:radiusY, Float:angularSpeed = 60.0, Float:startAngle = 0.0, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined PlayerTextDrawSetPos
		#pragma unused playerid, textid, centerX, centerY, radiusX, radiusY, angularSpeed, startAngle, duration, extra_id, callback
		return 0;
	#else
		if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindPlayer(playerid, textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;

		new Float:origX = 0.0, Float:origY = 0.0;
		TDFX_GetPlayerPos(playerid, textid, origX, origY);

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = false;
		TDFX_Type[slot] = TDFX_TYPE_ORBIT;
		TDFX_Player[slot] = playerid;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = centerX;
		TDFX_FloatB[slot] = centerY;
		TDFX_FloatC[slot] = radiusX;
		TDFX_FloatD[slot] = radiusY;
		TDFX_BoundMinX[slot] = angularSpeed;
		TDFX_BoundMinY[slot] = startAngle;
		TDFX_BoundMaxX[slot] = origX;
		TDFX_BoundMaxY[slot] = origY;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		PlayerTextDrawShow(playerid, textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock ShakePlayerTextDraw(playerid, PlayerText:textid, Float:intensity = 6.0, duration = 800, extra_id = 0, const callback[] = "")
{
	#if !defined PlayerTextDrawSetPos
		#pragma unused playerid, textid, intensity, duration, extra_id, callback
		return 0;
	#else
		if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindPlayer(playerid, textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

		new Float:baseX = 0.0, Float:baseY = 0.0;
		TDFX_GetPlayerPos(playerid, textid, baseX, baseY);

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = false;
		TDFX_Type[slot] = TDFX_TYPE_SHAKE;
		TDFX_Player[slot] = playerid;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = baseX;
		TDFX_FloatB[slot] = baseY;
		TDFX_FloatC[slot] = intensity;
		TDFX_FloatD[slot] = 0.0;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = false;
		TDFX_Start[slot] = GetTickCount();

		PlayerTextDrawShow(playerid, textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock NeonFlickerPlayerTextDraw(playerid, PlayerText:textid, duration = 3000, brokenRatio = 40, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	if (brokenRatio < 0) brokenRatio = 0;
	if (brokenRatio > 100) brokenRatio = 100;

	new baseCol = 0xFFFFFFFF, boxCol = 0;
	if (TDFX_ValidPlayer(playerid, textid))
	{
		if (TDFX_RemText[playerid][_:textid]) baseCol = TDFX_RemText[playerid][_:textid];
		boxCol = TDFX_RemBox[playerid][_:textid];
	}
	#if defined _TDFX_PTDGetColour
		baseCol = _TDFX_PTDGetColour(playerid, textid);
	#endif
	#if defined _TDFX_PTDGetBoxColour
		boxCol = _TDFX_PTDGetBoxColour(playerid, textid);
	#endif

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_NEON;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_Step[slot] = brokenRatio;
	TDFX_Base[slot] = baseCol;
	TDFX_BoxBase[slot] = boxCol;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock WaveDriftPlayerTextDraw(playerid, PlayerText:textid, Float:amplitudeX, Float:amplitudeY, speedMs = 1500, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined PlayerTextDrawSetPos
		#pragma unused playerid, textid, amplitudeX, amplitudeY, speedMs, duration, extra_id, callback
		return 0;
	#else
		if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindPlayer(playerid, textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;
		if (speedMs < 100) speedMs = 1500;

		new Float:baseX = 0.0, Float:baseY = 0.0;
		TDFX_GetPlayerPos(playerid, textid, baseX, baseY);

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = false;
		TDFX_Type[slot] = TDFX_TYPE_WAVE;
		TDFX_Player[slot] = playerid;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = baseX;
		TDFX_FloatB[slot] = baseY;
		TDFX_FloatC[slot] = amplitudeX;
		TDFX_FloatD[slot] = amplitudeY;
		TDFX_Step[slot] = speedMs;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		PlayerTextDrawShow(playerid, textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock FillPlayerTextDrawProgress(playerid, PlayerText:textid, Float:startWidth, Float:targetWidth, duration = 2000, easing = TDFX_EASE_OUT_QUAD, Float:lineHeight = 0.0, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	if (lineHeight <= 0.0)
	{
		if (TDFX_ValidPlayer(playerid, textid)) lineHeight = TDFX_RemSizeY[playerid][_:textid];
		#if defined PlayerTextDrawGetTextSize
			new Float:dummyW = 0.0;
			PlayerTextDrawGetTextSize(playerid, textid, dummyW, lineHeight);
		#endif
		if (lineHeight <= 0.0) lineHeight = 12.0;
	}

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_PROGRESS;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_FloatA[slot] = startWidth;
	TDFX_FloatB[slot] = lineHeight;
	TDFX_FloatC[slot] = targetWidth;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	new align = 1;
	if (TDFX_ValidPlayer(playerid, textid)) align = TDFX_RemAlign[playerid][_:textid];
	#if defined PlayerTextDrawGetAlignment
		align = _:PlayerTextDrawGetAlignment(playerid, textid);
	#endif
	if (align == 2) PlayerTextDrawTextSize(playerid, textid, lineHeight, startWidth);
	else PlayerTextDrawTextSize(playerid, textid, startWidth, lineHeight);
	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock GlitchPlayerTextDraw(playerid, PlayerText:textid, const text[] = "", intensity = 2, duration = 3000, extra_id = 0, const callback[] = "")
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < 0) duration = 0;
	if (intensity < 1) intensity = 1;
	if (intensity > 5) intensity = 5;

	new baseCol = 0xFFFFFFFF;
	if (TDFX_ValidPlayer(playerid, textid) && TDFX_RemText[playerid][_:textid])
		baseCol = TDFX_RemText[playerid][_:textid];
	#if defined _TDFX_PTDGetColour
		baseCol = _TDFX_PTDGetColour(playerid, textid);
	#endif

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = false;
	TDFX_Type[slot] = TDFX_TYPE_GLITCH;
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_Step[slot] = intensity;
	TDFX_Base[slot] = baseCol;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);

	if (text[0])
	{
		strmid(TDFX_Text[slot], text, 0, strlen(text), TDFX_MAX_TEXT);
	}
	else
	{
		#if defined PlayerTextDrawGetString
			PlayerTextDrawGetString(playerid, textid, TDFX_Text[slot], TDFX_MAX_TEXT);
		#else
			TDFX_Text[slot][0] = 0;
		#endif
	}

	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = (duration == 0);
	TDFX_Start[slot] = GetTickCount();

	PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock StopPlayerTextDrawFX(playerid, PlayerText:textid)
{
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot == -1) return 0;
	TDFX_Free(slot, true);
	return 1;
}
#define StopPlayerTextDrawBlink StopPlayerTextDrawFX

stock bool:IsPlayerTextDrawFXActive(playerid, PlayerText:textid)
{
	return TDFX_FindPlayer(playerid, textid) != -1;
}
#define IsPlayerTextDrawBlinking IsPlayerTextDrawFXActive

// -----------------------------------------------------------------------------
// Public API - Global TextDraws
// -----------------------------------------------------------------------------

stock BlinkTextDraw(Text:textid, interval = 500, duration = 0, bool:perChar = false, colour = 0, const text[] = "", baseColour = 0)
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	if (duration < 0) duration = 0;
	new bool:known = TDFX_ValidGlobal(textid), font = 1, bool:box = false;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (known)
		{
			font = TDFX_GInfo[_:textid] & 7;
			box = (TDFX_GInfo[_:textid] & 8) != 0;
		}
	#endif
	#if defined TextDrawGetFont
		font = _:TextDrawGetFont(textid);
	#endif
	#if defined TextDrawIsBox
		box = TextDrawIsBox(textid);
	#endif
	if (font == 4 || font == 5) perChar = false;
	new boxBase = 0;
	if (colour)
	{
		#if defined _TDFX_TDGetColour
			if (!baseColour) baseColour = _TDFX_TDGetColour(textid);
		#endif
		#if TDFX_MAX_GLOBAL_TD > 0
			if (!baseColour && known) baseColour = TDFX_GRemText[_:textid];
			if (box)
			{
				#if defined _TDFX_TDGetBoxColour
					boxBase = _TDFX_TDGetBoxColour(textid);
				#endif
				if (!boxBase && known) boxBase = TDFX_GRemBox[_:textid];
			}
		#endif
		if (!baseColour) return 0;
	}
	TDFX_Text[slot][0] = 0;
	if (perChar)
	{
		if (text[0]) strmid(TDFX_Text[slot], text, 0, strlen(text), TDFX_MAX_TEXT);
		#if defined TextDrawGetString
		else TextDrawGetString(textid, TDFX_Text[slot], TDFX_MAX_TEXT);
		#endif
		TDFX_Count[slot] = TDFX_CountChars(TDFX_Text[slot]);
		if (TDFX_Count[slot] == 0) perChar = false;
	}
	if (perChar && duration > 0 && TDFX_Count[slot] * interval > duration)
	{
		interval = duration / TDFX_Count[slot];
		if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	}
	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_BLINK;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_PerChar[slot] = perChar;
	TDFX_Preview[slot] = false;
	TDFX_Duration[slot] = duration;
	TDFX_Colour[slot] = colour;
	TDFX_Base[slot] = baseColour;
	TDFX_BoxBase[slot] = boxBase;
	TDFX_ExtraId[slot] = 0;
	TDFX_Callback[slot][0] = 0;
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	if (perChar)
	{
		TDFX_Step[slot] = 0;
		new part[2]; part[0] = ' '; part[1] = 0;
		TextDrawSetString(textid, part);
	}
	else TDFX_Step[slot] = 1;

	if (colour)
	{
		_TDFX_TDColour(textid, colour);
		if (boxBase) _TDFX_TDBoxColour(textid, colour);
	}
	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", interval, true, "i", slot);
	return 1;
}

stock FadeInTextDraw(Text:textid, duration = 1000, bool:fadeBox = true, targetAlpha = 255, extra_id = 0, const callback[] = "", bool:autoHide = false)
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	new bool:known = TDFX_ValidGlobal(textid), baseCol = 0, boxCol = 0;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (known)
		{
			baseCol = TDFX_GRemText[_:textid];
			boxCol = TDFX_GRemBox[_:textid];
		}
	#endif
	#if defined _TDFX_TDGetColour
		baseCol = _TDFX_TDGetColour(textid);
	#endif
	#if defined _TDFX_TDGetBoxColour
		boxCol = _TDFX_TDGetBoxColour(textid);
	#endif
	if (!baseCol) baseCol = 0xFFFFFFFF;
	if (!boxCol) boxCol = 0x000000AA;

	new startCol = (baseCol & 0xFFFFFF00);
	new endCol = (baseCol & 0xFFFFFF00) | (targetAlpha & 0xFF);
	new startBox = (boxCol & 0xFFFFFF00);
	new endBox = (boxCol & 0xFFFFFF00) | (targetAlpha & 0xFF);

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_FADE;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Preview[slot] = false;
	TDFX_Duration[slot] = duration;
	TDFX_Base[slot] = endCol;
	TDFX_BoxBase[slot] = endBox;
	TDFX_ColFrom[slot] = startCol;
	TDFX_ColTo[slot] = endCol;
	TDFX_BoxColFrom[slot] = startBox;
	TDFX_BoxColTo[slot] = endBox;
	TDFX_FadeBox[slot] = fadeBox;
	TDFX_Easing[slot] = TDFX_EASE_LINEAR;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = autoHide;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	_TDFX_TDColour(textid, startCol);
	if (fadeBox) _TDFX_TDBoxColour(textid, startBox);
	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock FadeOutTextDraw(Text:textid, duration = 1000, bool:fadeBox = true, extra_id = 0, const callback[] = "", bool:autoHide = true)
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	new bool:known = TDFX_ValidGlobal(textid), baseCol = 0, boxCol = 0;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (known)
		{
			baseCol = TDFX_GRemText[_:textid];
			boxCol = TDFX_GRemBox[_:textid];
		}
	#endif
	#if defined _TDFX_TDGetColour
		baseCol = _TDFX_TDGetColour(textid);
	#endif
	#if defined _TDFX_TDGetBoxColour
		boxCol = _TDFX_TDGetBoxColour(textid);
	#endif
	if (!baseCol) baseCol = 0xFFFFFFFF;
	if (!boxCol) boxCol = 0x000000AA;

	new startCol = baseCol;
	new endCol = (baseCol & 0xFFFFFF00);
	new startBox = boxCol;
	new endBox = (boxCol & 0xFFFFFF00);

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_FADE;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Preview[slot] = false;
	TDFX_Duration[slot] = duration;
	TDFX_Base[slot] = baseCol;
	TDFX_BoxBase[slot] = boxCol;
	TDFX_ColFrom[slot] = startCol;
	TDFX_ColTo[slot] = endCol;
	TDFX_BoxColFrom[slot] = startBox;
	TDFX_BoxColTo[slot] = endBox;
	TDFX_FadeBox[slot] = fadeBox;
	TDFX_Easing[slot] = TDFX_EASE_LINEAR;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = autoHide;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock ScaleTextDraw(Text:textid, Float:startSizeX, Float:startSizeY, Float:targetSizeX, Float:targetSizeY, duration = 1000, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_SCALE;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_FloatA[slot] = startSizeX;
	TDFX_FloatB[slot] = startSizeY;
	TDFX_FloatC[slot] = targetSizeX;
	TDFX_FloatD[slot] = targetSizeY;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	TextDrawLetterSize(textid, startSizeX, startSizeY);
	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock PulseTextDraw(Text:textid, Float:amplitude = 0.25, interval = 300, cycles = 0, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	new Float:baseX = 0.48, Float:baseY = 1.12;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			if (TDFX_GRemLetX[_:textid] > 0.0) baseX = TDFX_GRemLetX[_:textid];
			if (TDFX_GRemLetY[_:textid] > 0.0) baseY = TDFX_GRemLetY[_:textid];
		}
	#endif
	#if defined TextDrawGetLetterSize
		TextDrawGetLetterSize(textid, baseX, baseY);
	#endif

	new duration = (cycles > 0) ? (cycles * interval) : 0;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_PULSE;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_FloatA[slot] = baseX;
	TDFX_FloatB[slot] = baseY;
	TDFX_FloatC[slot] = amplitude;
	TDFX_FloatD[slot] = float(cycles);
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = (cycles == 0);
	TDFX_Start[slot] = GetTickCount();

	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock SlideTextDraw(Text:textid, Float:targetX, Float:targetY, duration = 1000, easing = TDFX_EASE_OUT_QUAD, extra_id = 0, const callback[] = "")
{
	#if !defined TextDrawSetPos
		#pragma unused textid, targetX, targetY, duration, easing, extra_id, callback
		return 0;
	#else
		if (_:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindGlobal(textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
		new Float:startX = 0.0, Float:startY = 0.0;
		#if defined TextDrawGetPos
			TextDrawGetPos(textid, startX, startY);
		#endif

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = true;
		TDFX_Type[slot] = TDFX_TYPE_SLIDE;
		TDFX_Player[slot] = INVALID_PLAYER_ID;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = startX;
		TDFX_FloatB[slot] = startY;
		TDFX_FloatC[slot] = targetX;
		TDFX_FloatD[slot] = targetY;
		TDFX_Easing[slot] = easing;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = false;
		TDFX_Start[slot] = GetTickCount();

		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock CountTextDrawNumber(Text:textid, startValue, targetValue, duration = 1000, const prefix[] = "", const suffix[] = "", bool:thousandsSep = false, easing = TDFX_EASE_LINEAR, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_COUNT;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_Step[slot] = startValue;
	TDFX_Count[slot] = targetValue;
	strmid(TDFX_Prefix[slot], prefix, 0, strlen(prefix), 16);
	strmid(TDFX_Suffix[slot], suffix, 0, strlen(suffix), 16);
	TDFX_ThousandSep[slot] = thousandsSep;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	new initial[TDFX_MAX_TEXT];
	TDFX_FormatNumber(initial, sizeof initial, startValue, prefix, suffix, thousandsSep);
	TextDrawSetString(textid, initial);
	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock ScrambleTextDraw(Text:textid, const finalText[], duration = 2000, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_SCRAMBLE;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	strmid(TDFX_Text[slot], finalText, 0, strlen(finalText), TDFX_MAX_TEXT);
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock BounceTextDraw(Text:textid, Float:speedX, Float:speedY, Float:minX, Float:minY, Float:maxX, Float:maxY, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined TextDrawSetPos
		#pragma unused textid, speedX, speedY, minX, minY, maxX, maxY, colorShift, duration, extra_id, callback
		return 0;
	#else
		if (_:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindGlobal(textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;
		new Float:curX = 0.0, Float:curY = 0.0;
		TDFX_GetGlobalPos(textid, curX, curY);

		new baseCol = 0xFFFFFFFF;
		#if TDFX_MAX_GLOBAL_TD > 0
			if (TDFX_ValidGlobal(textid) && TDFX_GRemText[_:textid])
				baseCol = TDFX_GRemText[_:textid];
		#endif
		#if defined _TDFX_TDGetColour
			baseCol = _TDFX_TDGetColour(textid);
		#endif

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = true;
		TDFX_Type[slot] = TDFX_TYPE_BOUNCE;
		TDFX_Player[slot] = INVALID_PLAYER_ID;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = curX;
		TDFX_FloatB[slot] = curY;
		TDFX_FloatC[slot] = speedX;
		TDFX_FloatD[slot] = speedY;
		TDFX_BoundMinX[slot] = minX;
		TDFX_BoundMinY[slot] = minY;
		TDFX_BoundMaxX[slot] = maxX;
		TDFX_BoundMaxY[slot] = maxY;
		TDFX_ColorShift[slot] = colorShift;
		TDFX_Base[slot] = baseCol;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		TextDrawShowForAll(textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock BounceCircleTextDraw(Text:textid, Float:speed, Float:angle, Float:centerX, Float:centerY, Float:radius, bool:colorShift = false, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined TextDrawSetPos
		#pragma unused textid, speed, angle, centerX, centerY, radius, colorShift, duration, extra_id, callback
		return 0;
	#else
		if (_:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindGlobal(textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;
		new Float:curX = 0.0, Float:curY = 0.0;
		TDFX_GetGlobalPos(textid, curX, curY);

		if (floatabs(curX - centerX) < 0.001 && floatabs(curY - centerY) < 0.001)
		{
			curX = centerX + radius * 0.40;
			curY = centerY - radius * 0.25;
			TextDrawSetPos(textid, curX, curY);
		}

		new baseCol = 0xFFFFFFFF;
		#if TDFX_MAX_GLOBAL_TD > 0
			if (TDFX_ValidGlobal(textid) && TDFX_GRemText[_:textid])
				baseCol = TDFX_GRemText[_:textid];
		#endif
		#if defined _TDFX_TDGetColour
			baseCol = _TDFX_TDGetColour(textid);
		#endif

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = true;
		TDFX_Type[slot] = TDFX_TYPE_BOUNCE_CIRC;
		TDFX_Player[slot] = INVALID_PLAYER_ID;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = curX;
		TDFX_FloatB[slot] = curY;
		TDFX_FloatC[slot] = speed * floatcos(angle, degrees);
		TDFX_FloatD[slot] = speed * floatsin(angle, degrees);
		TDFX_BoundMinX[slot] = centerX;
		TDFX_BoundMinY[slot] = centerY;
		TDFX_BoundMaxX[slot] = radius;
		TDFX_BoundMaxY[slot] = 0.0;
		TDFX_ColorShift[slot] = colorShift;
		TDFX_Base[slot] = baseCol;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		TextDrawShowForAll(textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock OrbitTextDraw(Text:textid, Float:centerX, Float:centerY, Float:radiusX, Float:radiusY, Float:angularSpeed = 60.0, Float:startAngle = 0.0, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined TextDrawSetPos
		#pragma unused textid, centerX, centerY, radiusX, radiusY, angularSpeed, startAngle, duration, extra_id, callback
		return 0;
	#else
		if (_:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindGlobal(textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;

		new Float:origX = 0.0, Float:origY = 0.0;
		TDFX_GetGlobalPos(textid, origX, origY);

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = true;
		TDFX_Type[slot] = TDFX_TYPE_ORBIT;
		TDFX_Player[slot] = INVALID_PLAYER_ID;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = centerX;
		TDFX_FloatB[slot] = centerY;
		TDFX_FloatC[slot] = radiusX;
		TDFX_FloatD[slot] = radiusY;
		TDFX_BoundMinX[slot] = angularSpeed;
		TDFX_BoundMinY[slot] = startAngle;
		TDFX_BoundMaxX[slot] = origX;
		TDFX_BoundMaxY[slot] = origY;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		TextDrawShowForAll(textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock ShakeTextDraw(Text:textid, Float:intensity = 6.0, duration = 800, extra_id = 0, const callback[] = "")
{
	#if !defined TextDrawSetPos
		#pragma unused textid, intensity, duration, extra_id, callback
		return 0;
	#else
		if (_:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindGlobal(textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

		new Float:baseX = 0.0, Float:baseY = 0.0;
		TDFX_GetGlobalPos(textid, baseX, baseY);

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = true;
		TDFX_Type[slot] = TDFX_TYPE_SHAKE;
		TDFX_Player[slot] = INVALID_PLAYER_ID;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = baseX;
		TDFX_FloatB[slot] = baseY;
		TDFX_FloatC[slot] = intensity;
		TDFX_FloatD[slot] = 0.0;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = false;
		TDFX_Start[slot] = GetTickCount();

		TextDrawShowForAll(textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock NeonFlickerTextDraw(Text:textid, duration = 3000, brokenRatio = 40, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;
	if (brokenRatio < 0) brokenRatio = 0;
	if (brokenRatio > 100) brokenRatio = 100;

	new baseCol = 0xFFFFFFFF, boxCol = 0;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			if (TDFX_GRemText[_:textid]) baseCol = TDFX_GRemText[_:textid];
			boxCol = TDFX_GRemBox[_:textid];
		}
	#endif
	#if defined _TDFX_TDGetColour
		baseCol = _TDFX_TDGetColour(textid);
	#endif
	#if defined _TDFX_TDGetBoxColour
		boxCol = _TDFX_TDGetBoxColour(textid);
	#endif

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_NEON;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_Step[slot] = brokenRatio;
	TDFX_Base[slot] = baseCol;
	TDFX_BoxBase[slot] = boxCol;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock WaveDriftTextDraw(Text:textid, Float:amplitudeX, Float:amplitudeY, speedMs = 1500, duration = 0, extra_id = 0, const callback[] = "")
{
	#if !defined TextDrawSetPos
		#pragma unused textid, amplitudeX, amplitudeY, speedMs, duration, extra_id, callback
		return 0;
	#else
		if (_:textid == 0xFFFF || _:textid < 0) return 0;
		new slot = TDFX_FindGlobal(textid);
		if (slot != -1) TDFX_Free(slot, false);
		else
		{
			slot = TDFX_Alloc();
			if (slot == -1) return 0;
		}
		if (duration < 0) duration = 0;
		if (speedMs < 100) speedMs = 1500;

		new Float:baseX = 0.0, Float:baseY = 0.0;
		TDFX_GetGlobalPos(textid, baseX, baseY);

		TDFX_Used[slot] = true;
		TDFX_IsGlobal[slot] = true;
		TDFX_Type[slot] = TDFX_TYPE_WAVE;
		TDFX_Player[slot] = INVALID_PLAYER_ID;
		TDFX_Td[slot] = _:textid;
		TDFX_Duration[slot] = duration;
		TDFX_FloatA[slot] = baseX;
		TDFX_FloatB[slot] = baseY;
		TDFX_FloatC[slot] = amplitudeX;
		TDFX_FloatD[slot] = amplitudeY;
		TDFX_Step[slot] = speedMs;
		TDFX_ExtraId[slot] = extra_id;
		strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
		TDFX_AutoHide[slot] = false;
		TDFX_AutoDestroy[slot] = false;
		TDFX_Loop[slot] = (duration == 0);
		TDFX_Start[slot] = GetTickCount();

		TextDrawShowForAll(textid);
		TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
		return 1;
	#endif
}

stock FillTextDrawProgress(Text:textid, Float:startWidth, Float:targetWidth, duration = 2000, easing = TDFX_EASE_OUT_QUAD, Float:lineHeight = 0.0, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < TDFX_MIN_INTERVAL) duration = TDFX_MIN_INTERVAL;

	if (lineHeight <= 0.0)
	{
		#if TDFX_MAX_GLOBAL_TD > 0
			if (TDFX_ValidGlobal(textid)) lineHeight = TDFX_GRemSizeY[_:textid];
		#endif
		#if defined TextDrawGetTextSize
			new Float:dummyW = 0.0;
			TextDrawGetTextSize(textid, dummyW, lineHeight);
		#endif
		if (lineHeight <= 0.0) lineHeight = 12.0;
	}

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_PROGRESS;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_FloatA[slot] = startWidth;
	TDFX_FloatB[slot] = lineHeight;
	TDFX_FloatC[slot] = targetWidth;
	TDFX_Easing[slot] = easing;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);
	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = false;
	TDFX_Start[slot] = GetTickCount();

	new gAlign = 1;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) gAlign = TDFX_GRemAlign[_:textid];
	#endif
	#if defined TextDrawGetAlignment
		gAlign = _:TextDrawGetAlignment(textid);
	#endif
	if (gAlign == 2) TextDrawTextSize(textid, lineHeight, startWidth);
	else TextDrawTextSize(textid, startWidth, lineHeight);
	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock GlitchTextDraw(Text:textid, const text[] = "", intensity = 2, duration = 3000, extra_id = 0, const callback[] = "")
{
	if (_:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		slot = TDFX_Alloc();
		if (slot == -1) return 0;
	}
	if (duration < 0) duration = 0;
	if (intensity < 1) intensity = 1;
	if (intensity > 5) intensity = 5;

	new baseCol = 0xFFFFFFFF;
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid) && TDFX_GRemText[_:textid])
			baseCol = TDFX_GRemText[_:textid];
	#endif
	#if defined _TDFX_TDGetColour
		baseCol = _TDFX_TDGetColour(textid);
	#endif

	TDFX_Used[slot] = true;
	TDFX_IsGlobal[slot] = true;
	TDFX_Type[slot] = TDFX_TYPE_GLITCH;
	TDFX_Player[slot] = INVALID_PLAYER_ID;
	TDFX_Td[slot] = _:textid;
	TDFX_Duration[slot] = duration;
	TDFX_Step[slot] = intensity;
	TDFX_Base[slot] = baseCol;
	TDFX_ExtraId[slot] = extra_id;
	strmid(TDFX_Callback[slot], callback, 0, strlen(callback), 32);

	if (text[0])
	{
		strmid(TDFX_Text[slot], text, 0, strlen(text), TDFX_MAX_TEXT);
	}
	else
	{
		#if defined TextDrawGetString
			TextDrawGetString(textid, TDFX_Text[slot], TDFX_MAX_TEXT);
		#else
			TDFX_Text[slot][0] = 0;
		#endif
	}

	TDFX_AutoHide[slot] = false;
	TDFX_AutoDestroy[slot] = false;
	TDFX_Loop[slot] = (duration == 0);
	TDFX_Start[slot] = GetTickCount();

	TextDrawShowForAll(textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", TDFX_FRAME_INTERVAL, true, "i", slot);
	return 1;
}

stock StopTextDrawFX(Text:textid)
{
	new slot = TDFX_FindGlobal(textid);
	if (slot == -1) return 0;
	TDFX_Free(slot, true);
	return 1;
}

stock bool:IsTextDrawFXActive(Text:textid)
{
	return TDFX_FindGlobal(textid) != -1;
}

// -----------------------------------------------------------------------------
// Intercepted Lifecycle & Styling Hooks (ALS)
// -----------------------------------------------------------------------------

stock PlayerText:TDFX_CreatePlayerTextDraw(playerid, Float:x, Float:y, const text[])
{
	new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, text);
	if (TDFX_ValidPlayer(playerid, td))
	{
		TDFX_RemPosX[playerid][_:td] = x;
		TDFX_RemPosY[playerid][_:td] = y;
	}
	return td;
}
#if defined _ALS_CreatePlayerTextDraw
	#undef CreatePlayerTextDraw
#else
	#define _ALS_CreatePlayerTextDraw
#endif
#define CreatePlayerTextDraw TDFX_CreatePlayerTextDraw

#if defined PlayerTextDrawSetPos
stock TDFX_PlayerTextDrawSetPos(playerid, PlayerText:textid, Float:x, Float:y)
{
	if (TDFX_ValidPlayer(playerid, textid))
	{
		TDFX_RemPosX[playerid][_:textid] = x;
		TDFX_RemPosY[playerid][_:textid] = y;
	}
	return PlayerTextDrawSetPos(playerid, textid, x, y);
}
#if defined _ALS_PlayerTextDrawSetPos
	#undef PlayerTextDrawSetPos
#else
	#define _ALS_PlayerTextDrawSetPos
#endif
#define PlayerTextDrawSetPos TDFX_PlayerTextDrawSetPos
#endif

stock TDFX_PlayerTextDrawTextSize(playerid, PlayerText:textid, Float:x, Float:y)
{
	if (TDFX_ValidPlayer(playerid, textid))
	{
		TDFX_RemSizeX[playerid][_:textid] = x;
		TDFX_RemSizeY[playerid][_:textid] = y;
	}
	return PlayerTextDrawTextSize(playerid, textid, x, y);
}
#if defined _ALS_PlayerTextDrawTextSize
	#undef PlayerTextDrawTextSize
#else
	#define _ALS_PlayerTextDrawTextSize
#endif
#define PlayerTextDrawTextSize TDFX_PlayerTextDrawTextSize

stock Text:TDFX_TextDrawCreate(Float:x, Float:y, const text[])
{
	new Text:td = TextDrawCreate(x, y, text);
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(td))
		{
			TDFX_GRemPosX[_:td] = x;
			TDFX_GRemPosY[_:td] = y;
		}
	#endif
	return td;
}
#if defined _ALS_TextDrawCreate
	#undef TextDrawCreate
#else
	#define _ALS_TextDrawCreate
#endif
#define TextDrawCreate TDFX_TextDrawCreate

#if defined TextDrawSetPos
stock TDFX_TextDrawSetPos(Text:textid, Float:x, Float:y)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			TDFX_GRemPosX[_:textid] = x;
			TDFX_GRemPosY[_:textid] = y;
		}
	#endif
	return TextDrawSetPos(textid, x, y);
}
#if defined _ALS_TextDrawSetPos
	#undef TextDrawSetPos
#else
	#define _ALS_TextDrawSetPos
#endif
#define TextDrawSetPos TDFX_TextDrawSetPos
#endif

stock TDFX_TextDrawTextSize(Text:textid, Float:x, Float:y)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			TDFX_GRemSizeX[_:textid] = x;
			TDFX_GRemSizeY[_:textid] = y;
		}
	#endif
	return TextDrawTextSize(textid, x, y);
}
#if defined _ALS_TextDrawTextSize
	#undef TextDrawTextSize
#else
	#define _ALS_TextDrawTextSize
#endif
#define TextDrawTextSize TDFX_TextDrawTextSize

#if defined TextDrawGetAlignment
stock TDFX_TextDrawAlignment(Text:textid, {TEXT_DRAW_ALIGN, _}:alignment)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemAlign[_:textid] = _:alignment;
	#endif
	return TextDrawAlignment(textid, TEXT_DRAW_ALIGN:alignment);
}
#else
stock TDFX_TextDrawAlignment(Text:textid, alignment)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemAlign[_:textid] = alignment;
	#endif
	return TextDrawAlignment(textid, alignment);
}
#endif
#if defined _ALS_TextDrawAlignment
	#undef TextDrawAlignment
#else
	#define _ALS_TextDrawAlignment
#endif
#define TextDrawAlignment TDFX_TextDrawAlignment

stock TDFX_PlayerTextDrawColor(playerid, PlayerText:textid, colour)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemText[playerid][_:textid] = colour;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_Colour[slot])
	{
		TDFX_Base[slot] = colour;
		return 1;
	}
	#if defined PlayerTextDrawColour
		return PlayerTextDrawColour(playerid, textid, colour);
	#else
		return PlayerTextDrawColor(playerid, textid, colour);
	#endif
}
#if defined _ALS_PlayerTextDrawColor
	#undef PlayerTextDrawColor
#else
	#define _ALS_PlayerTextDrawColor
#endif
#define PlayerTextDrawColor TDFX_PlayerTextDrawColor

#if defined PlayerTextDrawColour
stock TDFX_PlayerTextDrawColour(playerid, PlayerText:textid, colour)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemText[playerid][_:textid] = colour;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_Colour[slot])
	{
		TDFX_Base[slot] = colour;
		return 1;
	}
	return PlayerTextDrawColour(playerid, textid, colour);
}
#if defined _ALS_PlayerTextDrawColour
	#undef PlayerTextDrawColour
#else
	#define _ALS_PlayerTextDrawColour
#endif
#define PlayerTextDrawColour TDFX_PlayerTextDrawColour
#endif

stock TDFX_PlayerTextDrawBoxColor(playerid, PlayerText:textid, colour)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemBox[playerid][_:textid] = colour;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_BoxBase[slot])
	{
		TDFX_BoxBase[slot] = colour;
		return 1;
	}
	#if defined PlayerTextDrawBoxColour
		return PlayerTextDrawBoxColour(playerid, textid, colour);
	#else
		return PlayerTextDrawBoxColor(playerid, textid, colour);
	#endif
}
#if defined _ALS_PlayerTextDrawBoxColor
	#undef PlayerTextDrawBoxColor
#else
	#define _ALS_PlayerTextDrawBoxColor
#endif
#define PlayerTextDrawBoxColor TDFX_PlayerTextDrawBoxColor

#if defined PlayerTextDrawBoxColour
stock TDFX_PlayerTextDrawBoxColour(playerid, PlayerText:textid, colour)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemBox[playerid][_:textid] = colour;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_BoxBase[slot])
	{
		TDFX_BoxBase[slot] = colour;
		return 1;
	}
	return PlayerTextDrawBoxColour(playerid, textid, colour);
}
#if defined _ALS_PlayerTextDrawBoxColour
	#undef PlayerTextDrawBoxColour
#else
	#define _ALS_PlayerTextDrawBoxColour
#endif
#define PlayerTextDrawBoxColour TDFX_PlayerTextDrawBoxColour
#endif

stock TDFX_PlayerTextDrawBackgroundColor(playerid, PlayerText:textid, colour)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemBack[playerid][_:textid] = colour;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_Preview[slot])
	{
		TDFX_Base[slot] = colour;
		return 1;
	}
	#if defined PlayerTextDrawBackgroundColour
		return PlayerTextDrawBackgroundColour(playerid, textid, colour);
	#else
		return PlayerTextDrawBackgroundColor(playerid, textid, colour);
	#endif
}
#if defined _ALS_PlayerTextDrawBackgroundColor
	#undef PlayerTextDrawBackgroundColor
#else
	#define _ALS_PlayerTextDrawBackgroundColor
#endif
#define PlayerTextDrawBackgroundColor TDFX_PlayerTextDrawBackgroundColor

#if defined PlayerTextDrawBackgroundColour
stock TDFX_PlayerTextDrawBackgroundColour(playerid, PlayerText:textid, colour)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemBack[playerid][_:textid] = colour;
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_Preview[slot])
	{
		TDFX_Base[slot] = colour;
		return 1;
	}
	return PlayerTextDrawBackgroundColour(playerid, textid, colour);
}
#if defined _ALS_PlayerTextDrawBackgroundColour
	#undef PlayerTextDrawBackgroundColour
#else
	#define _ALS_PlayerTextDrawBackgroundColour
#endif
#define PlayerTextDrawBackgroundColour TDFX_PlayerTextDrawBackgroundColour
#endif

stock TDFX_PlayerTextDrawLetterSize(playerid, PlayerText:textid, Float:x, Float:y)
{
	if (TDFX_ValidPlayer(playerid, textid))
	{
		TDFX_RemLetX[playerid][_:textid] = x;
		TDFX_RemLetY[playerid][_:textid] = y;
	}
	return PlayerTextDrawLetterSize(playerid, textid, x, y);
}
#if defined _ALS_PlayerTextDrawLetterSize
	#undef PlayerTextDrawLetterSize
#else
	#define _ALS_PlayerTextDrawLetterSize
#endif
#define PlayerTextDrawLetterSize TDFX_PlayerTextDrawLetterSize

#if defined PlayerTextDrawGetFont
stock TDFX_PlayerTextDrawFont(playerid, PlayerText:textid, {TEXT_DRAW_FONT, _}:font)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_Info[playerid][_:textid] = (TDFX_Info[playerid][_:textid] & ~7) | (_:font & 7);
	return PlayerTextDrawFont(playerid, textid, TEXT_DRAW_FONT:font);
}
#else
stock TDFX_PlayerTextDrawFont(playerid, PlayerText:textid, font)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_Info[playerid][_:textid] = (TDFX_Info[playerid][_:textid] & ~7) | (_:font & 7);
	return PlayerTextDrawFont(playerid, textid, font);
}
#endif
#if defined _ALS_PlayerTextDrawFont
	#undef PlayerTextDrawFont
#else
	#define _ALS_PlayerTextDrawFont
#endif
#define PlayerTextDrawFont TDFX_PlayerTextDrawFont

#if defined PlayerTextDrawIsBox
stock TDFX_PlayerTextDrawUseBox(playerid, PlayerText:textid, {bool, _}:use)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_Info[playerid][_:textid] = use ? (TDFX_Info[playerid][_:textid] | 8) : (TDFX_Info[playerid][_:textid] & ~8);
	return PlayerTextDrawUseBox(playerid, textid, bool:use);
}
#else
stock TDFX_PlayerTextDrawUseBox(playerid, PlayerText:textid, {bool, _}:use)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_Info[playerid][_:textid] = use ? (TDFX_Info[playerid][_:textid] | 8) : (TDFX_Info[playerid][_:textid] & ~8);
	return PlayerTextDrawUseBox(playerid, textid, use);
}
#endif
#if defined _ALS_PlayerTextDrawUseBox
	#undef PlayerTextDrawUseBox
#else
	#define _ALS_PlayerTextDrawUseBox
#endif
#define PlayerTextDrawUseBox TDFX_PlayerTextDrawUseBox

#if defined PlayerTextDrawGetAlignment
stock TDFX_PlayerTextDrawAlignment(playerid, PlayerText:textid, {TEXT_DRAW_ALIGN, _}:alignment)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemAlign[playerid][_:textid] = _:alignment;
	return PlayerTextDrawAlignment(playerid, textid, TEXT_DRAW_ALIGN:alignment);
}
#else
stock TDFX_PlayerTextDrawAlignment(playerid, PlayerText:textid, alignment)
{
	if (TDFX_ValidPlayer(playerid, textid)) TDFX_RemAlign[playerid][_:textid] = alignment;
	return PlayerTextDrawAlignment(playerid, textid, alignment);
}
#endif
#if defined _ALS_PlayerTextDrawAlignment
	#undef PlayerTextDrawAlignment
#else
	#define _ALS_PlayerTextDrawAlignment
#endif
#define PlayerTextDrawAlignment TDFX_PlayerTextDrawAlignment

stock TDFX_PlayerTextDrawSetString(playerid, PlayerText:textid, const string[])
{
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1 && (TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_PerChar[slot]))
	{
		strmid(TDFX_Text[slot], string, 0, strlen(string), TDFX_MAX_TEXT);
		TDFX_Count[slot] = TDFX_CountChars(TDFX_Text[slot]);
		if (TDFX_Step[slot] > TDFX_Count[slot]) TDFX_Step[slot] = TDFX_Count[slot];
		return 1;
	}
	return PlayerTextDrawSetString(playerid, textid, string);
}
#if defined _ALS_PlayerTextDrawSetString
	#undef PlayerTextDrawSetString
#else
	#define _ALS_PlayerTextDrawSetString
#endif
#define PlayerTextDrawSetString TDFX_PlayerTextDrawSetString

stock TDFX_PlayerTextDrawHide(playerid, PlayerText:textid)
{
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	return PlayerTextDrawHide(playerid, textid);
}
#if defined _ALS_PlayerTextDrawHide
	#undef PlayerTextDrawHide
#else
	#define _ALS_PlayerTextDrawHide
#endif
#define PlayerTextDrawHide TDFX_PlayerTextDrawHide

stock TDFX_PlayerTextDrawDestroy(playerid, PlayerText:textid)
{
	new slot = TDFX_FindPlayer(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	if (TDFX_ValidPlayer(playerid, textid))
	{
		TDFX_Info[playerid][_:textid] = 0;
		TDFX_RemText[playerid][_:textid] = 0;
		TDFX_RemBox[playerid][_:textid] = 0;
		TDFX_RemBack[playerid][_:textid] = 0;
		TDFX_RemLetX[playerid][_:textid] = 0.0;
		TDFX_RemLetY[playerid][_:textid] = 0.0;
		TDFX_RemPosX[playerid][_:textid] = 0.0;
		TDFX_RemPosY[playerid][_:textid] = 0.0;
		TDFX_RemSizeX[playerid][_:textid] = 0.0;
		TDFX_RemSizeY[playerid][_:textid] = 0.0;
		TDFX_RemAlign[playerid][_:textid] = 1;
	}
	return PlayerTextDrawDestroy(playerid, textid);
}
#if defined _ALS_PlayerTextDrawDestroy
	#undef PlayerTextDrawDestroy
#else
	#define _ALS_PlayerTextDrawDestroy
#endif
#define PlayerTextDrawDestroy TDFX_PlayerTextDrawDestroy

// Global TextDraw Hooks
stock TDFX_TextDrawColor(Text:textid, colour)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemText[_:textid] = colour;
	#endif
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_Colour[slot])
	{
		TDFX_Base[slot] = colour;
		return 1;
	}
	#if defined TextDrawColour
		return TextDrawColour(textid, colour);
	#else
		return TextDrawColor(textid, colour);
	#endif
}
#if defined _ALS_TextDrawColor
	#undef TextDrawColor
#else
	#define _ALS_TextDrawColor
#endif
#define TextDrawColor TDFX_TextDrawColor

#if defined TextDrawColour
stock TDFX_TextDrawColour(Text:textid, colour)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemText[_:textid] = colour;
	#endif
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_Colour[slot])
	{
		TDFX_Base[slot] = colour;
		return 1;
	}
	return TextDrawColour(textid, colour);
}
#if defined _ALS_TextDrawColour
	#undef TextDrawColour
#else
	#define _ALS_TextDrawColour
#endif
#define TextDrawColour TDFX_TextDrawColour
#endif

stock TDFX_TextDrawBoxColor(Text:textid, colour)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemBox[_:textid] = colour;
	#endif
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_BoxBase[slot])
	{
		TDFX_BoxBase[slot] = colour;
		return 1;
	}
	#if defined TextDrawBoxColour
		return TextDrawBoxColour(textid, colour);
	#else
		return TextDrawBoxColor(textid, colour);
	#endif
}
#if defined _ALS_TextDrawBoxColor
	#undef TextDrawBoxColor
#else
	#define _ALS_TextDrawBoxColor
#endif
#define TextDrawBoxColor TDFX_TextDrawBoxColor

#if defined TextDrawBoxColour
stock TDFX_TextDrawBoxColour(Text:textid, colour)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemBox[_:textid] = colour;
	#endif
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1 && TDFX_Type[slot] == TDFX_TYPE_BLINK && TDFX_BoxBase[slot])
	{
		TDFX_BoxBase[slot] = colour;
		return 1;
	}
	return TextDrawBoxColour(textid, colour);
}
#if defined _ALS_TextDrawBoxColour
	#undef TextDrawBoxColour
#else
	#define _ALS_TextDrawBoxColour
#endif
#define TextDrawBoxColour TDFX_TextDrawBoxColour
#endif

stock TDFX_TextDrawBackgroundColor(Text:textid, colour)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemBack[_:textid] = colour;
	#endif
	#if defined TextDrawBackgroundColour
		return TextDrawBackgroundColour(textid, colour);
	#else
		return TextDrawBackgroundColor(textid, colour);
	#endif
}
#if defined _ALS_TextDrawBackgroundColor
	#undef TextDrawBackgroundColor
#else
	#define _ALS_TextDrawBackgroundColor
#endif
#define TextDrawBackgroundColor TDFX_TextDrawBackgroundColor

#if defined TextDrawBackgroundColour
stock TDFX_TextDrawBackgroundColour(Text:textid, colour)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid)) TDFX_GRemBack[_:textid] = colour;
	#endif
	return TextDrawBackgroundColour(textid, colour);
}
#if defined _ALS_TextDrawBackgroundColour
	#undef TextDrawBackgroundColour
#else
	#define _ALS_TextDrawBackgroundColour
#endif
#define TextDrawBackgroundColour TDFX_TextDrawBackgroundColour
#endif

stock TDFX_TextDrawLetterSize(Text:textid, Float:x, Float:y)
{
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			TDFX_GRemLetX[_:textid] = x;
			TDFX_GRemLetY[_:textid] = y;
		}
	#endif
	return TextDrawLetterSize(textid, x, y);
}
#if defined _ALS_TextDrawLetterSize
	#undef TextDrawLetterSize
#else
	#define _ALS_TextDrawLetterSize
#endif
#define TextDrawLetterSize TDFX_TextDrawLetterSize

stock TDFX_TextDrawDestroy(Text:textid)
{
	new slot = TDFX_FindGlobal(textid);
	if (slot != -1) TDFX_Free(slot, false);
	#if TDFX_MAX_GLOBAL_TD > 0
		if (TDFX_ValidGlobal(textid))
		{
			TDFX_GInfo[_:textid] = 0;
			TDFX_GRemText[_:textid] = 0;
			TDFX_GRemBox[_:textid] = 0;
			TDFX_GRemBack[_:textid] = 0;
			TDFX_GRemLetX[_:textid] = 0.0;
			TDFX_GRemLetY[_:textid] = 0.0;
			TDFX_GRemPosX[_:textid] = 0.0;
			TDFX_GRemPosY[_:textid] = 0.0;
			TDFX_GRemSizeX[_:textid] = 0.0;
			TDFX_GRemSizeY[_:textid] = 0.0;
			TDFX_GRemAlign[_:textid] = 1;
		}
	#endif
	return TextDrawDestroy(textid);
}
#if defined _ALS_TextDrawDestroy
	#undef TextDrawDestroy
#else
	#define _ALS_TextDrawDestroy
#endif
#define TextDrawDestroy TDFX_TextDrawDestroy

// -----------------------------------------------------------------------------
// Connection & Disconnection Handling
// -----------------------------------------------------------------------------

public OnPlayerDisconnect(playerid, reason)
{
	if (0 <= playerid < MAX_PLAYERS)
	{
		for (new i = 0; i < TDFX_MAX_FX; i++)
		{
			if (TDFX_Used[i] && !TDFX_IsGlobal[i] && TDFX_Player[i] == playerid)
			{
				TDFX_Free(i, false, false);
			}
		}
		for (new t = 0; t < TDFX_MAX_TD; t++)
		{
			TDFX_Info[playerid][t] = 0;
			TDFX_RemText[playerid][t] = 0;
			TDFX_RemBox[playerid][t] = 0;
			TDFX_RemBack[playerid][t] = 0;
			TDFX_RemLetX[playerid][t] = 0.0;
			TDFX_RemLetY[playerid][t] = 0.0;
			TDFX_RemPosX[playerid][t] = 0.0;
			TDFX_RemPosY[playerid][t] = 0.0;
			TDFX_RemSizeX[playerid][t] = 0.0;
			TDFX_RemSizeY[playerid][t] = 0.0;
			TDFX_RemAlign[playerid][t] = 1;
		}
	}
	#if defined TDFX_OnPlayerDisconnect
		return TDFX_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect TDFX_OnPlayerDisconnect
#if defined TDFX_OnPlayerDisconnect
	forward TDFX_OnPlayerDisconnect(playerid, reason);
#endif

// -----------------------------------------------------------------------------
// Forward declarations for event callbacks
// -----------------------------------------------------------------------------

forward OnPlayerTextDrawFXFinish(playerid, PlayerText:textid, fx_type, extra_id);
forward OnTextDrawFXFinish(Text:textid, fx_type, extra_id);
