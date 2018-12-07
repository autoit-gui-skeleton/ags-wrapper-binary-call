#include <math.h>
#include <stdlib.h>
#include "ez-draw.c"

enum { ACT_UP, ACT_DOWN, ACT_LEFT, ACT_RIGHT, ACT_ROTATE_CW, ACT_ROTATE_CCW, ACT_DROP, ACT_PAUSE };

#define CHECK_BIT(var, pos)		((var) & ( 1 << (pos)))

#define DEFAULT_WIDTH	10
#define DEFAULT_HEIGHT	20
#define DEFAULT_TILE	20

#define MIN_WIDTH		5
#define MIN_HEIGHT		5
#define MAX_WIDTH		400
#define MAX_HEIGHT		200
#define MIN_TILE		4
#define MAX_TILE		100
#define TOP_HEIGHT		4

#define MAX_LEVEL		25
#define MAX_TYPE		7
#define MAX_DIRECTION	4

#define EMPTY_CELL		-1
#define PIECE_SIZE		4

#define COLOR_MEDIUMGREY		0x646464
#define COLOR_LIGHTGREY			0xC0C0C0

typedef struct
{
	char** board;
	int width, height, tile_size;
	int x, y;
	int level, clear_lines, score, game_over, pause;
	int type, direction, next_type, next_direction;
} Game;

short piece_def[MAX_TYPE][MAX_DIRECTION] = {
	{0x4444, 0x0F00, 0x2222, 0x00F0}, /* I */
	{0x0660, 0x0660, 0x0660, 0x0660}, /* O */
	{0x4C40, 0x04E0, 0x4640, 0x0E40}, /* T */
	{0x4C80, 0xC600, 0x4C80, 0xC600}, /* Z */
	{0x4620, 0x6C00, 0x4620, 0x6C00}, /* S */
	{0xE200, 0x2260, 0x08E0, 0xC880}, /* J */
	{0x02E0, 0x88C0, 0xE800, 0xC440}  /* L */
};

