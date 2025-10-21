/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 08:44:22 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/21 10:48:00 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

static int	verify_map_borders(t_map *map)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (map->grid[i])
	{
		j = 0;
		while (map->grid[i][j])
		{
			if (map->grid[i][j] == '0' || ft_is_player_coord(map->grid[i][j]))
			{
				if (i == 0 || i == (size_t)map->height - 1)
					return (1);
				if (j == 0 || j == ft_strlen(map->grid[i]) - 2)
					return (1);
				if (j >= ft_strlen(map->grid[i - 1]) - 1
					|| j >= ft_strlen(map->grid[i + 1]) - 1)
					return (1);
				if (map->grid[i][j - 1] == ' ' || map->grid[i][j + 1] == ' '
					|| map->grid[i - 1][j] == ' ' || map->grid[i + 1][j] == ' ')
					return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static void	copy_map_grid(t_data *data, int fd)
{
	char	*line;
	int		row;

	line = get_next_line(fd);
	row = 0;
	while (line)
	{
		data->map.grid[row++] = ft_strdup(line);
		free(line);
		line = get_next_line(fd);
	}
	data->map.grid[row] = NULL;
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

static void	set_player_start(t_data *data)
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

int	get_map_grid(t_data *data, char *file)
{
	int		fd;
	char	*line;
	int		lines_readed;

	lines_readed = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (ft_putendl_fd(ERR_OPEN_FILE, 2), 1);
	while (++lines_readed < data->file_coords_line)
	{
		line = get_next_line(fd);
		free(line);
	}
	data->map.grid = malloc((data->map.height + 1) * sizeof(char *));
	if (!data->map.grid)
		return (ft_putendl_fd(ERR_MALLOC, 2), close(fd), 1);
	copy_map_grid(data, fd);
	if (verify_map_borders(&data->map))
		return (ft_putendl_fd(OPEN_MAP, 2), 1);
	set_player_start(data);
	return (close(fd), 0);
}
