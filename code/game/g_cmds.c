/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
#include "g_local.h"

#include "../../ui/menudef.h"			// for the voice chats

/*
==================
DeathmatchScoreboardMessage

==================
*/
void DeathmatchScoreboardMessage( gentity_t *ent ) {
	char		entry[1024];
	char		string[1400];
	int			stringlength;
	int			i, j;
	gclient_t	*cl;
	int			numSorted, scoreFlags, accuracy, perfect;
	
	if(g_scoreboardlock.integer){
	return;
	}

	// send the latest information on all clients
	string[0] = 0;
	stringlength = 0;
	scoreFlags = 0;

	numSorted = level.numConnectedClients;

	for (i=0 ; i < numSorted ; i++) {
		int		ping;

		cl = &level.clients[level.sortedClients[i]];

		if ( cl->pers.connected == CON_CONNECTING ) {
			ping = -1;
		} else {
//unlagged - true ping
			//ping = cl->ps.ping < 999 ? cl->ps.ping : 999;
			ping = cl->pers.realPing < 999 ? cl->pers.realPing : 999;
//unlagged - true ping
		}

		if( cl->accuracy_shots ) {
			accuracy = cl->accuracy_hits * 100 / cl->accuracy_shots;
		}
		else {
			accuracy = 0;
		}
		perfect = ( cl->ps.persistant[PERS_RANK] == 0 && cl->ps.persistant[PERS_KILLED] == 0 ) ? 1 : 0;

		if(g_gametype.integer == GT_LMS) {
			Com_sprintf (entry, sizeof(entry),
				" %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i", level.sortedClients[i],
				cl->ps.persistant[PERS_SCORE], ping, (level.time - cl->pers.enterTime)/60000,
				scoreFlags, g_entities[level.sortedClients[i]].s.powerups, accuracy,
				cl->ps.persistant[PERS_IMPRESSIVE_COUNT],
				cl->ps.persistant[PERS_EXCELLENT_COUNT],
				cl->ps.persistant[PERS_GAUNTLET_FRAG_COUNT],
				cl->ps.persistant[PERS_DEFEND_COUNT],
				cl->ps.persistant[PERS_ASSIST_COUNT],
				perfect,
				cl->ps.persistant[PERS_CAPTURES],
				cl->pers.livesLeft + (cl->isEliminated?0:1));
		}
		else {
			Com_sprintf (entry, sizeof(entry),
				" %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i", level.sortedClients[i],
				cl->ps.persistant[PERS_SCORE], ping, (level.time - cl->pers.enterTime)/60000,
				scoreFlags, g_entities[level.sortedClients[i]].s.powerups, accuracy,
				cl->ps.persistant[PERS_IMPRESSIVE_COUNT],
				cl->ps.persistant[PERS_EXCELLENT_COUNT],
				cl->ps.persistant[PERS_GAUNTLET_FRAG_COUNT],
				cl->ps.persistant[PERS_DEFEND_COUNT],
				cl->ps.persistant[PERS_ASSIST_COUNT],
				perfect,
				cl->ps.persistant[PERS_CAPTURES],
				cl->isEliminated);
		}
		j = strlen(entry);
		if (stringlength + j > 1024)
			break;
		strcpy (string + stringlength, entry);
		stringlength += j;
	}

	trap_SendServerCommand( ent-g_entities, va("scores %i %i %i %i%s", i,
		level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE], level.roundStartTime,
		string ) );
}

void G_SendWeaponProperties(gentity_t *ent) {
	char string[2048];
	Com_sprintf(string, sizeof(string), "%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
	            mod_sgspread, mod_sgcount, mod_lgrange, mod_mgspread, mod_cgspread, mod_jumpheight, mod_gdelay, mod_mgdelay ,mod_sgdelay, mod_gldelay, mod_rldelay, mod_lgdelay, mod_pgdelay, mod_rgdelay, mod_bfgdelay, mod_ngdelay, mod_pldelay, mod_cgdelay, mod_ftdelay, mod_scoutfirespeed, mod_ammoregenfirespeed, mod_doublerfirespeed, mod_guardfirespeed, mod_hastefirespeed, mod_noplayerclip, mod_ammolimit, mod_invulmove, mod_amdelay, mod_teamred_firespeed, mod_teamblue_firespeed, mod_medkitlimit, mod_medkitinf, mod_teleporterinf, mod_portalinf, mod_kamikazeinf, mod_invulinf, mod_accelerate, mod_slickmove, mod_overlay, mod_roundmode, mod_zround, mod_gravity, mod_dayangle, mod_daydefault);
	trap_SendServerCommand(ent-g_entities, va( "weaponProperties %s", string));
}


/*
==================
AccMessage

==================
*/
void AccMessage( gentity_t *ent ) {
	char		entry[1024];

	Com_sprintf (entry, sizeof(entry),
				" %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i ",
                                   ent->client->accuracy[WP_MACHINEGUN][0], ent->client->accuracy[WP_MACHINEGUN][1],
                                  ent->client->accuracy[WP_SHOTGUN][0], ent->client->accuracy[WP_SHOTGUN][1],
                                  ent->client->accuracy[WP_GRENADE_LAUNCHER][0], ent->client->accuracy[WP_GRENADE_LAUNCHER][1],
                                  ent->client->accuracy[WP_ROCKET_LAUNCHER][0], ent->client->accuracy[WP_ROCKET_LAUNCHER][1],
                                  ent->client->accuracy[WP_LIGHTNING][0], ent->client->accuracy[WP_LIGHTNING][1],
                                  ent->client->accuracy[WP_RAILGUN][0], ent->client->accuracy[WP_RAILGUN][1],
                                  ent->client->accuracy[WP_PLASMAGUN][0], ent->client->accuracy[WP_PLASMAGUN][1],
                                  ent->client->accuracy[WP_BFG][0], ent->client->accuracy[WP_BFG][1],
                                   0,0, //Hook
                                    ent->client->accuracy[WP_NAILGUN][0], ent->client->accuracy[WP_NAILGUN][1],
                                    0,0,
                                    ent->client->accuracy[WP_CHAINGUN][0], ent->client->accuracy[WP_CHAINGUN][1]
                                 );

	trap_SendServerCommand( ent-g_entities, va("accs%s", entry ));
}


/*
==================
DominationPointStatusMessage

==================
*/
void DominationPointStatusMessage( gentity_t *ent ) {
	char		entry[10]; //Will more likely be 2... in fact cannot be more since we are the server
	char		string[10*(MAX_DOMINATION_POINTS+1)];
	int			stringlength;
	int i, j;

	string[0] = 0;
	stringlength = 0;

	for(i = 0;i<MAX_DOMINATION_POINTS && i<level.domination_points_count; i++) {
		Com_sprintf (entry, sizeof(entry)," %i",level.pointStatusDom[i]);
		j = strlen(entry);
		if (stringlength + j > 10*MAX_DOMINATION_POINTS)
			break;
		strcpy (string + stringlength, entry);
		stringlength += j;
	}

	trap_SendServerCommand( ent-g_entities, va("domStatus %i%s", level.domination_points_count, string ) );
}

/*
==================
EliminationMessage

==================
*/

void EliminationMessage(gentity_t *ent) {
	trap_SendServerCommand( ent-g_entities, va("elimination %i %i %i",
		level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE], level.roundStartTime) );
}

void RespawnTimeMessage(gentity_t *ent, int time) {
    trap_SendServerCommand( ent-g_entities, va("respawn %i", time) );
}

/*
==================
DoubleDominationScoreTime

==================
*/
void DoubleDominationScoreTimeMessage( gentity_t *ent ) {
	trap_SendServerCommand( ent-g_entities, va("ddtaken %i", level.timeTaken));
}

/*
==================
DominationPointNames
==================
*/

void DominationPointNamesMessage( gentity_t *ent ) {
	char text[MAX_DOMINATION_POINTS_NAMES*MAX_DOMINATION_POINTS];
	int i,j;
	qboolean nullFound;
	for(i=0;i<MAX_DOMINATION_POINTS;i++) {
		Q_strncpyz(text+i*MAX_DOMINATION_POINTS_NAMES,level.domination_points_names[i],MAX_DOMINATION_POINTS_NAMES-1);
		if(i!=MAX_DOMINATION_POINTS-1) {
			//Don't allow "/0"!
			nullFound = qfalse;
			for(j=i*MAX_DOMINATION_POINTS_NAMES; j<(i+1)*MAX_DOMINATION_POINTS_NAMES;j++) {
				if(text[j]==0)
					nullFound = qtrue;
				if(nullFound)
					text[j] = ' ';
			}
		}
		text[MAX_DOMINATION_POINTS_NAMES*MAX_DOMINATION_POINTS-2]=0x19;
		text[MAX_DOMINATION_POINTS_NAMES*MAX_DOMINATION_POINTS-1]=0;
	}
	trap_SendServerCommand( ent-g_entities, va("dompointnames %i \"%s\"", level.domination_points_count, text));
}

/*
==================
YourTeamMessage
==================
*/

void YourTeamMessage( gentity_t *ent) {
    int team = level.clients[ent-g_entities].sess.sessionTeam;

    switch(team) {
        case TEAM_RED:
            trap_SendServerCommand( ent-g_entities, va("team \"%s\"", g_redTeamClientNumbers.string));
            break;
        case TEAM_BLUE:
            trap_SendServerCommand( ent-g_entities, va("team \"%s\"", g_blueTeamClientNumbers.string));
            break;
        default:
            trap_SendServerCommand( ent-g_entities, "team \"all\"");
    };
}

/*
==================
AttackingTeamMessage

==================
*/
void AttackingTeamMessage( gentity_t *ent ) {
	int team;
	if ( (level.eliminationSides+level.roundNumber)%2 == 0 )
		team = TEAM_RED;
	else
		team = TEAM_BLUE;
	trap_SendServerCommand( ent-g_entities, va("attackingteam %i", team));
}

/*

 */

void ObeliskHealthMessage() {
    if(level.MustSendObeliskHealth) {
        trap_SendServerCommand( -1, va("oh %i %i",level.healthRedObelisk,level.healthBlueObelisk) );
        level.MustSendObeliskHealth = qfalse;
    }
}

/*
==================
ChallengeMessage

==================
*/

void ChallengeMessage(gentity_t *ent, int challenge) {
        if ( level.warmupTime != 0)
		return; //We don't send anything doring warmup
	trap_SendServerCommand( ent-g_entities, va("ch %u", challenge) );
        G_LogPrintf( "Challenge: %i %i %i: Client %i got award %i\n",ent-g_entities,challenge,1,ent-g_entities,challenge);
}

/*
==================
SendCustomVoteCommands

==================
*/

void SendCustomVoteCommands(int clientNum) {
	trap_SendServerCommand( clientNum, va("customvotes %s", custom_vote_info) );
}

/*
==================
Cmd_Score_f

Request current scoreboard information
==================
*/
void Cmd_Score_f( gentity_t *ent ) {
	DeathmatchScoreboardMessage( ent );
}


/*
==================
 Cmd_Acc_f
 Request current scoreboard information
==================
*/
void Cmd_Acc_f( gentity_t *ent ) {
    AccMessage( ent );
}


/*
==================
CheatsOk
==================
*/
qboolean	CheatsOk( gentity_t *ent ) {
	if ( !g_cheats.integer ) {
		trap_SendServerCommand( ent-g_entities, va("print \"Cheats are not enabled on this server.\n\""));
		return qfalse;
	}
	if ( ent->health <= 0 ) {
		trap_SendServerCommand( ent-g_entities, va("print \"You must be alive to use this command.\n\""));
		return qfalse;
	}
	return qtrue;
}


/*
==================
ConcatArgs
==================
*/
char	*ConcatArgs( int start ) {
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_Argc();
	for ( i = start ; i < c ; i++ ) {
		trap_Argv( i, arg, sizeof( arg ) );
		tlen = strlen( arg );
		if ( len + tlen >= MAX_STRING_CHARS - 1 ) {
			break;
		}
		memcpy( line + len, arg, tlen );
		len += tlen;
		if ( i != c - 1 ) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}

/*
==================
ClientNumberFromString

Returns a player number for either a number or name string
Returns -1 if invalid
==================
*/
int ClientNumberFromString( gentity_t *to, char *s ) {
	gclient_t	*cl;
	int			idnum;
    char        cleanName[MAX_STRING_CHARS];

	// numeric values are just slot numbers
	if (s[0] >= '0' && s[0] <= '9') {
		idnum = atoi( s );
		if ( idnum < 0 || idnum >= level.maxclients ) {
			trap_SendServerCommand( to-g_entities, va("print \"Bad client slot: %i\n\"", idnum));
			return -1;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected != CON_CONNECTED ) {
			trap_SendServerCommand( to-g_entities, va("print \"Client %i is not active\n\"", idnum));
			return -1;
		}
		return idnum;
	}

	// check for a name match
	for ( idnum=0,cl=level.clients ; idnum < level.maxclients ; idnum++,cl++ ) {
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
        Q_strncpyz(cleanName, cl->pers.netname, sizeof(cleanName));
        Q_CleanStr(cleanName);
        if ( Q_strequal( cleanName, s ) ) {
			return idnum;
		}
	}

	trap_SendServerCommand( to-g_entities, va("print \"User %s is not on the server\n\"", s));
	return -1;
}

/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void Cmd_Give_f (gentity_t *ent)
{
	char		*name;
	gitem_t		*it;
	int			i;
	qboolean	give_all;
	gentity_t		*it_ent;
	trace_t		trace;

	/*if ( !CheatsOk( ent ) ) {
		return;
	}*/
	if(!g_building.integer){ return; }
	if(!g_allowitems.integer){ return; }

	name = ConcatArgs( 1 );

	if Q_strequal(name, "all")
		give_all = qtrue;
	else
		give_all = qfalse;

	if (give_all || Q_strequal( name, "health"))
	{
		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
		if (!give_all)
			return;
	}

	if (give_all || Q_strequal(name, "weapons"))
	{
		ent->client->ps.stats[STAT_WEAPONS] = (1 << WP_NUM_WEAPONS) - 1 - ( 1 << WP_NONE );
			for(i = 1 ; i <= (WEAPONS_NUM-1)-15 ; i++){
				ent->swep_list[i+15] = 1; 
				ent->swep_ammo[i+15] = 9999; 
			}
			trap_SendServerCommand( ent->client->ps.clientNum, va("allswep_1", 0) );
		if (!give_all)
			return;
	}

	if (give_all || Q_strequal(name, "ammo"))
	{
		for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
			ent->client->ps.ammo[i] = 9999;
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_strequal(name, "armor"))
	{
		ent->client->ps.stats[STAT_ARMOR] = 200;

		if (!give_all)
			return;
	}
	
	if (give_all || Q_strequal(name, "money"))
	{
		ent->client->pers.oldmoney = 9999;

		if (!give_all)
			return;
	}

	if (Q_strequal(name, "excellent")) {
		ent->client->ps.persistant[PERS_EXCELLENT_COUNT]++;
		return;
	}
	if (Q_strequal(name, "impressive")) {
		ent->client->ps.persistant[PERS_IMPRESSIVE_COUNT]++;
		return;
	}
	if (Q_strequal(name, "gauntletaward")) {
		ent->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT]++;
		return;
	}
	if (Q_strequal(name, "defend")) {
		ent->client->ps.persistant[PERS_DEFEND_COUNT]++;
		return;
	}
	if (Q_strequal(name, "assist")) {
		ent->client->ps.persistant[PERS_ASSIST_COUNT]++;
		return;
	}

	// spawn a specific item right on the player
	if ( !give_all ) {
		it = BG_FindItem (name);
		if (!it) {
			return;
		}

		it_ent = G_Spawn();
		VectorCopy( ent->r.currentOrigin, it_ent->s.origin );
		it_ent->classname = it->classname;
		G_SpawnItem (it_ent, it);
		FinishSpawningItem(it_ent );
		memset( &trace, 0, sizeof( trace ) );
		Touch_Item (it_ent, ent, &trace);
		if (it_ent->inuse) {
			G_FreeEntity( it_ent );
		}
	}
}


