#include "argparser.h"
#include <ctype.h>
#include <stdbool.h>

uint8_t	isallnum(const char *s)
{
	while (*s)
	{
		if (!isdigit(*s))
		{
			return false;
		}
		++s;
	}
	return true;
}

int	main(int ac, const char **av)
{
	struct argparser	p;
	struct arg			*arg;

	exparg("host", 0, NULL, STR_T, NULL);
	exparg("verbose", 'v', "verbose", BOOL_T, NULL);
	exparg("help", '?', "help", BOOL_T, NULL);
	exparg("size", 's', "size", INT_T, isallnum);


	p = parse_args(av);

	arg = get_arg(p.args, "host");
	if (arg) {printf("host: %s\n", arg->val.pval);}
	arg = get_arg(p.args, "verbose");
	if (arg) {printf("verbose: %d\n", arg->val.ival);}
	arg = get_arg(p.args, "help");
	if (arg) {printf("help: %d\n", arg->val.ival);}
	arg = get_arg(p.args, "size");
	if (arg) {printf("size: %d\n", arg->val.ival);}

	free_args(p.args);
	return 0;
}
