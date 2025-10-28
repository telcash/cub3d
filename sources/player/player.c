/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:04:39 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/28 18:48:16 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

static void	translate_player(t_data *data, float cos_angle, float sin_angle,
		int speed)
{
	t_player	*player;
	int			forward;
	int			strafe;
	float		move_x;
	float		move_y;

	player = &data->player;
	forward = 0;
	if (player->key_up)
		forward += 1;
	if (player->key_down)
		forward -= 1;
	strafe = 0;
	if (player->key_right)
		strafe += 1;
	if (player->key_left)
		strafe -= 1;
	move_x = (cos_angle * forward - sin_angle * strafe) * speed;
	move_y = (sin_angle * forward + cos_angle * strafe) * speed;
	player_apply_movement(data, move_x, move_y);
}

static void	rotate_player(t_player *player, float angle_speed)
{
	if (player->rot_left)
		player->angle -= angle_speed;
	if (player->rot_right)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle = 0;
	if (player->angle < 0)
		player->angle = 2 * PI;
}

int	move_player(t_data *data)
{
	int			speed;
	float		angle_speed;
	float		cos_angle;
	float		sin_angle;
	t_player	*player;

	player = &data->player;
	speed = 3;
	angle_speed = 0.03;
	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	rotate_player(player, angle_speed);
	translate_player(data, cos_angle, sin_angle, speed);
	return (player->rot_left || player->rot_right || player->key_up
		|| player->key_down || player->key_left || player->key_right);
}

static void	set_angle_from_view(t_player *player)
{
	if (player->view == 'E')
		player->angle = 0;
	else if (player->view == 'S')
		player->angle = PI / 2;
	else if (player->view == 'W')
		player->angle = PI;
	else
		player->angle = (3 * PI) / 2;
}

void	set_player_start(t_data *data)
{
	int		row;
	int		col;
	float	center;

	row = data->player.row;
	col = data->player.col;
	center = 0.5f;
	data->player.x = ((float)col + center) * BLOCK;
	data->player.y = ((float)row + center) * BLOCK;
	data->map.grid[row][col] = '0';
	set_angle_from_view(&data->player);
}