/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void Cmd_God_f (gentity_t *ent)
{
	char	*msg;

	if ( !CheatsOk( ent ) ) {
		return;
	}

	ent->flags ^= FL_GODMODE;
	if (!(ent->flags & FL_GODMODE) )
		msg = "godmode OFF\n";
	else
		msg = "godmode ON\n";

	trap_SendServerCommand( ent-g_entities, va("print \"%s\"", msg));
}


/*
==================
Cmd_Notarget_f

Sets client to notarget

argv(0) notarget
==================
*/
void Cmd_Notarget_f( gentity_t *ent ) {
	char	*msg;

	if ( !CheatsOk( ent ) ) {
		return;
	}

	ent->flags ^= FL_NOTARGET;
	if (!(ent->flags & FL_NOTARGET) )
		msg = "notarget OFF\n";
	else
		msg = "notarget ON\n";

	trap_SendServerCommand( ent-g_entities, va("print \"%s\"", msg));
}


/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void Cmd_Noclip_f( gentity_t *ent ) {
	char	*msg;

	/*if ( !CheatsOk( ent ) ) {
		return;
	}*/
	if(!g_building.integer){ return; }
	if(!g_allownoclip.integer){ return; }

	if ( ent->client->noclip ) {
		msg = "noclip OFF\n";
	} else {
		msg = "noclip ON\n";
	}
	ent->client->noclip = !ent->client->noclip;

	trap_SendServerCommand( ent-g_entities, va("print \"%s\"", msg));
}


/*
==================
Cmd_LevelShot_f

This is just to help generate the level pictures
for the menus.  It goes to the intermission immediately
and sends over a command to the client to resize the view,
hide the scoreboard, and take a special screenshot
==================
*/
void Cmd_LevelShot_f( gentity_t *ent ) {
	if ( !CheatsOk( ent ) ) {
		return;
	}

	// doesn't work in single player
	if ( g_gametype.integer != 0 ) {
		trap_SendServerCommand( ent-g_entities,
			"print \"Must be in g_gametype 0 for levelshot\n\"" );
		return;
	}

        if(!ent->client->pers.localClient)
	{
		trap_SendServerCommand(ent-g_entities,
		"print \"The levelshot command must be executed by a local client\n\"");
		return;
	}


	BeginIntermission();
	trap_SendServerCommand( ent-g_entities, "clientLevelShot" );
}


/*
==================
Cmd_LevelShot_f

This is just to help generate the level pictures
for the menus.  It goes to the intermission immediately
and sends over a command to the client to resize the view,
hide the scoreboard, and take a special screenshot
==================
*/
void Cmd_TeamTask_f( gentity_t *ent ) {
	char userinfo[MAX_INFO_STRING];
	char		arg[MAX_TOKEN_CHARS];
	int task;
	int client = ent->client - level.clients;

	if ( trap_Argc() != 2 ) {
		return;
	}
	trap_Argv( 1, arg, sizeof( arg ) );
	task = atoi( arg );

	trap_GetUserinfo(client, userinfo, sizeof(userinfo));
	Info_SetValueForKey(userinfo, "teamtask", va("%d", task));
	trap_SetUserinfo(client, userinfo);
	ClientUserinfoChanged(client);
}



/*
=================
Cmd_Kill_f
=================
*/
void Cmd_Kill_f( gentity_t *ent ) {
	if ( (ent->client->sess.sessionTeam == TEAM_SPECTATOR) || ent->client->isEliminated ) {
		return;
	}
	if (ent->health <= 0) {
		return;
	}
	if (g_kill.integer == 0) {
		return;
	}
	ent->flags &= ~FL_GODMODE;
	ent->client->ps.stats[STAT_HEALTH] = ent->health = -999;
        if(ent->client->lastSentFlying>-1)
            //If player is in the air because of knockback we give credit to the person who sent it flying
            player_die (ent, ent, &g_entities[ent->client->lastSentFlying], 100000, MOD_FALLING);
        else
            player_die (ent, ent, ent, 100000, MOD_SUICIDE);
}

/*
=================
BroadCastTeamChange

Let everyone know about a team change
=================
*/
void BroadcastTeamChange( gclient_t *client, int oldTeam )
{
	if(g_connectmsg.integer == 1){
	if ( client->sess.sessionTeam == TEAM_RED ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the red team.\n\"",
			client->pers.netname) );
	} else if ( client->sess.sessionTeam == TEAM_BLUE ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the blue team.\n\"",
		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_SPECTATOR && oldTeam != TEAM_SPECTATOR ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the spectators.\n\"",
		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_FREE ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the battle.\n\"",
		client->pers.netname));
	}
	}
}

// oatmeal begin

void ThrowArmor( gentity_t *ent ) {
	gclient_t	*client;
	usercmd_t	*ucmd;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	byte i;
	int amount;

//	if(om_drop_armor.integer==0){ return; }

	client = ent->client;
	ucmd = &ent->client->pers.cmd;

	if(client->ps.stats[STAT_ARMOR]>50){
		client->ps.stats[STAT_ARMOR] -= 50;
		xr_item = BG_FindItem( "Armor" );
		xr_drop = Throw_Item( ent, xr_item, 0 );
		xr_drop->count = 50;
	} else if(client->ps.stats[STAT_ARMOR]>5){
		client->ps.stats[STAT_ARMOR] -= 5;
		xr_item = BG_FindItem( "Armor Shard" );
		xr_drop = Throw_Item( ent, xr_item, 0 );
		xr_drop->count = 5;
	}
}

void Cmd_DropArmor_f( gentity_t *ent ) {
	ThrowArmor( ent );
}

void ThrowHealth( gentity_t *ent ) {
	gclient_t	*client;
	usercmd_t	*ucmd;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	byte i;
	int amount;

//	if(om_drop_health.integer==0){ return; }

	client = ent->client;
	ucmd = &ent->client->pers.cmd;

	if(ent->health>5){
		ent->health -= 5;
		xr_item = BG_FindItem( "5 Health" );
		xr_drop = Throw_Item( ent, xr_item, 0 );
		xr_drop->count = 5;
	}
}

void Cmd_DropHealth_f( gentity_t *ent ) {
	ThrowHealth( ent );
}

void ThrowFlag( gentity_t *ent ) {
	gclient_t	*client;
	usercmd_t	*ucmd;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	byte i;
	int amount;

//	if(om_drop_health.integer==0){ return; }

	client = ent->client;
	ucmd = &ent->client->pers.cmd;

	if ( ent->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
		Throw_Item( ent, BG_FindItemForPowerup( PW_NEUTRALFLAG ), 0 );
		ent->client->ps.powerups[PW_NEUTRALFLAG] = 0;
	}
	else if ( ent->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
		Throw_Item( ent, BG_FindItemForPowerup( PW_REDFLAG ), 0 );
		ent->client->ps.powerups[PW_REDFLAG] = 0;
	}
	else if ( ent->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
		Throw_Item( ent, BG_FindItemForPowerup( PW_BLUEFLAG ), 0 );
		ent->client->ps.powerups[PW_BLUEFLAG] = 0;
	}
}

void Cmd_DropFlag_f( gentity_t *ent ) {
	ThrowFlag( ent );
}

void ThrowRune( gentity_t *ent ) {
	gclient_t	*client;
	usercmd_t	*ucmd;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	byte i;
	int amount;

	if(1==1){ return; }

	client = ent->client;
	ucmd = &ent->client->pers.cmd;

	if ( ent->client->ps.stats[STAT_PERSISTANT_POWERUP] == PW_GUARD ) {		// only happens in One Flag CTF
		Throw_Item( ent, BG_FindItem( "Guard" ), 0 );
		ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
	}
	else if ( ent->client->ps.stats[STAT_PERSISTANT_POWERUP] == PW_DOUBLER ) {		// only happens in standard CTF
		Throw_Item( ent, BG_FindItem( "Doubler" ), 0 );
		ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
	}
	else if ( ent->client->ps.stats[STAT_PERSISTANT_POWERUP] == PW_AMMOREGEN ) {	// only happens in standard CTF
		Throw_Item( ent, BG_FindItem( "Ammo Regen" ), 0 );
		ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
	}
	else if ( ent->client->ps.stats[STAT_PERSISTANT_POWERUP] == PW_SCOUT ) {	// only happens in standard CTF
		Throw_Item( ent, BG_FindItem( "Scout" ), 0 );
		ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
	}
}

void Cmd_DropRune_f( gentity_t *ent ) {
	ThrowRune( ent );
}

void ThrowHoldable( gentity_t *ent ) {
	gclient_t	*client;
	usercmd_t	*ucmd;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	byte i;
	int amount;

//	if(om_drop_health.integer==0){ return; }

	client = ent->client;
	ucmd = &ent->client->pers.cmd;

	if ( client->ps.stats[STAT_HOLDABLE_ITEM] & (1 << HI_TELEPORTER) ) {
		Throw_Item( ent, BG_FindItem( "Personal Teleporter" ), 0 );
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] -= (1 << HI_TELEPORTER);
	}
	else if ( client->ps.stats[STAT_HOLDABLE_ITEM] & (1 << HI_MEDKIT) ) {
		Throw_Item( ent, BG_FindItem( "Medkit" ), 0 );
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] -= (1 << HI_MEDKIT);
	}
	else if ( client->ps.stats[STAT_HOLDABLE_ITEM] & (1 << HI_KAMIKAZE) ) {
		Throw_Item( ent, BG_FindItem( "Kamikaze" ), 0 );
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] -= (1 << HI_KAMIKAZE);
	}
	else if ( client->ps.stats[STAT_HOLDABLE_ITEM] & (1 << HI_INVULNERABILITY) ) {
		Throw_Item( ent, BG_FindItem( "Invulnerability" ), 0 );
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] -= (1 << HI_INVULNERABILITY);
	}
	else if ( client->ps.stats[STAT_HOLDABLE_ITEM] & (1 << HI_PORTAL) ) {
		Throw_Item( ent, BG_FindItem( "Portal" ), 0 );
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] -= (1 << HI_PORTAL);
	}
}

void Cmd_DropHoldable_f( gentity_t *ent ) {
	ThrowHoldable( ent );
}

void ThrowAmmo( gentity_t *ent ) {
	gclient_t	*client;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	int amount;
	int weapon;

	weapon = ent->swep_id;

	client = ent->client;

if(weapon == WP_GAUNTLET){ return; }
if(weapon == WP_GRAPPLING_HOOK){ return; }

if(weapon <= 15){
	if(!(client->ps.stats[STAT_WEAPONS] & (1 << weapon ))){ return; }
	amount = client->ps.ammo[weapon];
	if(amount < 50){
	client->ps.ammo[weapon] = 0;
	} else {
	amount = 50;
	client->ps.ammo[weapon] -= 50;
	}
	xr_item = BG_FindSwepAmmo( weapon );
	if(!xr_item->classname){ return; }
	xr_drop = Throw_Item( ent, xr_item, 0 );
	xr_drop->count = amount;
} else {
	if(!ent->swep_list[weapon]){ return; }
	amount = ent->swep_ammo[weapon];
	if(amount < 50){
	ent->swep_ammo[weapon] = 0;
	} else {
	amount = 50;
	ent->swep_ammo[weapon] -= 50;
	}
	xr_item = BG_FindSwepAmmo( weapon );
	if(!xr_item->classname){ return; }
	xr_drop = Throw_Item( ent, xr_item, 0 );
	xr_drop->count = amount;
}

}

void Cmd_DropAmmo_f( gentity_t *ent ) {
	ThrowAmmo( ent );
}

void ThrowWeapon( gentity_t *ent ) {
	gclient_t	*client;
	gitem_t		*xr_item;
	gentity_t	*xr_drop;
	int amount;
	int weapon;
	
	weapon = ent->swep_id;

	client = ent->client;

if(weapon == WP_GAUNTLET){ return; }

if(weapon <= 15){
	amount = client->ps.ammo[weapon];
	if(amount == 0){ return; }
	client->ps.ammo[weapon] = 0;
	Add_Weapon( ent, weapon, 0);
	client->ps.weapon = WP_GAUNTLET;
	client->ps.generic2 = WP_GAUNTLET;
	ent->swep_id = WP_GAUNTLET;
	xr_item = BG_FindSwep( weapon );
	if(!xr_item->classname){ return; }
	xr_drop = Throw_Item( ent, xr_item, 0 );
	xr_drop->count = amount;
} else {
	amount = ent->swep_ammo[weapon];
	if(amount == 0){ return; }
	ent->swep_ammo[weapon] = 0;
	Add_Weapon( ent, weapon, 0);
	client->ps.weapon = WP_GAUNTLET;
	client->ps.generic2 = WP_GAUNTLET;
	ent->swep_id = WP_GAUNTLET;
	xr_item = BG_FindSwep( weapon );
	if(!xr_item->classname){ return; }
	xr_drop = Throw_Item( ent, xr_item, 0 );
	xr_drop->count = amount;
}

}

void Cmd_DropWeapon_f( gentity_t *ent ) {
	ThrowWeapon( ent );
}

// oatmeal end

