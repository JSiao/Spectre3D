#include "spectre.h"
#include "font.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

#define PLAYER_SPEED 2.0
#define SPRITE_SPEED 5.0

struct spectre_vars spectre_globals;

void _game_init(struct game_struct *game)
{
  srand(time(NULL));
  game_width_set(640);
  game_height_set(480);
  
  reset_game();

  spectre_globals.key_update[STATE_PLAY]    = &play_key_update;
  spectre_globals.logic_update[STATE_PLAY]  = &play_logic_update;
  spectre_globals.video_update[STATE_PLAY]  = &play_video_update;

  spectre_globals.key_update[STATE_HELP]    = &help_key_update;
  spectre_globals.logic_update[STATE_HELP]  = &help_logic_update;
  spectre_globals.video_update[STATE_HELP]  = &help_video_update;

  spectre_globals.key_update[STATE_ANIM]    = &anim_key_update;
  spectre_globals.logic_update[STATE_ANIM]  = &anim_logic_update;
  spectre_globals.video_update[STATE_ANIM]  = &anim_video_update;

  spectre_globals.key_update[STATE_RETRY]   = &retry_key_update;
  spectre_globals.logic_update[STATE_RETRY] = &retry_logic_update;
  spectre_globals.video_update[STATE_RETRY] = &retry_video_update;

  spectre_globals.state = STATE_HELP;
  spectre_globals.pixel_buffer = _mem_alloc(sizeof(double) * game->width);
  video_init(&spectre_globals);
  debug_msg("Spectre 3D - by Justin Siao");
}

void video_init(struct spectre_vars *this)
{
  this->pallette[0] = pallette_val(  70,  95, 140); // Blue 1
  this->pallette[1] = pallette_val(  25,  55,  98); // Blue 2
  this->pallette[2] = pallette_val(  10,  28,  68); // Blue 3
  this->pallette[3] = pallette_val(  35,  49,  70); // Blue 4
  this->pallette[4] = pallette_val(  13,  27,  49); // Blue 5
  this->pallette[5] = pallette_val(   5,  14,  29); // Blue 6
  this->pallette[6] = pallette_val(   0, 255,   0); // Green
  this->pallette[7] = pallette_val(   0,   0,   0); // Black
  this->pallette[8] = pallette_val( 255, 255, 255); // White
  this->pallette[9] = pallette_val( 127, 127, 127); // Grey
}

U32 pallette_val(U8 r, U8 g, U8 b)
{
  U32 return_val = 0;
  return_val |= r << 16;
  return_val |= g <<  8;
  return_val |= b;
  return return_val;
}

void _game_update(unsigned long long tick)
{
  //Process Keys first, then Video
  // Inputs Section
  (*spectre_globals.key_update[spectre_globals.state])(&spectre_globals, tick);
  // Logic Section
  (*spectre_globals.logic_update[spectre_globals.state])(&spectre_globals, tick);
  // Video Section
  (*spectre_globals.video_update[spectre_globals.state])(&spectre_globals, tick);
}

