/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wetieven <wetieven@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 11:43:29 by wetieven          #+#    #+#             */
/*   Updated: 2021/12/17 16:27:14 by wetieven         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "cub3d.h"
#include "cub_data.h"
#include "cub_map.h"

t_tile	*tile(t_game *game, size_t col, size_t row)
{
	return ((t_tile *)(game->map.grid->data + col * sizeof(t_tile)
		+ (row * game->map.cols) * sizeof(t_tile)));
}

void	print_map_vctr(t_game *game, t_map map)
{
	size_t	c;
	size_t	r;

	r = 0;
	while (r < map.rows)
	{
		c = 0;
		while (c < map.cols)
			ft_printf("%i", *tile(game, c++, r));
		ft_printf("\n");
		r++;
	}
}

t_error	cub_shutdown(t_game *game, t_error cause)
{
	int		i;
	t_txtr	*texture;

	if (cause == MEM_ALLOC)
		ft_printf("Error\nMemory is full, consider closing Google Chrome.\n");
	i = CUB_DATA_RANGE;
	while (i-- > FLO)
		if (game->data[i].ctnt != NULL)
			free(game->data[i].ctnt);
	while (i--)
	{
		texture = game->data[i].ctnt;
		if (game->data[i].ctnt != NULL)
			mlx_destroy_image(game->fov->mlx.lnk, texture->img.ptr);
	}
	if (game->map.grid != NULL)
		vctr_exit(game->map.grid);
	//then some shit to take care of the fov, and before that some consideration whether to where we should put the fov structure wise
	return (cause);
}

t_error	cub_gnl_loop(t_game *game, t_cub_reader mode, t_newline *nl)
{
	t_error	error;

	error = CLEAR;
	nl->count = 0;
	game->map.rows = 0; //count rows twice as we parse for plyr positioning
	while (!error)
	{
		nl->status = get_next_line(nl->fd, &nl->line);
		if (nl->status >= 0)
		{
			nl->count++;
			if (mode == cub_map && nl->count < game->map_offset)
				;
			else
				error = mode(game, nl);
			free(nl->line); // are we sure of this
		}
		if (nl->status <= 0)
			break ;
	}
	if (!error && mode == cub_data)
	{
		if (cub_chk(game->data) != CLEAR)
			return (ft_err_msg("The cub file is missing data.", PARSE));
		if (!game->map.rows)
			return (ft_err_msg("The cub file is missing a map.", PARSE));
		if (game->map.cols <= 2)
			error = ft_err_msg("Insufficient map width.", PARSE);
		if (game->map.rows <= 2)
			error = ft_err_msg("Insufficient map height.", PARSE);
	}
	return (error);
}

static t_error	cub_read_conf(t_game *game, const char *cub_path)
{
	t_error		error;
	t_newline	nl;

	if (fd_opener(cub_path, &nl.fd) != CLEAR)
	{
		ft_printf("Error\nThe map file path \"%s\" leads nowhere\n", cub_path);
		return (FD_OPENING);
	}
	error = cub_gnl_loop(game, cub_data, &nl);
	fd_killer(nl.fd);
	if (error)
		return (error);
	if (fd_opener(cub_path, &nl.fd) != CLEAR)
		return (ft_err_msg("No luck. You can't open a lousy fd.", FD_OPENING));
	if (vctr_init(&game->map.grid, sizeof(t_tile), 512) != CLEAR)
		return (MEM_ALLOC);
	error = cub_gnl_loop(game, cub_map, &nl);
	fd_killer(nl.fd);
	return (error);
}

int	main(int ac, char **av)
{
	t_game	game;
	t_fov	fov;
	t_error	error;

	if (ac != 2)
		return (ft_err_msg("USAGE : ./cub3d <MAP_PATH>.cub", PARSE));
	if (file_ext_chk(av[1], ".cub") != CLEAR)
		return (ft_err_msg("Config file extension must be \".cub\"", PARSE));
	game.map.grid = NULL;
	game.map.cols = 0;
	game.map_offset = 0;
	game.plyr.exists = false;
	game.fov = &fov;
	fov.mlx.lnk = mlx_init();
	if (!fov.mlx.lnk)
		return (MEM_ALLOC);
	error = CLEAR;
	if (!error)
		error = cub_read_conf(&game, av[1]);
	if (!error)
		print_map_vctr(&game, game.map);
		// init fov struct upon success
	return (cub_shutdown(&game, error));
}