/*
=================
SetTeam
KK-OAX Modded this to accept a forced admin change.
=================
*/
void SetTeam( gentity_t *ent, char *s ) {
	int					team, oldTeam;
	gclient_t			*client;
	int					clientNum;
	spectatorState_t	specState;
	int					specClient;
	int					teamLeader;
    char	            userinfo[MAX_INFO_STRING];
    qboolean            force;

	force = G_admin_permission(ent, ADMF_FORCETEAMCHANGE);

	//
	// see what change is requested
	//
	client = ent->client;

	clientNum = client - level.clients;
        trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
	specClient = 0;
	specState = SPECTATOR_NOT;
	if ( Q_strequal( s, "scoreboard" ) || Q_strequal( s, "score" )  ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_SCOREBOARD;
	} else if ( Q_strequal( s, "follow1" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FOLLOW;
		specClient = -1;
	} else if ( Q_strequal( s, "follow2" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FOLLOW;
		specClient = -2;
	} else if ( Q_strequal( s, "spectator" ) || Q_strequal( s, "s" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FREE;
	} else if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		// if running a team game, assign player to one of the teams
		specState = SPECTATOR_NOT;
		if ( Q_strequal( s, "red" ) || Q_strequal( s, "r" ) ) {
			team = TEAM_RED;
		} else if ( Q_strequal( s, "blue" ) || Q_strequal( s, "b" ) ) {
			team = TEAM_BLUE;
		} else {
			// pick the team with the least number of players
			team = PickTeam( clientNum );
		}
        if ( !force ) {
		    if ( g_teamForceBalance.integer  ) {
			    int		counts[TEAM_NUM_TEAMS];

			    counts[TEAM_BLUE] = TeamCount( ent->client->ps.clientNum, TEAM_BLUE );
			    counts[TEAM_RED] = TeamCount( ent->client->ps.clientNum, TEAM_RED );

			    // We allow a spread of two
			    if ( team == TEAM_RED && counts[TEAM_RED] - counts[TEAM_BLUE] > 1 ) {
				    trap_SendServerCommand( ent->client->ps.clientNum,
					    "cp \"Red team has too many players.\n\"" );
				    return; // ignore the request
			    }
			    if ( team == TEAM_BLUE && counts[TEAM_BLUE] - counts[TEAM_RED] > 1 ) {
				    trap_SendServerCommand( ent->client->ps.clientNum,
					    "cp \"Blue team has too many players.\n\"" );
				    return; // ignore the request
			    }

			    // It's ok, the team we are switching to has less or same number of players
		    }
        }
	} else {
		// force them to spectators if there aren't any spots free
		team = TEAM_FREE;
	}
    if ( !force ) {
	    // override decision if limiting the players
	    if ( (g_gametype.integer == GT_TOURNAMENT)
		    && level.numNonSpectatorClients >= 2 ) {
		    team = TEAM_SPECTATOR;
	    } else if ( g_maxGameClients.integer > 0 &&
		    level.numNonSpectatorClients >= g_maxGameClients.integer ) {
		    team = TEAM_SPECTATOR;
	    }
	}

	//
	// decide if we will allow the change
	//
	oldTeam = client->sess.sessionTeam;
	if ( team == oldTeam && team != TEAM_SPECTATOR ) {
		return;
	}
    //KK-OAX Check to make sure the team is not locked from Admin
    if ( !force ) {
        if ( team == TEAM_RED && level.RedTeamLocked ) {
            trap_SendServerCommand( ent->client->ps.clientNum,
            "cp \"The Red Team has been locked by the Admin! \n\"" );
            return;
        }
        if ( team == TEAM_BLUE && level.BlueTeamLocked ) {
            trap_SendServerCommand( ent->client->ps.clientNum,
            "cp \"The Blue Team has been locked by the Admin! \n\"" );
            return;
        }
        if ( team == TEAM_FREE && level.FFALocked ) {
            trap_SendServerCommand( ent->client->ps.clientNum,
            "cp \"This Deathmatch has been locked by the Admin! \n\"" );
            return;
        }
    }
	//
	// execute the team change
	//

	// if the player was dead leave the body
	if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
		CopyToBodyQue(ent);
	}

	// he starts at 'base'
	client->pers.teamState.state = TEAM_BEGIN;
	if ( oldTeam != TEAM_SPECTATOR ) {
                int teamscore = -99;
                //Prevent a team from loosing point because of player leaving team
                if(g_gametype.integer == GT_TEAM && ent->client->ps.stats[STAT_HEALTH])
                    teamscore = level.teamScores[ ent->client->sess.sessionTeam ];
		// Kill him (makes sure he loses flags, etc)
		ent->flags &= ~FL_GODMODE;
		ent->client->ps.stats[STAT_HEALTH] = ent->health = 0;
		player_die (ent, ent, ent, 100000, MOD_SUICIDE);
                if(teamscore != -99)
                    level.teamScores[ ent->client->sess.sessionTeam ] = teamscore;

	}

        if(oldTeam!=TEAM_SPECTATOR)
            PlayerStore_store(Info_ValueForKey(userinfo,"cl_guid"),client->ps);

	// they go to the end of the line for tournements
        if(team == TEAM_SPECTATOR && oldTeam != team)
                AddTournamentQueue(client);

	client->sess.sessionTeam = team;
	client->sess.spectatorState = specState;
	client->sess.spectatorClient = specClient;

	client->sess.teamLeader = qfalse;
	if ( team == TEAM_RED || team == TEAM_BLUE ) {
		teamLeader = TeamLeader( team );
		// if there is no team leader or the team leader is a bot and this client is not a bot
		if ( teamLeader == -1 || ( !(g_entities[clientNum].r.svFlags & SVF_BOT) && (g_entities[teamLeader].r.svFlags & SVF_BOT) ) ) {
			SetLeader( team, clientNum );
		}
	}
	// make sure there is a team leader on the team the player came from
	if ( oldTeam == TEAM_RED || oldTeam == TEAM_BLUE ) {
		CheckTeamLeader( oldTeam );
	}

	BroadcastTeamChange( client, oldTeam );

	// get and distribute relevent paramters
	ClientUserinfoChanged( clientNum );

	ClientBegin( clientNum );
}

/*
=================
StopFollowing

If the client being followed leaves the game, or you just want to drop
to free floating spectator mode
=================
*/
void StopFollowing( gentity_t *ent ) {
	if(g_gametype.integer<GT_ELIMINATION || g_gametype.integer>GT_LMS)
	{
		//Shouldn't this already be the case?
		ent->client->ps.persistant[ PERS_TEAM ] = TEAM_SPECTATOR;
		ent->client->sess.sessionTeam = TEAM_SPECTATOR;
	}
	else {
		ent->client->ps.stats[STAT_HEALTH] = 0;
		ent->health = 0;
	}
	ent->client->sess.spectatorState = SPECTATOR_FREE;
	ent->client->ps.pm_flags &= ~PMF_FOLLOW;
	ent->r.svFlags &= ~SVF_BOT;
	ent->client->ps.clientNum = ent - g_entities;
}

/*
=================
Cmd_Team_f
=================
*/
void Cmd_Team_f( gentity_t *ent ) {
	int			oldTeam;
	char		s[MAX_TOKEN_CHARS];
	qboolean    force;

	if ( trap_Argc() != 2 ) {
		oldTeam = ent->client->sess.sessionTeam;
		switch ( oldTeam ) {
		case TEAM_BLUE:
			trap_SendServerCommand( ent-g_entities, "print \"Blue team\n\"" );
			break;
		case TEAM_RED:
			trap_SendServerCommand( ent-g_entities, "print \"Red team\n\"" );
			break;
		case TEAM_FREE:
			trap_SendServerCommand( ent-g_entities, "print \"Deathmatch-Playing\n\"" );
			break;
		case TEAM_SPECTATOR:
			trap_SendServerCommand( ent-g_entities, "print \"Spectator team\n\"" );
			break;
		}
		return;
	}

    force = G_admin_permission(ent, ADMF_FORCETEAMCHANGE);

	if( !force ) {
	    if ( ent->client->switchTeamTime > level.time ) {
		    trap_SendServerCommand( ent-g_entities, "print \"May not switch teams more than once per 5 seconds.\n\"" );
		    return;
		}
	}

	// if they are playing a tournement game, count as a loss
	if ( (g_gametype.integer == GT_TOURNAMENT )
		&& ent->client->sess.sessionTeam == TEAM_FREE ) {
		ent->client->sess.losses++;
	}

	trap_Argv( 1, s, sizeof( s ) );

	SetTeam( ent, s );

	ent->client->switchTeamTime = level.time + 5000;
}


/*
=================
Cmd_Follow_f
=================
*/
void Cmd_Follow_f( gentity_t *ent ) {
	int		i;
	char	arg[MAX_TOKEN_CHARS];

	if ( trap_Argc() != 2 ) {
		if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
			StopFollowing( ent );
		}
		return;
	}


	trap_Argv( 1, arg, sizeof( arg ) );
	i = ClientNumberFromString( ent, arg );
	if ( i == -1 ) {
		return;
	}



	// can't follow self
	if ( &level.clients[ i ] == ent->client ) {
		return;
	}

	// can't follow another spectator (or an eliminated player)
	if ( (level.clients[ i ].sess.sessionTeam == TEAM_SPECTATOR) || level.clients[ i ].isEliminated) {
		return;
	}

        if ( (g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION) && g_elimination_lockspectator.integer
            &&  ((ent->client->sess.sessionTeam == TEAM_RED && level.clients[ i ].sess.sessionTeam == TEAM_BLUE) ||
                 (ent->client->sess.sessionTeam == TEAM_BLUE && level.clients[ i ].sess.sessionTeam == TEAM_RED) ) ) {
            return;
        }

	// if they are playing a tournement game, count as a loss
	if ( (g_gametype.integer == GT_TOURNAMENT )
		&& ent->client->sess.sessionTeam == TEAM_FREE ) {
		ent->client->sess.losses++;
	}

	// first set them to spectator
	//if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
	if ( ent->client->sess.spectatorState == SPECTATOR_NOT ) {
		SetTeam( ent, "spectator" );
	}

	ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
	ent->client->sess.spectatorClient = i;
}

/*
=================
Cmd_FollowCycle_f
KK-OAX Modified to trap arguments.
=================
*/
void Cmd_FollowCycle_f( gentity_t *ent ) {
	int		clientnum;
	int		original;
    int     count;
    char    args[11];
    int     dir;

    if( ent->client->sess.sessionTeam == TEAM_NONE ) {
        dir = 1;
    }

    trap_Argv( 0, args, sizeof( args ) );
    if( Q_strequal( args, "followprev" )) {
        dir = -1;
    } else if( Q_strequal( args, "follownext" )) {
        dir = 1;
    } else {
        dir = 1;
    }

	// if they are playing a tournement game, count as a loss
	if ( (g_gametype.integer == GT_TOURNAMENT )
		&& ent->client->sess.sessionTeam == TEAM_FREE ) {
		ent->client->sess.losses++;
	}
	// first set them to spectator
	if ( ent->client->sess.spectatorState == SPECTATOR_NOT ) {
		SetTeam( ent, "spectator" );
	}

	if ( dir != 1 && dir != -1 ) {
		G_Error( "Cmd_FollowCycle_f: bad dir %i", dir );
	}

	clientnum = ent->client->sess.spectatorClient;
	original = clientnum;
        count = 0;
	do {
		clientnum += dir;
                count++;
		if ( clientnum >= level.maxclients ) {
			clientnum = 0;
		}
		if ( clientnum < 0 ) {
			clientnum = level.maxclients - 1;
		}

                if(count>level.maxclients) //We have looked at all clients at least once and found nothing
                    return; //We might end up in an infinite loop here. Stop it!

		// can only follow connected clients
		if ( level.clients[ clientnum ].pers.connected != CON_CONNECTED ) {
			continue;
		}

		// can't follow another spectator
		if ( (level.clients[ clientnum ].sess.sessionTeam == TEAM_SPECTATOR) || level.clients[ clientnum ].isEliminated) {
			continue;
		}

                //Stop players from spectating players on the enemy team in elimination modes.
                if ( (g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION) && g_elimination_lockspectator.integer
                    &&  ((ent->client->sess.sessionTeam == TEAM_RED && level.clients[ clientnum ].sess.sessionTeam == TEAM_BLUE) ||
                         (ent->client->sess.sessionTeam == TEAM_BLUE && level.clients[ clientnum ].sess.sessionTeam == TEAM_RED) ) ) {
                    continue;
                }

		// this is good, we can use it
		ent->client->sess.spectatorClient = clientnum;
		ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
		return;
	} while ( clientnum != original );

	// leave it where it was
}


/*
==================
G_Say
==================
*/

static void G_SayTo( gentity_t *ent, gentity_t *other, int mode, int color, const char *name, const char *message ) {
	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}
	if ( other->client->pers.connected != CON_CONNECTED ) {
		return;
	}
	/*if ( mode == SAY_TEAM  && !OnSameTeam(ent, other) ) {
		return;
	}*/

        if ((ent->r.svFlags & SVF_BOT) && trap_Cvar_VariableValue( "bot_nochat" )>1) return;

	// no chatting to players in tournements
	/*if ( (g_gametype.integer == GT_TOURNAMENT )
		&& other->client->sess.sessionTeam == TEAM_FREE
		&& ent->client->sess.sessionTeam != TEAM_FREE ) {
		return;
	}*/

	trap_SendServerCommand( other-g_entities, va("%s \"%s%c%c%s\"",
		mode == SAY_TEAM ? "tchat" : "chat",
		name, Q_COLOR_ESCAPE, color, message));
}

#define EC		"\x19"