void play_key_update(struct spectre_vars *this, U64 tick)
{
  const double rotation  = PLAYER_SPEED * ((double)tick/CLOCKS_PER_SEC);
  const double move      = PLAYER_SPEED * ((double)tick/CLOCKS_PER_SEC);
  char (*_map)[24][24]   = this->map;
  struct player *player  = &this->player;
  if (game_key(0x1B))  // ESCAPE
  {
    game_kill();
  }
  if (game_key('Q'))
  {
    if (!(*_map)[(int)(player->pos[S_X] - player->dir[S_Y] * move)][(int)(player->pos[S_Y])])
    {
        player->pos[S_X] -= player->dir[S_Y] * move;
    }
    if (!(*_map)[(int)(player->pos[S_X])][(int)(player->pos[S_Y] + player->dir[S_X] * move)])
    {
        player->pos[S_Y] += player->dir[S_X] * move;
    }
  }
  if (game_key('E'))
  {
    if (!(*_map)[(int)(player->pos[S_X] + player->dir[S_Y] * move)][(int)(player->pos[S_Y])])
    {
        player->pos[S_X] += player->dir[S_Y] * move;
    }
    if (!(*_map)[(int)(player->pos[S_X])][(int)(player->pos[S_Y] - player->dir[S_X] * move)])
    {
        player->pos[S_Y] -= player->dir[S_X] * move;
    }
  }
  if (game_key('W'))
  {
      if (!(*_map)[(int)(player->pos[S_X] + player->dir[S_X] * move)][(int)(player->pos[S_Y])])
      {
          player->pos[S_X] += player->dir[S_X] * move;
      }
      if (!(*_map)[(int)(player->pos[S_X])][(int)(player->pos[S_Y] + player->dir[S_Y] * move)])
      {
          player->pos[S_Y] += player->dir[S_Y] * move;
      }
  }
  if (game_key('S'))
  {
    if (!(*_map)[(int)(player->pos[S_X] - player->dir[S_X] * move)][(int)(player->pos[S_Y])])
    {
        player->pos[S_X] -= player->dir[S_X] * move;
    }
    if (!(*_map)[(int)(player->pos[S_X])][(int)(player->pos[S_Y] - player->dir[S_Y] * move)])
    {
        player->pos[S_Y] -= player->dir[S_Y] * move;
    }
  }
  if (game_key('D'))
  {
      double old_x     = player->dir[S_X];
      double old_y     = player->dir[S_Y];
      double plane_x   = this->plane[S_X];
      double plane_y   = this->plane[S_Y];
      player->dir[S_X] = old_x * cos(-rotation) - old_y * sin(-rotation);
      player->dir[S_Y] = old_x * sin(-rotation) + old_y * cos(-rotation);
      this->plane[S_X] = plane_x * cos(-rotation) - plane_y * sin(-rotation);
      this->plane[S_Y] = plane_x * sin(-rotation) + plane_y * cos(-rotation);
  }
  if (game_key('A'))
  {
      double old_x     = player->dir[S_X];
      double old_y     = player->dir[S_Y];
      double plane_x   = this->plane[S_X];
      double plane_y   = this->plane[S_Y];
      player->dir[S_X] = old_x * cos(rotation) - old_y * sin(rotation);
      player->dir[S_Y] = old_x * sin(rotation) + old_y * cos(rotation);
      this->plane[S_X] = plane_x * cos(rotation) - plane_y * sin(rotation);
      this->plane[S_Y] = plane_x * sin(rotation) + plane_y * cos(rotation);
  }
  if (game_key('H'))
  {
    this->state = STATE_HELP;
  }
}
void play_logic_update(struct spectre_vars *this, U64 tick)
{
  if (collide(&this->player, &this->sprite_list[0]))
  {
    this->score ++;
    if ((rand() % 3) == 0)
    {
      this->map  = &_maps[rand()%NUMBER_OF_MAPS];
      teleport(this);
    }
    do
    {
      this->sprite_list[0].pos[S_X] = rand() % 24;
      this->sprite_list[0].pos[S_Y] = rand() % 24;
    } while ((*this->map)[(int)this->sprite_list[0].pos[S_X]][(int)this->sprite_list[0].pos[S_Y]]);
    this->sprite_list[0].pos[S_X] += 0.5f;
    this->sprite_list[0].pos[S_Y] += 0.5f;
  }
  if (collide(&this->player, &this->sprite_list[1]) || collide(&this->player, &this->sprite_list[2]) || collide(&this->player, &this->sprite_list[3]))
  {
    this->state = STATE_ANIM;
  }
  for (int x = 1; x < 4; x ++)
  {
    move_sprite(this, x, tick);
  }
}

U8 collide(const struct player *player, const struct sprite *sprite)
{
  if (sprite->visible)
  {
    double d_x = fabs(player->pos[S_X] - sprite->pos[S_X]);
    double d_y = fabs(player->pos[S_Y] - sprite->pos[S_Y]);
    double distance = d_x * d_x + d_y * d_y;
    if (distance < 0.0625)
    {
      return 1;
    }
  }
  return 0;
}
void play_video_update(struct spectre_vars *this, U64 tick)
{
  unsigned int height  = game_height();
  unsigned int width   = game_width();
  for (unsigned int y = 0; y < height / 2; y++)
  {
    for (unsigned int x = 0; x < width ; x++)
    {
      game_pixel_set(x, y, pallette_val(0, 0, 0));
    }
  }
  for (unsigned int y = height/2; y < height ; y++)
  {
    for (unsigned int x = 0; x < width ; x++)
    {
      game_pixel_set(x, y, pallette_val(10, 10, 10));
    }
  }
  video_render_wall(this);
  sort_sprites(this);
  for (int y = 0; y < 4; y++)
  {
    this->sprite_list[this->sprite_stack[y]].rendered = video_render_sprite(this, this->sprite_stack[y]);
  }
  static char score[32];
  sprintf(score, "Current Score: %d", this->score);
  font_puts(score, 5, 5, 0x0000FF00);
}

