#include "time.h"
#include "getopt.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <iostream>
#include <string>
#include <sstream>
#include "snake.hpp"
using namespace std;

Uint32 game_timeout_time        = 0;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;
bool SHOW_STATUS_BAR = false;
const int STATUS_BAR = (int)SCREEN_HEIGHT * 0.9;


SDL_Surface *image      = NULL;
SDL_Surface *screen     = NULL;
SDL_Surface *message = NULL;

//The font that's going to be used
TTF_Font *font = NULL;
//The color of the font
SDL_Color textColor = { 255, 255, 255 };

SDL_Event event;

SDL_Surface *load_image( std::string filename )
{
  //Temporary storage for the image that's loaded
  SDL_Surface* loadedImage = NULL;

  //The optimized image that will be used
  SDL_Surface* optimizedImage = NULL;

  //Load the image
  loadedImage = SDL_LoadBMP(filename.c_str() );

  if( loadedImage != NULL )
  {
    //Create an optimized image
    optimizedImage = SDL_DisplayFormat(loadedImage);

    //Free the old image
    SDL_FreeSurface( loadedImage );
  }

  return optimizedImage;
}

void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(source, NULL, destination, &offset);
}


bool init_sdl()
{
  if(SDL_Init (SDL_INIT_EVERYTHING) == -1)
  {
    return false;
  }

  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

  if ( screen == NULL )
  {
    return false;
  }

  //Initialize SDL_ttf
  if( TTF_Init() == -1 )
  {
    return false;
  }

  SDL_WM_SetCaption("Snake", NULL);

  return true;

}

void init_snake()
{
  snake_length          = 2;
  snake_move_distance   = 20;
  snake_velocity_x      = 1;
  snake_velocity_y      = 0;

  snake_init_width      = 20;
  snake_init_height     = 20;
  //initial snake
  for (int i = 0; i < 99; ++i)
  {
    //postion
    snake[i].x        = (SCREEN_WIDTH / 2) - ((i + 1) * snake_init_width);
    snake[i].y        = (SCREEN_HEIGHT / 2) - snake_init_height;
    //body of size
    snake[i].w        = snake_init_width;
    snake[i].h        = snake_init_height;
    //color
    // snake_color[i] = 0xffffff;
    snake_color[i] =  0xB22222;
    if(i==0)
    {
      //snake head
      snake_color[i] = 0xff0000;
    }
  }

}

void init_fruit()
{
  fruit_recover_time    = 0;
  fruit_tick_period     = 1000;

  int fruit_init_x = (int) (game_screen_width / 2);
  int fruit_init_y = (int) (game_screen_height / 2) - 20;
  fruit[0].x    = fruit_init_x;
  fruit[0].y    = fruit_init_y;
  fruit[0].w    = 20;
  fruit[0].h    = 20;
}

void init_game()
{
  game_quit = false;
  game_global_time      = SDL_GetTicks();
  game_init_time        = game_global_time;
  if(is_speed == false)
  {
    game_tick_period      = 80;
  }
  game_score            = 0;
  game_screen_height = SCREEN_HEIGHT;
  game_screen_width = SCREEN_WIDTH;

  if(is_level_mode)
  {
    game_level = 3;
    game_level_current = 0;
    is_game_level_quit = false;
    for(int i=0; i<game_level; i++)
    {
      game_level_cond[i] = (i + 1) * 2;
    }
  }

  if(SHOW_STATUS_BAR)
  {
    game_screen_height =(int) SCREEN_HEIGHT * 0.9;
  }
}
void init_obstacle()
{
  srand((unsigned)time( NULL ));

  //body
  obstacle[0].x = rand()%game_screen_width;
  obstacle[0].y = rand()%game_screen_height;
  obstacle[0].w = snake_move_distance * 4;
  obstacle[0].h = snake_move_distance;

  obstacle[1].x = rand()%game_screen_width;
  obstacle[1].y = rand()%game_screen_height;
  obstacle[1].w = snake_move_distance;
  obstacle[1].h = snake_move_distance * 4;

  obstacle[2].x = rand()%game_screen_width;
  obstacle[2].y = rand()%game_screen_height;
  obstacle[2].w = snake_move_distance * 2;
  obstacle[2].h = snake_move_distance;

  obstacle[3].x = obstacle[2].x + snake_move_distance;
  obstacle[3].y = obstacle[2].y + snake_move_distance;
  obstacle[3].w = snake_move_distance * 2;
  obstacle[3].h = snake_move_distance;

  obstacle[4].x = rand()%game_screen_width;
  obstacle[4].y = rand()%game_screen_height;
  obstacle[4].w = snake_move_distance * 2;
  obstacle[4].h = snake_move_distance;

  obstacle[5].x = obstacle[4].x;
  obstacle[5].y = obstacle[4].y + snake_move_distance;
  obstacle[5].w = snake_move_distance * 2;
  obstacle[5].h = snake_move_distance;

  obstacle[6].x = rand()%game_screen_width;
  obstacle[6].y = rand()%game_screen_height;
  obstacle[6].w = snake_move_distance;
  obstacle[6].h = snake_move_distance;

  obstacle[7].x = obstacle[6].x - snake_move_distance;
  obstacle[7].y = obstacle[6].y + snake_move_distance;
  obstacle[7].w = snake_move_distance * 3;
  obstacle[7].h = snake_move_distance;

  for(int i = 0; i < 99; i++)
  {
    obstacle_color[i] =  0x8B8B7A;
  }
}
void init_wall()
{
  //body
  wall[0].x = 0;
  wall[0].y = 0;
  wall[0].w = game_screen_width;
  wall[0].h = snake_move_distance;

  wall[1].x = 0;
  wall[1].y = 0;
  wall[1].w = snake_move_distance;
  wall[1].h = game_screen_height;

  wall[2].x = game_screen_width - snake_move_distance;
  wall[2].y = 0;
  wall[2].w = snake_move_distance;
  wall[2].h = game_screen_height;

  wall[3].x = 0;
  wall[3].y = game_screen_height - snake_move_distance;
  wall[3].w = game_screen_width;
  wall[3].h = snake_move_distance;
  //color
  for(int i = 0; i < 99; i++)
  {
    wall_color[i] =  0x8B8B7A;
  }
}