void G_Say( gentity_t *ent, gentity_t *target, int mode, const char *chatText ) {
	int			j;
	gentity_t	*other;
	int			color;
	char		name[64];
	// don't let text be too long for malicious reasons
	char		text[MAX_SAY_TEXT];
	char		location[64];

    if ((ent->r.svFlags & SVF_BOT) && trap_Cvar_VariableValue( "bot_nochat" )>1) return;

	/*if ( (g_gametype.integer < GT_TEAM || g_ffa_gt == 1) && mode == SAY_TEAM ) {
		mode = SAY_ALL;
	}*/

	switch ( mode ) {
	default:
	case SAY_ALL:
		G_LogPrintf( "say: %s: %s\n", ent->client->pers.netname, chatText );
		Com_sprintf (name, sizeof(name), "%s%c%c"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_GREEN;
		break;
	case SAY_TEAM:
		G_LogPrintf( "sayteam: %s: %s\n", ent->client->pers.netname, chatText );
		if (Team_GetLocationMsg(ent, location, sizeof(location)))
			Com_sprintf (name, sizeof(name), EC"(%s%c%c"EC") (%s)"EC": ",
				ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE, location);
		else
			Com_sprintf (name, sizeof(name), EC"(%s%c%c"EC")"EC": ",
				ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_CYAN;
		break;
	case SAY_TELL:
		if (target && g_gametype.integer >= GT_TEAM && g_ffa_gt != 1 &&
			target->client->sess.sessionTeam == ent->client->sess.sessionTeam &&
			Team_GetLocationMsg(ent, location, sizeof(location)))
			Com_sprintf (name, sizeof(name), EC"[%s%c%c"EC"] (%s)"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE, location );
		else
			Com_sprintf (name, sizeof(name), EC"[%s%c%c"EC"]"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_MAGENTA;
		break;
	}

	Q_strncpyz( text, chatText, sizeof(text) );

	if ( target ) {
		G_SayTo( ent, target, mode, color, name, text );
		return;
	}

	// echo the text to the console
	if ( g_dedicated.integer ) {
		G_Printf( "%s%s\n", name, text);
	}

	// send it to all the apropriate clients
	for (j = 0; j < level.maxclients; j++) {
		other = &g_entities[j];
		G_SayTo( ent, other, mode, color, name, text );
	}
	//KK-OAX Admin Command Check from Say/SayTeam line
	if( g_adminParseSay.integer )
	{
	    G_admin_cmd_check ( ent, qtrue );
	}
}


/*
==================
Cmd_Say_f
KK-OAX Modified this to trap the additional arguments from console.
==================
*/
static void Cmd_Say_f( gentity_t *ent ){
	char		*p;
	char        arg[MAX_TOKEN_CHARS];
	int         mode = SAY_ALL;

    trap_Argv( 0, arg, sizeof( arg ) );
    if( Q_strequal( arg, "say_team" ) )
        mode = SAY_TEAM ;
    // KK-OAX Disabled until PM'ing is added
    // support parsing /m out of say text since some people have a hard
    // time figuring out what the console is.
    /*if( !Q_stricmpn( args, "say /m ", 7 ) ||
      !Q_stricmpn( args, "say_team /m ", 12 ) ||
      !Q_stricmpn( args, "say /mt ", 8 ) ||
      !Q_stricmpn( args, "say_team /mt ", 13 ) )
    {
        Cmd_PrivateMessage_f( ent );
        return;
    }

    // support parsing /a out of say text for the same reason
    if( !Q_stricmpn( args, "say /a ", 7 ) ||
    !Q_stricmpn( args, "say_team /a ", 12 ) )
    {
        Cmd_AdminMessage_f( ent );
        return;
    }*/

    if( trap_Argc( ) < 2 )
        return;

    p = ConcatArgs( 1 );

    G_Say( ent, NULL, mode, p );
}

/*
==================
Cmd_PickTarget_f
KK-OAX Added for QSandbox.
==================
*/
static void Cmd_PickTarget_f( gentity_t *ent ){
	char		*p;
	char        arg[MAX_TOKEN_CHARS];
	gentity_t 	*act;

    trap_Argv( 0, arg, sizeof( arg ) );

    if( trap_Argc( ) < 2 )
        return;

    p = ConcatArgs( 1 );
	
	ent->target = p;

	act = G_PickTarget( ent->target );
	if ( act && act->use ) {
		act->use( act, ent, ent );
	}
}

/*
==================
Cmd_BuildProp_f
Added for QSandbox.
==================
*/
static void Cmd_BuildProp_f( gentity_t *ent ){
	vec3_t		end, start, forward, up, right;
	char		model[MAX_TOKEN_CHARS];
	char		class[MAX_TOKEN_CHARS];
	char		priv[MAX_TOKEN_CHARS];
	char		mix[MAX_TOKEN_CHARS];
	char		grid[MAX_TOKEN_CHARS];
	char		sf[MAX_TOKEN_CHARS];
	trace_t		tr;
	
	if(!g_building.integer){ return; }
	if(!g_allowprops.integer){ return; }
		
	//tr.endpos
	trap_Argv( 1, model, sizeof( model ) );
	trap_Argv( 2, class, sizeof( class ) );
	trap_Argv( 3, priv, sizeof( priv ) );
	trap_Argv( 4, mix, sizeof( mix ) );
	trap_Argv( 5, grid, sizeof( grid ) );
	trap_Argv( 6, sf, sizeof( sf ) );
	
	if( trap_Argc( ) < 5 ){
	G_Printf( "usage: <model> <class> <private> <min-max> <grid> <sf>\n" );
    return;
	}
	
	//Set Aiming Directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(ent, forward, right, up, start);
	VectorMA (start, 4096, forward, end);
	VectorScale( forward, 0, forward );

	//Trace Position
	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SELECT );
	
	if(g_safe.integer){
	if(!Q_stricmp (class, "target_cmd")){
	return;
	}
	if(!Q_stricmp (class, "target_clientcmd")){
	return;
	}
	if(!Q_stricmp (class, "target_modify")){
	return;
	}
	}
	
	G_BuildProp( model, class, tr.endpos, ent, atoi(priv), atoi(mix), atoi(grid), atoi(sf) );
}

/*
==================
Cmd_SpawnList_Item_f
Added for QSandbox.
==================
*/
static void Cmd_SpawnList_Item_f( gentity_t *ent ){
	vec3_t		end, start, forward, up, right;
	trace_t		tr;
	gentity_t 	*tent;
	char		arg01[64];
	char		arg02[64];
	char		arg03[64];
	char		arg04[64];
	char		arg05[64];
	char		arg06[64];
	char		arg07[64];
	char		arg08[64];
	char		arg09[64];
	char		arg10[64];
	char		arg11[64];
	char		arg12[64];
	char		arg13[64];
	char		arg14[64];
	char		arg15[64];
	char		arg16[64];
	char		arg17[64];
	char		arg18[64];
	char		arg19[64];
	char		arg20[64];
	char		arg21[64];
	char		arg22[64];
	char		arg23[64];
	char		arg24[64];
	char		arg25[64];
	char		arg26[64];
	char		arg27[64];
	char		arg28[64];
	char		arg29[64];
	char		arg30[64];
	char		arg31[64];
	char		arg32[64];
	char		arg33[64];
	char		arg34[64];
	char		arg35[64];
	char		arg36[64];
	char		arg37[64];
	char		arg38[64];
	char		arg39[64];
	char		arg40[64];
	
	if(!g_building.integer){ return; }
		
	//tr.endpos
	trap_Argv( 1, arg01, sizeof( arg01 ) );
	trap_Argv( 2, arg02, sizeof( arg02 ) );
	trap_Argv( 3, arg03, sizeof( arg03 ) );
	trap_Argv( 4, arg04, sizeof( arg04 ) );
	trap_Argv( 5, arg05, sizeof( arg05 ) );
	trap_Argv( 6, arg06, sizeof( arg06 ) );
	trap_Argv( 7, arg07, sizeof( arg07 ) );
	trap_Argv( 8, arg08, sizeof( arg08 ) );
	trap_Argv( 9, arg09, sizeof( arg09 ) );
	trap_Argv( 10, arg10, sizeof( arg10 ) );
	trap_Argv( 11, arg11, sizeof( arg11 ) );
	trap_Argv( 12, arg12, sizeof( arg12 ) );
	trap_Argv( 13, arg13, sizeof( arg13 ) );
	trap_Argv( 14, arg14, sizeof( arg14 ) );
	trap_Argv( 15, arg15, sizeof( arg15 ) );
	trap_Argv( 16, arg16, sizeof( arg16 ) );
	trap_Argv( 17, arg17, sizeof( arg17 ) );
	trap_Argv( 18, arg18, sizeof( arg18 ) );
	trap_Argv( 19, arg19, sizeof( arg19 ) );
	trap_Argv( 20, arg20, sizeof( arg20 ) );
	trap_Argv( 21, arg21, sizeof( arg21 ) );
	trap_Argv( 22, arg22, sizeof( arg22 ) );
	trap_Argv( 23, arg23, sizeof( arg23 ) );
	trap_Argv( 24, arg24, sizeof( arg24 ) );
	trap_Argv( 25, arg25, sizeof( arg25 ) );
	trap_Argv( 26, arg26, sizeof( arg26 ) );
	trap_Argv( 27, arg27, sizeof( arg27 ) );
	trap_Argv( 28, arg28, sizeof( arg28 ) );
	trap_Argv( 29, arg29, sizeof( arg29 ) );
	trap_Argv( 30, arg30, sizeof( arg30 ) );
	trap_Argv( 31, arg31, sizeof( arg31 ) );
	trap_Argv( 32, arg32, sizeof( arg32 ) );
	trap_Argv( 33, arg33, sizeof( arg33 ) );
	trap_Argv( 34, arg34, sizeof( arg34 ) );
	trap_Argv( 35, arg35, sizeof( arg35 ) );
	trap_Argv( 36, arg36, sizeof( arg36 ) );
	trap_Argv( 37, arg37, sizeof( arg37 ) );
	trap_Argv( 38, arg38, sizeof( arg38 ) );
	trap_Argv( 39, arg39, sizeof( arg39 ) );
	trap_Argv( 40, arg40, sizeof( arg40 ) );
	
	//Set Aiming Directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(ent, forward, right, up, start);
	VectorMA (start, 4096, forward, end);
	VectorScale( forward, 0, forward );

	//Trace Position
	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SELECT );
	
	if(!Q_stricmp (arg01, "prop")){
	if(!g_allowprops.integer){ return; }
		
	if(g_safe.integer){
	if(!Q_stricmp (arg03, "target_cmd")){
	return;
	}
	if(!Q_stricmp (arg03, "target_clientcmd")){
	return;
	}
	if(!Q_stricmp (arg03, "target_modify")){
	return;
	}
	}
	G_BuildPropSL( arg02, arg03, tr.endpos, ent, arg04, arg05, arg06, arg07, arg08, arg09, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35, arg36, arg37, arg38, arg39, arg40);
	
	
	return;
	}
	if(!Q_stricmp (arg01, "npc")){
	if(!g_allownpc.integer){ return; }
	
	tent = G_TempEntity( tr.endpos, EV_PLAYER_TELEPORT_OUT );
	VectorCopy( tr.endpos, tent->s.origin);
	tent->s.origin[2] += 48;
	tent->classname = "target_botspawn";
	CopyAlloc(tent->clientname, arg02);
	tent->skill = atof(arg04);
	tent->health = atoi(arg05);
	tent->spawnflags = atoi(arg08);
	if(!Q_stricmp (arg07, "0") ){
	CopyAlloc(tent->target, arg02);	
	} else {
	CopyAlloc(tent->target, arg07);	
	}
	if(!Q_stricmp (arg03, "NPC_Enemy")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_ENEMY );
	}
	if(!Q_stricmp (arg03, "NPC_Citizen")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_CITIZEN );
	}
	if(!Q_stricmp (arg03, "NPC_Guard")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_GUARD );
	}
	if(!Q_stricmp (arg03, "NPC_Partner")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_PARTNER );
	}
	
	trap_Cvar_Set("g_spSkill", arg04);


	return;
	}
	
	
}

/*
==================
Cmd_PropNpc_AS_f
Added for QSandbox.
==================
*/
static void Cmd_PropNpc_AS_f( gentity_t *ent ){
	vec3_t		end;
	gentity_t 	*tent;
	char		cord_x[64];
	char		cord_y[64];
	char		cord_z[64];
	char		arg01[64];
	char		arg02[64];
	char		arg03[64];
	char		arg04[64];
	char		arg05[64];
	char		arg06[64];
	char		arg07[64];
	char		arg08[64];
	char		arg09[64];
	char		arg10[64];
	char		arg11[64];
	char		arg12[64];
	char		arg13[64];
	char		arg14[64];
	char		arg15[64];
	char		arg16[64];
	char		arg17[64];
	char		arg18[64];
	char		arg19[64];
	char		arg20[64];
	char		arg21[64];
	char		arg22[64];
	char		arg23[64];
	char		arg24[64];
	char		arg25[64];
	char		arg26[64];
	char		arg27[64];
	char		arg28[64];
	char		arg29[64];
	char		arg30[64];
	char		arg31[64];
	char		arg32[64];
	char		arg33[64];
	char		arg34[64];
	char		arg35[64];
	char		arg36[64];
	char		arg37[64];
	char		arg38[64];
	char		arg39[64];
	char		arg40[64];
	
	if(!g_building.integer){ return; }
		
	//tr.endpos
	trap_Argv( 1, cord_x, sizeof( cord_x ) );
	trap_Argv( 2, cord_y, sizeof( cord_y ) );
	trap_Argv( 3, cord_z, sizeof( cord_z ) );
	trap_Argv( 4, arg01, sizeof( arg01 ) );
	trap_Argv( 5, arg02, sizeof( arg02 ) );
	trap_Argv( 6, arg03, sizeof( arg03 ) );
	trap_Argv( 7, arg04, sizeof( arg04 ) );
	trap_Argv( 8, arg05, sizeof( arg05 ) );
	trap_Argv( 9, arg06, sizeof( arg06 ) );
	trap_Argv( 10, arg07, sizeof( arg07 ) );
	trap_Argv( 11, arg08, sizeof( arg08 ) );
	trap_Argv( 12, arg09, sizeof( arg09 ) );
	trap_Argv( 13, arg10, sizeof( arg10 ) );
	trap_Argv( 14, arg11, sizeof( arg11 ) );
	trap_Argv( 15, arg12, sizeof( arg12 ) );
	trap_Argv( 16, arg13, sizeof( arg13 ) );
	trap_Argv( 17, arg14, sizeof( arg14 ) );
	trap_Argv( 18, arg15, sizeof( arg15 ) );
	trap_Argv( 19, arg16, sizeof( arg16 ) );
	trap_Argv( 20, arg17, sizeof( arg17 ) );
	trap_Argv( 21, arg18, sizeof( arg18 ) );
	trap_Argv( 22, arg19, sizeof( arg19 ) );
	trap_Argv( 23, arg20, sizeof( arg20 ) );
	trap_Argv( 24, arg21, sizeof( arg21 ) );
	trap_Argv( 25, arg22, sizeof( arg22 ) );
	trap_Argv( 26, arg23, sizeof( arg23 ) );
	trap_Argv( 27, arg24, sizeof( arg24 ) );
	trap_Argv( 28, arg25, sizeof( arg25 ) );
	trap_Argv( 29, arg26, sizeof( arg26 ) );
	trap_Argv( 30, arg27, sizeof( arg27 ) );
	trap_Argv( 31, arg28, sizeof( arg28 ) );
	trap_Argv( 32, arg29, sizeof( arg29 ) );
	trap_Argv( 33, arg30, sizeof( arg30 ) );
	trap_Argv( 34, arg31, sizeof( arg31 ) );
	trap_Argv( 35, arg32, sizeof( arg32 ) );
	trap_Argv( 36, arg33, sizeof( arg33 ) );
	trap_Argv( 37, arg34, sizeof( arg34 ) );
	trap_Argv( 38, arg35, sizeof( arg35 ) );
	trap_Argv( 39, arg36, sizeof( arg36 ) );
	trap_Argv( 40, arg37, sizeof( arg37 ) );
	trap_Argv( 41, arg38, sizeof( arg38 ) );
	trap_Argv( 42, arg39, sizeof( arg39 ) );
	trap_Argv( 43, arg40, sizeof( arg40 ) );
	
	end[0] = atof(cord_x);
	end[1] = atof(cord_y);
	end[2] = atof(cord_z);
	
	if(!Q_stricmp (arg01, "prop")){
	if(!g_allowprops.integer){ return; }
		
	if(g_safe.integer){
	if(!Q_stricmp (arg03, "target_cmd")){
	return;
	}
	if(!Q_stricmp (arg03, "target_clientcmd")){
	return;
	}
	if(!Q_stricmp (arg03, "target_modify")){
	return;
	}
	}
	G_BuildPropSL( arg02, arg03, end, ent, arg04, arg05, arg06, arg07, arg08, arg09, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31, arg32, arg33, arg34, arg35, arg36, arg37, arg38, arg39, arg40);
	
	
	return;
	}
	if(!Q_stricmp (arg01, "npc")){
	if(!g_allownpc.integer){ return; }
	
	tent = G_TempEntity( end, EV_PLAYER_TELEPORT_OUT );
	VectorCopy( end, tent->s.origin);
	tent->s.origin[2] += 48;
	tent->classname = "target_botspawn";
	CopyAlloc(tent->clientname, arg02);
	tent->skill = atof(arg04);
	tent->health = atoi(arg05);
	tent->spawnflags = atoi(arg08);
	if(!Q_stricmp (arg07, "0") ){
	CopyAlloc(tent->target, arg02);	
	} else {
	CopyAlloc(tent->target, arg07);	
	}
	if(!Q_stricmp (arg03, "NPC_Enemy")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_ENEMY );
	}
	if(!Q_stricmp (arg03, "NPC_Citizen")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_CITIZEN );
	}
	if(!Q_stricmp (arg03, "NPC_Guard")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_GUARD );
	}
	if(!Q_stricmp (arg03, "NPC_Partner")){
	G_AddBot(tent->clientname, atof(arg04), "Blue", 0, arg06, tent->s.number, tent->target, NPC_PARTNER );
	}
	
	trap_Cvar_Set("g_spSkill", arg04);


	return;
	}
	
	
}

