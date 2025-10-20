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

static void	update_wall_bounds(int *top, int *bottom, int height)
{
	*top = (HEIGHT / 2) - (height / 2);
	*bottom = *top + height - 1;
	if (*top < 0)
		*top = 0;
	if (*bottom >= HEIGHT)
		*bottom = HEIGHT - 1;
}

static int	sample_texture(const int *texture, int tex_x, float tex_pos,
		int tex_size)
{
	int	tex_y;

	if (!texture)
		return (0x8B4513);
	tex_y = (int)tex_pos;
	if (tex_y < 0)
		tex_y = 0;
	else if (tex_y >= tex_size)
		tex_y = tex_size - 1;
	return (texture[tex_y * tex_size + tex_x]);
}

void	draw_line(t_data *data, float start_x, int i)
{
	const float	dir_x = cosf(start_x);
	const float	dir_y = sinf(start_x);
	float		rx;
	float		ry;
	float		prev_x;
	float		prev_y;
	float		hit_x;
	float		hit_y;
	float		vertical_t;
	float		horizontal_t;
	bool		vertical_hit;
	float		distance;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			y;
	int			tex_x;
	float		tex_pos;
	float		tex_step;
	float		wall_fraction;
	int			*texture;

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
	if (fabsf(dir_x) > 1e-6f)
	{
		const float boundary_x = ((dir_x > 0.f)
				? (floorf(prev_x / BLOCK) + 1.f)
				: floorf(prev_x / BLOCK)) * BLOCK;
		const float delta = (boundary_x - prev_x) / dir_x;

		if (delta >= 0.f)
			vertical_t = delta;
	}
	if (fabsf(dir_y) > 1e-6f)
	{
		const float boundary_y = ((dir_y > 0.f)
				? (floorf(prev_y / BLOCK) + 1.f)
				: floorf(prev_y / BLOCK)) * BLOCK;
		const float delta = (boundary_y - prev_y) / dir_y;

		if (delta >= 0.f)
			horizontal_t = delta;
	}
	vertical_hit = vertical_t < horizontal_t;
	if (vertical_t == FLT_MAX && horizontal_t != FLT_MAX)
		vertical_hit = false;
	else if (horizontal_t == FLT_MAX && vertical_t != FLT_MAX)
		vertical_hit = true;
	if (vertical_hit)
	{
		hit_x = ((dir_x > 0.f)
				? (floorf(prev_x / BLOCK) + 1.f)
				: floorf(prev_x / BLOCK)) * BLOCK;
		hit_y = prev_y + dir_y * vertical_t;
		wall_fraction = (hit_y / BLOCK) - floorf(hit_y / BLOCK);
		tex_x = (int)(wall_fraction * data->texture_size);
		if (tex_x < 0)
			tex_x = 0;
		else if (tex_x >= data->texture_size)
			tex_x = data->texture_size - 1;
		if (dir_x > 0.f)
		{
			texture = data->ea_texture;
			tex_x = data->texture_size - tex_x - 1;
		}
		else
			texture = data->we_texture;
	}
	else
	{
		hit_y = ((dir_y > 0.f)
				? (floorf(prev_y / BLOCK) + 1.f)
				: floorf(prev_y / BLOCK)) * BLOCK;
		hit_x = prev_x + dir_x * horizontal_t;
		wall_fraction = (hit_x / BLOCK) - floorf(hit_x / BLOCK);
		tex_x = (int)(wall_fraction * data->texture_size);
		if (tex_x < 0)
			tex_x = 0;
		else if (tex_x >= data->texture_size)
			tex_x = data->texture_size - 1;
		if (dir_y > 0.f)
			texture = data->so_texture;
		else
		{
			texture = data->no_texture;
			tex_x = data->texture_size - tex_x - 1;
		}
	}
	distance = sqrtf((hit_x - data->player.x) * (hit_x - data->player.x)
			+ (hit_y - data->player.y) * (hit_y - data->player.y));
	if (distance < 1e-4f)
		distance = 1e-4f;
	line_height = (BLOCK * HEIGHT) / distance;
	if (line_height < 1)
		line_height = 1;
	update_wall_bounds(&draw_start, &draw_end, line_height);
	tex_step = (float)data->texture_size / (float)line_height;
	tex_pos = (draw_start - ((HEIGHT / 2) - (line_height / 2))) * tex_step;
	y = 0;
	while (y < draw_start)
		put_pixel(i, y++, data->ceiling_color, data);
	while (y <= draw_end)
	{
		put_pixel(i, y, sample_texture(texture, tex_x, tex_pos, data->texture_size),
			data);
		tex_pos += tex_step;
		y++;
	}
	while (y < HEIGHT)
		put_pixel(i, y++, data->floor_color, data);
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