void video_render_wall(struct spectre_vars *this)
{
  char (*world_map)[24][24] = this->map;
  U32 size_x = game_width();
  U32 size_y = game_height();
  struct player *player = &this->player;
  double (*plane)[2] = &this->plane;
  for (int x = 0; x < size_x; x++)
  {
    double camera_x = (2 * x / (double)size_x) - 1.0f;
    double ray_x    = player->dir[S_X] + (*plane)[S_X] * camera_x;
    double ray_y    = player->dir[S_Y] + (*plane)[S_Y] * camera_x;
    int    map_x    = (int) player->pos[S_X];
    int    map_y    = (int) player->pos[S_Y];
    double delta_x  = fabs(1.0f/ray_x);
    double delta_y  = fabs(1.0f/ray_y);
    char   wall     = 0;
    char   side;
    int step_x, step_y;
    double side_x, side_y;
    double distance;
    if (ray_x < 0)
    {
      step_x = -1;
      side_x = (player->pos[S_X] - map_x) * delta_x;
    }
    else
    {
      step_x = 1;
      side_x = (map_x + 1.0f - player->pos[S_X]) * delta_x;
    }
    if (ray_y < 0)
    {
      step_y = -1;
      side_y = (player->pos[S_Y] - map_y) * delta_y;
    }
    else
    {
      step_y = 1;
      side_y = (map_y + 1.0f - player->pos[S_Y]) * delta_y;
    }
    while (!wall)
    {
      if (side_x < side_y)
      {
        side_x += delta_x;
        map_x  += step_x;
        side    = 0;
      }
      else
      {
        side_y += delta_y;
        map_y  += step_y;
        side    = 1;
      }
      wall = !!((*world_map)[map_x][map_y]);
    }
    if (!side)
      distance = (map_x - player->pos[S_X] + (1 - step_x) / 2) / ray_x;
    else
      distance = (map_y - player->pos[S_Y] + (1 - step_y) / 2) / ray_y;
    int line_height = (int)(size_y / distance);
    int start = (-line_height / 2) + (size_y / 2);
    if (start < 0)
      start = 0;
    int end   = (line_height / 2) + (size_y / 2);
    if (end >= size_y)
      end   = size_y - 1;
    // calculate wall_x
    double wall_x;
    if (!side)
      wall_x = player->pos[S_Y] + distance * ray_y;
    else
      wall_x = player->pos[S_X] + distance * ray_x;
    wall_x -= floor(wall_x);
    int tex_x = (int)(wall_x * 16.0f);
    //if ((side == 0 && player->dir[S_X] > 0) || (side == 1 && player->dir[S_Y] < 0))
    //  tex_x = 16 - tex_x - 1;
    video_render_line(this, x, start, end, tex_x, side, line_height);
    this->pixel_buffer[x] = distance;
  }
}

void video_render_line(struct spectre_vars *this, int x, int start, int end, int tex_x, int side, int line_height)
{
  U32 size_x  = game_width();
  U32 size_y  = game_height();
  for (int y = start; y < end; y++)
  {
    int d = y * 256 - size_y * 128 + line_height * 128;
    int tex_y = ((d * 16) / line_height) / 256;
    int color = wall_sprite[tex_y][tex_x];
    int xcolor;
    if (side)
      xcolor = this->pallette[color - 1];
    else
      xcolor = this->pallette[color + 2];
    game_pixel_set(x, y, xcolor);
  }
}

void _game_end(struct game_struct *this)
{
  spectre_end();
}

void spectre_end(void)
{
  _mem_free(spectre_globals.pixel_buffer);
}

