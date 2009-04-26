#include <assert.h>
#include "rcbc_data_hookup.h"
#include "console.h"

/**
 * Hookup deconstructor.
 * @param hookup The hookup to free.
 * \memberof Hookup
 **/
void Hookup_0Hookup(Hookup* hookup) {
	DEBUG_M("Entering function...");

	free(hookup->id);
	hookup->ptr = NULL;
	hookup->id = NULL;
	free(hookup);

	DEBUG_H("finished...");
	return;
}

/**
 * String class functions.
 * \memberof Hookup
 */
static const ClassFunctions Hookup_c = {
	(void*)Hookup_0Hookup
};

/**
 * Allocates a hookup
 * @param id The XML id.
 * @param pointer The pointer to the data, or the pointer to the place.
 * @return A pointer to a new Hookup or NULL on error.
 * \memberof Hookup
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
 * @param roothookup The head node of the List containing the Hookups.
 * @param id The XML id of the node to find.
 * @return The hookup if found or NULL.
 * \memberof Hookup
 */
Hookup* Hookup_Find(List* roothookup, char* id) {
	ListNode* node = roothookup->first;
	DEBUG_M("%sHookup_Find(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YELLOW, id, COLOUR_LIGHT_BLUE);
	while(node) {
		DEBUG_H("\tCHECKING: '%s'", ((Hookup*)node->data)->id);
		if(node->data 
			&& strcasecmp(((Hookup*)node->data)->id, id) == 0)
		{
			DEBUG_H("\tFound matching node %s", SYMBOL_SMILEY);
			return node->data;
		}

		node = node->next;
	}

	return NULL;
}

/**
 * Prints a list of hookups for debugging
 * @param rootnode Lost of nodes to print.
 * \memberof Hookup
 */
void Hookup_Debug(List* rootnode) {
	ListNode* itr;
	Hookup* hookup;
	DEBUG(DEBUG_MEDIUM, "%sHookup_Debug", COLOUR_YELLOW);
	for(itr = rootnode->first; itr; itr = itr->next) {
		hookup = itr->data;
		DEBUG(DEBUG_HIGH, "ptr: %p, ID: '%s', pointer:%p", hookup, hookup->id, hookup->ptr);
	}
}


/**
 * Links a source hookup data to the sinks pointer.
 * @param source The hookup with the pointer to the source data.
 * @param sink The hookup with the poitner to the data destiantion.
 * \memberof Hookup
 */
void Hookup_Execute_Link(Hookup* source, Hookup* destination) {
	DEBUG_H("Entering function...", source->id);
	if(!destination) {
		ERROR("No destination for node.");
		return;
	}
	
	if(!destination->ptr) {
		ERROR("No destination->ptr for node.");
		return;
	}
	source->hooked = 1;
	destination->hooked = 1;
	*destination->ptr = source->ptr;
}

/** 
 * Connects a single source data hookup to all matching sinks.
 * @param source The hookup to be hooked up.
 * @param sinks A List of sink hookups.
 * \memberof Hookup
 */
void Hookup_Execute(Hookup* source, List* sinks) {
	ListNode* node = sinks->first;
	DEBUG_M("%sHookup_Find(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YELLOW, source->id, COLOUR_LIGHT_BLUE);
	while(node) {
		DEBUG_H("\tCHECKING: '%s'", ((Hookup*)node->data)->id);
		if(node->data 
			&& strcasecmp(((Hookup*)node->data)->id, source->id) == 0)
		{
			DEBUG_H("\tFound matching node %s", SYMBOL_SMILEY);
			//return node->data;
			Hookup_Execute_Link(source, node->data);
		}

		node = node->next;
	}
}

/**
 * Connects all the source data to sink pointers.
 * For instance a jpeg filename with the material, and the material with
 * the triangle mesh.
 * @param sources The List containting source data to be hooked up.
 * @param sinks The List containing pointers to poitner to put the data.
 * \memberof Hookup
 */
void Hookups_Execute(List* sources, List* sinks) {
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

		Hookup_Execute(source, sinks);
	}
}

/**
 * Removes node form list.
 * @param node The node to remove from the secondary list.
 * @param sinks The sinks.
 * \memberof Hookup
 */
/*void HoopkUps_DitchSecondary(ListNode* node, List* sinks) {
	assert(node);
	assert(sinks);

	ListNode* itr = sinks->first;
	while(itr) {
		//Hookup* hookup = itr->data;
		if(itr->data == node->data) {
			DEBUG_H("Removing secondary...");
			List_DeleteNode(sinks, itr);
		}
		itr = itr->next;
	}
}*/


/**
 * DELETE's any hookup data that failed to find a sink from a List.
 * Also deletes the hookup itself and the list node.
 * @param list Pointer to the List head node containing the source hookups.
 * \memberof Hookup
 */
void Hookups_DeleteMissing(List* list, List* sinks) {
	DEBUG_M("Entering function...");
	Hookup* hookup;
	ListNode* itr;
	ListNode* self;

	//for(itr = list->first; itr; itr = itr->next) {
	itr = list->first;
	while(itr) {
		hookup = itr->data;
		self = itr;
		itr = itr->next;

		if(hookup->hooked == 0) {
			DEBUG_M("\tMissing hookup found.....");
			if(hookup->ptr) {

				// Don't delete Images as they are shared, but deref...
				if(isImage(hookup->ptr)) {
					DEBUG_H("\t\tIMAGE FOUND NOT DELETED YAY!");
					Image* image = hookup->ptr;
					image->refs--;
				} else {
					DELETE(hookup->ptr);
				}
				hookup->ptr = NULL;
			}
		}
#warning ['TODO'] Remember why this is done here and not in the if above
		DELETE(hookup);
		List_DeleteNode(list, self);
	}
	DEBUG_H("Exiting function...");
}

/**
 * Removes a refrence to an unused Image from a List of images.
 * Will also remove it from the other List if its there too.
 * @param images The list of Images.
 * @param sinks Another list that the Image will also be removed from.
 * \memberof Hookup
 */
/*void Hookups_DerefrenceMissingImages(List* images) {
	DEBUG_M("Entering function...");
	Hookup* hookup;
	Image* testimg = NEW(Image, "");
	ListNode* itr;
	ListNode* self;

	itr = images->first;
	while(itr) {
		hookup = itr->data;
		self = itr;
		itr = itr->next;

		if(hookup->hooked == 0) {
			Image* image = hookup->ptr;
			// Check to ensure its an image...
			//if(image->class_->deconstructor == testimg->class_->deconstructor) {
				DEBUG_M("\tUnused image found.....");
				//DELETE(hookup->ptr);
				//hookup->ptr = NULL;
				Image_DeRefrence(hookup->ptr);
				hookup->ptr = NULL;
				self->data = NULL;
				DELETE(hookup);
				List_DeleteNode(images, self);
			//}
		}
	}
	DELETE(testimg);
	DEBUG_H("Exiting...");
}*/
