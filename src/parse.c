/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:48:48 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/16 13:41:51 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

struct exparg	*get_exparg()
{
	struct exparg	*cur;

	cur = explist;
	while (cur)
	{
		if (!cur->shval && !cur->lgval &&!cur->found)
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

struct exparg	*get_expopt(const char *str)
{
	struct exparg	*cur;

	while (*str == '-')
	{
		++str;
	}
	cur = explist;
	while (cur)
	{
		if ((cur->lgval || cur->shval) &&
			(!strncmp(cur->lgval, str, strlen(cur->lgval)) ||
			*str == cur->shval))
		{
			break;
		}
		cur = cur->next;
	}
	return cur;
}

union argval	get_optval(int type, const char **av, int *i, size_t pos)
{
	union argval	val;
	const char		*sval;

	if (type == BOOL_T)
	{
		val.ival = 1;
		printf("[DEBUG] value = %d\n", val.ival);
		return val;
	}
	sval = av[*i][pos] ? av[*i] + pos : av[++(*i)];
	if (type == INT_T)
	{
		val.ival = atoi(sval);
		printf("[DEBUG] value = %d\n", val.ival);
	}
	else
	{
		val.pval = sval;
		printf("[DEBUG] value = %s\n", val.pval);
	}
	return val;
}

int	parse_shopt(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;

	for (int j = 1; av[*i][j]; ++j)
	{
		printf("[DEBUG] current option = %c\n", av[*i][j]);
		exp = get_expopt(&av[*i][j]);
		if (!exp)
		{
			return -1;
		}
		val = get_optval(exp->type, av, i, j + 1);
		arg(args, exp->name, exp->type, val);
		if (exp->type != BOOL_T)
		{
			return 0;
		}
	}
	return 0;
}

int	parse_arg(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;

	printf("[DEBUG] string = %s\n", av[*i]);
	printf("[DEBUG] is option = %d\n", IS_OPT(av[*i]));
	if (IS_OPT(av[*i]))
	{
		if (av[*i][1] != '-')
		{
			return parse_shopt(args, av, i);
		}
		else
		{
			exp = get_expopt(av[*i]);
			if (!exp)
			{
				return -1;
			}
		}
	}
	else
	{
		exp = get_exparg();
	}
	if (exp)
	{
		if (exp->lgval)
		{
			val = get_optval(exp->type, av, i, strlen(exp->lgval) + 2);
		}
		else
		{
			val.pval = av[*i];
			printf("[DEBUG] value = %s\n", val.pval);
		}
		return arg(args, exp->name, exp->type, val);
	}
	return 0;
}

struct argparser	parse_args(const char **av)
{
	struct argparser	p;

	p.args = NULL;
	p.err = 0;
	for (int i = 1; av[i]; ++i)
	{
		printf("%s\n", av[i]);
		p.err = parse_arg(&p.args, av, &i);
		if (p.err)
		{
			break;
		}
	}
	return p;
}
