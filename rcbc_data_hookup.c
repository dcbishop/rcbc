#include <assert.h>
#include "rcbc_data_hookup.h"
#include "console.h"

/**
 * Hookup deconstructor.
 **/
void Hookup_0Hookup(Hookup* hookup) {
	DEBUG_M("Entering function...");

	free(hookup->id);
	free(hookup);
	
	DEBUG_H("finished...");
	return;
}

/**
 * String class functions.
 */
static const ClassFunctions Hookup_c = {
	(void*)Hookup_0Hookup
};

/**
 * Allocates a hookup
 */
Hookup* Hookup_Hookup(char* id, void* pointer) {
	DEBUG_M("Entering function...");
	if(!id) {
		WARNING("Tried to generate NULL hookup.");
		return;
	}

	ALLOCATE(Hookup, hookup);
	hookup->class_ = &Hookup_c;
		
	int id_len = (strlen(id) + 1);
	hookup->id = malloc(id_len * sizeof(char));
	strncpy(hookup->id, id, id_len);
	hookup->ptr = pointer;
	hookup->hooked = 0;
	
	DEBUG_H("exiting function...");

	return hookup;
}

/**
 * Finds a hookup based on its id.
 */
Hookup* HookupFind(List* roothookup, char* id) {
	ListNode* node = roothookup->first;
	DEBUG(DEBUG_MEDIUM, "%sHookupFind(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YELLOW, id, COLOUR_LIGHT_BLUE);
	while(node) {
		DEBUG(DEBUG_VERY_HIGH, "\tCHECKING: '%s'", ((Hookup*)node->data)->id);
		if(node->data 
			&& strcasecmp(((Hookup*)node->data)->id, id) == 0)
		{
			DEBUG(DEBUG_HIGH, "\tFound matching node %s", SYMBOL_SMILEY);
			return node->data;
		}

		node = node->next;
	}

	return NULL;
}

/**
 * Prints a list of hookups for debugging
 */
void Hookup_Debug(List* rootnode) {
	ListNode* itr;
	Hookup* hookup;
	DEBUG(DEBUG_MEDIUM, "%sHookup_Debug", COLOUR_YELLOW);
	for(itr = rootnode->first; itr; itr = itr->next) {
		hookup = itr->data;
		DEBUG(DEBUG_VERY_HIGH, "ptr: %p, ID: '%s', pointer:%p", hookup, hookup->id, hookup->ptr);
	}
}

/**
 * Connects source data to sink pointers
 */
void Hookup_Execute(List* sources, List* sinks) {
	DEBUG_M("Entering function...");
	if(sources == NULL) {
		return;
	}
	assert(sources);
	assert(sinks);

	Hookup* source;
	Hookup* destination;
	ListNode* itr;

	DEBUG_V("\tLoop begiing...");
	for(itr = sources->first; itr; itr = itr->next) {
		DEBUG(DEBUG_VERY_HIGH, "\t\tLoop......");
		source = itr->data;
		if(!source) {
			continue;
		}
		DEBUG_V("\t\tsearching for '%s'...", source->id);
		destination = HookupFind(sinks, source->id);
		if(!destination) {
			#warning ['TODO']: Change error to a warning...
			ERROR("Hookup failed to find sink '%s'", source->id);
			continue;
		}
		DEBUG_H("\t\tfound '%s'...", source->id);
		if(!destination->ptr) {
			DEBUG(DEBUG_HIGH, "No sink in hookup...");
			continue;
		}
		source->hooked = 1;
		destination->hooked = 1;
		*destination->ptr = source->ptr;
	}
}

void Hookups_DeleteMissing(List* list) {
	Hookup* hookup;
	ListNode* itr;

	for(itr = list->first; itr; itr = itr->next) {
		hookup = itr->data;

		if(hookup->hooked == 0) {
			DEBUG_M("\tMissing hookup found.....");

			DELETE(hookup->ptr);
			hookup->ptr = NULL;
		}
	}
}