/*
==================
Cmd_SpawnBot_f
Added for QSandbox.
==================
*/
static void Cmd_SpawnBot_f( gentity_t *ent ){
	vec3_t		end, start, forward, up, right;
	gentity_t 	*tent;
	char		model[MAX_TOKEN_CHARS];
	char		class[MAX_TOKEN_CHARS];
	char		skill[MAX_TOKEN_CHARS];
	char		health[MAX_TOKEN_CHARS];
	char		name[MAX_TOKEN_CHARS];
	char		target[MAX_TOKEN_CHARS];
	char		weapon[MAX_TOKEN_CHARS];
	trace_t		tr;
	
	if(!g_building.integer){ return; }
	if(!g_allownpc.integer){ return; }
		
	//tr.endpos
	trap_Argv( 1, model, sizeof( model ) );
	trap_Argv( 2, class, sizeof( class ) );
	trap_Argv( 3, skill, sizeof( skill ) );
	trap_Argv( 4, health, sizeof( health ) );
	trap_Argv( 5, name, sizeof( name ) );
	trap_Argv( 6, target, sizeof( target ) );
	trap_Argv( 7, weapon, sizeof( weapon ) );
	
	if(atoi(health) <= 0){
	strcpy(health, "100");
	}
	
	//Set Aiming Directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(ent, forward, right, up, start);
	VectorMA (start, 4096, forward, end);
	VectorScale( forward, 0, forward );

	//Trace Position
	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SELECT );
	tent = G_TempEntity( tr.endpos, EV_PLAYER_TELEPORT_OUT );
	VectorCopy( tr.endpos, tent->s.origin);
	tent->s.origin[2] += 48;
	tent->classname = "target_botspawn";
	CopyAlloc(tent->clientname, model);
	tent->skill = atof(skill);
	tent->health = atoi(health);
	tent->spawnflags = atoi(weapon);
	if(!Q_stricmp (target, "0") ){
	CopyAlloc(tent->target, model);	
	} else {
	CopyAlloc(tent->target, target);	
	}
	if(!Q_stricmp (class, "NPC_Enemy")){
	G_AddBot(tent->clientname, atof(skill), "Blue", 0, name, tent->s.number, tent->target, NPC_ENEMY );
	}
	if(!Q_stricmp (class, "NPC_Citizen")){
	G_AddBot(tent->clientname, atof(skill), "Blue", 0, name, tent->s.number, tent->target, NPC_CITIZEN );
	}
	if(!Q_stricmp (class, "NPC_Guard")){
	G_AddBot(tent->clientname, atof(skill), "Blue", 0, name, tent->s.number, tent->target, NPC_GUARD );
	}
	if(!Q_stricmp (class, "NPC_Partner")){
	G_AddBot(tent->clientname, atof(skill), "Blue", 0, name, tent->s.number, tent->target, NPC_PARTNER );
	}
	
	trap_Cvar_Set("g_spSkill", skill);
	
	//G_Printf( va("usage: %s %s %s %s %s %s %s\n", model, class, skill, health, name, music, weapon) );
	
    if( trap_Argc( ) < 6 ){
	G_Printf( "usage: <model> <class> <skill> <health> <name> <music> <weapon>\n" );
    return;
	}
	
	
}

/*
==================
Cmd_DistanceProp_f
Added for QSandbox.
==================
*/
static void Cmd_DistanceProp_f( gentity_t *ent ){
	vec3_t		end, start, forward, up, right;
	char		mode[MAX_TOKEN_CHARS];
	trace_t		tr;
	
	if(!g_building.integer){
	return;	
	}
	
	trap_Argv( 1, mode, sizeof( mode ) );
	if(atoi(mode) == 1){
	ent->InteractDist -= 5;
	if(ent->InteractDist < 55){
	ent->InteractDist = 55;
	}
	}
	if(atoi(mode) == 2){
	ent->InteractDist += 5;
	if(ent->InteractDist > 4096){
	ent->InteractDist = 4096;
	}
	}
	
}

/*
==================
Cmd_ModifyProp_f
KK-OAX Added for QSandbox.
==================
*/
static void Cmd_ModifyProp_f( gentity_t *ent ){
	char		modifier[MAX_TOKEN_CHARS];
	char		value1[MAX_TOKEN_CHARS];
	char		value2[MAX_TOKEN_CHARS];
	gentity_t		*entscr;
	
	if(!g_building.integer){
	return;	
	}
		
	//tr.endpos
	trap_Argv( 1, modifier, sizeof( modifier ) );
	trap_Argv( 2, value1, sizeof( value1 ) );
	trap_Argv( 3, value2, sizeof( value2 ) );
	
	while ( (entscr = G_Find(entscr, FOFS(selectedpr), "1")) != NULL ) {
	if(entscr->s.generic2){
		if(atoi(modifier) == 0){
			entscr->s.generic2 = 0;
			entscr->selectedpr = "0";
		}
		if(atoi(modifier) == 1){
		
		}
		if(atoi(modifier) == 2){
			G_FreeEntity(entscr);
		}
		if(atoi(modifier) == 3){
			entscr->s.modelindex = G_ModelIndex( va("props/%s",value1) );
			CopyAlloc(entscr->sb_model, value1);
		}
	}
	}

    if( trap_Argc( ) < 2 ){
	G_Printf( "usage: <modifier> <value1> <value2>\n" );
    return;
	}
	
	
}


static void Cmd_FlashlightOn_f( gentity_t *ent ){
	char        arg[MAX_TOKEN_CHARS];
		
	ent->flashon = 1;

    trap_Argv( 0, arg, sizeof( arg ) );

	Laser_Gen( ent );//Flashlight toggle
	
}

static void Cmd_FlashlightOff_f( gentity_t *ent ){
	char        arg[MAX_TOKEN_CHARS];
	
	ent->flashon = 0;

    trap_Argv( 0, arg, sizeof( arg ) );

if(ent->client->lasersight){
	G_FreeEntity( ent->client->lasersight );
	ent->client->lasersight = NULL;	
	return;
}
if(!ent->client->lasersight){
ent->flashon = 1;
Cmd_FlashlightOn_f( ent );
}
	
}


/*
==================
Cmd_Flashlight_f
KK-OAX Added for QSandbox.
==================
*/
static void Cmd_Flashlight_f( gentity_t *ent ){
	char        arg[MAX_TOKEN_CHARS];

if(ent->flashon != 1){
	if ( ent->client->ps.stats[STAT_FLASH] <= 20 ){
		return;	
	}
	Cmd_FlashlightOn_f( ent );
	return;
}
if(ent->flashon == 1){
	Cmd_FlashlightOff_f( ent );
	return;
}
	
}

/*
==================
Cmd_Tell_f
==================
*/
static void Cmd_Tell_f( gentity_t *ent ) {
	int			targetNum;
	gentity_t	*target;
	char		*p;
	char		arg[MAX_TOKEN_CHARS];

	if ( trap_Argc () < 2 ) {
		return;
	}

	trap_Argv( 1, arg, sizeof( arg ) );
	targetNum = atoi( arg );
	if ( targetNum < 0 || targetNum >= level.maxclients ) {
		return;
	}

	target = &g_entities[targetNum];
	if ( !target || !target->inuse || !target->client ) {
		return;
	}

	p = ConcatArgs( 2 );

	G_LogPrintf( "tell: %s to %s: %s\n", ent->client->pers.netname, target->client->pers.netname, p );
	G_Say( ent, target, SAY_TELL, p );
	// don't tell to the player self if it was already directed to this player
	// also don't send the chat back to a bot
	if ( ent != target && !(ent->r.svFlags & SVF_BOT)) {
		G_Say( ent, ent, SAY_TELL, p );
	}
}


static void G_VoiceTo( gentity_t *ent, gentity_t *other, int mode, const char *id, qboolean voiceonly ) {
	int color;
	char *cmd;

	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}
	if ( mode == SAY_TEAM && !OnSameTeam(ent, other) ) {
		return;
	}
	// no chatting to players in tournements
	if ( (g_gametype.integer == GT_TOURNAMENT )) {
		return;
	}

	if (mode == SAY_TEAM) {
		color = COLOR_CYAN;
		cmd = "vtchat";
	}
	else if (mode == SAY_TELL) {
		color = COLOR_MAGENTA;
		cmd = "vtell";
	}
	else {
		color = COLOR_GREEN;
		cmd = "vchat";
	}

	trap_SendServerCommand( other-g_entities, va("%s %d %d %d %s", cmd, voiceonly, ent->s.number, color, id));
}

void G_Voice( gentity_t *ent, gentity_t *target, int mode, const char *id, qboolean voiceonly ) {
	int			j;
	gentity_t	*other;

	if ( (g_gametype.integer < GT_TEAM || g_ffa_gt==1 ) && mode == SAY_TEAM ) {
		mode = SAY_ALL;
	}

	if ( target ) {
		G_VoiceTo( ent, target, mode, id, voiceonly );
		return;
	}

	// echo the text to the console
	if ( g_dedicated.integer ) {
		G_Printf( "voice: %s %s\n", ent->client->pers.netname, id);
	}

	// send it to all the apropriate clients
	for (j = 0; j < level.maxclients; j++) {
		other = &g_entities[j];
		G_VoiceTo( ent, other, mode, id, voiceonly );
	}
}

/*
==================
Cmd_Voice_f
KK-OAX Modified this to trap args.

In the original, every call to this function would always set "arg0" to false, and it was
never passed along to other functions, so I removed/commented it out.
==================
*/
static void Cmd_Voice_f( gentity_t *ent ) {
	char		*p;
    char        arg[MAX_TOKEN_CHARS];
    int         mode = SAY_ALL;
    qboolean    voiceonly = qfalse;

	trap_Argv( 0, arg, sizeof( arg ) );
	if((Q_strequal( arg, "vsay_team" ) ) ||
	    Q_strequal( arg, "vosay_team" ) )
	    mode = SAY_TEAM;

	if((Q_strequal( arg, "vosay" ) ) ||
	    Q_strequal( arg, "vosay_team" ) )
	    voiceonly = qtrue;

    //KK-OAX Removed "arg0" since it will always be set to qfalse.
	if ( trap_Argc () < 2  ) {
		return;
	}
    //KK-OAX This was tricky to figure out, but since none of the original command handlings
    //set it to "qtrue"...

	/*if (arg0)
	{
		p = ConcatArgs( 0 );
	}
	else
	{*/
	p = ConcatArgs( 1 );
	//}

	G_Voice( ent, NULL, mode, p, voiceonly );
}

/*
==================
Cmd_VoiceTell_f
KK-OAX Modified this to trap args.
==================
*/
static void Cmd_VoiceTell_f( gentity_t *ent ) {
	int			targetNum;
	gentity_t	*target;
	char		*id;
	char		arg[MAX_TOKEN_CHARS];
	qboolean    voiceonly = qfalse;

	if ( trap_Argc () < 2 ) {
		return;
	}

	trap_Argv( 0, arg, sizeof( arg ) );
	if( Q_strequal( arg, "votell" ) )
	    voiceonly = qtrue;

	trap_Argv( 1, arg, sizeof( arg ) );
	targetNum = atoi( arg );
	if ( targetNum < 0 || targetNum >= level.maxclients ) {
		return;
	}

	target = &g_entities[targetNum];
	if ( !target || !target->inuse || !target->client ) {
		return;
	}

	id = ConcatArgs( 2 );

	G_LogPrintf( "vtell: %s to %s: %s\n", ent->client->pers.netname, target->client->pers.netname, id );
	G_Voice( ent, target, SAY_TELL, id, voiceonly );
	// don't tell to the player self if it was already directed to this player
	// also don't send the chat back to a bot
	if ( ent != target && !(ent->r.svFlags & SVF_BOT)) {
		G_Voice( ent, ent, SAY_TELL, id, voiceonly );
	}
}


/*
==================
Cmd_VoiceTaunt_f
==================
*/
static void Cmd_VoiceTaunt_f( gentity_t *ent ) {
	gentity_t *who;
	int i;

	if (!ent->client) {
		return;
	}

	// insult someone who just killed you
	if (ent->enemy && ent->enemy->client && ent->enemy->client->lastkilled_client == ent->s.number) {
		// i am a dead corpse
		if (!(ent->enemy->r.svFlags & SVF_BOT)) {
			G_Voice( ent, ent->enemy, SAY_TELL, VOICECHAT_DEATHINSULT, qfalse );
		}
		if (!(ent->r.svFlags & SVF_BOT)) {
			G_Voice( ent, ent,        SAY_TELL, VOICECHAT_DEATHINSULT, qfalse );
		}
		ent->enemy = NULL;
		return;
	}
	// insult someone you just killed
	if (ent->client->lastkilled_client >= 0 && ent->client->lastkilled_client != ent->s.number) {
		who = g_entities + ent->client->lastkilled_client;
		if (who->client) {
			// who is the person I just killed
			if (who->client->lasthurt_mod == MOD_GAUNTLET) {
				if (!(who->r.svFlags & SVF_BOT)) {
					G_Voice( ent, who, SAY_TELL, VOICECHAT_KILLGAUNTLET, qfalse );	// and I killed them with a gauntlet
				}
				if (!(ent->r.svFlags & SVF_BOT)) {
					G_Voice( ent, ent, SAY_TELL, VOICECHAT_KILLGAUNTLET, qfalse );
				}
			} else {
				if (!(who->r.svFlags & SVF_BOT)) {
					G_Voice( ent, who, SAY_TELL, VOICECHAT_KILLINSULT, qfalse );	// and I killed them with something else
				}
				if (!(ent->r.svFlags & SVF_BOT)) {
					G_Voice( ent, ent, SAY_TELL, VOICECHAT_KILLINSULT, qfalse );
				}
			}
			ent->client->lastkilled_client = -1;
			return;
		}
	}

	if (g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		// praise a team mate who just got a reward
		for(i = 0; i < MAX_CLIENTS; i++) {
			who = g_entities + i;
			if (who->client && who != ent && who->client->sess.sessionTeam == ent->client->sess.sessionTeam) {
				if (who->client->rewardTime > level.time) {
					if (!(who->r.svFlags & SVF_BOT)) {
						G_Voice( ent, who, SAY_TELL, VOICECHAT_PRAISE, qfalse );
					}
					if (!(ent->r.svFlags & SVF_BOT)) {
						G_Voice( ent, ent, SAY_TELL, VOICECHAT_PRAISE, qfalse );
					}
					return;
				}
			}
		}
	}

	// just say something
	G_Voice( ent, NULL, SAY_ALL, VOICECHAT_TAUNT, qfalse );
}



