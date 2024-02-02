/*
=======================================================================

MGUI - QSandbox

=======================================================================
*/
#include "ui_local.h"

typedef struct {
	menuframework_s	menu;
	menuobject_s	item[100];
	
} mgui_t;

static mgui_t s_mgui;

/*
=================
MGUI_Event
=================
*/
void MGUI_Event (void* ptr, int event) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
		
	case 1:
		trap_System("start calc");
	break;
	
	}
}

sfxHandle_t MGUI_Key(int key)
{
	switch ( key )
	{
		case K_ESCAPE:
			UI_PopMenu();
			return menu_out_sound;
	}
	return 0;
}

/*
===============
MGUI_MenuDraw
===============
*/

static void MGUI_MenuDraw( void ) {
	int i;
	vec4_t color_mgui	    = {1.00f, 1.00f, 1.00f, 1.00f};
	float			x, y, w, h;
	vec4_t			color1 = {0.85, 0.9, 1.0, 1};

	Menu_Draw( &s_mgui.menu );

	if (uis.debug) {
	UI_DrawString( vx(50), vy(0.4), "MGUI v1.0 by HyperNoiRe", UI_CENTER|UI_SMALLFONT, color1 );
	}
}

/*
===============
UI_MGUI
===============
*/
void UI_MGUI( void ) {
	int i;
	char text[1024];
	vec4_t color_mgui	    = {1.00f, 0.00f, 1.00f, 1.00f};

	memset( &s_mgui, 0 ,sizeof(mgui_t) );

	MainMenu_Cache();

	s_mgui.menu.draw = MGUI_MenuDraw;
	s_mgui.menu.fullscreen = qtrue;
	s_mgui.menu.wrapAround = qtrue;
	//s_mgui.menu.key = MGUI_Key;

	for ( i = 1; i < 99; i++ ) {
	trap_Cvar_VariableStringBuffer(UI_ArenaScriptAutoChar(va("mitem%i_str", i)), text, sizeof( text ));
	s_mgui.item[i].generic.type			= MTYPE_UIOBJECT;
	s_mgui.item[i].generic.flags		= QMF_CENTER_JUSTIFY|QMF_HIGHLIGHT_IF_FOCUS;
	s_mgui.item[i].generic.id			= i;
	s_mgui.item[i].generic.callback		= MGUI_Event;
	s_mgui.item[i].type					= UI_ArenaScriptAutoInt(va("mitem%i_type", i));
	s_mgui.item[i].generic.x			= vx(UI_ArenaScriptAutoFloat(va("mitem%i_x", i)));
	s_mgui.item[i].generic.y			= vy(UI_ArenaScriptAutoFloat(va("mitem%i_y", i)));
	s_mgui.item[i].width				= vx(UI_ArenaScriptAutoFloat(va("mitem%i_w", i)));
	s_mgui.item[i].height				= vy(UI_ArenaScriptAutoFloat(va("mitem%i_h", i)));
    s_mgui.item[i].string			    = text;
	s_mgui.item[i].color				= color_mgui;
	s_mgui.item[i].style		    	= UI_SMALLFONT;
	strcpy(s_mgui.item[i].string, text);
	}

	for ( i = 1; i < 99; i++ ) {
		if(s_mgui.item[i].type >= 1){
			Menu_AddItem( &s_mgui.menu,	&s_mgui.item[i] );
		}
	}

	trap_Key_SetCatcher( KEYCATCH_UI );
}

void MGUI_Load( void ) {
	UI_MGUI();
	UI_PushMenu( &s_mgui.menu );
}

