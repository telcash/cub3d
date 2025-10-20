/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:03:18 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/20 10:04:44 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"
#include <float.h>

void put_pixel(int x, int y, int color, t_data *data)
{
    int index;
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        return ;
    index = (y * data->line_length) + (x * (data->bpp / 8));
    data->addr[index] = color & 0xFF;
    data->addr[index + 1] = (color >> 8) & 0xFF;
    data->addr[index + 2] = (color >> 16) & 0xFF;
}

void clear_image(t_data *data)
{
    int x;
    int y;

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            put_pixel(x, y, 0, data);
            x++;
        }
        y++;
    }
}

bool touch(float px, float py, t_data *data)
{
    int x;
    int y;

    x = (int)(px / BLOCK);
    y = (int)(py / BLOCK);
    if (x < 0 || x >= data->map.width || y < 0 || y >= data->map.height)
        return true;
    if(data->map.grid[y][x] == '1')
        return true;
    return false;
}

void	draw_line(t_data *data, float start_x, int i)
{
	float	dir_x;
	float	dir_y;
	float	rx;
	float	ry;
	float	prev_x;
	float	prev_y;
	float	hit_x;
	float	hit_y;
	float	vertical_t;
	float	horizontal_t;
	float	distance;
	bool	vertical_hit;
	int		wall_top;
	int		wall_bottom;
	int		draw_start;
	int		draw_end;
	int		y;
	int		*texture;
	float	tex_step;
	float	tex_pos;
	int		tex_x;
	int		tex_y;
	int		color;
	int		line_height;
	float	prev_block_x;
	float	prev_block_y;
	float	offset;

	dir_x = cosf(start_x);
	dir_y = sinf(start_x);
	rx = data->player.x;
	ry = data->player.y;
	prev_x = rx;
	prev_y = ry;
	while (!touch(rx, ry, data))
	{
		prev_x = rx;
		prev_y = ry;
		rx += dir_x;
		ry += dir_y;
	}
	vertical_t = FLT_MAX;
	horizontal_t = FLT_MAX;
	prev_block_x = floorf(prev_x / BLOCK);
	prev_block_y = floorf(prev_y / BLOCK);
	if (fabsf(dir_x) > 1e-6f)
	{
		float boundary_x;

		boundary_x = ((dir_x > 0.f) ? (prev_block_x + 1.f) : prev_block_x)
			* BLOCK;
		vertical_t = (boundary_x - prev_x) / dir_x;
		if (vertical_t < 0.f)
			vertical_t = FLT_MAX;
	}
	if (fabsf(dir_y) > 1e-6f)
	{
		float boundary_y;

		boundary_y = ((dir_y > 0.f) ? (prev_block_y + 1.f) : prev_block_y)
			* BLOCK;
		horizontal_t = (boundary_y - prev_y) / dir_y;
		if (horizontal_t < 0.f)
			horizontal_t = FLT_MAX;
	}
	vertical_hit = vertical_t < horizontal_t;
	if (vertical_t == FLT_MAX && horizontal_t != FLT_MAX)
		vertical_hit = false;
	else if (horizontal_t == FLT_MAX && vertical_t != FLT_MAX)
		vertical_hit = true;
	if (vertical_hit)
	{
		hit_x = ((dir_x > 0.f) ? (prev_block_x + 1.f) : prev_block_x) * BLOCK;
		hit_y = prev_y + dir_y * vertical_t;
		texture = (dir_x > 0.f) ? data->ea_texture : data->we_texture;
		offset = fmodf(hit_y, BLOCK);
		if (offset < 0.f)
			offset += BLOCK;
		tex_x = (int)((offset / BLOCK) * data->texture_size);
	}
	else
	{
		hit_y = ((dir_y > 0.f) ? (prev_block_y + 1.f) : prev_block_y) * BLOCK;
		hit_x = prev_x + dir_x * horizontal_t;
		texture = (dir_y > 0.f) ? data->so_texture : data->no_texture;
		offset = fmodf(hit_x, BLOCK);
		if (offset < 0.f)
			offset += BLOCK;
		tex_x = (int)((offset / BLOCK) * data->texture_size);
	}
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= data->texture_size)
		tex_x = data->texture_size - 1;
	distance = sqrtf((hit_x - data->player.x) * (hit_x - data->player.x)
			+ (hit_y - data->player.y) * (hit_y - data->player.y));
	if (distance < 1e-4f)
		distance = 1e-4f;
	line_height = (BLOCK * HEIGHT) / distance;
	if (line_height < 1)
		line_height = 1;
	wall_top = (HEIGHT / 2) - (line_height / 2);
	wall_bottom = wall_top + line_height - 1;
	draw_start = (wall_top < 0) ? 0 : wall_top;
	draw_end = (wall_bottom >= HEIGHT) ? HEIGHT - 1 : wall_bottom;
	tex_step = (float)data->texture_size / (float)line_height;
	tex_pos = (draw_start - wall_top) * tex_step;
	if (!texture)
		texture = data->we_texture;
	y = 0;
	while (y < draw_start)
	{
		put_pixel(i, y, data->ceiling_color, data);
		y++;
	}
	while (y <= draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= data->texture_size)
			tex_y = data->texture_size - 1;
		if (texture)
			color = texture[tex_y * data->texture_size + tex_x];
		else
			color = 0x8B4513;
		put_pixel(i, y, color, data);
		tex_pos += tex_step;
		y++;
	}
	while (y < HEIGHT)
	{
		put_pixel(i, y, data->floor_color, data);
		y++;
	}
}

int draw_loop(t_data *data)
{   
    float fraction;
    float start_x;
    int i;

    t_player *player = &data->player;
    if (move_player(player))
    {
        clear_image(data);
        fraction = PI / 3 / WIDTH;
        start_x = player->angle - (PI / 6);
        i = 0;
        while (i < WIDTH)
        {
            draw_line(data, start_x, i);
            start_x += fraction;
            i++;
        }
        mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
    }
    return 0;
}