static char	*gc_orders[] = {
	"hold your position",
	"hold this position",
	"come here",
	"cover me",
	"guard location",
	"search and destroy",
	"report"
};

void Cmd_GameCommand_f( gentity_t *ent ) {
	int		player;
	int		order;
	char	str[MAX_TOKEN_CHARS];

	trap_Argv( 1, str, sizeof( str ) );
	player = atoi( str );
	trap_Argv( 2, str, sizeof( str ) );
	order = atoi( str );

	if ( player < 0 || player >= MAX_CLIENTS ) {
		return;
	}
	if ( order < 0 || order > sizeof(gc_orders)/sizeof(char *) ) {
		return;
	}
	G_Say( ent, &g_entities[player], SAY_TELL, gc_orders[order] );
	G_Say( ent, ent, SAY_TELL, gc_orders[order] );
}

/*
==================
Cmd_Where_f
==================
*/
void Cmd_Where_f( gentity_t *ent ) {

	trap_SendServerCommand( ent-g_entities, va("print \"%s\n\"", vtos(ent->r.currentOrigin) ) );

}

static const char *gameNames[] = {
	"Free For All",
	"Tournament",
	"Single Player",
	"Team Deathmatch",
	"Capture the Flag",
	"One Flag CTF",
	"Overload",
	"Harvester",
	"Elimination",
	"CTF Elimination",
	"Last Man Standing",
	"Double Domination",
	"Domination"
};



/*
==================
Cmd_CallVote_f
==================
*/
void Cmd_CallVote_f( gentity_t *ent ) {
        char*	c;
	int		i;
	char	arg1[MAX_STRING_TOKENS];
	char	arg2[MAX_STRING_TOKENS];
        char    buffer[256];

	if ( !g_allowVote.integer ) {
		trap_SendServerCommand( ent-g_entities, "print \"Voting not allowed here.\n\"" );
		return;
	}

	if ( level.voteTime ) {
		trap_SendServerCommand( ent-g_entities, "print \"A vote is already in progress.\n\"" );
		return;
	}
	if ( ent->client->pers.voteCount >= g_maxvotes.integer ) {
		trap_SendServerCommand( ent-g_entities, "print \"You have called the maximum number of votes.\n\"" );
		return;
	}
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		trap_SendServerCommand( ent-g_entities, "print \"Not allowed to call a vote as spectator.\n\"" );
		return;
	}

	// make sure it is a valid command to vote on
	trap_Argv( 1, arg1, sizeof( arg1 ) );
	trap_Argv( 2, arg2, sizeof( arg2 ) );

	// check for command separators in arg2
	for( c = arg2; *c; ++c) {
		switch(*c) {
			case '\n':
			case '\r':
			case ';':
				trap_SendServerCommand( ent-g_entities, "print \"Invalid vote string.\n\"" );
				return;
			break;
		}
        }


	if ( !Q_stricmp( arg1, "map_restart" ) ) {
	} else if ( !Q_stricmp( arg1, "nextmap" ) ) {
	} else if ( !Q_stricmp( arg1, "map" ) ) {
	} else if ( !Q_stricmp( arg1, "g_gametype" ) ) {
	} else if ( !Q_stricmp( arg1, "kick" ) ) {
	} else if ( !Q_stricmp( arg1, "clientkick" ) ) {
	} else if ( !Q_stricmp( arg1, "g_doWarmup" ) ) {
	} else if ( !Q_stricmp( arg1, "timelimit" ) ) {
	} else if ( !Q_stricmp( arg1, "fraglimit" ) ) {
        } else if ( !Q_stricmp( arg1, "custom" ) ) {
        } else if ( !Q_stricmp( arg1, "shuffle" ) ) {
	} else {
		trap_SendServerCommand( ent-g_entities, "print \"Invalid vote string.\n\"" );
		//trap_SendServerCommand( ent-g_entities, "print \"Vote commands are: map_restart, nextmap, map <mapname>, g_gametype <n>, kick <player>, clientkick <clientnum>, g_doWarmup, timelimit <time>, fraglimit <frags>.\n\"" );
                buffer[0] = 0;
                strcat(buffer,"print \"Vote commands are: ");
                if(allowedVote("map_restart"))
                    strcat(buffer, "map_restart, ");
                if(allowedVote("nextmap"))
                    strcat(buffer, "nextmap, ");
                if(allowedVote("map"))
                    strcat(buffer, "map <mapname>, ");
                if(allowedVote("g_gametype"))
                    strcat(buffer, "g_gametype <n>, ");
                if(allowedVote("kick"))
                    strcat(buffer, "kick <player>, ");
                if(allowedVote("clientkick"))
                    strcat(buffer, "clientkick <clientnum>, ");
                if(allowedVote("g_doWarmup"))
                    strcat(buffer, "g_doWarmup, ");
                if(allowedVote("timelimit"))
                    strcat(buffer, "timelimit <time>, ");
                if(allowedVote("fraglimit"))
                    strcat(buffer, "fraglimit <frags>, ");
                if(allowedVote("shuffle"))
                    strcat(buffer, "shuffle, ");
                if(allowedVote("custom"))
                    strcat(buffer, "custom <special>, ");
                buffer[strlen(buffer)-2] = 0;
                strcat(buffer, ".\"");
                trap_SendServerCommand( ent-g_entities, buffer);
		return;
	}

        if(!allowedVote(arg1)) {
                trap_SendServerCommand( ent-g_entities, "print \"Not allowed here.\n\"" );
                buffer[0] = 0;
                strcat(buffer,"print \"Vote commands are: ");
                if(allowedVote("map_restart"))
                    strcat(buffer, "map_restart, ");
                if(allowedVote("nextmap"))
                    strcat(buffer, "nextmap, ");
                if(allowedVote("map"))
                    strcat(buffer, "map <mapname>, ");
                if(allowedVote("g_gametype"))
                    strcat(buffer, "g_gametype <n>, ");
                if(allowedVote("kick"))
                    strcat(buffer, "kick <player>, ");
                if(allowedVote("clientkick"))
                    strcat(buffer, "clientkick <clientnum>, ");
                if(allowedVote("shuffle"))
                    strcat(buffer, "shuffle, ");
                if(allowedVote("g_doWarmup"))
                    strcat(buffer, "g_doWarmup, ");
                if(allowedVote("timelimit"))
                    strcat(buffer, "timelimit <time>, ");
                if(allowedVote("fraglimit"))
                    strcat(buffer, "fraglimit <frags>, ");
                if(allowedVote("custom"))
                    strcat(buffer, "custom <special>, ");
                buffer[strlen(buffer)-2] = 0;
                strcat(buffer, ".\"");
                trap_SendServerCommand( ent-g_entities, buffer);
		return;
        }

	// if there is still a vote to be executed
	if ( level.voteExecuteTime ) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );
	}

        level.voteKickClient = -1; //not a client
        level.voteKickType = 0; //not a ban

	// special case for g_gametype, check for bad values
	if ( !Q_stricmp( arg1, "g_gametype" ) ) {
                char	s[MAX_STRING_CHARS];
		i = atoi( arg2 );
		if( i == GT_SINGLE_PLAYER || i < GT_FFA || i >= GT_MAX_GAME_TYPE) {
			trap_SendServerCommand( ent-g_entities, "print \"Invalid gametype.\n\"" );
			return;
		}

                if( i== g_gametype.integer ) {
                    trap_SendServerCommand( ent-g_entities, "print \"This is current gametype\n\"" );
			return;
                }

                if(!allowedGametype(arg2)){
                    trap_SendServerCommand( ent-g_entities, "print \"Gametype is not available.\n\"" );
                    return;
                }

                trap_Cvar_VariableStringBuffer( "nextmap", s, sizeof(s) );
		if (*s) {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "%s %d; map_restart; set nextmap \"%s\"", arg1, i,s );
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Change gametype to: %s?", gameNames[i] );
                } else {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "%s %d; mao_restart", arg1, i );
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Change gametype to: %s?", gameNames[i] );
                }
	} else if ( !Q_stricmp( arg1, "map" ) ) {
		// special case for map changes, we want to reset the nextmap setting
		// this allows a player to change maps, but not upset the map rotation
		char	s[MAX_STRING_CHARS];

                if(!allowedMap(arg2)){
                    trap_SendServerCommand( ent-g_entities, "print \"Map is not available.\n\"" );
                    return;
                }

		trap_Cvar_VariableStringBuffer( "nextmap", s, sizeof(s) );
		if (*s) {
			Com_sprintf( level.voteString, sizeof( level.voteString ), "%s %s; set nextmap \"%s\"", arg1, arg2, s );
		} else {
			Com_sprintf( level.voteString, sizeof( level.voteString ), "%s %s", arg1, arg2 );
		}
		//Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", level.voteString );
                Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Change map to: %s?", arg2 );
	} else if ( !Q_stricmp( arg1, "nextmap" ) ) {
		char	s[MAX_STRING_CHARS];

                //Sago: Needs to think about this, we miss code to parse if nextmap has arg2
                /*if(!allowedMap(arg2)){
                    trap_SendServerCommand( ent-g_entities, "print \"Map is not available.\n\"" );
                    return;
                }*/

                if(g_autonextmap.integer) {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "endgamenow");
                } else {
                    trap_Cvar_VariableStringBuffer( "nextmap", s, sizeof(s) );
                    if (!*s) {
                            trap_SendServerCommand( ent-g_entities, "print \"nextmap not set.\n\"" );
                            return;
                    }
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "vstr nextmap");
                }

		//Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", level.voteString );
                Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", "Next map?" );
        } else if ( !Q_stricmp( arg1, "fraglimit" ) ) {
                i = atoi(arg2);
                if(!allowedFraglimit(i)) {
                    trap_SendServerCommand( ent-g_entities, "print \"Cannot set fraglimit.\n\"" );
                    return;
                }

                Com_sprintf( level.voteString, sizeof( level.voteString ), "%s \"%d\"", arg1, i );
                if(i)
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Change fraglimit to: %d", i );
                else
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Remove fraglimit?");
        } else if ( !Q_stricmp( arg1, "timelimit" ) ) {
                i = atoi(arg2);
                if(!allowedTimelimit(i)) {
                    trap_SendServerCommand( ent-g_entities, "print \"Cannot set timelimit.\n\"" );
                    return;
                }

                Com_sprintf( level.voteString, sizeof( level.voteString ), "%s \"%d\"", arg1, i );
                if(i)
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Change timelimit to: %d", i );
                else
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Remove timelimit?" );
        } else if ( !Q_stricmp( arg1, "map_restart" ) ) {
                Com_sprintf( level.voteString, sizeof( level.voteString ), "map_restart" );
		Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Restart map?" );
        } else if ( !Q_stricmp( arg1, "g_doWarmup" ) ) {
                i = atoi(arg2);
                if(i) {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "g_doWarmup \"1\"" );
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Enable warmup?" );
                }
                else {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "g_doWarmup \"0\"" );
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Disable warmup?" );
                }
        } else if ( !Q_stricmp( arg1, "clientkick" ) ) {
                i = atoi(arg2);

                if(i>=MAX_CLIENTS) { //Only numbers <128 is clients
                    trap_SendServerCommand( ent-g_entities, "print \"Cannot kick that number.\n\"" );
                    return;
                }
                level.voteKickClient = i;
                if(g_voteBan.integer<1) {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "clientkick_game \"%d\"", i );
                } else {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "!ban \"%d\" \"%dm\" \"Banned by public vote\"", i, g_voteBan.integer );
                    level.voteKickType = 1; //ban
                }
		Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Kick %s?" , level.clients[i].pers.netname );
        } else if ( !Q_stricmp( arg1, "shuffle" ) ) {
                if(g_gametype.integer<GT_TEAM || g_ffa_gt==1) { //Not a team game
                    trap_SendServerCommand( ent-g_entities, "print \"Can only be used in team games.\n\"" );
                    return;
                }

                Com_sprintf( level.voteString, sizeof( level.voteString ), "shuffle" );
		Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Shuffle teams?" );
        } else if ( !Q_stricmp( arg1, "kick" ) ) {
                i = 0;
                while(Q_stricmp(arg2,(g_entities+i)->client->pers.netname)) {
                    //Not client i, try next
                    i++;
                    if(i>=MAX_CLIENTS){ //Only numbers <128 is clients
                        trap_SendServerCommand( ent-g_entities, "print \"Cannot find the playername. Try clientkick instead.\n\"" );
                        return;
                    }
                }
                level.voteKickClient = i;
                if(g_voteBan.integer<1) {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "clientkick_game \"%d\"", i );
                } else {
                    Com_sprintf( level.voteString, sizeof( level.voteString ), "!ban \"%d\" \"%dm\" \"Banned by public vote\"", i, g_voteBan.integer );
                    level.voteKickType = 1; //ban
                }
		Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "Kick %s?" , level.clients[i].pers.netname );
        } else if ( !Q_stricmp( arg1, "custom" ) ) {
                t_customvote customvote;
                //Sago: There must always be a test to ensure that length(arg2) is non-zero or the client might be able to execute random commands.
                if(strlen(arg2)<1) {
                    trap_SendServerCommand( ent-g_entities, va("print \"Custom vote commands are: %s\n\"",custom_vote_info) );
                    return;
                }
                customvote = getCustomVote(arg2);
                if(Q_stricmp(customvote.votename,arg2)) {
                    trap_SendServerCommand( ent-g_entities, "print \"Command could not be found\n\"" );
                    return;
                }
                Com_sprintf( level.voteString, sizeof( level.voteString ), "%s", customvote.command );
                if(strlen(customvote.displayname))
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", customvote.displayname );
                else
                    Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", customvote.command );
	} else {
		//Com_sprintf( level.voteString, sizeof( level.voteString ), "%s \"%s\"", arg1, arg2 );
		//Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", level.voteString );
                trap_SendServerCommand( ent-g_entities, "print \"Server vality check failed, appears to be my fault. Sorry\n\"" );
                return;
	}

        ent->client->pers.voteCount++;
	trap_SendServerCommand( -1, va("print \"%s called a vote.\n\"", ent->client->pers.netname ) );

	// start the voting, the caller autoamtically votes yes
	level.voteTime = level.time;
	level.voteYes = 1;
	level.voteNo = 0;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		level.clients[i].ps.eFlags &= ~EF_VOTED;
                level.clients[i].vote = 0;
	}
	ent->client->ps.eFlags |= EF_VOTED;
        ent->client->vote = 1;
        //Do a first count to make sure that numvotingclients is correct!
        CountVotes();

	trap_SetConfigstring( CS_VOTE_TIME, va("%i", level.voteTime ) );
	trap_SetConfigstring( CS_VOTE_STRING, level.voteDisplayString );
	trap_SetConfigstring( CS_VOTE_YES, va("%i", level.voteYes ) );
	trap_SetConfigstring( CS_VOTE_NO, va("%i", level.voteNo ) );
}

