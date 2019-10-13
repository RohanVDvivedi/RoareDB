#ifndef DATA_H
#define DATA_H

#include<rwlock.h>


typedef enum TypeOfData TypeOfData;
enum TypeOfData
{
	// simple data structures

	// array
	ARRAY,

	// stack
	STACK,

	// queue
	QUEUE,

	// linkedlist
	LINKED_LIST,

	// complex data structures, that store key value pair

	// balancedbst
	BINARY_SEARCH_TREE,

	// hashmap
	HASHTABLE,

	// heap
	HEAP,

	// Datatypes supported

	// dstring from cutlery
	STRING,

	// long long int
	NUM_INTEGER,

	// double
	NUM_FLOAT,

	// a number stored as string
	NUM_DECIMAL,

	// unsigned long long int, storing the nanoseconds since epoch
	TIME_STAMP,

	// this is of type unsigned character, it maps to a string stored in system tables
	ENUMERATION,

	// this is address to another data, it can be in any other node, and any where
	// yet to be defined properly
	POINTER,

	// code that can be executed
	PROGRAM
};

typedef struct Data Data;
struct Data
{
	// the type of the data, it stores
	TypeOfData type;

	// lock (reader writer lock)
	rwlock* rwL;

	// the pointer to the data
	void* value;
};

Data* get_new_simple_data(TypeOfData type);

void delete_simple_data(Data* data_p);

#endif