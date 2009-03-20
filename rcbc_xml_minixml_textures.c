#include "rcbc_xml_minixml_textures.h"
#include "console.h"
#include "rcbc.h"
#include "rcbc_xml_minixml.h"


/* Process the <library_images> section of COLLADA */
void RCBC_MiniXML_ProcessTextureImages(ModelTempory *tempory, mxml_node_t *node) {
	DEBUG_M("Entering function...");
	if(!node) {
		return;
	}
	int debug = 0;

	const char* id;
	mxml_node_t* child;
	char* fullname = NULL;
	char* filename = NULL;
	char* rname = NULL;
	int rname_len = 0;

	/* Loop through all the image nodes */
	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT 
		&& strcasecmp(node->value.element.name, "image") == 0) {

			id = mxmlElementGetAttr(node, "id");

			/* Process the image filename */
			for(child = node->child; child != NULL; child = child->next) {
				if(child->type == MXML_ELEMENT 
				&& strcasecmp(child->value.element.name, "init_from") == 0) {

					fullname = child->child->value.opaque;
					/* Find the final path element since we want only the filename */
					filename = strrchr(fullname, '/')+1; /* Sane OSes path seperator */
					if(!filename) {
						filename = strrchr(fullname, '\\')+1; /* Or try for a windows style path seperator */
					}
					if(!filename) { /* If we didn't find a path seperator, assume filename only */
						rname = fullname;
					} else { /* Append texture directory name */
						rname_len = strlen(DIRECTORY_TEXTURES)+strlen(filename)+1;
						rname = malloc(rname_len * sizeof(char));
						snprintf(rname, rname_len, "%s%s", DIRECTORY_TEXTURES, filename);
					}

					Image* image = NEW(Image, rname);
					ListAdd(tempory->images, image);

					Hookup* hookup = NEW(Hookup, (char*)id, (void*)image);
					ListAdd(tempory->sources, hookup);
				}
			}

		}
	}
}

/* Process the <library_materials> section of COLLADA */
void RCBC_MiniXML_ProcessTextureMaterial(ModelTempory *tempory, mxml_node_t *node) {
	DEBUG_M("Entering function...");
	mxml_node_t* child;
	const char* id;
	const char* url;

	/* Loop through all the material nodes */
	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT && 
			strcasecmp(node->value.element.name, "material") == 0) {

			id = mxmlElementGetAttr(node, "id");

			for(child = node->child; child != NULL; child = child->next) {
				if(child->type == MXML_ELEMENT && 
					strcasecmp(child->value.element.name, "instance_effect") == 0) {
			
					url = mxmlElementGetAttr(child, "url");
					DEHASH(url);

					Hookup* material_hookup = NEW(Hookup, (char*)id, NULL);
					ListAdd(tempory->sources, material_hookup);
				
					Hookup* fx_hookup = NEW(Hookup, (char*)url, &material_hookup->ptr);
					ListAdd(tempory->sinks, fx_hookup);	
				}
			}
		
		}
	}
}

/* Process the <library_effects> section of COLLADA */
void RCBC_MiniXML_ProcessTextureEffects(ModelTempory *tempory, mxml_node_t *node) {
	DEBUG_M("Entering function...");
	const char* id;
	mxml_node_t* child;

	/* Loop through all the effect nodes */
	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT	&&
			strcasecmp(node->value.element.name, "effect") == 0) {

			id = mxmlElementGetAttr(node, "id");

			for(child = node->child; child != NULL; child = child->next) {
				if(child->type == MXML_ELEMENT &&
					strcasecmp(child->value.element.name, "profile_COMMON") == 0) {
					
					Hookup* fx_hookup =  NEW(Hookup, (char*)id, NULL);
					ListAdd(tempory->sources, fx_hookup);
					
					RCBC_MiniXML_ProcessTextureEffects_Profile(tempory, child, fx_hookup);
				}
			}

		}
	}
}

/* Process the <library_effects><effect><profile_COMMON> section of COLLADA */
void RCBC_MiniXML_ProcessTextureEffects_Profile(ModelTempory *tempory, mxml_node_t *node, Hookup* fx_hookup) {
	DEBUG_M("Entering function...");
	mxml_node_t* child;

	/* Loop through all the newparam nodes */
	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT
		&& strcasecmp(node->value.element.name, "newparam") == 0) {

			RCBC_MiniXML_ProcessTextureEffects_Newparam(tempory, node, fx_hookup);

		}
	}
}

#warning TODO: Remove unused, commeneted out code
/* Process the <library_effects><effect><profile_COMMON><newparam> section of COLLADA */
void RCBC_MiniXML_ProcessTextureEffects_Newparam(ModelTempory *tempory, mxml_node_t *node, Hookup* fx_hookup) {
	DEBUG_M("Entering function...");
	mxml_node_t* child;
	//const char* newparam_sid = mxmlElementGetAttr(node, "id");
	//const char* surface_type;
	const char* init_from;
	
	DumpNodeInfo(node);

	for(node = node->child; node != NULL; node = node->next) {
		DumpNodeInfo(node);
		if(node->type == MXML_ELEMENT
		&& strcasecmp(node->value.element.name, "surface") == 0) {
			
			for(child = node->child; child != NULL; child = child->next) {
				if(child->type == MXML_ELEMENT
				&& strcasecmp(child->value.element.name, "init_from") == 0) {

					//surface_type = mxmlElementGetAttr(child, "type");
					init_from = child->child->value.opaque;
					Hookup* img_hookup = NEW(Hookup, (char*)init_from, &fx_hookup->ptr);
					ListAdd(tempory->sinks, img_hookup);
				}
			}
				
		}
	}
}
