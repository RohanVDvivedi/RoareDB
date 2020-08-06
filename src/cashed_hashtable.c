#include<cashed_hashtable.h>

#include<jenkinshash.h>

void init_hashbucket(hashbucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

void deinit_hashbucket(hashbucket* bucket)
{
	deinitialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

void init_hashtable(hashtable* hashtable_p, unsigned int bucket_count)
{
	hashtable_p->bucket_count = bucket_count;
	pthread_mutex_init(&(hashtable_p->data_count_lock), NULL);
	hashtable_p->data_count = 0;
	hashtable_p->hashbuckets = calloc(hashtable_p->bucket_count, sizeof(hashbucket));
	for(unsigned int i = 0; i < hashtable_p->bucket_count; i++)
		init_hashbucket(hashtable_p->hashbuckets + i);
}

int get_hashtable(hashtable* hashtable_p, const dstring* key, dstring* return_value)
{
	unsigned int index = jenkins_hash_dstring(key) % hashtable_p->bucket_count;
	hashbucket* bucket = hashtable_p->hashbuckets + index;
	read_lock(&(bucket->data_list_lock));
		data* data_test = bucket->data_list;
		while(data_test != NULL && compare_key(data_test, key) != 0){}
		if(data_test != NULL)
			read_lock(&(data_test->data_value_lock));
	read_unlock(&(bucket->data_list_lock));
	if(data_test != NULL)
	{
		append_data_value(data_test, return_value);
		read_unlock(&(data_test->data_value_lock));

		// bump data_text in the LRU
	}
	return data_test != NULL;
}

int set_hashtable(hashtable* hashtable_p, const dstring* key, const dstring* value)
{
	unsigned int index = jenkins_hash_dstring(key) % hashtable_p->bucket_count;
	hashbucket* bucket = hashtable_p->hashbuckets + index;
	
	write_lock(&(bucket->data_list_lock));
		data* data_test = bucket->data_list;
		data* prev = NULL;
		while(data_test != NULL && compare_key(data_test, key) != 0){prev = data_test;}
		if(data_test != NULL)
		{
			if(data_test->value_size < value->bytes_occupied-1)
			{
				if(prev != NULL)
					prev->hashtable_next = data_test->hashtable_next;
				else
					bucket->data_list = data_test->hashtable_next;
				data_test->hashtable_next = NULL;

				// remove data_test from the LRU
				// deallocate data_test

				// allocate new data and insert it into
			}
			else
			{
				write_lock(&(data_test->data_value_lock));
					update_value(data_test, value);
				write_unlock(&(data_test->data_value_lock));
			}
		}
		else
		{
			// allocate new data and insert it into
		}
	write_unlock(&(bucket->data_list_lock));

	return data_test != NULL;
}

int del_hashtable(hashtable* hashtable_p, const dstring* key)
{
	unsigned int index = jenkins_hash_dstring(key) % hashtable_p->bucket_count;
	hashbucket* bucket = hashtable_p->hashbuckets + index;

	write_lock(&(bucket->data_list_lock));
		data* data_test = bucket->data_list;
		data* prev = NULL;
		while(data_test != NULL && compare_key(data_test, key) != 0){prev = data_test;}
		if(data_test != NULL)
		{
			if(prev != NULL)
				prev->hashtable_next = data_test->hashtable_next;
			else
				bucket->data_list = data_test->hashtable_next;
			data_test->hashtable_next = NULL;
		}
	write_unlock(&(bucket->data_list_lock));

	if(data_test != NULL)
	{
		// remove data_test from LRU
		// and deallocate data_test
	}

	return data_test != NULL;
}

void deinit_hashtable(hashtable* hashtable_p, unsigned int bucket_count)
{
	pthread_mutex_destroy(&(hashtable_p->data_count_lock));
	if(hashtable_p->hashbuckets != NULL)
	{
		for(unsigned int i = 0; i < hashtable_p->bucket_count; i++)
			deinit_hashbucket(hashtable_p->hashbuckets + i);
		free(hashtable_p->hashbuckets);
		hashtable_p->hashbuckets = NULL;
	}
}
