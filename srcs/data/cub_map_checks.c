/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_map_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wetieven <wetieven@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 07:30:13 by wetieven          #+#    #+#             */
/*   Updated: 2021/12/20 11:04:56 by wetieven         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub_data.h"
#include "cub_map.h"
#include "cub_map_checks.h"

bool	is_map_elem(char c)
{
	if (c == ' ' || c == '1' || c == '0'
		|| c == 'N' || c == 'W' || c == 'S' || c == 'E')
		return (true);
	else
		return (false);
}

t_error	measure_map(t_game *game, t_newline *nl)
{
	t_error			error;

	if (*nl->line == '\0')
		return (CLEAR); //just an empty line is valid, keep going
	error = CLEAR;
	if (cub_completion(game->data) != CLEAR)
		error = PARSE;
	if (!is_map_elem(*nl->line) || (error && is_map_elem(*nl->line)))
		error = ft_err_msg("Syntax error in cub file.", PARSE);
	if (!error)
	{
		if (!game->map_offset)
			game->map_offset = nl->count;
		if (ft_strlen(nl->line) > game->map.cols)
			game->map.cols = ft_strlen(nl->line);
		game->map.rows++;
	}
	return (error);
}

static int	map_fill(t_map map, size_t col, size_t row)
{
	int		c;
	int		r;
	t_error	error;

	if (col >= map.cols || row >= map.rows || *tile(&map, col, row) == OUTS)
	{
		ft_printf("Error:\nMap breach found around col %i row %i\n", col, row);
		return (PARSE);
	}
	else if (*tile(&map, col, row) == WALL || *tile(&map, col, row) == FILL)
		return (CLEAR);
	*tile(&map, col, row) = FILL;
	error = CLEAR;
	r = -1;
	while (!error && r <= 1)
	{
		c = -1;
		while (!error && c <= 1)
		{
			error = map_fill(map, col + c, row + r);
			c++;
		}
		r++;
	}
	return (error);
}

t_error	map_breached(t_game *game)
{
	t_error	error;
	t_map	test_map;

	error = vctr_init(&test_map.grid, sizeof(t_tile), game->map.grid->entries);
	if (error)
		return (error);
	test_map.cols = game->map.cols;
	test_map.rows = game->map.rows;
	ft_memcpy(test_map.grid->data, game->map.grid->data,
			game->map.grid->entries * sizeof(t_tile));
	map_fill(test_map, game->plyr.pos.col, game->plyr.pos.row);
	ft_printf("Map fill :\n"); //TESTING
	print_map_vctr(test_map); //TESTING
	vctr_exit(test_map.grid);
	return (error);
}

void	print_map_vctr(t_map map)
{ // Map parsing tester
	size_t	c;
	size_t	r;

	r = 0;
	while (r < map.rows)
	{
		c = 0;
		while (c < map.cols)
			ft_printf("%i", *tile(&map, c++, r));
		ft_printf("\n");
		r++;
	}
	ft_printf("\n");
}
