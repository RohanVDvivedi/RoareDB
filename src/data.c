#include<data.h>
#include<jenkinshash.h>

Data* get_new_data(TypeOfData type, void* init_params)
{
	Data* data_p = (Data*)malloc(sizeof(Data));
	data_p->type = type;
	data_p->rwL  = get_rwlock();
	data_p->value = NULL;
	switch(type)
	{
		case NUM_DECIMAL :
		case STRING :
		{
			data_p->value = (void*)get_dstring(((char*)init_params), 0);
			break;
		}
		case NUM_FLOAT :
		{
			data_p->value = malloc(sizeof(double));
			break;
		}
		case NUM_INTEGER :
		{
			data_p->value = malloc(sizeof(long long int));
			break;
		}
		case TIME_STAMP :
		{
			data_p->value = malloc(sizeof(unsigned long long int));
			break;
		}
		default :
		{
			break;
		}
	}
	return data_p;
}

void transfer_data(Data* destination, Data* source)
{
	delete_data_contents(destination);
	(*destination) = (*source);
	source->value = NULL;
}

int compare_data(const Data* data_p1, const Data* data_p2)
{
	if(data_p1->type != data_p2->type)
	{
		return data_p1->type - data_p2->type;
	}
	switch(data_p1->type)
	{
		case NUM_DECIMAL :
		{
			return compare_decimal(((dstring*)(data_p1->value)), ((dstring*)(data_p2->value)));
		}
		case STRING :
		{
			return compare_dstring(((dstring*)(data_p1->value)), ((dstring*)(data_p2->value)));
		}
		case NUM_FLOAT :
		{
			return (*((double*)(data_p1->value))) - (*((double*)(data_p2->value)));
		}
		case NUM_INTEGER :
		{
			return (*((long long int*)(data_p1->value))) - (*((long long int*)(data_p2->value)));
		}
		case TIME_STAMP :
		{
			return (*((unsigned long long int*)(data_p1->value))) - (*((unsigned long long int*)(data_p2->value)));
		}
		default :
		{
			return 0;
		}
	}
}

unsigned long long int hash_data(const Data* data_p)
{
	switch(data_p->type)
	{
		case NUM_DECIMAL :
		case STRING :
		{
			return jenkins_hash(((dstring*)data_p)->cstring, ((dstring*)data_p)->bytes_occupied);
		}
		case NUM_FLOAT :
		{
			return jenkins_hash(data_p, sizeof(double));
		}
		case NUM_INTEGER :
		{
			return jenkins_hash(data_p, sizeof(long long int));
		}
		case TIME_STAMP :
		{
			return jenkins_hash(data_p, sizeof(unsigned long long int));
		}
		default :
		{
			return 0;
		}
	}
}

void serealize_data(dstring* destination, const Data* data_p)
{
	if(data_p == NULL)
	{
		append_to_dstring(destination, "NULL");
	}
	else
	{
		switch(data_p->type)
		{
			case NUM_DECIMAL :
			{
				append_to_dstring(destination, "NUM_DECIMAL(");
				concatenate_dstring(destination, ((dstring*)(data_p->value)));
				append_to_dstring(destination, ")");
				break;
			}
			case STRING :
			{
				append_to_dstring(destination, "STRING(");
				concatenate_dstring(destination, ((dstring*)(data_p->value)));
				append_to_dstring(destination, ")");
				break;
			}
			case NUM_FLOAT :
			{
				char num[50];sscanf(num, "NUM_FLOAT(%lf)", (*((double*)(data_p->value))));
				append_to_dstring(destination, num);
				break;
			}
			case NUM_INTEGER :
			{
				char num[50];sscanf(num, "NUM_INTEGER(%lld", (*((long long int*)(data_p->value))));
				append_to_dstring(destination, num);
				break;
			}
			case TIME_STAMP :
			{
				char num[50];sscanf(num, "TIME_STAMP(%llu)", (*((unsigned long long int*)(data_p->value))));
				append_to_dstring(destination, num);
				break;
			}
			default :
			{
				append_to_dstring(destination, "UNIDENTIFIED");
				break;
			}
		}
	}
}

void delete_data_contents(Data* data_p)
{
	if(data_p->value != NULL)
	{
		switch(data_p->type)
		{
			case NUM_DECIMAL :
			case STRING :
			{
				delete_dstring(((dstring*)(data_p->value)));
				break;
			}
			case NUM_FLOAT :
			case NUM_INTEGER :
			case TIME_STAMP :
			{
				free(data_p->value);
				break;
			}
			default :
			{
				break;
			}
		}
	}
	data_p->value = NULL;
}

void delete_data(Data* data_p)
{
	delete_data_contents(data_p);
	get_rwlock(data_p->rwL);
	free(data_p);
}