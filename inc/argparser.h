/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:07:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/14 18:39:04 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define ERR_INOPT	"invalid option"

/* TYPES */
typedef uint8_t (*f)(const char *)	check_ft;
typedef void	(*f)(void*, void*)	act_ft;

struct arg
{
	int			id;			// Argument id
	uint8_t		opt;		// Argument is an option
	char		*shval;		// Short option value
	char		*lgval;		// Long option value
	enum
	{
		TYPE_INT,
		TYPE_PTR,
		TYPE_BOOL
	} arg_type;				// Type of option
	uint8_t		*err;		// Error message
	check_ft	check;		// Check function for argument value
	union
	{
		int		ival;		// Int value
		char	*sval;		// String value
	} val;
	#define ival	val.ival
	#define sval	val.sval
	act_ft		action;		// Action to perform
	void		*data;		// Data on which to perform the action
	struct arg	*next;		// Pointer to next arg
};

/* MACROS */
/* Check if string 's' is in option format */
#define IS_OPT(s)	((strncmp("--", s, 2) && strlen(s) > 2) || \
					(*s == '-' && s[1]))

/* FUNCTION DECLARATIONS */
struct arg	*add_arg(struct arg **list, struct arg *arg);

#endif
