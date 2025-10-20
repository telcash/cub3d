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

# define ERR_MALLOC "Error allocating memory"
# define ERR_NUM_ARGS "Invalid number of arguments"
# define ERR_FILE_CUB "Invalid file. Expected *.cub"
# define ERR_OPEN_FILE "Error opening file"
# define BAD_DATA_FORMAT "Invalid format on data file"
# define DUP_TEXTURE_FILE "Duplicated texture assignment on data file"
# define NOT_TEXTURE_FILE_SPEC "Not texture file specified"
# define ERR_FILE_XPM "Invalid texture file. Expected *.xpm"
# define DUP_COLOR "Duplicated color assignment on data file"
# define NOT_COLOR_SPEC "Not color specified"
# define INV_COLOR_FORMAT "Invalid color format on data file"
# define INV_RGB_VALUE "Invalid RGB color value on data file. RGB values must be between 0-255"
# define DUP_PLAYER "Invalid map, must be only one player"
# define BAD_MAP_DIMS "Invalid map dimmensions"
# define NO_PLAYER "Invalid map, there is no player"
# define OPEN_MAP "Invalid map, the map must be surrounded by walls"

# include "libft.h"
# include "mlx.h"
# include <X11/X.h>
#include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>

enum e_line_reading_status
{
    READING_PARAMS = 0,
    READING_COORDS = 1,
    DONE = 2
};

typedef struct s_rgb
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
} t_rgb;

typedef struct s_map
{
    int width;
    int height;
    char **grid;
}          t_map;

typedef struct s_player
{
    float x;
    float y;
    float angle;
    char view;
    int row;
    int col;
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
    int file_coords_line;
    char *no_t_file;
    char *so_t_file;
    char *we_t_file;
    char *ea_t_file;
    t_rgb *f;
    t_rgb *c;
    t_map map;
    t_player player;
}            t_data;

int    key_press(int keycode, t_player *player);
int    key_release(int keycode, t_player *player);
void move_player(t_player *player);
int draw_loop(t_data *data);
int init_data(t_data *data, char *file);
int verify_args(int argc, char **argv);
int ft_isspace(char c);
int ft_is_player_coord(char c);
void free_split(char **split);
int parse_file(t_data *data, char *file);
int get_color_from_line(char *line, int i, t_rgb **color);
int get_map_grid(t_data *data, char *file);

#endif