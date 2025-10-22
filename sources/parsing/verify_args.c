/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csalazar <csalazar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:59:16 by csalazar          #+#    #+#             */
/*   Updated: 2025/10/22 15:57:28 by csalazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Cube3D.h"

int	verify_args(int argc, char **argv)
{
	if (argc != 2)
		return (ft_putendl_fd(ERR_NUM_ARGS, 2), 1);
	if (ft_strlen(argv[1]) < 5 || ft_strncmp(&argv[1][ft_strlen(argv[1]) - 4],
		".cub", 4) != 0)
		return (ft_putendl_fd(ERR_FILE_CUB, 2), 1);
	return (0);
}
