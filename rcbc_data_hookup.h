#ifndef RCBC_DATA_HOOKUP_H
#define RCBC_DATA_HOOKUP_H

#include "rcbc_data.h"

/**
 * This is used to link between XML named id's and the actual data 
 * pointers.
 */
typedef struct RCBC_Hookup {
	char* id; /**< The XML id string. */
	
	/** Either pointer to the data or a pointer to the pointer where 
	 * the data is intened to be linked to
	 */
	void** ptr;
} RCBC_Hookup;

RCBC_Hookup* RCBC_HookupGenerate(char* id, void* pointer);
RCBC_Hookup* RCBC_HookupFind(LL* roothookup, char* id);
void RCBC_HookupFree(LL* roothookup);
void RCBC_Hookup_Debug(LL* rootnode);
void RCBC_Hookup_Execute(LL* sources, LL* sinks);

#endif
