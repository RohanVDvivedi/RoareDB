#ifndef COMMAND_H
#define COMMAND_H

typedef enum command command;
enum command
{
	// for the main hash table and intenal hashes, bsts
	GET,
	SET,
	DEL,

	// for internal queues, stacks and heaps
	PUSH,
	POP,

	// for internal hashmaps, balanced bsts, bucketed linkedlist
	INSERT,
	SEARCH,
	UPDATE,
	DELETE,

	// for numbers
	INCREMENT_BY_1,
	DECREMENT_BY_1,

	// for string
	APPEND_WITH,

	// crerate a data structure type for your purpose
	NEW,

	// this command can execute a PROGRAM datatye, and a result is returned to you
	EXEC,

	// to exit the connection
	EXIT
};

#endif