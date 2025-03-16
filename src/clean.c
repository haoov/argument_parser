/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:10:32 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/16 14:46:39 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

extern struct exparg	*explist;

void	clean_expargs()
{
	struct exparg	*args, *next;

	args = explist;
	while (args)
	{
		next = args->next;
		free(args);
		args = next;
	}
}

void	clean_args(struct arg *args)
{
	struct arg	*next;

	clean_expargs();
	while (args)
	{
		next = args->next;
		free(args);
		args = next;
	}
}

