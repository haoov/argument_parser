/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:34 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/14 18:41:02 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

/* Return pointer to first empty argument in list 'list' 
 * or NULL if no empty arg was found
 * */
struct arg	*empty_arg(struct arg *list)
{
	while (list->next)
	{
		if (!list->opt && !list->sval)
		{
			return list;
		}
	}
	return NULL;
}

/* Return the arg corresponding to the option
 * or NULL if no arg was found
 * */
struct arg	*get_opt(struct arg *list, const char *val)
{
	while (list->next)
	{
		if (!strcmp(val, list->lgval) || *val == *list->shval)
		{
			return list;
		}
	}
	return NULL;
}

int	parse_arg(struct arg *list, const char *val)
{
	struct arg	*arg;

	arg = empty_arg(list);
	// To many argument where passed => ignore it
	if (!arg)
	{
		return 0;
	}
	arg->sval = val;
	if (arg->check && !arg->check(val))
	{
		fprintf(STDERR_FILENO, "%s: %s `%s`\n", __progname, arg->err, arg->val);
		return -1;
	}
}

int	get_opt_val(struct arg *opt, const char *str)
{
	if (opt->check && !opt->check(str))
	{
		return -1
	}
	switch (opt->type)
	{
		case TYPE_INT:
			opt->ival = atoi(str);
			break;
		case TYPE_PTR:
			opt->sval = str;
			break;
		default: break;
	}
	return 0;
}

int	parse_lgopt(struct arg *list, const char **argv, int *i)
{
	struct arg	*opt;
	size_t		optlen;
	int			err;

	opt = get_opt(list, argv[*i]);
	if (!opt)
	{
		fprintf(STDERR_FILENO, "%s: %s '--%s'\n",
				__progname, ERR_UNOPT, argv[*i]);
		return -1;
	}
	optlen = strlen(opt->lgval);
	if (*(argv[*i] + optlen))
	{

	}
}

int	parse_shopt(struct arg *list, const char **argv, int *i)
{
	struct arg	*opt;
	int			err;

	opt = get_opt(list, argv[*i]);
	while (opt)
	{
		if (opt->type == TYPE_BOOL)
		{
			opt->ival = 1;
			opt = get_opt(++argv[*i]);
		}
		else
		{
			if (argv[*i][1])
			{
				err = get_opt_val(opt, argv[*i] + 1);
			}
			else
			{
				*i++;
				err = get_opt_val(opt, argv[*i]);
			}
			if (err)
			{
				fprintf(STDERR_FILENO, "%s: %s `%s`\n", __progname,
						ERR_INVAL, 
				return -1;
			}
			return 0;
		}
	}
	fprintf(STDERR_FILENO, "%s: %s '-%s'\n", __progname, ERR_INOPT, argv[*i]);
	return -1;
}

int	parse_opt(struct arg *list, const char **argv, int *i)
{
	int			sh;
	struct arg	*opt;

	sh = argv[*i][1] != '-' ? 1 : 0;
	argv[*i] += sh ? 1 : 2;
	if (sh)
	{
		return parse_shopt(list, str, i);
	}
	else
	{
		return parse_lgopt(list, str, i);
	}
}

int	parse_argv(struct arg *list, const char **argv)
{
	for (int i = 0; argv[i]; ++i)
	{
		if (IS_OPT(argv[i]))
		{
			if (parse_opt(list, argv, &i) == -1)
			{
				return -1;
			}
		}
		else if (parse_arg(list, argv[i]) == -1)
		{
			return -1;
		}
		++i;
	}
	return check_args(list);
}
