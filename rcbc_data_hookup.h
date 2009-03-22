#ifndef RCBC_DATA_HOOKUP_H
#define RCBC_DATA_HOOKUP_H

#include "ooc.h"
#include "rcbc_data.h"

/**
 * This is used to link between XML named id's and the actual data 
 * pointers.
 */
typedef struct Hookup {
	const ClassFunctions* class;
	char* id; /**< The XML id string. */
	
	/** Either pointer to the data or a pointer to the pointer where 
	 * the data is intened to be linked to
	 */
	void** ptr;
	int hooked;
} Hookup;

Hookup* Hookup_Hookup(char* id, void* pointer);
Hookup* HookupFind(List* roothookup, char* id);
void HookupFree(List* roothookup);
void Hookup_Debug(List* rootnode);
void Hookup_Execute(List* sources, List* sinks);

#endif
