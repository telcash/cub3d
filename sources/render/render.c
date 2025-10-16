/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:03:18 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/16 09:39:39 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

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
    if(data->map[y][x] == '1')
        return true;
    return false;
}

void draw_line(t_data *data, float start_x, int i)
{
    float cos;
    float sin;
    float rx;
    float ry;

    cos = cosf(start_x);
    sin = sinf(start_x);
    rx = data->player.x;
    ry = data->player.y;
    while (!touch(rx, ry, data))
    {
        rx += cos;
        ry += sin;
    }
    float dist = sqrt((rx - data->player.x) * (rx - data->player.x) +
                      (ry - data->player.y) * (ry - data->player.y));
    int line_height = (BLOCK * HEIGHT) / dist;
    int start = (HEIGHT / 2) - (line_height / 2);
    int end = start + line_height;
    if (start < 0)
        start = 0;
    if (end >= HEIGHT)
        end = HEIGHT - 1;
    int y = 0;
    while (y < HEIGHT)
    {
        if (y < start)
            put_pixel(i, y, 0x87CEEB, data); // Sky color
        else if (y >= start && y <= end)
            put_pixel(i, y, 0x8B4513, data); // Wall color
        else
            put_pixel(i, y, 0x228B22, data); // Floor color
        y++;
    }
}

int draw_loop(t_data *data)
{   
    float fraction;
    float start_x;
    int i;

    t_player *player = &data->player;
    move_player(player);
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
    return 0;
}