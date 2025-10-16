/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 09:59:12 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/15 10:02:30 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

int key_press(int keycode, t_player *player)
{
    if(keycode == W)
        player->key_up = true;
    if(keycode == S)
        player->key_down = true;
    if(keycode == A)
        player->key_left = true;
    if(keycode == D)
        player->key_right = true;
    if(keycode == LEFT)
        player->rot_left = true;
    if(keycode == RIGHT)
        player->rot_right = true;
    return 0;
}

int key_release(int keycode, t_player *player)
{
    if(keycode == W)
        player->key_up = false;
    if(keycode == S)
        player->key_down = false;
    if(keycode == A)
        player->key_left = false;
    if(keycode == D)
        player->key_right = false;
    if(keycode == LEFT)
        player->rot_left = false;
    if(keycode == RIGHT)
        player->rot_right = false;
    return 0;
}