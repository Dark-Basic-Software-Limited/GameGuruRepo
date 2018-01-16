//----------------------------------------------------
//--- GAMEGURU - M-Game
//----------------------------------------------------

#include "cstr.h"

void game_masterroot ( void );
void game_setresolution ( void );
void game_postresolutionchange ( void );
void game_oneoff_nongraphics ( void );
void game_loadinentitiesdatainlevel ( void );
void game_loadinleveldata ( void );
void game_preparelevel ( void );
void game_preparelevel_forplayer ( void );
void game_preparelevel_finally ( void );
void game_stopallsounds ( int iMode );
void game_freelevel ( void );
void game_init ( void );
void game_freegame ( void );
void game_hidemouse ( void );
void game_showmouse ( void );
void game_timeelapsed_init ( void );
void game_timeelapsed ( void );
void game_main_snapshotsoundloopcheckpoint ( void );
void game_main_snapshotsoundloopresume ( void );
void game_main_loop ( void );
void game_sync ( void );
void game_main_stop ( void );
void game_jump_to_level_from_lua ( void );
void game_finish_level_from_lua ( void );
void game_end_of_level_check ( void );
void game_setup_character_shader_entities ( bool bMode );
void game_check_character_shader_entities ( void );
