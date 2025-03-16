/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:00:18 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/16 11:58:23 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

extern char	*__progname;

void	arg_err(struct arg *arg, const char *err)
{
	fprintf(stderr, "%s: ", __progname);
	fprintf(stderr, "%s `%s`\n", err, arg->val.pval);
}

