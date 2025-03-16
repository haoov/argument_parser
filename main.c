#include "argparser.h"

int	main(int ac, const char **av)
{
	struct argparser	p;

	exparg("host", 0, NULL, STR_T);
	exparg("verbose", 'v', "verbose", BOOL_T);
	exparg("help", '?', "help", BOOL_T);
	exparg("size", 's', "size", INT_T);


	p = parse_args(av);

	struct arg	*cur = p.args;

	while (cur)
	{
		printf("argument %s:\n", cur->name);
		if (cur->type == BOOL_T)
		{
			printf("\tyes\n");
		}
		if (cur->type == INT_T)
		{
			printf("\tint value: %d\n", cur->val.ival);
		}
		else if (cur->type == STR_T)
		{
			printf("\tstring value: %s\n", cur->val.pval);
		}
		printf("\n");
		cur = cur->next;
	}

	return 0;
}
