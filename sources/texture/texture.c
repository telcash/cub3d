/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:15:59 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/20 11:04:56 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

static int	get_texture(t_data *data, char *file, int **texture)
{
	int				x;
	int				y;
	int				bytes_per_pixel;
	int				tex_width;
	int				tex_height;
	unsigned char	*src;
	int				*pixels;

	reset_data_img(data);
	data->img = mlx_xpm_file_to_image(data->mlx, file, &tex_width, &tex_height);
	if (!data->img)
		return (ft_putendl_fd(TEXTURE_FILE_ERR, 2), 1);
	if (!data->texture_size)
		data->texture_size = tex_width;
	else if (data->texture_size != tex_width || tex_width != tex_height)
	{
		ft_putendl_fd(TEXTURE_FILE_ERR, 2);
		reset_data_img(data);
		return (1);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_length,
			&data->endian);
	bytes_per_pixel = data->bpp / 8;
	if (bytes_per_pixel <= 0)
		bytes_per_pixel = 4;
	pixels = ft_calloc(tex_width * tex_height, sizeof(int));
	if (!pixels)
	{
		reset_data_img(data);
		ft_putendl_fd(ERR_MALLOC, 2);
		return (1);
	}
	y = 0;
	while (y < tex_height)
	{
		x = 0;
		while (x < tex_width)
		{
			src = (unsigned char *)(data->addr + y * data->line_length
					+ x * bytes_per_pixel);
			pixels[y * tex_width + x] = src[0] | (src[1] << 8)
				| (src[2] << 16) | (bytes_per_pixel > 3 ? (src[3] << 24) : 0);
			x++;
		}
		y++;
	}
	reset_data_img(data);
	*texture = pixels;
	return (0);
}

int init_textures(t_data * data)
{
    if (!data->no_t_file || !data->so_t_file || !data->we_t_file || !data->ea_t_file || !data->f || !data->c)
        return (ft_putendl_fd(MISSING_TEXTURES_ERR, 2), 1);
    data->ceiling_color = rgb_to_hex(data->c);
    data->floor_color = rgb_to_hex(data->f);
    data->texture_size = 0;
    if (get_texture(data, data->no_t_file, &data->no_texture) ||
        get_texture(data, data->so_t_file, &data->so_texture) ||
        get_texture(data, data->we_t_file, &data->we_texture) ||
        get_texture(data, data->ea_t_file, &data->ea_texture))
        return (1);
    return (0);
}