U8 video_render_sprite(struct spectre_vars *this, int sprite_id)
{
  struct sprite *sprite = &this->sprite_list[sprite_id];
  struct player *player = &this->player;
  U32 size_x  = game_width();
  U32 size_y  = game_height();
  U8 return_val = 0;
  if (sprite->visible)
  {
    double x = player->pos[S_X];
    double y = player->pos[S_Y];
    double sprite_x = sprite->pos[S_X] - x;
    double sprite_y = sprite->pos[S_Y] - y;
    double dir_x = player->dir[S_X];
    double dir_y = player->dir[S_Y];
    double plane_x = this->plane[S_X];
    double plane_y = this->plane[S_Y];
    double determinant = 1.0f / (plane_x * dir_y - dir_x * plane_y);
    double transform_x = determinant * (dir_y * sprite_x - dir_x * sprite_y);
    double transform_y = determinant * (-plane_y * sprite_x + plane_x * sprite_y);

    int screen_x = (int)((size_x/2) * (1 + transform_x / transform_y));
    int side     = abs((int)(size_y/transform_y));

    int start_x  = -side / 2 + screen_x;
    int start_y  = -side / 2 + (size_y / 2);
    int end_x    =  side / 2 + screen_x;
    int end_y    =  side / 2 + (size_y / 2);
    // if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    // if (end_x   >= size_x) end_x = size_x - 1;
    if (end_y   >= size_y) end_y = size_y - 1;
    for (int stripe = start_x; stripe < end_x; stripe ++)
    {
      int tex_x = (int)(256 * (stripe - (-side / 2 + screen_x)) * SPRITE_SIZE / side) / 256;
      if (transform_y > 0 && stripe > 0 && stripe < size_x && transform_y < this->pixel_buffer[stripe] )
      {
        video_sprite_line(this, stripe, start_y, end_y, tex_x, sprite->type, sprite->sprite, side, sprite->opacity);
        return_val = 1;
      }
    }
  }
  return return_val;
}

void video_sprite_line(struct spectre_vars *this, int x, int start, int end, int tex_x, int type, char (*sprite)[SPRITE_SIZE][SPRITE_SIZE], int line_height, U8 opacity)
{
  U32 size_x  = game_width();
  U32 size_y  = game_height();
  for(int y = start; y < end; y++)
  {
    int d = (y * 256) - (size_y * 128) + (line_height * 128);
    int tex_y = ((d * SPRITE_SIZE) / line_height) / 256;
    int color = (*sprite)[tex_y][tex_x];
    int c = this->pallette[9];
    switch (color)
    {
      case 1:
        c = this->pallette[0];
        break;
      case 2:
        c = this->pallette[1];
        break;
      case 3:
        c = this->pallette[2];
        break;
      case 4:
        c = this->pallette[6];
        break;
      case 5:
        c = this->pallette[7];
        break;
      case 6:
        c = this->pallette[8];
        break;
      case 7:
        c = this->pallette[9];
        break;
      default:
        color = 0;
        break;
    }
    if (color)
    {
      //buffer[y * size_x + x] = c;
      if (opacity == 0xFF)
        game_pixel_set(x, y, c);
      else
        game_pixel_mix(x, y, c, opacity);
    }
  }
}

double distances(const struct sprite *sprite, const struct player *player)
{
  double dx = player->pos[S_X] - sprite->pos[S_X];
  double dy = player->pos[S_Y] - sprite->pos[S_Y];
  return dx * dx + dy * dy;
}

void sort_sprites(struct spectre_vars *this)
{
  struct sprite *sprite_list = this->sprite_list;
  struct player *player      = &this->player;
  int           *stack       = this->sprite_stack;
  float distance[4];
  distance[0] = distances(sprite_list, player);
  distance[1] = distances(sprite_list + 1, player);
  distance[2] = distances(sprite_list + 2, player);
  distance[3] = distances(sprite_list + 3, player);
  for (int x = 0; x < 3; x++)
  {
    for (int y = 0; y < 3 - x; y++)
    {
      if (distance[stack[y]] < distance[stack[y+1]])
      {
        stack[y]   ^= stack[y+1];
        stack[y+1] ^= stack[y];
        stack[y]   ^= stack[y+1];
      }
    }
  }
}

void teleport(struct spectre_vars *this)
{
  char tp = 0;
  while ((*this->map)[(int)this->player.pos[S_X]][(int)this->player.pos[S_Y]])
  {
    tp = 1;
    this->player.pos[S_X] = rand() % 24;
    this->player.pos[S_Y] = rand() % 24;
  }
  if (tp)
  {
    this->player.pos[S_X] += 0.5f;
    this->player.pos[S_Y] += 0.5f;
  }
  for (int x = 1; x < 4; x++)
  {
    tp = 0;
    while ((*this->map)[(int)this->sprite_list[x].pos[S_X]][(int)this->sprite_list[x].pos[S_Y]] || distances(&this->sprite_list[x], &this->player) < 25.0f)
    {
      tp = 1;
      this->sprite_list[x].pos[S_X] = (double)((rand() % 22) + 1);
      this->sprite_list[x].pos[S_Y] = (double)((rand() % 22) + 1);
    } 
    if (tp)
    {
      this->sprite_list[x].pos[S_X] = this->sprite_list[x].pos[S_X] + 0.5f;
      this->sprite_list[x].pos[S_Y] = this->sprite_list[x].pos[S_Y] + 0.5f;
    }
  }
}

