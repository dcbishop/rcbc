#include <assert.h>
#include "rcbc_data_hookup.h"
#include "console.h"
/**
 * Allocates a hookup
 */
RCBC_Hookup* RCBC_HookupGenerate(char* id, void* pointer) {
	DEBUG(DEBUG_MEDIUM, "Entering function...");
	if(!id) {
		WARNING("Tried to generate NULL hookup.");
		return;
	}

	RCBC_Hookup* hookup = malloc(sizeof(RCBC_Hookup));
	if(!hookup) {
		ERROR("Failed to allocate space for hookup.");
		return NULL;
	}

	DEBUG_H("Values: ID:'%s', pointer:%p", id, pointer);

	hookup->id = id;
	hookup->ptr = pointer;
	DEBUG_H("exiting function...");

	return hookup;
}

/**
 * Finds a hookup based on its id.
 */
RCBC_Hookup* RCBC_HookupFind(LL* roothookup, char* id) {
	LLNode* node = roothookup->first;
	DEBUG(DEBUG_MEDIUM, "%sRCBC_HookupFind(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YELLOW, id, COLOUR_LIGHT_BLUE);
	while(node) {
		DEBUG(DEBUG_VERY_HIGH, "\tCHECKING: '%s'", ((RCBC_Hookup*)node->data)->id);
		if(node->data 
			&& strcasecmp(((RCBC_Hookup*)node->data)->id, id) == 0)
		{
			DEBUG(DEBUG_HIGH, "\tFound matching node %s", SYMBOL_SMILEY);
			return node->data;
		}

		node = node->next;
	}

	return NULL;
}

/**
 * Recursivly frees all the hookups.
 **/
void RCBC_HookupFree(LL* roothookup) {
	#warning TODO: Free memory here...
	DEBUG(DEBUG_MEDIUM, "%sRCBC_HookupFree", COLOUR_LIGHT_BLUE);
	LLNode* node_ptr = roothookup->first;
	DEBUG(DEBUG_HIGH, "\tLooping through each hookup...");
	while(node_ptr) {
		DEBUG(DEBUG_VERY_HIGH, "\t\tloop...");
		if(node_ptr->data) {
			//free(((RCBC_Hookup*)node_ptr->data)->id); #warning TODO: free hookups
			//free(node_ptr->data);
			DEBUG(DEBUG_VERY_HIGH, "\t\tNode has data...");
			node_ptr->data = NULL;
		}
		node_ptr = node_ptr->next;
	}

	DEBUG(DEBUG_LOW, "RCBC_HookupFree finish...");
	LLFree(roothookup);
	return;
}

/**
 * Prints a list of hookups for debugging
 */
void RCBC_Hookup_Debug(LL* rootnode) {
	LLNode* itr;
	RCBC_Hookup* hookup;
	DEBUG(DEBUG_MEDIUM, "%sRCBC_Hookup_Debug", COLOUR_YELLOW);
	for(itr = rootnode->first; itr; itr = itr->next) {
		hookup = itr->data;
		DEBUG(DEBUG_VERY_HIGH, "ptr: %p, ID: '%s', pointer:%p", hookup, hookup->id, hookup->ptr);
	}
}

/**
 * Connects source data to sink pointers
 */
void RCBC_Hookup_Execute(LL* sources, LL* sinks) {
	DEBUG_M("Entering function...");
	if(sources == NULL) {
		return;
	}
	assert(sources);
	assert(sinks);

	RCBC_Hookup* source;
	RCBC_Hookup* destination;
	LLNode* itr;

	DEBUG_V("\tLoop begiing...");
	for(itr = sources->first; itr; itr = itr->next) {
		DEBUG(DEBUG_VERY_HIGH, "\t\tLoop......");
		source = itr->data;
		if(!source) {
			continue;
		}
		DEBUG_V("\t\tsearching for '%s'...", source->id);
		destination = RCBC_HookupFind(sinks, source->id);
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
