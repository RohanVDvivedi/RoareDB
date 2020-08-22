#include<cashed_expiry_manager.h>

#include<cashed_hashtable_def.h>

static void* expiry_manager_job_function(void* cem_v_p)
{
	c_expiry_manager* cem = cem_v_p;

	// run the job in while 1 loop, until someone calls exit
	while(1)
	{
		pthread_mutex_lock(&(cem->expiry_heap_lock));

		// remove all the elements one by one
		// check if they need to be removed, based on the current time
		// +0.005 or -0.005 seconds of error is allowed, since we only ensure to expiry within
		// seconds of said time, this is our precission

		// check the expiry time of the current top element

		// go to sleep until that time is about to occur

		pthread_mutex_unlock(&(cem->expiry_heap_lock));
	}

	return NULL;
}

static void expiry_heap_index_update_callback_function(const void* data_v_p, unsigned int index, const void* callback_params)
{
	c_data* data_p = (c_data*) data_v_p;
	data_p->expiry_heap_manager_index = index;
}

void init_expiry_heap(c_expiry_manager* cem, unsigned int min_element_count)
{
	pthread_mutex_init(&(cem->expiry_heap_lock), NULL);
	initialize_heap(&(cem->expiry_heap), min_element_count, MIN_HEAP, , expiry_heap_index_update_callback_function, NULL);
	initialize_job(&(cem->expiry_manager_job), expiry_manager_job_function, cem);
	pthread_cond_init(&(cem->conditional_wakeup_on_expiry), NULL);
}

void register_data_for_expiry(c_expiry_manager* cem, c_data* data_p)
{
	// check if the expiry_seconds is -1
	// then return

	pthread_mutex_lock(&(cem->expiry_heap_lock));

	// insert the data element that needs to be expired in future

	pthread_cond_signal(&(cem->conditional_wakeup_on_expiry));

	pthread_mutex_unlock(&(cem->expiry_heap_lock));
}

void deinit_expiry_heap(c_expiry_manager* cem)
{
	// ask the expiry_manager_job_function to die
	// wait for it to die

	deinitialize_heap(&(cem->expiry_heap));
	pthread_mutex_destroy(&(cem->expiry_heap_lock));
	pthread_cond_destroy(&(cem->conditional_wakeup_on_expiry));
}