/*
==================
Cmd_Vote_f
==================
*/
void Cmd_Vote_f( gentity_t *ent ) {
	char		msg[64];

	if ( !level.voteTime ) {
		trap_SendServerCommand( ent-g_entities, "print \"No vote in progress.\n\"" );
		return;
	}
	/*if ( ent->client->ps.eFlags & EF_VOTED ) {
		trap_SendServerCommand( ent-g_entities, "print \"Vote already cast.\n\"" );
		return;
	}*/
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		trap_SendServerCommand( ent-g_entities, "print \"Not allowed to vote as spectator.\n\"" );
		return;
	}

	trap_SendServerCommand( ent-g_entities, "print \"Vote cast.\n\"" );

	ent->client->ps.eFlags |= EF_VOTED;

	trap_Argv( 1, msg, sizeof( msg ) );

	if ( msg[0] == 'y' || msg[1] == 'Y' || msg[1] == '1' ) {
                ent->client->vote = 1;
	} else {
                ent->client->vote = -1;
	}

        //Re count the votes
        CountVotes();

	// a majority will be determined in CheckVote, which will also account
	// for players entering or leaving
}

/*
==================
Cmd_CallTeamVote_f
==================
*/
void Cmd_CallTeamVote_f( gentity_t *ent ) {
	int		i, team, cs_offset;
	char	arg1[MAX_STRING_TOKENS];
	char	arg2[MAX_STRING_TOKENS];

	team = ent->client->sess.sessionTeam;
	if ( team == TEAM_RED )
		cs_offset = 0;
	else if ( team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !g_allowVote.integer ) {
		trap_SendServerCommand( ent-g_entities, "print \"Voting not allowed here.\n\"" );
		return;
	}

	if ( level.teamVoteTime[cs_offset] ) {
		trap_SendServerCommand( ent-g_entities, "print \"A team vote is already in progress.\n\"" );
		return;
	}
	if ( ent->client->pers.teamVoteCount >= g_maxvotes.integer ) {
		trap_SendServerCommand( ent-g_entities, "print \"You have called the maximum number of team votes.\n\"" );
		return;
	}
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		trap_SendServerCommand( ent-g_entities, "print \"Not allowed to call a vote as spectator.\n\"" );
		return;
	}

	// make sure it is a valid command to vote on
	trap_Argv( 1, arg1, sizeof( arg1 ) );
	arg2[0] = '\0';
	for ( i = 2; i < trap_Argc(); i++ ) {
		if (i > 2)
			strcat(arg2, " ");
		trap_Argv( i, &arg2[strlen(arg2)], sizeof( arg2 ) - strlen(arg2) );
	}

	if( strchr( arg1, ';' ) || strchr( arg2, ';' ) ) {
		trap_SendServerCommand( ent-g_entities, "print \"Invalid vote string.\n\"" );
		return;
	}

	if ( !Q_stricmp( arg1, "leader" ) ) {
		char netname[MAX_NETNAME], leader[MAX_NETNAME];

		if ( !arg2[0] ) {
			i = ent->client->ps.clientNum;
		}
		else {
			// numeric values are just slot numbers
			for (i = 0; i < 3; i++) {
				if ( !arg2[i] || arg2[i] < '0' || arg2[i] > '9' )
					break;
			}
			if ( i >= 3 || !arg2[i]) {
				i = atoi( arg2 );
				if ( i < 0 || i >= level.maxclients ) {
					trap_SendServerCommand( ent-g_entities, va("print \"Bad client slot: %i\n\"", i) );
					return;
				}

				if ( !g_entities[i].inuse ) {
					trap_SendServerCommand( ent-g_entities, va("print \"Client %i is not active\n\"", i) );
					return;
				}
			}
			else {
				Q_strncpyz(leader, arg2, sizeof(leader));
				Q_CleanStr(leader);
				for ( i = 0 ; i < level.maxclients ; i++ ) {
					if ( level.clients[i].pers.connected == CON_DISCONNECTED )
						continue;
					if (level.clients[i].sess.sessionTeam != team)
						continue;
					Q_strncpyz(netname, level.clients[i].pers.netname, sizeof(netname));
					Q_CleanStr(netname);
					if ( !Q_stricmp(netname, leader) ) {
						break;
					}
				}
				if ( i >= level.maxclients ) {
					trap_SendServerCommand( ent-g_entities, va("print \"%s is not a valid player on your team.\n\"", arg2) );
					return;
				}
			}
		}
		Com_sprintf(arg2, sizeof(arg2), "%d", i);
	} else {
		trap_SendServerCommand( ent-g_entities, "print \"Invalid vote string.\n\"" );
		trap_SendServerCommand( ent-g_entities, "print \"Team vote commands are: leader <player>.\n\"" );
		return;
	}

	Com_sprintf( level.teamVoteString[cs_offset], sizeof( level.teamVoteString[cs_offset] ), "%s %s", arg1, arg2 );

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected == CON_DISCONNECTED )
			continue;
		if (level.clients[i].sess.sessionTeam == team)
			trap_SendServerCommand( i, va("print \"%s called a team vote.\n\"", ent->client->pers.netname ) );
	}

	// start the voting, the caller autoamtically votes yes
	level.teamVoteTime[cs_offset] = level.time;
	level.teamVoteYes[cs_offset] = 1;
	level.teamVoteNo[cs_offset] = 0;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam == team)
			level.clients[i].ps.eFlags &= ~EF_TEAMVOTED;
	}
	ent->client->ps.eFlags |= EF_TEAMVOTED;

	trap_SetConfigstring( CS_TEAMVOTE_TIME + cs_offset, va("%i", level.teamVoteTime[cs_offset] ) );
	trap_SetConfigstring( CS_TEAMVOTE_STRING + cs_offset, level.teamVoteString[cs_offset] );
	trap_SetConfigstring( CS_TEAMVOTE_YES + cs_offset, va("%i", level.teamVoteYes[cs_offset] ) );
	trap_SetConfigstring( CS_TEAMVOTE_NO + cs_offset, va("%i", level.teamVoteNo[cs_offset] ) );
}

/*
==================
Cmd_TeamVote_f
==================
*/
void Cmd_TeamVote_f( gentity_t *ent ) {
	int			team, cs_offset;
	char		msg[64];

	team = ent->client->sess.sessionTeam;
	if ( team == TEAM_RED )
		cs_offset = 0;
	else if ( team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !level.teamVoteTime[cs_offset] ) {
		trap_SendServerCommand( ent-g_entities, "print \"No team vote in progress.\n\"" );
		return;
	}
	if ( ent->client->ps.eFlags & EF_TEAMVOTED ) {
		trap_SendServerCommand( ent-g_entities, "print \"Team vote already cast.\n\"" );
		return;
	}
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		trap_SendServerCommand( ent-g_entities, "print \"Not allowed to vote as spectator.\n\"" );
		return;
	}

	trap_SendServerCommand( ent-g_entities, "print \"Team vote cast.\n\"" );

	ent->client->ps.eFlags |= EF_TEAMVOTED;

	trap_Argv( 1, msg, sizeof( msg ) );

	if ( msg[0] == 'y' || msg[1] == 'Y' || msg[1] == '1' ) {
		level.teamVoteYes[cs_offset]++;
		trap_SetConfigstring( CS_TEAMVOTE_YES + cs_offset, va("%i", level.teamVoteYes[cs_offset] ) );
	} else {
		level.teamVoteNo[cs_offset]++;
		trap_SetConfigstring( CS_TEAMVOTE_NO + cs_offset, va("%i", level.teamVoteNo[cs_offset] ) );
	}

	// a majority will be determined in TeamCheckVote, which will also account
	// for players entering or leaving
}


/*
=================
Cmd_SetViewpos_f
=================
*/
void Cmd_SetViewpos_f( gentity_t *ent ) {
	vec3_t		origin, angles;
	char		buffer[MAX_TOKEN_CHARS];
	int			i;

	if ( !g_cheats.integer ) {
		trap_SendServerCommand( ent-g_entities, va("print \"Cheats are not enabled on this server.\n\""));
		return;
	}
	if ( trap_Argc() != 5 ) {
		trap_SendServerCommand( ent-g_entities, va("print \"usage: setviewpos x y z yaw\n\""));
		return;
	}

	VectorClear( angles );
	for ( i = 0 ; i < 3 ; i++ ) {
		trap_Argv( i + 1, buffer, sizeof( buffer ) );
		origin[i] = atof( buffer );
	}

	trap_Argv( 4, buffer, sizeof( buffer ) );
	angles[YAW] = atof( buffer );

	TeleportPlayer( ent, origin, angles );
}

/*
=================
Cmd_UseTarget_f
=================
*/
void Cmd_UseTarget_f( gentity_t *ent ) {
	char		*p;
	char        arg[MAX_TOKEN_CHARS];
	gentity_t 	*act;

    trap_Argv( 0, arg, sizeof( arg ) );

    if( trap_Argc( ) < 2 )
        return;

    p = ConcatArgs( 1 );
	
	ent->target = p;

	act = G_PickTarget( ent->target );
	if ( act && act->use && act->allowuse ) {
		act->use( act, ent, ent );
	}
}

