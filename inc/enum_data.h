#ifndef ENUM_DATA_H
#define ENUM_DATA_H

typedef struct Enumeration Enumeration;
struct Enumeration
{
	dstring* enum_type;
	uint8_t enum_value;
};

Enumeration* get_new_Enumeration();

void delete_Enumeration(Enumeration* enum_data_p);

#endif