/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:07:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 18:29:02 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#define ERROR	-1
#define SUCCESS	0

#define PERR_INOPT	"invalid option"
#define PERR_IVAL	"invalid value"
#define PERR_REQARG	"option requires an argument"

/* TYPES */
typedef bool	(*check_ft)(const char *);
typedef void	(*act_ft)(void*, void*);

enum argtype
{
	BOOL_T,
	INT_T,
	STR_T
};

union argval
{
	int			ival;
	const char	*pval;
};

/* Struct to store arguments */
struct arg
{
	const char		*name;
	char			shval;	// Short option value
	char			*lgval;	// Long option value
	enum argtype	 type;	// Type of argument
	bool			found;	// Argument has been found
	union argval	val;	// Argument value
	struct arg		*next;	// Pointer to next arg
};

struct argparser
{
	struct arg		*args;
	int				err;
};

/* MACROS */
/* Check if string 's' is in option format */
#define IS_OPT(s)	((!strncmp("--", s, 2) && strlen(s) > 2) || \
					(*s == '-' && s[1]))

/* FUNCTION DECLARATIONS */
struct argparser	*new_parser();
void				add_argument(struct argparser *p, char *name,
									char sh, char *lg, int type);
const char			*get_strarg(struct arg *list, const char *name);
int					get_intarg(struct arg *list, const char *name);
void				arg_err(const char *fmt, ...);
void				parse_args(struct argparser *p, const char **av);
void				free_args(struct arg *args);
void				free_parser(struct argparser *p);

#endif
