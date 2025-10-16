/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:28:55 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/16 09:31:48 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"


char **get_map(void)
{
    char **map = malloc(sizeof(char *) * 11);
    map[0] = "11111111111111111111";
    map[1] = "10000000000000000001";
    map[2] = "10000000000000000001";
    map[3] = "10000010000000000001";
    map[4] = "10000000000000000001";
    map[5] = "10000001000000000001";
    map[6] = "10000100000000000001";
    map[7] = "10000000000000000001";
    map[8] = "10000000000000000001";
    map[9] = "11111111111111111111";
    map[10] = NULL;
    return (map);
}
void init_data(t_data *data)
{
    data->player.x = WIDTH / 2;
    data->player.y = HEIGHT / 2;
    data->player.angle = PI / 2;

    data->player.key_up = false;
    data->player.key_down = false;
    data->player.key_right = false;
    data->player.key_left = false;

    data->player.rot_left = false;
    data->player.rot_right = false;

    data->map = get_map();
    data->mlx = mlx_init();
    data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "Game");
    data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
    data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_length, &data->endian);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}