/*
=================
Cmd_UseCvar_f
=================
*/
void Cmd_UseCvar_f( gentity_t *ent ) {
	char		p1[64];
	char		p2[64];
	char		p3[64];
	int			i;
	static const char* cvar_items[] = {
	"g_minigame",
	"g_ghspeed",
	"g_ghtimeout",
	"g_gdelay",
	"g_gdamage",
	"g_grange",
	"g_gknockback",
	"g_mgdelay",
	"g_mgdamage",
	"g_mgspread",
	"g_mgexplode",
	"g_mgsdamage",
	"g_mgsradius",
	"g_mgvampire",
	"g_mginf",
	"g_mgknockback",
	"g_sgdelay",
	"g_sgdamage",
	"g_sgspread",
	"g_sgexplode",
	"g_sgsdamage",
	"g_sgsradius",
	"g_sgcount",
	"g_sgvampire",
	"g_sginf",
	"g_sgknockback",
	"g_gldelay",
	"g_glspeed",
	"g_gltimeout",
	"g_glsradius",
	"g_glsdamage",
	"g_gldamage",
	"g_glbounce",
	"g_glgravity",
	"g_glvampire",
	"g_glinf",
	"g_glbouncemodifier",
	"g_glknockback",
	"g_rldelay",
	"g_rlspeed",
	"g_rltimeout",
	"g_rlsradius",
	"g_rlsdamage",
	"g_rldamage",
	"g_rlbounce",
	"g_rlgravity",
	"g_rlvampire",
	"g_rlinf",
	"g_rlbouncemodifier",
	"g_rlknockback",
	"g_lgdamage",
	"g_lgdelay",
	"g_lgrange",
	"g_lgexplode",
	"g_lgsdamage",
	"g_lgsradius",
	"g_lgvampire",
	"g_lginf",
	"g_lgknockback",
	"g_rgdelay",
	"g_rgdamage",
	"g_rgvampire",
	"g_rginf",
	"g_rgknockback",
	"g_pgdelay",
	"g_pgsradius",
	"g_pgspeed",
	"g_pgsdamage",
	"g_pgdamage",
	"g_pgtimeout",
	"g_pgbounce",
	"g_pggravity",
	"g_pgvampire",
	"g_pginf",
	"g_pgbouncemodifier",
	"g_pgknockback",
	"g_bfgdelay",
	"g_bfgspeed",
	"g_bfgtimeout",
	"g_bfgsradius",
	"g_bfgsdamage",
	"g_bfgdamage",
	"g_bfgbounce",
	"g_bfggravity",
	"g_bfgvampire",
	"g_bfginf",
	"g_bfgbouncemodifier",
	"g_bfgknockback",
	"g_ngdelay",
	"g_ngspeed",
	"g_ngspread",
	"g_ngdamage",
	"g_ngtimeout",
	"g_ngcount",
	"g_ngbounce",
	"g_nggravity",
	"g_ngrandom",
	"g_ngvampire",
	"g_nginf",
	"g_ngbouncemodifier",
	"g_ngknockback",
	"g_pldelay",
	"g_plspeed",
	"g_pltimeout",
	"g_plsradius",
	"g_plsdamage",
	"g_plgravity",
	"g_pldamage",
	"g_plvampire",
	"g_plinf",
	"g_plknockback",
	"g_cgdelay",
	"g_cgdamage",
	"g_cgspread",
	"g_cgvampire",
	"g_cginf",
	"g_cgknockback",
	"g_ftdelay",
	"g_ftsradius",
	"g_ftspeed",
	"g_ftsdamage",
	"g_ftdamage",
	"g_fttimeout",
	"g_ftbounce",
	"g_ftgravity",
	"g_ftvampire",
	"g_ftinf",
	"g_ftbouncemodifier",
	"g_ftknockback",
	"g_amdelay",
	"g_amsradius",
	"g_amspeed",
	"g_amsdamage",
	"g_amdamage",
	"g_amtimeout",
	"g_ambounce",
	"g_amgravity",
	"g_amvampire",
	"g_aminf",
	"g_ambouncemodifier",
	"g_amknockback",
	"g_glhoming",
	"g_glguided",
	"g_rlhoming",
	"g_rlguided",
	"g_pghoming",
	"g_pgguided",
	"g_bfghoming",
	"g_bfgguided",
	"g_nghoming",
	"g_ngguided",
	"g_fthoming",
	"g_ftguided",
	"g_amhoming",
	"g_amguided",
	"g_scoutspeedfactor",
	"g_scoutfirespeed",
	"g_scoutdamagefactor",
	"g_scoutgravitymodifier",
	"g_scout_infammo",
	"g_scouthealthmodifier",
	"g_doublerspeedfactor",
	"g_doublerfirespeed",
	"g_doublerdamagefactor",
	"g_doublergravitymodifier",
	"g_doubler_infammo",
	"g_doublerhealthmodifier",
	"g_guardhealthmodifier",
	"g_guardspeedfactor",
	"g_guardfirespeed",
	"g_guarddamagefactor",
	"g_guardgravitymodifier",
	"g_guard_infammo",
	"g_ammoregenspeedfactor",
	"g_ammoregenfirespeed",
	"g_ammoregen_infammo",
	"g_ammoregendamagefactor",
	"g_ammoregengravitymodifier",
	"g_ammoregenhealthmodifier",
	"g_mgammocount",
	"g_sgammocount",
	"g_glammocount",
	"g_rlammocount",
	"g_lgammocount",
	"g_rgammocount",
	"g_pgammocount",
	"g_bfgammocount",
	"g_ngammocount",
	"g_plammocount",
	"g_cgammocount",
	"g_ftammocount",
	"g_mgweaponcount",
	"g_sgweaponcount",
	"g_glweaponcount",
	"g_rlweaponcount",
	"g_lgweaponcount",
	"g_rgweaponcount",
	"g_pgweaponcount",
	"g_bfgweaponcount",
	"g_ngweaponcount",
	"g_plweaponcount",
	"g_cgweaponcount",
	"g_ftweaponcount",
	"g_amweaponcount",
	"g_teamblue_speed",
	"g_teamblue_gravityModifier",
	"g_teamblue_firespeed",
	"g_teamblue_damage",
	"g_teamblue_infammo",
	"g_teamblue_respawnwait",
	"g_teamblue_pickupitems",
	"g_teamred_speed",
	"g_teamred_gravityModifier",
	"g_teamred_firespeed",
	"g_teamred_damage",
	"g_teamred_infammo",
	"g_teamred_respawnwait",
	"g_teamred_pickupitems",
	"g_flightlimit",
	"g_flightregen",
	"g_flightpower",
	"g_regenarmor",
	"g_spectatorspeed",
	"eliminationrespawn",
	"eliminationredrespawn",
	"g_overlay",
	"g_slickmove",
	"g_accelerate",
	"g_randomItems",
	"g_locationdamage",
	"g_kill",
	"g_kamikazeinf",
	"g_invulinf",
	"g_medkitinf",
	"g_teleporterinf",
	"g_portalinf",
	"g_medkitlimit",
	"g_waterdamage",
	"g_lavadamage",
	"g_slimedamage",
	"g_maxweaponpickup",
	"g_randomteleport",
	"g_falldamagesmall",
	"g_falldamagebig",
	"g_noplayerclip",
	"g_flagrespawn",
	"g_portaltimeout",
	"g_portalhealth",
	"g_quadtime",
	"g_bsuittime",
	"g_hastetime",
	"g_invistime",
	"g_regentime",
	"g_flighttime",
	"g_invulmove",
	"g_invultime",
	"g_fasthealthregen",
	"g_slowhealthregen",
	"g_droppeditemtime",
	"g_autoflagreturn",
	"g_hastefirespeed",
	"g_medkitmodifier",
	"g_armorprotect",
	"g_respawnwait",
	"g_ammolimit",
	"g_jumpheight",
	"g_speedfactor",
	"g_spawnselect",
	"g_drowndamage",
	"g_armorrespawn",
	"g_healthrespawn",
	"g_ammorespawn",
	"g_holdablerespawn",
	"g_megahealthrespawn",
	"g_poweruprespawn",
	"g_speed",
	"g_gravity",
	"g_gravityModifier",
	"g_damageModifier",
	"g_knockback",
	"g_quadfactor",
	"g_forcerespawn",
	"g_respawntime",
	"g_weaponRespawn",
	"g_weaponTeamRespawn",
	"g_proxMineTimeout",
	"elimination_selfdamage",
	"elimination_startHealth",
	"elimination_startArmor",
	"elimination_bfg",
	"elimination_grapple",
	"elimination_roundtime",
	"elimination_warmup",
	"elimination_activewarmup",
	"elimination_allgametypes",
	"elimination_gauntlet",
	"elimination_machinegun",
	"elimination_shotgun",
	"elimination_grenade",
	"elimination_rocket",
	"elimination_railgun",
	"elimination_lightning",
	"elimination_plasmagun",
	"elimination_chain",
	"elimination_mine",
	"elimination_nail",
	"elimination_flame",
	"elimination_antimatter",
	"elimination_quad",
	"elimination_haste",
	"elimination_bsuit",
	"elimination_invis",
	"elimination_regen",
	"elimination_flight",
	"elimination_items",
	"elimination_holdable",
	"eliminationred_startHealth",
	"eliminationred_startArmor",
	"eliminationred_bfg",
	"eliminationred_grapple",
	"eliminationred_gauntlet",
	"eliminationred_machinegun",
	"eliminationred_shotgun",
	"eliminationred_grenade",
	"eliminationred_rocket",
	"eliminationred_railgun",
	"eliminationred_lightning",
	"eliminationred_plasmagun",
	"eliminationred_chain",
	"eliminationred_mine",
	"eliminationred_nail",
	"eliminationred_flame",
	"eliminationred_antimatter",
	"eliminationred_quad",
	"eliminationred_haste",
	"eliminationred_bsuit",
	"eliminationred_invis",
	"eliminationred_regen",
	"eliminationred_flight",
	"eliminationred_holdable",
	"g_vampire",
	"g_vampireMaxHealth",
	"g_regen",
	0
};
if(!g_building.integer){ return; }
if(!g_allowsettings.integer){ return; }

    trap_Argv( 1, p1, sizeof( p1 ) );
	trap_Argv( 2, p2, sizeof( p2 ) );
	trap_Argv( 3, p3, sizeof( p3 ) );
	
	if(atoi(p3) >= 1){
	trap_SendServerCommand( -1, va("print \"Variable %s is %s\n\"", p1, G_ArenaScriptAutoChar( p1 )));
	return;
	}
	
	for (i = 0; i < 343; i++) {
    if (Q_stricmp(p1, cvar_items[i]) == 0) {
	trap_Cvar_Set(p1, va("%s", p2));
	trap_SendServerCommand( -1, va("print \"Variable %s changed to %s\n\"", p1, p2));
	break;
	}
	}
}

/*
=================
Cmd_Stats_f
=================
*/
void Cmd_Stats_f( gentity_t *ent ) {
/*
	int max, n, i;

	max = trap_AAS_PointReachabilityAreaIndex( NULL );

	n = 0;
	for ( i = 0; i < max; i++ ) {
		if ( ent->client->areabits[i >> 3] & (1 << (i & 7)) )
			n++;
	}

	//trap_SendServerCommand( ent-g_entities, va("print \"visited %d of %d areas\n\"", n, max));
	trap_SendServerCommand( ent-g_entities, va("print \"%d%% level coverage\n\"", n * 100 / max));
*/
}

void Cmd_GetMappage_f( gentity_t *ent ) {
        t_mappage page;
        char string[(MAX_MAPNAME+1)*MAPS_PER_PAGE+1];
        char arg[MAX_STRING_TOKENS];
        trap_Argv( 1, arg, sizeof( arg ) );
        page = getMappage(atoi(arg));
        Q_strncpyz(string,va("mappage %d %s %s %s %s %s %s %s %s %s %s",page.pagenumber,page.mapname[0],\
                page.mapname[1],page.mapname[2],page.mapname[3],page.mapname[4],page.mapname[5],\
                page.mapname[6],page.mapname[7],page.mapname[8],page.mapname[9]),sizeof(string));
        //G_Printf("Mappage sent: \"%s\"\n", string);
	trap_SendServerCommand( ent-g_entities, string );
}

//KK-OAX This is the table that ClientCommands runs the console entry against.
commands_t cmds[ ] =
{
  // normal commands
  { "team", 0, Cmd_Team_f },
  { "vote", 0, Cmd_Vote_f },
  /*{ "ignore", 0, Cmd_Ignore_f },
  { "unignore", 0, Cmd_Ignore_f },*/

  // communication commands
  { "tell", CMD_MESSAGE, Cmd_Tell_f },
  { "callvote", CMD_MESSAGE, Cmd_CallVote_f },
  { "callteamvote", CMD_MESSAGE|CMD_TEAM, Cmd_CallTeamVote_f },
  // can be used even during intermission
  { "say", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Say_f },
  { "say_team", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Say_f },
  { "vsay", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Voice_f },
  { "vsay_team", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Voice_f },
  { "vsay_local", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Voice_f },
  { "vtell", CMD_MESSAGE|CMD_INTERMISSION, Cmd_VoiceTell_f },
  { "vosay", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Voice_f },
  { "vosay_team", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Voice_f },
  { "vosay_local", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Voice_f },
  { "votell", CMD_MESSAGE|CMD_INTERMISSION, Cmd_VoiceTell_f },
  { "vtaunt", CMD_MESSAGE|CMD_INTERMISSION, Cmd_VoiceTaunt_f },
  /*{ "m", CMD_MESSAGE|CMD_INTERMISSION, Cmd_PrivateMessage_f },
  { "mt", CMD_MESSAGE|CMD_INTERMISSION, Cmd_PrivateMessage_f },
  { "a", CMD_MESSAGE|CMD_INTERMISSION, Cmd_AdminMessage_f },*/

  { "score", CMD_INTERMISSION, Cmd_Score_f },
  { "acc", CMD_INTERMISSION, Cmd_Acc_f},

  // cheats
  { "give", CMD_LIVING, Cmd_Give_f },
  { "god", CMD_CHEAT|CMD_LIVING, Cmd_God_f },
  { "notarget", CMD_CHEAT|CMD_LIVING, Cmd_Notarget_f },
  { "levelshot", CMD_CHEAT, Cmd_LevelShot_f },
  { "setviewpos", CMD_CHEAT, Cmd_SetViewpos_f },
  { "noclip", CMD_LIVING, Cmd_Noclip_f },

  { "kill", CMD_TEAM|CMD_LIVING, Cmd_Kill_f },
  { "buildprop", CMD_LIVING, Cmd_BuildProp_f },
  { "sl", CMD_LIVING, Cmd_SpawnList_Item_f },
  { "create", 0, Cmd_PropNpc_AS_f },
  { "spawnbot", CMD_LIVING, Cmd_SpawnBot_f },
  { "distprop", CMD_LIVING, Cmd_DistanceProp_f },
  { "modifyprop", CMD_LIVING, Cmd_ModifyProp_f },
  { "flashlight", CMD_LIVING, Cmd_Flashlight_f },
 // { "flashlighton", CMD_LIVING, Cmd_FlashlightOn_f },
 // { "flashlightoff", CMD_LIVING, Cmd_FlashlightOff_f },
 // { "crosshair", CMD_LIVING, Cmd_Crosshair_f },
 // { "crosshairon", CMD_LIVING, Cmd_CrosshairOn_f },
 // { "crosshairoff", CMD_LIVING, Cmd_CrosshairOff_f },
  { "ammo", CMD_TEAM|CMD_LIVING, Cmd_DropAmmo_f },
  { "health", CMD_TEAM|CMD_LIVING, Cmd_DropHealth_f },
  { "armor", CMD_TEAM|CMD_LIVING, Cmd_DropArmor_f },
  { "dropweapon", CMD_TEAM|CMD_LIVING, Cmd_DropWeapon_f },
  { "dropflag", CMD_TEAM|CMD_LIVING, Cmd_DropFlag_f },
  { "droprune", CMD_TEAM|CMD_LIVING, Cmd_DropRune_f },
  { "dropholdable", CMD_TEAM|CMD_LIVING, Cmd_DropHoldable_f },
  { "usetarget", CMD_LIVING, Cmd_UseTarget_f },
  { "usecvar", 0, Cmd_UseCvar_f },
  { "where", 0, Cmd_Where_f },

  // game commands

  { "follow", CMD_NOTEAM, Cmd_Follow_f },
  { "follownext", CMD_NOTEAM, Cmd_FollowCycle_f },
  { "followprev", CMD_NOTEAM, Cmd_FollowCycle_f },

  { "teamvote", CMD_TEAM, Cmd_TeamVote_f },
  { "teamtask", CMD_TEAM, Cmd_TeamTask_f },
  //KK-OAX
  { "freespectator", CMD_NOTEAM, StopFollowing },
  { "getmappage", 0, Cmd_GetMappage_f },
  { "gc", 0, Cmd_GameCommand_f }

};

static int numCmds = sizeof( cmds ) / sizeof( cmds[ 0 ] );

/*
=================
ClientCommand
KK-OAX, Takes the client command and runs it through a loop which matches
it against the table.
=================
*/
void ClientCommand( int clientNum )
{
    gentity_t *ent;
    char      cmd[ MAX_TOKEN_CHARS ];
    int       i;

    ent = g_entities + clientNum;
    if( !ent->client )
        return;   // not fully in game yet

    trap_Argv( 0, cmd, sizeof( cmd ) );

    for( i = 0; i < numCmds; i++ )
    {
        if( Q_stricmp( cmd, cmds[ i ].cmdName ) == 0 )
            break;
    }

    if( i == numCmds )
    {   // KK-OAX Admin Command Check
        if( !G_admin_cmd_check( ent, qfalse ) )
            trap_SendServerCommand( clientNum,
                va( "print \"Unknown command %s\n\"", cmd ) );
            return;
    }

  // do tests here to reduce the amount of repeated code
    if( !( cmds[ i ].cmdFlags & CMD_INTERMISSION ) && level.intermissiontime )
        return;

    if( cmds[ i ].cmdFlags & CMD_CHEAT && !g_cheats.integer )
    {
        trap_SendServerCommand( clientNum,
            "print \"Cheats are not enabled on this server\n\"" );
        return;
    }
    //KK-OAX When the corresponding code is integrated, I will activate these.
    //if( cmds[ i ].cmdFlags & CMD_MESSAGE &&
    //    ( ent->client->pers.muted || G_FloodLimited( ent ) ) )
    //    return;

    //KK-OAX Do I need to change this for FFA gametype?
    if( cmds[ i ].cmdFlags & CMD_TEAM &&
        ent->client->sess.sessionTeam == TEAM_SPECTATOR )
    {
        trap_SendServerCommand( clientNum, "print \"Join a team first\n\"" );
        return;
    }

    if( ( cmds[ i ].cmdFlags & CMD_NOTEAM ||
        ( cmds[ i ].cmdFlags & CMD_CHEAT_TEAM && !g_cheats.integer ) ) &&
        ent->client->sess.sessionTeam != TEAM_NONE )
    {
        trap_SendServerCommand( clientNum,
            "print \"Cannot use this command when on a team\n\"" );
        return;
    }

    if( cmds[ i ].cmdFlags & CMD_RED &&
        ent->client->sess.sessionTeam != TEAM_RED )
    {
        trap_SendServerCommand( clientNum,
            "print \"Must be on the Red Team to use this command\n\"" );
        return;
    }

    if( cmds[ i ].cmdFlags & CMD_BLUE &&
        ent->client->sess.sessionTeam != TEAM_BLUE )
    {
        trap_SendServerCommand( clientNum,
            "print \"Must be on the Blue Team to use this command\n\"" );
        return;
    }

    if( ( ent->client->ps.pm_type == PM_DEAD ) && ( cmds[ i ].cmdFlags & CMD_LIVING ) )
    {
        trap_SendServerCommand( clientNum,
            "print \"Must be alive to use this command\n\"" );
        return;
    }

    cmds[ i ].cmdHandler( ent );
}