void help_key_update(struct spectre_vars *this, U64 tick)
{
  if (game_key(0x1B))  // ESCAPE
  {
    game_kill();
  }
  if (game_key('\r'))
  {
    this->state = STATE_PLAY;
  }
}
void help_logic_update(struct spectre_vars *this, U64 tick)
{
  return;
}
void help_video_update(struct spectre_vars *this, U64 tick)
{
  unsigned int height  = game_height();
  unsigned int width   = game_width();
  for (unsigned int y = 0; y < height; y++)
  {
    for (unsigned int x = 0; x < width ; x++)
    {
      game_pixel_set(x, y, pallette_val(0, 0, 0));
    }
  }

  font_puts("SPECTRE 3D (v0.7)- JUSTIN SIAO", 5, 5, 0x0000FF00);
  font_puts("Instructions", 5, 20, 0x0000FF00);
  font_puts("WASD to move", 5, 35, 0x0000FF00);
  font_puts("Q & E to strafe", 5, 50, 0x0000FF00);
  font_puts("H for help", 5, 65, 0x0000FF00);
  font_puts("ESC to exit", 5, 80, 0x0000FF00);
  font_puts("Collect the runes to score", 5, 95, 0x0000FF00);
  font_puts("Avoid the spectres", 5, 110, 0x0000FF00);
  font_puts("Press Enter to play", 5, 125, 0x0000FF00);
}


void retry_key_update(struct spectre_vars *this, U64 tick)
{
  if (game_key(0x1B) || game_key('N'))  // ESCAPE
  {
    game_kill();
  }
  if (game_key('Y'))
  {
    reset_game();
    this->state = STATE_PLAY;
  }
}
void retry_logic_update(struct spectre_vars *this, U64 tick)
{
  return;
}
void retry_video_update(struct spectre_vars *this, U64 tick)
{
  unsigned int height  = game_height();
  unsigned int width   = game_width();
  for (unsigned int y = 0; y < height; y++)
  {
    for (unsigned int x = 0; x < width ; x++)
    {
      game_pixel_set(x, y, pallette_val(0, 0, 0));
    }
  }

  font_puts("CAUGHT! Retry? [Y/N]", (width / 2) - 75, (height / 2) - 25, 0x00FF0000);
}

