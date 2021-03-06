#include "SDL/SDL.h"
bool game_quit;

bool  is_timeout_mode = false;
bool  is_status_bar   = false;
bool  is_score_mode   = false;
bool  is_obstacle_mode = false;
bool  is_no_die_mode   = false;
bool  is_wall_mode = false;
bool  is_level_mode = false;
bool  is_speed = false;

Uint32 game_global_time;
Uint32 game_init_time;
int game_screen_height;
int game_screen_width;
int game_tick_period;
int game_score;

int game_level;
int game_level_current;
int game_level_cond[3];
bool is_game_level_quit;

//about snake
int snake_length;
int snake_move_distance;
SDL_Rect snake[99];
int snake_init_width;
int snake_init_height;
int snake_velocity_x;
int snake_velocity_y;
Uint32 snake_color[99];
// end of about snake

//about fruit
SDL_Rect fruit[99];
bool be_ate_fruit[99];
Uint32 fruit_recover_time;
int fruit_tick_period;
// end of about fruit

//about snake and fruit
int snake_length_addup;
int score_addup;
// end of about snake and fruit

//about obstacle
SDL_Rect obstacle[99];
Uint32 obstacle_color[99];
//end of about obstacle

//about wall
SDL_Rect wall[99];
Uint32 wall_color[99];
//end of about wall
