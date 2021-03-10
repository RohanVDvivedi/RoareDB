#include<cashed_query.h>

#include<cashed_command.h>

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

void init_query(c_query* query_p, c_command cmd)
{
	query_p->cmd = cmd;
	query_p->params_count = 0;
	query_p->params_capacity = 3;
	query_p->params = malloc(query_p->params_capacity * sizeof(dstring));
}

void serialize_query(dstring* str, c_query* query_p)
{
	serialize_command(query_p->cmd, str);
	concatenate_dstring(str, dstring_DUMMY_CSTRING("("));
	for(unsigned int i = 0; i < query_p->params_count; i++)
	{
		if(i > 0)
			concatenate_dstring(str, dstring_DUMMY_CSTRING(","));
		concatenate_dstring(str, query_p->params + i);
	}
	concatenate_dstring(str, dstring_DUMMY_CSTRING(");\r\n"));
}

void add_query_param(c_query* query_p, dstring* new_param)
{
	if(query_p->params_count == query_p->params_capacity)
	{
		query_p->params_capacity = 2 * query_p->params_capacity + 3;
		query_p->params = realloc(query_p->params, query_p->params_capacity * sizeof(dstring));
	}
	query_p->params[query_p->params_count++] = *new_param;
}

const dstring* get_query_param(const c_query* query_p, unsigned int index)
{
	if(index >= query_p->params_capacity)
		return NULL;
	return query_p->params + index;
}

void deserialize_query(dstring* str, c_query* query_p)
{
	int iter = 0;

	query_p->cmd = deserialize_command(str);

	if(query_p->cmd == ERR)
		return;

	iter += strlen(c_command_strings[query_p->cmd]);

	while(str->cstring[iter] != '(' && iter < str->bytes_occupied){iter++;}

	while(str->cstring[iter] != ')' && iter < str->bytes_occupied)
	{
		int start = ++iter;
		while(str->cstring[iter] != ',' && str->cstring[iter] != ')' && iter < str->bytes_occupied){iter++;}
		int end = iter;

		dstring* new_param = malloc(sizeof(dstring));
		init_dstring(new_param, str->cstring + start, end - start);

		add_query_param(query_p, new_param);

		free(new_param);
	}
}

void print_query(c_query* query_p)
{
	printf("%s\n", c_command_strings[query_p->cmd]);
	printf("params_count = %u\n", query_p->params_count);
	for(unsigned int i = 0; i < query_p->params_count; i++)
	{
		printf("\t");
		printf_dstring(query_p->params + i);
		printf("\n");
	}
}

void deinit_query(c_query* query_p)
{
	for(unsigned int i = 0; i < query_p->params_count; i++)
		deinit_dstring(query_p->params + i);
	free(query_p->params);
}