/* http://tetris.wikia.com/wiki/SRS */
int piece_srs_adjust[MAX_TYPE][MAX_DIRECTION][2] =
	{
		{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
		{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
		{{0, 0}, {0, 1}, {0, -1}, {0, 0}},
		{{-1, 0}, {1, -1}, {0, 1}, {0, 0}},
		{{0, 0}, {0, -1}, {1, 1}, {-1, 0}},
		{{1, -1}, {1, 1}, {-1, 1}, {-1, -1}},
		{{0, 1}, {-1, -1}, {1, -1}, {0, 1}}
	};

unsigned int color_def[] = { 0xFFD400, 0x2ED1F4, 0xAA17CE, 0x0202D2, 0x00E896, 0xFF4D00, 0x0099FF };

void draw_tile(Ez_window win, int tile_size, int px, int py, int color, int shadow)
{
	ez_set_color(color);
	ez_fill_rectangle(win, px, py, px + tile_size, py + tile_size);

	ez_set_color(ez_white);
	ez_draw_line(win, px + 1, py + 1, px + tile_size - 1, py + 1);
	ez_draw_line(win, px + 1, py + 2, px + 1, py + tile_size - 2);

	if(shadow) ez_fill_rectangle(win, px + 4, py + 4, px + tile_size - 4, py + tile_size - 4);

	ez_set_color(ez_grey);
	ez_draw_line(win, px + 1, py + tile_size - 2, px + tile_size - 2, py + tile_size - 2);
	ez_draw_line(win, px + tile_size - 2, py + 2, px + tile_size - 2, py + tile_size - 2);
}

void draw_piece(Ez_window win, int tile_size, int x, int y, int type, int direction, int shadow)
{
	int i, j;

	for(i = 0; i < PIECE_SIZE; i++)
	for(j = 0; j < PIECE_SIZE; j++)
	{
		if(CHECK_BIT(piece_def[type][direction], i * PIECE_SIZE + j))
		{
			draw_tile(win, tile_size, (j + x) * tile_size, (i - TOP_HEIGHT + y) * tile_size, color_def[type], shadow);
		}
	}
}

void draw_board(Ez_window win)
{
	Game* game = ez_get_data(win);
	int tile_size = game->tile_size;
	int i, j;

	for(i = 0; i < game->height; i++)
	for(j = 0; j < game->width; j++)
	{
		if(game->board[j][i] != EMPTY_CELL)
		{
			draw_tile(win, tile_size, j * tile_size, (i - TOP_HEIGHT) * tile_size, game->game_over ? COLOR_LIGHTGREY : color_def[game->board[j][i]], 0);
		}
	}
}

void draw_current_piece(Ez_window win)
{
	Game* game = ez_get_data(win);
	int tile_size = game->tile_size;
	int shadow_y = 0;

	if(game->game_over) return;
	while(!game_check_collision(game, 0, shadow_y + 1, 0)) shadow_y++;
	draw_piece(win, tile_size, game->x, game->y + shadow_y, game->type, game->direction, 1);

	draw_piece(win, tile_size, game->x, game->y, game->type, game->direction, 0);
}

void draw_info(Ez_window win)
{
	Game* game = ez_get_data(win);
	int tile_size = game->tile_size;

	ez_set_color(COLOR_MEDIUMGREY);
	ez_fill_rectangle(win, game->width * tile_size, 0, (game->width + 7) * tile_size, (game->height - TOP_HEIGHT) * tile_size);

	ez_set_color(COLOR_LIGHTGREY);
	ez_fill_rectangle(win, (game->width + 1) * tile_size, 2 * tile_size, (game->width + 1 + 5) * tile_size, (2 + 6) * tile_size);

	ez_set_color(ez_white);
	ez_draw_rectangle(win, (game->width + 1) * tile_size, 2 * tile_size, (game->width + 1 + 5) * tile_size, (2 + 6) * tile_size);

	ez_set_color(ez_white);
	ez_draw_text(win, EZ_BL, (game->width + 2) * tile_size, 10 * tile_size, "Score");
	ez_draw_text(win, EZ_BL, (game->width + 1) * tile_size, 11 * tile_size, "%6i", game->score);

	ez_draw_text(win, EZ_BL, (game->width + 2) * tile_size, 13 * tile_size, "Lines");
	ez_draw_text(win, EZ_BL, (game->width + 1) * tile_size, 14 * tile_size, "%6i", game->clear_lines);

	ez_draw_text(win, EZ_BL, (game->width + 2) * tile_size, 16 * tile_size, "Level");
	ez_draw_text(win, EZ_BL, (game->width + 1) * tile_size, 17 * tile_size, "%6i", game->level);

	ez_draw_text(win, EZ_BL, (game->width + 1.5) * tile_size, 19 * tile_size, "H: Help");

	if(game->game_over)
		ez_draw_text(win, EZ_BL, (game->width + 1) * tile_size, 4 * tile_size, "Game Over");
	else
		draw_piece(win, game->tile_size, game->width + 1, 7, game->next_type, game->next_direction, 0);

	if(game->pause)
	{
		ez_set_color(ez_black);
		ez_draw_text(win, EZ_BL, 1 * tile_size, 5 * tile_size, "Arrow: Move");
		ez_draw_text(win, EZ_BL, 1 * tile_size, 6 * tile_size, "Z/X: Rotate");
		ez_draw_text(win, EZ_BL, 1 * tile_size, 7 * tile_size, "Space: Drop");
		ez_draw_text(win, EZ_BL, 1 * tile_size, 8 * tile_size, "Q: Quit");
	}
}

int game_check_collision(Game* game, int dx, int dy, int dd)
{
	int i, j;
	int x = game->x + dx;
	int y = game->y + dy;
	int direction = (game->direction + dd) % MAX_DIRECTION;

	for(i = 0; i < PIECE_SIZE; i++)
	for(j = 0; j < PIECE_SIZE; j++)
	{
		if(CHECK_BIT(piece_def[game->type][direction], i * PIECE_SIZE + j))
		{
			if(j + x < 0 || j + x >= game->width || i + y >= game->height) return 1;

			if(i + y >= 0 && i + y < game->height && j + x >= 0 && j + x < game->width)
			{
				if(game->board[j + x][i + y] != EMPTY_CELL) return 1;
			}
		}
	}
	return 0;
}

int game_check_emergence(Game* game)
{
	int i, j;
	int y = game->y + 1;

	for(i = 0; i < PIECE_SIZE; i++)
	for(j = 0; j < PIECE_SIZE; j++)
	{
		if(CHECK_BIT(piece_def[game->type][game->direction], i * PIECE_SIZE + j))
		{
			if(i + y > TOP_HEIGHT) return 1;
		}
	}
	return 0;
}

void game_piece_place(Game* game)
{
	int i, j, n, row_full;
	int score_base, bottom = 0, clear_line_once = 0;

	for(i = 0; i < PIECE_SIZE; i++)
	for(j = 0; j < PIECE_SIZE; j++)
	{
		if(CHECK_BIT(piece_def[game->type][game->direction], i * PIECE_SIZE + j))
		{
			game->board[j + game->x][i + game->y] = game->type;
			bottom = (i + game->y > bottom ? i + game->y : bottom);
		}
	}
	bottom = game->height - 1 - bottom;

	for(i = game->height - 1, n = game->height - 1; i >= 0; i--, n--)
	{
		while(n >= 0)
		{
			row_full = 1;
			for(j = 0; j < game->width; j++)
			{
				if(game->board[j][n] == EMPTY_CELL)
				{
					row_full = 0;
					break;
				}
			}

			if(!row_full) break;
			n--;
			game->clear_lines++;
			clear_line_once++;
		}

		for(j = 0; j < game->width; j++)
		{
			game->board[j][i] = (n >= 0 ? game->board[j][n] : EMPTY_CELL);
		}
	}

	score_base = bottom + game->level + 1;
	game->level = (game->clear_lines / 10 > MAX_LEVEL ? MAX_LEVEL : game->clear_lines / 10);

	switch(clear_line_once)
	{
		case 4:
			game->score += 8 * score_base;
			break;
		case 3:
			game->score += 5 * score_base;
			break;
		case 2:
			game->score += 3 * score_base;
			break;
		case 1:
			game->score += 1 * score_base;
	}
}

void game_piece_rotate(Game* game, int new_direction, int clockwise)
{
	static int wall_kick_adjust[] = {0, 1, -1, 2, -2};
	int i, dx, dy;

	if(clockwise)
	{
		dx = piece_srs_adjust[game->type][new_direction][0];
		dy = piece_srs_adjust[game->type][new_direction][1];
	}
	else
	{
		dx = -piece_srs_adjust[game->type][(new_direction + 1) % MAX_DIRECTION][0];
		dy = -piece_srs_adjust[game->type][(new_direction + 1) % MAX_DIRECTION][1];
	}

	for(i = 0; i < sizeof(wall_kick_adjust) / sizeof(int); i++)
	{
		if(!game_check_collision(game, dx + wall_kick_adjust[i], dy, new_direction - game->direction))
		{
			game->x += dx + wall_kick_adjust[i];
			game->y += dy;
			game->direction = new_direction;
			return;
		}
	}
}

void game_piece_next(Game* game)
{
	game->type = game->next_type;
	game->direction = game->next_direction;

	game->next_type = rand() % MAX_TYPE;
	game->next_direction = rand() % MAX_DIRECTION;

	game->x = (game->width / 2) - (PIECE_SIZE / 2);
	game->y = -TOP_HEIGHT;

	while(!game_check_emergence(game)) game->y++;
	if(game_check_collision(game, 0, 0, 0)) game->game_over = 1;
}

int game_action(Game* game, int action)
{
	if(action == ACT_PAUSE)
	{
		game->pause ^= 1;
		return 1;
	}

	if(game->game_over || game->pause) return 0;

	switch(action)
	{
		case ACT_UP:
			if(!game_check_collision(game, 0, -1, 0)) game->y--;
			break;

		case ACT_DOWN:
			if(!game_check_collision(game, 0, 1, 0))
			{
				game->y++;
			}
			else
			{
				game_piece_place(game);
				game_piece_next(game);
			}
			break;

		case ACT_LEFT:
			if(!game_check_collision(game, -1, 0, 0)) game->x--;
			break;

		case ACT_RIGHT:
			if(!game_check_collision(game, 1, 0, 0)) game->x++;
			break;

		case ACT_DROP:
			while(!game_check_collision(game, 0, 1, 0)) game->y++;
			game_piece_place(game);
			game_piece_next(game);
			break;

		case ACT_ROTATE_CW:
			game_piece_rotate(game, (game->direction + 1) % MAX_DIRECTION, 1);
			break;

		case ACT_ROTATE_CCW:
			game_piece_rotate(game, (game->direction + 3) % MAX_DIRECTION, 0);

	}
	return 1;
}

void game_start(Game* game)
{
	int i, j;

	game->next_type = rand() % MAX_TYPE;
	game->next_direction = rand() % MAX_DIRECTION;

	game->score = 0;
	game->level = 0;
	game->clear_lines = 0;
	game->game_over = 0;
	game->pause = 0;

	for(j = 0; j < game->height; j++)
	for(i = 0; i < game->width; i++)
	{
		game->board[i][j] = EMPTY_CELL;
	}

	game_piece_next(game);
}

Game* game_new(int width, int height, int tile_size)
{
	Game* game;
	int i;

	width = (width > MAX_WIDTH ? MAX_WIDTH : (width < MIN_WIDTH ? MIN_WIDTH : width ));
	height = (height > MAX_HEIGHT ? MAX_HEIGHT : (height < MIN_HEIGHT ? MIN_HEIGHT : height ));
	tile_size = (tile_size > MAX_TILE ? MAX_TILE : (tile_size < MIN_TILE ? MIN_TILE : tile_size ));

	height += TOP_HEIGHT;

	game = malloc(sizeof(Game));
	if(game == 0) return 0;

	game->board = malloc(width * sizeof(char*));
	if(game->board == 0) return 0;

	for(i = 0; i < width; i++)
	{
		game->board[i] = malloc(height * sizeof(char));
		if(game->board[i] == 0) return 0;
	}

	game->width = width;
	game->height = height;
	game->tile_size = tile_size;

	return game;
}

void win_on_expose(Ez_event *ev)
{
	draw_board(ev->win);
	draw_current_piece(ev->win);
	draw_info(ev->win);
}

void win_on_key_press(Ez_event *ev)
{
	Game* game = ez_get_data(ev->win);
	int send_expose = 0;

    switch(ev->key_sym)
	{
		case XK_q: case XK_Q: case XK_Escape:
			ez_quit();
			break;

		case XK_Down:
			send_expose = game_action(game, ACT_DOWN);
			break;

		case XK_Left:
			send_expose = game_action(game, ACT_LEFT);
			break;

		case XK_Right:
			send_expose = game_action(game, ACT_RIGHT);
			break;

		case XK_x: case XK_X: case XK_r: case XK_R: case XK_Up:
			send_expose = game_action(game, ACT_ROTATE_CW);
			break;

		case XK_z: case XK_Z: case XK_e: case XK_E:
			send_expose = game_action(game, ACT_ROTATE_CCW);
			break;

		case XK_h: case XK_H: case XK_p: case XK_P:
			send_expose = game_action(game, ACT_PAUSE);
			break;

		case XK_space:
			if(game->game_over)
			{
				game_start(game);
				send_expose = 1;
			}
			else
			{
				send_expose = game_action(game, ACT_DROP);
			}
			break;

    }

	if(send_expose) ez_send_expose(ev->win);
}

void win_on_timer(Ez_event *ev)
{
	Game* game = ez_get_data(ev->win);

	if(game_action(game, ACT_DOWN))
	{
		ez_send_expose(ev->win);
	}
	ez_start_timer(ev->win, (int)(1000.0 * pow(0.9, game->level)));
}


void win_event(Ez_event *ev)
{
	switch(ev->type)
	{
		case Expose			: win_on_expose		(ev); break;
		case KeyPress		: win_on_key_press	(ev); break;
		case TimerNotify	: win_on_timer		(ev); break;
	}
}

void game_window_init(Game* game)
{
	Ez_window win;
	char buffer[20];

	sprintf(buffer, "%dx%d", game->tile_size / 2, game->tile_size);
	ez_font_load(0, buffer);
	ez_set_nfont(0);

	win = ez_window_create((game->width + 7) * game->tile_size, (game->height - TOP_HEIGHT) * game->tile_size, "Tetris", win_event);
	ez_set_data(win, game);
	ez_window_dbuf(win, 1);
	ez_start_timer(win, 1000);
}

void main(int argc, char** argv)
{
	Game* game;
	int i, width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT, tile_size = DEFAULT_TILE;

	srand(time(NULL));

	if(argc >= 1)
	{
		width = atol(argv[1]);
		if(width == 0) width = DEFAULT_WIDTH;
	}

	if(argc >= 2)
	{
		height = atol(argv[2]);
		if(height == 0) height = DEFAULT_WIDTH;
	}

	if(argc >= 3)
	{
		tile_size = atol(argv[3]);
		if(tile_size == 0) tile_size = DEFAULT_TILE;
	}

	game = game_new(width, height, tile_size);
	if(!game || ez_init() < 0) exit(1);

	game_window_init(game);
	game_start(game);
	ez_main_loop();
	exit(0);
}
