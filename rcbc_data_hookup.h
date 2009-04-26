#ifndef RCBC_DATA_HOOKUP_H
#define RCBC_DATA_HOOKUP_H

#include "ooc.h"
#include "rcbc_data.h"

/**
 * \defgroup Hookup
 */
/*@{*/
/**
 * This is used to link between XML named id's and the actual data 
 * pointers.
 */
typedef struct Hookup {
	const ClassFunctions* class_; /**< Class functions. */
	char* id; /**< The XML id string. */
	
	/** 
	 * Either pointer to the data or a pointer to the pointer where 
	 * the data is intened to be linked to.
	 */
	void** ptr;
	
	/**
	 * 1 if it was successfully hooked up or 0 if not
	 */
	int hooked;
} Hookup;

Hookup* Hookup_Hookup(char* id, void* pointer);
void Hookup_0Hookup(Hookup* hookup);
Hookup* Hookup_Find(List* roothookup, char* id);
void Hookup_Debug(List* rootnode);
void Hookups_Execute(List* sources, List* sinks);
void Hookup_Execute(Hookup* source, List* sinks);
void Hookup_Execute_Link(Hookup* source, Hookup* destination);
void Hookups_DeleteMissing(List* list, List* sinks);

/*@{*/

#endif