void wait_any_key()
{
  bool done = false;
  while ( (done == false) && (SDL_WaitEvent(&event)))
  {
    if (event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.sym)
      {
        case SDLK_SPACE:
          done = true;
          break;
        default: ;
      }
    }
  }
}

bool load_files()
{
  font = TTF_OpenFont( "/usr/share/fonts/TTF/DejaVuSans.ttf", 26 );

  // image = load_image("hello.bmp" );
  // if (image == NULL)
  // {
  //    return false;
  // }

  //If there was an error in loading the font
  if( font == NULL )
  {
    return false;
  }

  //If everything loaded fine
  return true;
}

void sdl_clean_up()
{
  SDL_FreeSurface ( image );
  SDL_FreeSurface ( screen );
  SDL_FreeSurface ( message );

  SDL_Quit();
}

void FillRect(int x, int y, int w, int h, int color)
{
  SDL_Rect rect = {x,y,w,h};
  SDL_FillRect(screen, &rect, color);
}

bool check_collision( SDL_Rect A, SDL_Rect B )
{
  //The sides of the rectangles
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  //Calculate the sides of rect A
  leftA = A.x;
  rightA        = A.x + A.w;
  topA  = A.y;
  bottomA       = A.y + A.h;

  //Calculate the sides of rect B
  leftB = B.x;
  rightB        = B.x + B.w;
  topB  = B.y;
  bottomB       = B.y + B.h;


  //If any of the sides from A are outside of B
  if( bottomA <= topB )
  {
    return false;
  }

  if( topA >= bottomB )
  {
    return false;
  }

  if( rightA <= leftB )
  {
    return false;
  }

  if( leftA >= rightB )
  {
    return false;
  }

  //If none of the sides from A are outside B
  return true;
}

void usage()
{
  cout << "    -w, --wall        " << "Wall mode" << endl;
  cout << "    -s, --score       " << "Score mode" << endl;
  cout << "    -o, --obstacle    " << "Obstacle mode" << endl;
  cout << "    -t, --timeout     " << "Time limit default:120sec" << endl;
  cout << "    -p, --speed       " << "Snake speed (0~MAX),number bigger will slowly" << endl;
  cout << "    -n, --nodie-body  " << "You will never die,when snake head touch snake body" << endl;
  cout << "    -l, --level       " << "Level mode" << endl;
  cout << "        --help        " << "Display this page" << endl;
  cout << "    Example:" << endl;
  cout << "    snake -s -b --timeout=360" << endl;
  cout << "    snake -w -o" << endl;
  exit(0);
}