void reset_game()
{
  spectre_globals.map  = &_maps[0];

  spectre_globals.sprite_list[0].pos[S_X]  = 12.5;
  spectre_globals.sprite_list[0].pos[S_Y]  = 11.5;
  spectre_globals.sprite_list[0].visible   = 1;
  spectre_globals.sprite_list[0].type      = TYPE_RUNE;
  spectre_globals.sprite_list[0].rendered  = 0;
  spectre_globals.sprite_list[0].direction = DIR_DISABLED;
  spectre_globals.sprite_list[0].sprite    = &rune;
  spectre_globals.sprite_list[0].opacity   = 0x7F;

  spectre_globals.sprite_list[1].pos[S_X]  = 1.5;
  spectre_globals.sprite_list[1].pos[S_Y]  = 1.5;
  spectre_globals.sprite_list[1].visible   = 1;
  spectre_globals.sprite_list[1].type      = TYPE_SPECTRE;
  spectre_globals.sprite_list[1].rendered  = 0;
  spectre_globals.sprite_list[1].direction = 1;
  spectre_globals.sprite_list[1].sprite    = &spectre;
  spectre_globals.sprite_list[1].opacity   = 0xFF;

  spectre_globals.sprite_list[2].pos[S_X]  = 22.5;
  spectre_globals.sprite_list[2].pos[S_Y]  = 1.5;
  spectre_globals.sprite_list[2].visible   = 1;
  spectre_globals.sprite_list[2].type      = TYPE_SPECTRE;
  spectre_globals.sprite_list[2].rendered  = 0;
  spectre_globals.sprite_list[2].direction = 1;
  spectre_globals.sprite_list[2].sprite    = &spectre;
  spectre_globals.sprite_list[2].opacity   = 0xFF;

  spectre_globals.sprite_list[3].pos[S_X]  = 5.5;
  spectre_globals.sprite_list[3].pos[S_Y]  = 22.5;
  spectre_globals.sprite_list[3].visible   = 1;
  spectre_globals.sprite_list[3].type      = TYPE_SPECTRE;
  spectre_globals.sprite_list[3].rendered  = 0;
  spectre_globals.sprite_list[3].direction = 1;
  spectre_globals.sprite_list[3].sprite    = &spectre;
  spectre_globals.sprite_list[3].opacity   = 0xFF;

  spectre_globals.sprite_stack[0] = 0;
  spectre_globals.sprite_stack[1] = 1;
  spectre_globals.sprite_stack[2] = 2;
  spectre_globals.sprite_stack[3] = 3;

  spectre_globals.plane[S_X] = 0.0f;
  spectre_globals.plane[S_Y] = 0.6666f;
  spectre_globals.player.pos[S_X] = 11.5;
  spectre_globals.player.pos[S_Y] = 11.5;
  spectre_globals.player.dir[S_X] = -1;
  spectre_globals.player.dir[S_Y] = 0;
  spectre_globals.score = 0;
  spectre_globals.anim  = 2;
  if (rand() % 2)
  {
    spectre_globals.animation = &animation1;
  }
  else
  {
    spectre_globals.animation = &animation2;
  }
}

void move_sprite(struct spectre_vars * this, int index, U64 tick)
{
  double speed = tick/(double)CLOCKS_PER_SEC;
  char (*world_map)[24][24] = this->map;
  struct sprite *sprite = &this->sprite_list[index];
  struct player *player = &this->player;
  if (!sprite->rendered)
  {
    double x = 0;
    double y = 0;
    double dx = 0;
    double dy = 0;
    switch(sprite->direction)
    {
    case 1:
      y = -0.5;
      dy = -SPRITE_SPEED;
      break;
    case 2:
      x = 0.5;
      dx = SPRITE_SPEED;
      break;
    case 3:
      y = 0.5;
      dy = SPRITE_SPEED;
      break;
    case 4:
      x = -0.5;
      dx = -SPRITE_SPEED;
      break;
    default:
      return;
    }
    if ((*world_map)[(int)(sprite->pos[S_X] + x)][(int)(sprite->pos[S_Y] + y)])
    {
      sprite->direction = (sprite->direction + rand()%3) % 4 + 1;
    }
    else
    {
      sprite->pos[S_Y] += dy * speed;
      sprite->pos[S_X] += dx * speed;
    }
  }
  else
  {
    double dirx = player->pos[S_X] - sprite->pos[S_X];
    double diry = player->pos[S_Y] - sprite->pos[S_Y];
    if (fabs(dirx) > fabs(diry))
    {
      if (dirx < 0)
      {
          sprite->direction = 4;
      }
      else
      {
          sprite->direction = 2;
      }
    }
    else
    {
      if (diry < 0)
      {
          sprite->direction = 1;
      }
      else
      {
          sprite->direction = 3;
      }
    }
  }
}

void anim_key_update(struct spectre_vars *this, U64 tick)
{
  ;
}
void anim_logic_update(struct spectre_vars *this, U64 tick)
{
  if (this->anim <= 0)
  {
    this->state = STATE_RETRY;
  }
  else
  {
    this->anim -= (double)tick/(double)CLOCKS_PER_SEC;
  }
}
void anim_video_update(struct spectre_vars *this, U64 tick)
{
  this->animation(this, tick);
}

void animation1(struct spectre_vars *this, U64 tick)
{
  unsigned int height  = game_height();
  unsigned int width   = game_width();
  for (unsigned int y = 0; y < rand(); y++)
  {
    game_pixel_mix(rand() % width, rand() % height, 0x003F0000, (rand() % 128));
  }
}

void animation2(struct spectre_vars *this, U64 tick)
{
  unsigned int height  = game_height();
  unsigned int width   = game_width();
  for (unsigned int y = 0; y < height; y++)
  {
    for (unsigned int x = 0; x < width ; x++)
    {
      game_pixel_mix(x, y, 0x003F0000, 0x0A);
    }
  }
}
