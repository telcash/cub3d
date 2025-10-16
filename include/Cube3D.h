#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265359

# include "libft.h"
# include "mlx.h"
# include <X11/X.h>
#include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>


typedef struct s_player
{
    float x;
    float y;
    float angle;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;
    bool rot_left;
    bool rot_right;
}           t_player;

typedef struct s_data
{
    void   *mlx;
    void   *win;
    void  *img;
    char *addr;
    int bpp;
    int line_length;
    int endian;
    t_player player;
    char **map;
}            t_data;

int    key_press(int keycode, t_player *player);
int    key_release(int keycode, t_player *player);
void move_player(t_player *player);
int draw_loop(t_data *data);
void init_data(t_data *data);

#endif