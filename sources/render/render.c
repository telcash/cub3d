/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:03:18 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/21 10:46:48 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

void	put_pixel(int x, int y, int color, t_data *data)
{
	int	index;

	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
		return ;
	index = (y * data->line_length) + (x * (data->bpp / 8));
	data->addr[index] = color & 0xFF;
	data->addr[index + 1] = (color >> 8) & 0xFF;
	data->addr[index + 2] = (color >> 16) & 0xFF;
}

void	clear_image(t_data *data)
{
	int	x;
	int	y;

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

bool	touch(float px, float py, t_data *data)
{
	int	x;
	int	y;

	x = (int)(px / BLOCK);
	y = (int)(py / BLOCK);
	if (x < 0 || x >= data->map.width || y < 0 || y >= data->map.height)
		return (true);
	if (data->map.grid[y][x] == '1' || data->map.grid[y][x] == ' ')
		return (true);
	return (false);
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

static float	inverse_abs(float value)
{
	float	result;

	if (value == 0.0f)
		return (1000000.0f);
	result = 1.0f / value;
	if (result < 0.0f)
		result = -result;
	return (result);
}

static int	compute_step(float direction)
{
	if (direction < 0.0f)
		return (-1);
	if (direction > 0.0f)
		return (1);
	return (0);
}

static float	initial_side(float pos, int map, float delta, int step)
{
	float	base;
	float	value;

	if (step == -1)
	{
		base = pos - map;
		value = base * delta;
		return (value);
	}
	if (step == 1)
	{
		base = map + 1.0f - pos;
		value = base * delta;
		return (value);
	}
	return (1000000.0f);
}

static void	init_ray(t_data *data, t_ray *ray, float angle)
{
	ray->angle = angle;
	ray->pos_x = data->player.x / BLOCK;
	ray->pos_y = data->player.y / BLOCK;
	ray->dir_x = cosf(angle);
	ray->dir_y = sinf(angle);
	ray->map_x = (int)ray->pos_x;
	ray->map_y = (int)ray->pos_y;
	ray->delta_x = inverse_abs(ray->dir_x);
	ray->delta_y = inverse_abs(ray->dir_y);
	ray->step_x = compute_step(ray->dir_x);
	ray->step_y = compute_step(ray->dir_y);
	ray->side_x = initial_side(ray->pos_x, ray->map_x, ray->delta_x,
			ray->step_x);
	ray->side_y = initial_side(ray->pos_y, ray->map_y, ray->delta_y,
			ray->step_y);
	ray->side = 0;
	ray->perp_dist = 0.0f;
	ray->wall_x = 0.0f;
	ray->tex_step = 0.0f;
	ray->tex_pos = 0.0f;
	ray->tex_x = 0;
	ray->line_height = 0;
	ray->texture = NULL;
}

static void	run_dda(t_data *data, t_ray *ray)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (ray->side_x < ray->side_y)
		{
			ray->side_x = ray->side_x + ray->delta_x;
			ray->map_x = ray->map_x + ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_y = ray->side_y + ray->delta_y;
			ray->map_y = ray->map_y + ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= data->map.width)
			hit = 1;
		else if (ray->map_y < 0 || ray->map_y >= data->map.height)
			hit = 1;
		else if (data->map.grid[ray->map_y][ray->map_x] == '1')
			hit = 1;
	}
}

static void	compute_perp_distance(t_ray *ray)
{
	float	offset;

	if (ray->side == 0)
	{
		offset = ray->map_x - ray->pos_x;
		offset = offset + (1 - ray->step_x) * 0.5f;
		ray->perp_dist = offset / ray->dir_x;
		ray->wall_x = ray->pos_y + ray->perp_dist * ray->dir_y;
	}
	else
	{
		offset = ray->map_y - ray->pos_y;
		offset = offset + (1 - ray->step_y) * 0.5f;
		ray->perp_dist = offset / ray->dir_y;
		ray->wall_x = ray->pos_x + ray->perp_dist * ray->dir_x;
	}
	if (ray->perp_dist < 0.0001f)
		ray->perp_dist = 0.0001f;
	ray->wall_x = ray->wall_x - floorf(ray->wall_x);
}

static void	select_texture(t_data *data, t_ray *ray)
{
	int	tex_size;
	int	tex_x;

	tex_size = data->texture_size;
	if (tex_size <= 0)
		tex_size = 1;
	tex_x = (int)(ray->wall_x * tex_size);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex_size)
		tex_x = tex_size - 1;
	if (ray->side == 0 && ray->dir_x > 0.0f)
		tex_x = tex_size - tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0.0f)
		tex_x = tex_size - tex_x - 1;
	if (ray->side == 0 && ray->dir_x > 0.0f)
		ray->texture = data->ea_texture;
	else if (ray->side == 0)
		ray->texture = data->we_texture;
	else if (ray->dir_y > 0.0f)
		ray->texture = data->so_texture;
	else
		ray->texture = data->no_texture;
	ray->tex_x = tex_x;
}

static void	setup_wall(t_data *data, t_ray *ray)
{
	int	line_height;
	int	tex_size;

	line_height = (int)(HEIGHT / ray->perp_dist);
	if (line_height < 1)
		line_height = 1;
	ray->line_height = line_height;
	tex_size = data->texture_size;
	if (tex_size <= 0)
		ray->tex_step = 0.0f;
	else
		ray->tex_step = (float)tex_size / ray->line_height;
	ray->tex_pos = 0.0f;
}

static void	render_column(t_data *data, t_ray *ray, int column)
{
	int	top;
	int	bottom;
	int	y;

	update_wall_bounds(&top, &bottom, ray->line_height);
	ray->tex_pos = top - (HEIGHT / 2 - ray->line_height / 2);
	ray->tex_pos = ray->tex_pos * ray->tex_step;
	y = 0;
	while (y < top)
	{
		put_pixel(column, y, data->ceiling_color, data);
		y++;
	}
	while (y <= bottom)
	{
		put_pixel(column, y, sample_texture(ray->texture, ray->tex_x,
				ray->tex_pos, data->texture_size), data);
		ray->tex_pos = ray->tex_pos + ray->tex_step;
		y++;
	}
	while (y < HEIGHT)
	{
		put_pixel(column, y, data->floor_color, data);
		y++;
	}
}

void	draw_line(t_data *data, float start_x, int column)
{
	t_ray	ray;

	init_ray(data, &ray, start_x);
	run_dda(data, &ray);
	compute_perp_distance(&ray);
	select_texture(data, &ray);
	setup_wall(data, &ray);
	render_column(data, &ray, column);
}

void	draw_scene(t_data *data)
{
	float		fraction;
	float		start_x;
	int			i;
	t_player	*player;

	player = &data->player;
	clear_image(data);
	fraction = PI / 3 / WIDTH;
	start_x = player->angle - (PI / 6);
	i = 0;
	while (i < WIDTH)
	{
		draw_line(data, start_x, i);
		start_x = start_x + fraction;
		i++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int	draw_loop(t_data *data)
{
	t_player	*player;

	player = &data->player;
	if (move_player(player))
		draw_scene(data);
	return (0);
}
