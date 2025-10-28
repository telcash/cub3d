/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 10:02:11 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/29 10:02:11 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

#define COLLISION_RADIUS 10.0f

static int	is_wall_cell(t_map *map, float x, float y)
{
	int		grid_x;
	int		grid_y;
	char	cell;

	if (x < 0.0f || y < 0.0f)
		return (1);
	grid_x = (int)(x / BLOCK);
	grid_y = (int)(y / BLOCK);
	if (grid_x < 0 || grid_x >= map->width || grid_y < 0
		|| grid_y >= map->height)
		return (1);
	cell = map->grid[grid_y][grid_x];
	if (cell == '1' || cell == ' ')
		return (1);
	return (0);
}

static int	collides(t_map *map, float x, float y)
{
	if (is_wall_cell(map, x - COLLISION_RADIUS, y - COLLISION_RADIUS))
		return (1);
	if (is_wall_cell(map, x + COLLISION_RADIUS, y - COLLISION_RADIUS))
		return (1);
	if (is_wall_cell(map, x - COLLISION_RADIUS, y + COLLISION_RADIUS))
		return (1);
	if (is_wall_cell(map, x + COLLISION_RADIUS, y + COLLISION_RADIUS))
		return (1);
	return (0);
}

void	player_apply_movement(t_data *data, float delta_x, float delta_y)
{
	t_player	*player;
	t_map		*map;

	if (delta_x == 0.0f && delta_y == 0.0f)
		return ;
	player = &data->player;
	map = &data->map;
	if (delta_x != 0.0f && !collides(map, player->x + delta_x, player->y))
		player->x += delta_x;
	if (delta_y != 0.0f && !collides(map, player->x, player->y + delta_y))
		player->y += delta_y;
}
