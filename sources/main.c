/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:45:25 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/16 09:27:56 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cube3D.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    t_data data;

    init_data(&data);
    mlx_hook(data.win, 2, 1L<<0, key_press, &data.player);
    mlx_hook(data.win, 3, 1L<<1, key_release, &data.player);
    mlx_loop_hook(data.mlx, draw_loop, &data);
    mlx_loop(data.mlx);

}