int parse_opt(int argc, char *argv[])
{
  char *l_opt_arg;
  enum
  {
    GETOPT_HELP_OPTION
  };

  const char* short_options = "p:t::wsonl";
  const struct option long_options[] = {
    {"speed",   required_argument, 0, 'p'},
    {"timeout", optional_argument, 0, 't'},
    {"wall",    no_argument,       0, 'w'},
    {"scroe",   no_argument,       0, 's'},
    {"obstacle",   no_argument,    0, 'o'},
    {"nodie-body",   no_argument,  0, 'n'},
    {"level",   no_argument,  0, 'l'},
    {"help", no_argument, 0, GETOPT_HELP_OPTION},
    {0,0,0,0},
  };

  int c;
  while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
  {
    switch (c)
    {
      case 'p':
        if(optarg)
        {
          is_speed = true;
          l_opt_arg = optarg;
          game_tick_period = atoi(l_opt_arg);
        }
        break;

      case 't':
        SHOW_STATUS_BAR = true;
        is_timeout_mode = true;
        game_timeout_time = 120;   // seconds

        if(optarg)
        {
          l_opt_arg = optarg;
          game_timeout_time = atoi(l_opt_arg);   // seconds
        }
        game_timeout_time *= 1000; // Convert to ms
        break;

      case 'w':
        is_wall_mode = true;
        break;

      case 's':
        SHOW_STATUS_BAR = true;
        is_score_mode = true;
        break;

      case 'o':
        is_obstacle_mode = true;
        break;

      case 'n':
        is_no_die_mode = true;
        break;

      case 'l':
        SHOW_STATUS_BAR = true;
        is_score_mode = true;

        is_obstacle_mode = true;

        is_level_mode = true;
        break;

      case GETOPT_HELP_OPTION:
        usage();
        break;
    }
  }


  return optind;
}
bool is_timeout_mode_kill()
{

  if (is_timeout_mode == true)
  {
    if (SDL_GetTicks() >= game_timeout_time)
    {
      return true;
    }
    // game_timeout_time -= game_global_time;
    // cout << game_timeout_time << endl;
    // cout << int((game_timeout_time - game_global_time) / 1000) << endl;
  }

  return false;
}

void game_handle_input()
{
  while ( SDL_PollEvent (&event) )
  {
    if (event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.sym)
      {
        case SDLK_c:
          SDL_Delay( 10000 );
          break;

        case SDLK_UP:
          if(snake_velocity_y == 1)
          {
            break;
          }
          snake_velocity_x =  0;
          snake_velocity_y = -1;
          break;

        case SDLK_DOWN:
          if(snake_velocity_y == -1)
          {
            break;
          }
          snake_velocity_x = 0;
          snake_velocity_y = 1;
          break;

        case SDLK_LEFT:
          if(snake_velocity_x == 1)
          {
            break;
          }
          snake_velocity_x = -1;
          snake_velocity_y =  0;
          break;

        case SDLK_RIGHT:
          if(snake_velocity_x == -1)
          {
            break;
          }
          snake_velocity_x = 1;
          snake_velocity_y = 0;
          break;

        case SDLK_ESCAPE:
          game_quit = true;
          break;

        default: ;
      }
    }

    if( event.type == SDL_QUIT )
    {
      //Quit the program
      game_quit = true;
    }
  }
}
void cal_snake_position()
{
  int i = snake_length - 1;
  while (i>=0)
  {
    if(i==0)
    {
      if(snake_velocity_x!=0)
      {
        int snake_velocity_x_move_distance = (snake_velocity_x * snake_move_distance);
        int current_snake_x = snake[i].x + snake_velocity_x_move_distance;
        snake[i].x = current_snake_x;
      }
      else if(snake_velocity_y!=0)
      {
        int snake_velocity_y_move_distance = (snake_velocity_y * snake_move_distance);
        int current_snake_y = snake[i].y + snake_velocity_y_move_distance;
        snake[i].y = current_snake_y;
      }
      break;
    }

    snake[i].x=snake[i-1].x;
    snake[i].y=snake[i-1].y;

    i--;
  }
}
void cal_snake_nowall_position()
{
  for(int i=0;i<snake_length;i++)
  {
    if (snake[i].x>game_screen_width || snake[i].w>game_screen_width)
    {
      snake[i].x=0;
    }
    if(snake[i].y>game_screen_height || snake[i].h>game_screen_height)
    {
      snake[i].y=0;
    }
    if(snake[i].x<0 || snake[i].w<0)
    {
      snake[i].x=game_screen_width;
    }
    if(snake[i].y<0 || snake[i].h<0)
    {
      snake[i].y=game_screen_height;
    }
  }
}

void draw_snake()
{
  for(int i=0;i<snake_length;i++)
  {
    FillRect(snake[i].x, snake[i].y, snake[i].w, snake[i].h, snake_color[i]);
  }
}

