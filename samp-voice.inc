//Libraries:
#include <a_samp>
#include <YSI_Coding\y_hooks>

//Macros:
#define MAX_VOICE_RANGE (30.0)

//Variables
new
	g_aLangData[][][] =
	{
		{ "Turn Off", "" },
		{ "English", "en-us" },
		{ "Portuguese", "pt-br" }
	},
	g_szLangList[768];

//Callbacks:
hook OnGameModeInit()
{
	for(new i; i < sizeof(g_aLangData); ++i)
	{
		strcat(g_szLangList, g_aLangData[i][0]);
		strcat(g_szLangList, "\n");
	}

	return 1;
}

hook OnPlayerText(playerid, text[])
{
	SendVoiceMessage(playerid, text, g_aLangData[GetPVarInt(playerid, "iLangIdx")][1]);
	return 1;
}

hook OnPlayerCommandText(playerid, cmdtext[])
{
	if(!strcmp(cmdtext, "/language", true))
	{
		ShowPlayerDialog(playerid, 10020, DIALOG_STYLE_LIST, "Choose the Language!", g_szLangList, "Select", "");
		return 1;
	}
	return 0;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(response)
	{
		switch(dialogid)
		{
			case 10020:
			{
				if(0 <= listitem < sizeof(g_aLangData))
				{
					SetPVarInt(playerid, "iLangIdx", listitem);

					static
						szStr[128];

					if(!listitem)
						SendClientMessage(playerid, 0xFF0000FF, "[INFO] You turned off the Text-To-Speech!");

					else
						format(szStr, sizeof(szStr), "[INFO] You changed language to %s", g_aLangData[listitem][0]),
						SendClientMessage(playerid, -1, szStr);
				}
			}
		}
	}
	return 1;
}

stock SendVoiceMessage(playerid, szMsg[], const szLang[])
{
	new
		Float: fPos[3];

	if(GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]) && szLang[0] != EOS)
	{
		static
			szUrl[60 + 128];

		format(szUrl, sizeof(szUrl), "https://translate.google.com/translate_tts?ie=UTF-8&tl=%s&client=tw-ob&q=%s", szLang, szMsg);

		for(new i, j = GetMaxPlayers(); i < j; ++i)
		{
			if(PlayAudioStreamForPlayer(i, szUrl, fPos[0], fPos[1], fPos[2], MAX_VOICE_RANGE, true))
			{
				continue;
			}
		}
		return 1;
	}
	return 0;
}