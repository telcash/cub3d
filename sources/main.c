/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:45:25 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/21 10:11:01 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cube3D.h"

void	print_test(t_data *data)
{
	int	h;
	int	w;

	printf("Textura NO: %s\n", data->no_t_file);
	printf("Textura SO: %s\n", data->so_t_file);
	printf("Textura EA: %s\n", data->ea_t_file);
	printf("Textura WE: %s\n", data->we_t_file);
	printf("Floor color: (%d, %d, %d)\n", data->f->r, data->f->g, data->f->b);
	printf("Ceiling color: (%d, %d, %d)\n", data->c->r, data->c->g, data->c->b);
	printf("Maps coors begin in line: %d\n", data->file_coords_line);
	printf("Width: %d\n", data->map.width);
	printf("Height: %d\n", data->map.height);
	printf("Player view direction: %c\n", data->player.view);
	printf("Player init row: %d\n", data->player.row);
	printf("Player init col: %d\n", data->player.col);
	printf("Coords: \n");
	h = 0;
	while (data->map.grid[h])
	{
		w = 0;
		while (data->map.grid[h][w])
		{
			printf("%c", data->map.grid[h][w]);
			w++;
		}
		h++;
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (verify_args(argc, argv) || init_data(&data, argv[1]))
		return (1);
	print_test(&data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	draw_scene(&data);
	mlx_loop_hook(data.mlx, draw_loop, &data);
	mlx_loop(data.mlx);
}
