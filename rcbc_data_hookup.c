#include <assert.h>
#include "rcbc_data_hookup.h"
#include "console.h"

/**
 * Hookup deconstructor.
 **/
void Hookup_0Hookup(List* roothookup) {
	#warning TODO: Free memory here...
	DEBUG(DEBUG_MEDIUM, "%sHookupFree", COLOUR_LIGHT_BLUE);
	ListNode* node_ptr = roothookup->first;
	DEBUG(DEBUG_HIGH, "\tLooping through each hookup...");
	while(node_ptr) {
		DEBUG(DEBUG_VERY_HIGH, "\t\tloop...");
		if(node_ptr->data) {
			//free(((Hookup*)node_ptr->data)->id); #warning TODO: free hookups
			//free(node_ptr->data);
			DEBUG(DEBUG_VERY_HIGH, "\t\tNode has data...");
			node_ptr->data = NULL;
		}
		node_ptr = node_ptr->next;
	}

	DEBUG(DEBUG_LOW, "HookupFree finish...");
	DELETE(roothookup);
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
	DEBUG(DEBUG_MEDIUM, "Entering function...");
	if(!id) {
		WARNING("Tried to generate NULL hookup.");
		return;
	}

	ALLOCATE(Hookup, hookup);
	
	hookup->class = &Hookup_c;

	DEBUG_H("Values: ID:'%s', pointer:%p", id, pointer);

	hookup->id = id;
	hookup->ptr = pointer;
	DEBUG_H("exiting function...");

	return hookup;
}

/**
 * Finds a hookup based on its id.
 */
Hookup* HookupFind(List* roothookup, char* id) {
	ListNode* node = roothookup->first;
	DEBUG(DEBUG_MEDIUM, "%sHookupFind(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YEListOW, id, COLOUR_LIGHT_BLUE);
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
	DEBUG(DEBUG_MEDIUM, "%sHookup_Debug", COLOUR_YEListOW);
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
			#warning TODO: Change error to a warning...
			ERROR("Hookup failed to find sink '%s'", source->id);
			continue;
		}
		DEBUG_H("\t\tfound '%s'...", source->id);
		if(!destination->ptr) {
			DEBUG(DEBUG_HIGH, "No sink in hookup...");
			continue;
		}
		*destination->ptr = source->ptr;
	}
}