void draw_game_info_statusbar()
{
  FillRect(0, STATUS_BAR+8, SCREEN_WIDTH,(int) SCREEN_HEIGHT*0.1-8, 0x0000FF);
  //Render the text
  stringstream game_information;

  if(is_score_mode)
  {
    game_information << "Score:" << game_score << "  ";
  }
  if(is_timeout_mode)
  {
    game_information << "Time:" << int((game_timeout_time - game_global_time) / 1000) << "s";
  }

  message = TTF_RenderText_Solid( font, game_information.str().c_str(), textColor );
  apply_surface( 0, STATUS_BAR+8, message, screen );
}

void draw_game_statbar(string text)
{
  FillRect(0, STATUS_BAR+8, SCREEN_WIDTH,(int) SCREEN_HEIGHT*0.1-8, 0x0000FF);
  //Render the text
  stringstream game_information;

  game_information << text;

  SDL_Surface *message_tmp = NULL;
  message_tmp = TTF_RenderText_Solid( font, game_information.str().c_str(), textColor );
  apply_surface( 0, STATUS_BAR+8, message_tmp, screen );

  SDL_Flip(screen);
}
void draw_game_win()
{
  draw_game_statbar("You Win The Game.");
}

void draw_game_lose()
{
  draw_game_statbar("Game Over.");
}

bool check_collision_snake_self()
{
  for(int i=1; i<snake_length; i++)
  {
    if(check_collision(snake[0],snake[i]))
    {
      return true;
    }
  }
  return false;
}
bool snake_ate_fruit()
{
  if(check_collision(snake[0], fruit[0]))
  {
    return true;
  }
  else
  {
    return false;
  }
}
void add_game_score(int n)
{
  if(is_score_mode)
  {
    game_score = game_score + n;
  }
}
void add_snake_length(int n)
{
  snake_length = snake_length + n;
}

bool check_die_snake_wall()
{
  for(int i=0; i<4 ;i++)
  {
    if(check_collision(snake[0],wall[i]))
    {
      return true;
    }
  }

  return false;
}

void draw_wall()
{
  for(int i=0; i<4 ;i++)
  {
    SDL_FillRect(screen, &wall[i], wall_color[i]);
  }
}

bool check_die_snake_obstacle()
{
  int size = 8;
  if(is_level_mode)
  {
    size = game_level_cond[game_level_current];
  }
  for(int i=0; i<size; i++)
  {
    if(check_collision(snake[0],obstacle[i]))
    {
      return true;
    }
  }

  return false;
}

void draw_obstacle(int size)
{
  for(int i=0; i<size; i++)
  {
    SDL_FillRect(screen, &obstacle[i], obstacle_color[0]);
  }
}

bool is_member(int a, int lat[])
{
  for(int i=0; (sizeof(lat)/sizeof(*lat)); i++)
  {
    if(a == lat[i])
    {
      return true;
    }
  }
  return false;
}

void random_fruit()
{
  fruit[0].x        = rand()%(game_screen_width - 9);
  fruit[0].y        = rand()%(game_screen_height - 9);
}

bool fruit_on_obstacle_p()
{
  if(is_obstacle_mode)
  {
    int size = 8;
    if(is_level_mode)
    {
      size = game_level_cond[game_level_current];
    }
    for(int i=0; i<size; i++)
    {
      if(check_collision(fruit[0],obstacle[i]))
      {
        return true;
      }
    }
  }
  return false;
}

bool fruit_on_wall_p()
{
  if(is_wall_mode)
  {
    for(int i=0; i<4; i++)
    {
      if(check_collision(fruit[0],wall[i]))
      {
        return true;
      }
    }
  }
  return false;
}

void get_game_bouns()
{
  score_addup = 1;
  add_game_score(score_addup);

  snake_length_addup = 1;
  add_snake_length(snake_length_addup);

  be_ate_fruit[0] = true;
}

void random_fruit_collision()
{
  bool repeat_check;
  do
  {
    repeat_check = false;
    if(fruit_on_obstacle_p() || fruit_on_wall_p())
    {
      repeat_check = true;
      random_fruit();
    }

  }while(repeat_check==true);
}

void game_update_wall()
{
  //wall
  if(is_wall_mode)
  {
    if(check_die_snake_wall())
    {
      game_quit = true;
    }
    else
    {
      draw_wall();
    }
  }
  //end of wall
}

