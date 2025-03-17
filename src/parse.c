/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:48:48 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 17:12:11 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

extern struct exparg	*explist;

struct exparg	*get_exparg()
{
	struct exparg	*cur;

	cur = explist;
	while (cur)
	{
		if (!cur->shval && !cur->lgval &&!cur->found)
		{
			cur->found = true;
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

struct exparg	*get_expopt(const char *str)
{
	struct exparg	*cur;

	cur = explist;
	while (cur)
	{
		if ((cur->lgval || cur->shval) &&
			(!strncmp(cur->lgval, str, strlen(cur->lgval)) ||
			*str == cur->shval))
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

int	get_int_value(const char *str, bool *err)
{
	long	lval;
	char	*endptr;

	*err = false;
	errno = 0;
	lval = strtol(str, &endptr, 10);
	if (errno || *endptr != '\0' || str == endptr ||
		lval > (long)INT_MAX || lval < (long)INT_MIN)
	{
		*err = true;
	}
	return (int)lval;
}

union argval	get_optval(struct exparg *exp,
							const char **av, int *i, size_t pos)
{
	int				ival;
	const char		*sval;
	bool			err;

	if (exp->type == BOOL_T)
	{
		return (union argval)1;
	}
	sval = av[*i][pos] ? av[*i] + pos : av[++(*i)];
	if (!sval || !*sval)
	{
		arg_err("%s `%s`", PERR_REQARG, exp->name);
		return (union argval)-1;
	}
	if (exp->type == INT_T)
	{
		ival = get_int_value(sval, &err);
		if (err)
		{
			arg_err("%s for %s `%s`", PERR_IVAL, exp->name, sval);
			return (union argval)-1;
		}
		return (union argval)ival;
	}
	else
	{
		return (union argval)sval;
	}
}

int	parse_shopt(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;

	for (int j = 1; av[*i][j]; ++j)
	{
		exp = get_expopt(&av[*i][j]);
		if (!exp)
		{
			arg_err("%s '-%c'", PERR_INOPT, av[*i][j]);
			return -1;
		}
		val = get_optval(exp, av, i, j + 1);
		if (val.ival == -1)
		{
			return -1;
		}
		arg(args, exp->name, exp->type, val);
		if (exp->type != BOOL_T)
		{
			return 0;
		}
	}
	return 0;
}

int	parse_lgopt(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;
	size_t			pos;

	pos = 2;
	exp = get_expopt(av[*i] + pos);
	if (!exp)
	{
		arg_err("%s '--%s'", PERR_INOPT, av[*i] + pos);
		return -1;
	}
	pos += strlen(exp->lgval);
	if (av[*i][pos] == '=' || av[*i][pos] == ' ')
	{
		++pos;
	}
	val = get_optval(exp, av, i, pos);
	if (val.ival == -1)
	{
		return -1;
	}
	return arg(args, exp->name, exp->type, val);
}

int	parse_arg(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;

	if (IS_OPT(av[*i]))
	{
		if (av[*i][1] != '-')
		{
			return parse_shopt(args, av, i);
		}
		else
		{
			return parse_lgopt(args, av, i);
		}
	}
	else
	{
		exp = get_exparg();
		if (!exp)
		{
			return 0;
		}
		val.pval = av[*i];
	}
	return arg(args, exp->name, exp->type, val);
}

struct argparser	parse_args(const char **av)
{
	struct argparser	p;

	p.args = NULL;
	p.err = 0;
	for (int i = 1; av[i]; ++i)
	{
		p.err = parse_arg(&p.args, av, &i);
		if (p.err)
		{
			break;
		}
	}
	return p;
}
