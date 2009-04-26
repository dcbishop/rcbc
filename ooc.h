#ifndef	SOOC_H
#define	SOOC_H

/* A safer malloc */
#define CALLOC(ptr,size) if(((ptr) = calloc(1, size)) == NULL) { ERROR("Failed to allocate memory."); return NULL; }
#define ALLOCATE(type, name) type* name; CALLOC(name, sizeof(type));

#define NEW(x, ...) x##_##x(__VA_ARGS__)
#define DELETE(x) if(x && ((ClassArchetype*)x)->class_ && ((ClassArchetype*)x)->class_->deconstructor) { ((ClassArchetype*)x)->class_->deconstructor(x); }

/**
 * Holds functions bindings for a class.
 */
/**\cond */
typedef struct ClassFunctions_s {
	void (*deconstructor)(void* obj);	/**< Pointer to the deconstructor function. */
} ClassFunctions;
/**\endcond */

/**
 * A generic class type for type casting.
 */
 
typedef struct ClassArchetype_s {
	ClassFunctions* class_;				/**< Class functions. */
} ClassArchetype;

#endif