void game_update_obstacle()
{
  //obstacle
  if(is_obstacle_mode)
  {
    if(check_die_snake_obstacle())
    {
      game_quit = true;
      return;
    }
    else
    {
      if(is_level_mode)
      {
        draw_obstacle(game_level_cond[game_level_current]);
        return;
      }
      else
      {
        draw_obstacle(8);
      }
    }
  }
  //end of obstacle
}

void game_update_fruit()
{
  //fruit
  if(snake_ate_fruit())
  {
    fruit[0].x = -1;
    fruit[0].y = -1;

    get_game_bouns();
  }
  else if (be_ate_fruit[0] == false)
  {
    //draw fruit
    SDL_FillRect( screen, &fruit[0], SDL_MapRGB( screen->format, 0x00, 0xFF, 0x00 ) );
    //sync recover time
    fruit_recover_time        = SDL_GetTicks();
  }
  else if((SDL_GetTicks() - fruit_recover_time) >= fruit_tick_period)
  {
    be_ate_fruit[0] = false;
    random_fruit();

    if(is_obstacle_mode || is_wall_mode)
    {
      random_fruit_collision();
    }
  }
  //end of fruit
}
void game_update_statusbar()
{
  //status bar
  if(SHOW_STATUS_BAR)
  {
    draw_game_info_statusbar();
  }
  //end of status bar
}
void game_update_snake()
{
  //snake
  cal_snake_position();

  if(is_wall_mode != true)
  {
    cal_snake_nowall_position();
  }

  if(is_no_die_mode != true)
  {
    if(check_collision_snake_self())
    {
      game_quit = true;
    }
  }
  draw_snake();
  //end of snake
}

void game_update_time()
{
  game_global_time = SDL_GetTicks();
}

bool is_game_fps()
{
  if ((SDL_GetTicks() - game_global_time) >= game_tick_period)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void game_update_level()
{
  if(is_level_mode)
  {
    if(game_score == game_level_cond[game_level_current])
    {
      game_level_current = game_level_current + 1;
      is_game_level_quit = true;
      game_quit = true;
      game_score = 0;
    }
  }
}

void game_main_loop()
{
  while (game_quit == false)
  {
    if (game_quit == true)
    {
      break;
    }

    //game limit kill
    if(is_timeout_mode_kill() == true)
    {
      game_quit = true;
      break;
    }

    //draw anything
    if ( is_game_fps() )
    {
      // snake handle keyboard
      game_handle_input();

      // realy show game screen
      SDL_Flip(screen);

      // clear screen
      SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

      game_update_wall();

      game_update_obstacle();

      game_update_fruit();
      
      game_update_level();

      game_update_statusbar();

      game_update_snake();

      game_update_time();
    }
  }
}

bool is_game_level_end()
{
  if (game_level_current >= game_level )
  {
    return true;
  }
  else
  {
    return false;
  }
}

void game_loop()
{
  do {
    game_main_loop(); //imporant game main loop function

    if(is_level_mode)
    {
      if(is_game_level_end() == false)
      {
        if(is_game_level_quit)
        {
          game_quit = false;
          is_game_level_quit = false;

          stringstream game_info;
          game_info << "Level " << game_level_current << ", Press SPACE key to next level .";
          string result = game_info.str();
          draw_game_statbar(result);
          wait_any_key();
        }
      }
    }
  } while(is_level_mode && (is_game_level_end() == false) && game_quit == false );
}

void start_game()
{
  srand((unsigned)time( NULL ));

  game_loop();
}

int setup()
{
  //sdl init
  if (init_sdl() == false)
  {
    return 1;
  }
  if (load_files() == false)
  {
    return 2;
  }
  //end of sdl init

  //init about snake game
  init_game();
  init_snake();
  init_fruit();

  if(is_obstacle_mode)
  {
    init_obstacle();
  }

  if(is_wall_mode)
  {
    init_wall();
  }
  //end of init about snake game

  return 0;
}

void wait_quit_key()
{
  bool done = false;
  while ( (done == false) && (SDL_WaitEvent(&event)))
  {
    if (event.type == SDL_KEYDOWN)
    {
      switch(event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          done = true;
          break;

        default: ;
      }
    }
  }
}

bool is_level_win()
{
  if(is_level_mode)
  {
    if(game_level == game_level_current)
    {
      return true;
    }
  }

  return false;
}

void clean()
{
  if (game_quit == true)
  {
    if(is_level_win())
    {
      draw_game_win();
    }
    else if(is_level_win() == false)
    {
      draw_game_lose();
    }
    wait_quit_key();
    sdl_clean_up();
  }
}

int main(int argc, char *argv[])
{
  parse_opt(argc, argv);

  setup();

  start_game();

  clean();

  return 0;
}
