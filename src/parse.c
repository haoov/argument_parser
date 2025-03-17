/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:48:48 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 18:39:48 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

struct arg	*get_arg(struct arg *args)
{
	struct arg	*cur;

	cur = args;
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

struct arg	*get_opt(struct arg *args, const char *str)
{
	struct arg	*cur;

	cur = args;
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

int	get_optval(struct arg *arg, const char **av, int *i, size_t pos)
{
	const char	*sval;
	bool		err;

	if (arg->type == BOOL_T)
	{
		arg->val.ival = true;
		return 0;
	}
	sval = av[*i][pos] ? av[*i] + pos : av[++(*i)];
	if (!sval || !*sval)
	{
		arg_err("%s `%s`", PERR_REQARG, arg->name);
		return -1;
	}
	if (arg->type == INT_T)
	{
		arg->val.ival = get_int_value(sval, &err);
		if (err)
		{
			arg_err("%s for %s `%s`", PERR_IVAL, arg->name, sval);
			return -1;
		}
	}
	else
	{
		arg->val.pval = sval;
	}
	return 0;
}

int	parse_shopt(struct arg **args, const char **av, int *i)
{
	struct arg	*arg;

	for (int j = 1; av[*i][j]; ++j)
	{
		arg = get_opt(*args, &av[*i][j]);
		if (!arg)
		{
			arg_err("%s '-%c'", PERR_INOPT, av[*i][j]);
			return -1;
		}
		if (get_optval(arg, av, i, j + 1) == -1)
		{
			return -1;
		}
		if (arg->type != BOOL_T)
		{
			return 0;
		}
	}
	return 0;
}

int	parse_lgopt(struct arg **args, const char **av, int *i)
{
	struct arg	*arg;
	size_t		pos;

	pos = 2;
	arg = get_opt(*args, av[*i] + pos);
	if (!arg)
	{
		arg_err("%s '--%s'", PERR_INOPT, av[*i] + pos);
		return -1;
	}
	pos += strlen(arg->lgval);
	if (av[*i][pos] == '=' || av[*i][pos] == ' ')
	{
		++pos;
	}
	if (get_optval(arg, av, i, pos) == -1)
	{
		return -1;
	}
	return 0;
}

int	parse_arg(struct arg **args, const char **av, int *i)
{
	struct arg	*arg;

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
		arg = get_arg(*args);
		if (!arg)
		{
			return 0;
		}
		arg->val.pval = av[*i];
	}
	return 0;
}

void	parse_args(struct argparser *p, const char **av)
{
	for (int i = 1; av[i]; ++i)
	{
		p->err = parse_arg(&p->args, av, &i);
		if (p->err)
		{
			break;
		}
	}
}

struct argparser	*new_parser()
{
	struct argparser	*parser;

	parser = calloc(1, sizeof(struct argparser));
	if (!parser)
	{
		return NULL;
	}
	return parser;
}
