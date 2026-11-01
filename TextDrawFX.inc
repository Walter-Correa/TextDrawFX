/*
================================================================================
    TextDrawFX - A high-performance textdraw visual effects, animation, and
    blinking library for SA-MP and Open.MP.

    Author:      Walter Corrêa
    Version:     1.0.0
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

#if !defined TDFX_MAX_BLINKS
	#define TDFX_MAX_BLINKS (32)
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

#if !defined TDFX_MIN_INTERVAL
	#define TDFX_MIN_INTERVAL (50)
#endif

#if defined PlayerTextDrawColour
	#define _TDFX_TDColour PlayerTextDrawColour
#else
	#define _TDFX_TDColour PlayerTextDrawColor
#endif

#if defined PlayerTextDrawBoxColour
	#define _TDFX_TDBoxColour PlayerTextDrawBoxColour
#else
	#define _TDFX_TDBoxColour PlayerTextDrawBoxColor
#endif

#if defined PlayerTextDrawBackgroundColour
	#define _TDFX_TDBackgroundColour PlayerTextDrawBackgroundColour
#else
	#define _TDFX_TDBackgroundColour PlayerTextDrawBackgroundColor
#endif

#if defined PlayerTextDrawGetColour
	#define _TDFX_TDGetColour PlayerTextDrawGetColour
#elseif defined PlayerTextDrawGetColor
	#define _TDFX_TDGetColour PlayerTextDrawGetColor
#endif

#if defined PlayerTextDrawGetBoxColour
	#define _TDFX_TDGetBoxColour PlayerTextDrawGetBoxColour
#elseif defined PlayerTextDrawGetBoxColor
	#define _TDFX_TDGetBoxColour PlayerTextDrawGetBoxColor
#endif

#if defined PlayerTextDrawGetBackgroundColour
	#define _TDFX_TDGetBackgroundColour PlayerTextDrawGetBackgroundColour
#elseif defined PlayerTextDrawGetBackgroundColor
	#define _TDFX_TDGetBackgroundColour PlayerTextDrawGetBackgroundColor
#endif

static stock TDFX_Info[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemText[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemBox[MAX_PLAYERS][TDFX_MAX_TD];
static stock TDFX_RemBack[MAX_PLAYERS][TDFX_MAX_TD];

static stock bool:TDFX_Used[TDFX_MAX_BLINKS];
static stock TDFX_Player[TDFX_MAX_BLINKS];
static stock PlayerText:TDFX_Td[TDFX_MAX_BLINKS];
static stock TDFX_Timer[TDFX_MAX_BLINKS];
static stock bool:TDFX_PerChar[TDFX_MAX_BLINKS];
static stock bool:TDFX_Preview[TDFX_MAX_BLINKS];
static stock TDFX_Step[TDFX_MAX_BLINKS];
static stock TDFX_Start[TDFX_MAX_BLINKS];
static stock TDFX_Duration[TDFX_MAX_BLINKS];
static stock TDFX_Count[TDFX_MAX_BLINKS];
static stock TDFX_Colour[TDFX_MAX_BLINKS];
static stock TDFX_Base[TDFX_MAX_BLINKS];
static stock TDFX_BoxBase[TDFX_MAX_BLINKS];
static stock TDFX_Text[TDFX_MAX_BLINKS][TDFX_MAX_TEXT];

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

static stock TDFX_Find(playerid, PlayerText:textid)
{
	for (new i = 0; i < TDFX_MAX_BLINKS; i++)
	{
		if (TDFX_Used[i] && TDFX_Player[i] == playerid && TDFX_Td[i] == textid) return i;
	}
	return -1;
}

static stock TDFX_Valid(playerid, PlayerText:textid)
{
	return 0 <= playerid < MAX_PLAYERS && 0 <= _:textid < TDFX_MAX_TD;
}

static stock bool:TDFX_Intercept(playerid, PlayerText:textid, which, colour)
{
	new slot = TDFX_Find(playerid, textid);
	if (slot == -1 || !TDFX_Colour[slot]) return false;
	new bool:lit = TDFX_PerChar[slot] || TDFX_Step[slot] != 0;
	if (which == 2)
	{
		if (!TDFX_Preview[slot]) return false;
		TDFX_Base[slot] = colour;
	}
	else if (which == 1)
	{
		if (TDFX_Preview[slot] || !TDFX_BoxBase[slot]) return false;
		TDFX_BoxBase[slot] = colour;
	}
	else
	{
		if (TDFX_Preview[slot]) return false;
		TDFX_Base[slot] = colour;
	}
	return lit;
}

static stock TDFX_Draw(slot)
{
	new part[TDFX_MAX_TEXT], cut = TDFX_Cut(TDFX_Text[slot], TDFX_Step[slot]);
	if (TDFX_Step[slot] >= TDFX_Count[slot]) cut = strlen(TDFX_Text[slot]);
	if (cut > 0) strmid(part, TDFX_Text[slot], 0, cut, sizeof part);
	else
	{
		part[0] = ' ';
		part[1] = 0;
	}
	PlayerTextDrawSetString(TDFX_Player[slot], TDFX_Td[slot], part);
}

static stock TDFX_Paint(slot, bool:lit)
{
	new playerid = TDFX_Player[slot], PlayerText:td = TDFX_Td[slot];
	new colour = lit ? TDFX_Colour[slot] : TDFX_Base[slot];
	if (TDFX_Preview[slot]) _TDFX_TDBackgroundColour(playerid, td, colour);
	else
	{
		_TDFX_TDColour(playerid, td, colour);
		if (TDFX_BoxBase[slot]) _TDFX_TDBoxColour(playerid, td, lit ? TDFX_Colour[slot] : TDFX_BoxBase[slot]);
	}
	PlayerTextDrawShow(playerid, td);
}

static stock TDFX_Free(slot, bool:restore)
{
	if (!TDFX_Used[slot]) return;
	KillTimer(TDFX_Timer[slot]);
	TDFX_Used[slot] = false;
	new playerid = TDFX_Player[slot];
	if (restore && IsPlayerConnected(playerid))
	{
		if (TDFX_PerChar[slot]) PlayerTextDrawSetString(playerid, TDFX_Td[slot], TDFX_Text[slot]);
		if (TDFX_Colour[slot]) TDFX_Paint(slot, false);
		else PlayerTextDrawShow(playerid, TDFX_Td[slot]);
	}
}

forward TDFX_Tick(slot);
public TDFX_Tick(slot)
{
	if (slot < 0 || slot >= TDFX_MAX_BLINKS || !TDFX_Used[slot]) return 0;
	new playerid = TDFX_Player[slot];
	if (!IsPlayerConnected(playerid))
	{
		TDFX_Free(slot, false);
		return 0;
	}
	if (TDFX_Duration[slot] > 0 && GetTickCount() - TDFX_Start[slot] >= TDFX_Duration[slot])
	{
		TDFX_Free(slot, true);
		return 1;
	}
	if (TDFX_PerChar[slot])
	{
		TDFX_Step[slot]++;
		if (TDFX_Step[slot] > TDFX_Count[slot]) TDFX_Step[slot] = 0;
		TDFX_Draw(slot);
	}
	else
	{
		TDFX_Step[slot] = TDFX_Step[slot] ? 0 : 1;
		if (TDFX_Colour[slot]) TDFX_Paint(slot, TDFX_Step[slot] != 0);
		else if (TDFX_Step[slot]) PlayerTextDrawShow(playerid, TDFX_Td[slot]);
		else PlayerTextDrawHide(playerid, TDFX_Td[slot]);
	}
	return 1;
}

stock BlinkPlayerTextDraw(playerid, PlayerText:textid, interval = 500, duration = 0, bool:perChar = false, colour = 0, const text[] = "", baseColour = 0)
{
	if (!IsPlayerConnected(playerid) || _:textid == 0xFFFF || _:textid < 0) return 0;
	new slot = TDFX_Find(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	else
	{
		for (new i = 0; i < TDFX_MAX_BLINKS; i++)
		{
			if (!TDFX_Used[i])
			{
				slot = i;
				break;
			}
		}
		if (slot == -1) return 0;
	}
	if (interval < TDFX_MIN_INTERVAL) interval = TDFX_MIN_INTERVAL;
	if (duration < 0) duration = 0;
	new bool:known = (0 <= _:textid < TDFX_MAX_TD), font = 1, bool:box = false;
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
			#if defined _TDFX_TDGetBackgroundColour
				if (!baseColour) baseColour = _TDFX_TDGetBackgroundColour(playerid, textid);
			#endif
			if (!baseColour && known) baseColour = TDFX_RemBack[playerid][_:textid];
		}
		else
		{
			#if defined _TDFX_TDGetColour
				if (!baseColour) baseColour = _TDFX_TDGetColour(playerid, textid);
			#endif
			if (!baseColour && known) baseColour = TDFX_RemText[playerid][_:textid];
			if (box)
			{
				#if defined _TDFX_TDGetBoxColour
					boxBase = _TDFX_TDGetBoxColour(playerid, textid);
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
	TDFX_Player[slot] = playerid;
	TDFX_Td[slot] = textid;
	TDFX_PerChar[slot] = perChar;
	TDFX_Preview[slot] = preview;
	TDFX_Duration[slot] = duration;
	TDFX_Colour[slot] = colour;
	TDFX_Base[slot] = baseColour;
	TDFX_BoxBase[slot] = boxBase;
	TDFX_Start[slot] = GetTickCount();
	if (perChar)
	{
		TDFX_Step[slot] = 0;
		TDFX_Draw(slot);
	}
	else TDFX_Step[slot] = 1;
	if (colour) TDFX_Paint(slot, true);
	else PlayerTextDrawShow(playerid, textid);
	TDFX_Timer[slot] = SetTimerEx("TDFX_Tick", interval, true, "i", slot);
	return 1;
}

stock StopPlayerTextDrawBlink(playerid, PlayerText:textid)
{
	new slot = TDFX_Find(playerid, textid);
	if (slot == -1) return 0;
	TDFX_Free(slot, true);
	return 1;
}

stock bool:IsPlayerTextDrawBlinking(playerid, PlayerText:textid)
{
	return TDFX_Find(playerid, textid) != -1;
}

stock TDFX_PlayerTextDrawColor(playerid, PlayerText:textid, colour)
{
	if (TDFX_Valid(playerid, textid)) TDFX_RemText[playerid][_:textid] = colour;
	if (TDFX_Intercept(playerid, textid, 0, colour)) return 1;
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

stock TDFX_PlayerTextDrawBoxColor(playerid, PlayerText:textid, colour)
{
	if (TDFX_Valid(playerid, textid)) TDFX_RemBox[playerid][_:textid] = colour;
	if (TDFX_Intercept(playerid, textid, 1, colour)) return 1;
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

stock TDFX_PlayerTextDrawBackgroundColor(playerid, PlayerText:textid, colour)
{
	if (TDFX_Valid(playerid, textid)) TDFX_RemBack[playerid][_:textid] = colour;
	if (TDFX_Intercept(playerid, textid, 2, colour)) return 1;
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

#if defined PlayerTextDrawColour
stock TDFX_PlayerTextDrawColour(playerid, PlayerText:textid, colour)
{
	if (TDFX_Valid(playerid, textid)) TDFX_RemText[playerid][_:textid] = colour;
	if (TDFX_Intercept(playerid, textid, 0, colour)) return 1;
	return PlayerTextDrawColour(playerid, textid, colour);
}
#if defined _ALS_PlayerTextDrawColour
	#undef PlayerTextDrawColour
#else
	#define _ALS_PlayerTextDrawColour
#endif
#define PlayerTextDrawColour TDFX_PlayerTextDrawColour
#endif

#if defined PlayerTextDrawBoxColour
stock TDFX_PlayerTextDrawBoxColour(playerid, PlayerText:textid, colour)
{
	if (TDFX_Valid(playerid, textid)) TDFX_RemBox[playerid][_:textid] = colour;
	if (TDFX_Intercept(playerid, textid, 1, colour)) return 1;
	return PlayerTextDrawBoxColour(playerid, textid, colour);
}
#if defined _ALS_PlayerTextDrawBoxColour
	#undef PlayerTextDrawBoxColour
#else
	#define _ALS_PlayerTextDrawBoxColour
#endif
#define PlayerTextDrawBoxColour TDFX_PlayerTextDrawBoxColour
#endif

#if defined PlayerTextDrawBackgroundColour
stock TDFX_PlayerTextDrawBackgroundColour(playerid, PlayerText:textid, colour)
{
	if (TDFX_Valid(playerid, textid)) TDFX_RemBack[playerid][_:textid] = colour;
	if (TDFX_Intercept(playerid, textid, 2, colour)) return 1;
	return PlayerTextDrawBackgroundColour(playerid, textid, colour);
}
#if defined _ALS_PlayerTextDrawBackgroundColour
	#undef PlayerTextDrawBackgroundColour
#else
	#define _ALS_PlayerTextDrawBackgroundColour
#endif
#define PlayerTextDrawBackgroundColour TDFX_PlayerTextDrawBackgroundColour
#endif

#if defined PlayerTextDrawGetFont
stock TDFX_PlayerTextDrawFont(playerid, PlayerText:textid, {TEXT_DRAW_FONT, _}:font)
{
	if (TDFX_Valid(playerid, textid)) TDFX_Info[playerid][_:textid] = (TDFX_Info[playerid][_:textid] & ~7) | (_:font & 7);
	return PlayerTextDrawFont(playerid, textid, TEXT_DRAW_FONT:font);
}
#else
stock TDFX_PlayerTextDrawFont(playerid, PlayerText:textid, font)
{
	if (TDFX_Valid(playerid, textid)) TDFX_Info[playerid][_:textid] = (TDFX_Info[playerid][_:textid] & ~7) | (_:font & 7);
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
	if (TDFX_Valid(playerid, textid)) TDFX_Info[playerid][_:textid] = use ? (TDFX_Info[playerid][_:textid] | 8) : (TDFX_Info[playerid][_:textid] & ~8);
	return PlayerTextDrawUseBox(playerid, textid, bool:use);
}
#else
stock TDFX_PlayerTextDrawUseBox(playerid, PlayerText:textid, {bool, _}:use)
{
	if (TDFX_Valid(playerid, textid)) TDFX_Info[playerid][_:textid] = use ? (TDFX_Info[playerid][_:textid] | 8) : (TDFX_Info[playerid][_:textid] & ~8);
	return PlayerTextDrawUseBox(playerid, textid, use);
}
#endif
#if defined _ALS_PlayerTextDrawUseBox
	#undef PlayerTextDrawUseBox
#else
	#define _ALS_PlayerTextDrawUseBox
#endif
#define PlayerTextDrawUseBox TDFX_PlayerTextDrawUseBox

stock TDFX_PlayerTextDrawSetString(playerid, PlayerText:textid, const string[])
{
	new slot = TDFX_Find(playerid, textid);
	if (slot != -1 && TDFX_PerChar[slot])
	{
		strmid(TDFX_Text[slot], string, 0, strlen(string), TDFX_MAX_TEXT);
		TDFX_Count[slot] = TDFX_CountChars(TDFX_Text[slot]);
		if (TDFX_Step[slot] > TDFX_Count[slot]) TDFX_Step[slot] = TDFX_Count[slot];
		TDFX_Draw(slot);
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
	new slot = TDFX_Find(playerid, textid);
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
	new slot = TDFX_Find(playerid, textid);
	if (slot != -1) TDFX_Free(slot, false);
	if (TDFX_Valid(playerid, textid))
	{
		TDFX_Info[playerid][_:textid] = 0;
		TDFX_RemText[playerid][_:textid] = 0;
		TDFX_RemBox[playerid][_:textid] = 0;
		TDFX_RemBack[playerid][_:textid] = 0;
	}
	return PlayerTextDrawDestroy(playerid, textid);
}
#if defined _ALS_PlayerTextDrawDestroy
	#undef PlayerTextDrawDestroy
#else
	#define _ALS_PlayerTextDrawDestroy
#endif
#define PlayerTextDrawDestroy TDFX_PlayerTextDrawDestroy

public OnPlayerDisconnect(playerid, reason)
{
	if (0 <= playerid < MAX_PLAYERS)
	{
		for (new i = 0; i < TDFX_MAX_BLINKS; i++)
		{
			if (TDFX_Used[i] && TDFX_Player[i] == playerid) TDFX_Free(i, false);
		}
		for (new t = 0; t < TDFX_MAX_TD; t++)
		{
			TDFX_Info[playerid][t] = 0;
			TDFX_RemText[playerid][t] = 0;
			TDFX_RemBox[playerid][t] = 0;
			TDFX_RemBack[playerid][t] = 0;
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
