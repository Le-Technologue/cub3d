/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wetieven <wetieven@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 11:43:26 by wetieven          #+#    #+#             */
/*   Updated: 2021/12/13 11:25:14 by wetieven         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"

# define MATCH 0

# define CUB_DATA_RANGE 6

typedef enum e_cub_key {
	NOR,
	SOU,
	WES,
	EAS,
	FLO,
	CEI
}	t_cub_key;

# define TILE_RANGE 7

typedef enum e_tile {
	OUTS,
	VOID,
	WALL,
	PL_N,
	PL_W,
	PL_S,
	PL_E
}	t_tile;

typedef struct s_plyr {
	bool		exists;
	t_pos		pos;
	t_tile		orientation;
}	t_plyr;

typedef struct s_game	t_game;
typedef struct s_cub	t_cub;

typedef t_error	(*t_cub_parser)(t_game *game, t_cub_key elem, t_newline *nl);

typedef struct s_cub {
	char			*flag;
	t_cub_parser	fct;
	void			*ctnt;
}	t_cub;

typedef struct s_fov {
	t_mlx	mlx;
	size_t	height;
	size_t	width;
	t_img	*frm;
	/* size_t	tile_hgt; */
	/* size_t	tile_wid; */
}	t_fov;

typedef struct s_game {
	t_cub	*data;
	t_map	map;
	t_plyr	plyr;
	t_fov	*fov;
}	t_game;

#endif
