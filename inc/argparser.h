/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:07:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/16 12:51:20 by rasbbah          ###   ########.fr       */
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

#define ERR_INOPT	"invalid option"
#define ERR_IVAL	"invalid value for"

/* TYPES */
typedef uint8_t (*check_ft)(const char *);
typedef void	(*act_ft)(void*, void*);

enum argtype
{
	BOOL_T,
	INT_T,
	STR_T
};

/* Struct to store expected arguments */
struct exparg
{
	const char		*name;	// Argument name
	char			shval;	// Short option value
	char			*lgval;	// Long option value
	enum argtype	 type;	// Type of argument
	uint8_t			found;	// Argument has been found
	struct exparg	*next;	// Pointer to next arg
};

union argval
{
	int			ival;
	const char	*pval;
};

/* Struct to store parsed arguments */
struct arg
{
	const char		*name;
	enum argtype	type;
	union argval	val;
	struct arg		*next;		// Pointer to next arg
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
int		arg(struct arg **args, const char *name, int type, union argval val);
int		exparg(char *name, char shval, char *lgval, int type);
void	arg_err(struct arg *arg, const char *msg);
struct	argparser	parse_args(const char **av);

extern struct exparg	*explist;

#endif
