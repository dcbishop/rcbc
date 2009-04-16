/* Contains COLLADA data scructures */
#ifndef _RCBC_DATA_DEF
#define _RCBC_DATA_DEF

#include <GL/gl.h>

#include "List.h"
#include "rcbc_data_hookup.h"

#define X_UP 0
#define Y_UP 1
#define Z_UP 2

/**
 * A container for a COLLADA model.
 */
typedef struct Model {
	const ClassFunctions* class_;	/**< Class functions. */
	struct SceneNode* visual_scene;	/**< The COLLADA visual scene */
	struct List* geometries;		/**< All the geometry mesh data */
} Model;

/**
 * A throwaway struct for holding tempory values while loading a model.
 * After processing the COLLADA model into OpenGL these need to be cleaned.
 */
typedef struct ModelTempory {
	const ClassFunctions* class_;	/**< Class functions. */
	Model* model;					/**< The model */
	List* sources;					/**< Sources of data to hookup*/
	List* sinks;					/**< A sink in which to plug the source */
	List* unsorted;					/**< Unsorted Triangles that need sorting */
	List* images;					/**< The list of texture images */
	List* freeme;					/**< misc data that needs a free() */
	List* deleteme;					/**< misc data that needs a DELETE() */
	int up_axis;					/**< The COLLADA axis direction */
} ModelTempory;

/**
 *  Contains rotation information for a COLLADA scene node.
 **/
typedef struct Rotate {
	const ClassFunctions* class_;	/**< Class functions. */
	float x;						/**< OpenGL X rotation */
	float y;						/**< OpenGL Y rotation */
	float z;						/**< OpenGL Z rotation */
	float angle;					/**< OpenGL rotation angle */
} Rotate;

/**
 * An array of floats.
 */
typedef struct FloatArray {
	const ClassFunctions* class_;	/**< Class functions. */
	int count;						/**< Number of floats in the array */
	GLfloat* values;				/**< Pointer to the actual floats. */
} FloatArray;

/** 
 * Contains a filename for a image, an id for opengl texture binding 
 * and the number of refrences to this image.
 */
typedef struct Image {
	const ClassFunctions* class_;	/**< Class functions. */
	int id;							/**< Unique ID used by image library */
	char* filename;					/**< The image filename */
	int refs;						/**< Number of times this image is in use */
} Image;

/**
 * Contains unprocessed triangle data as it is read from the COLLADA
 * file.
 */
typedef struct UnsortedTriangles {
	const ClassFunctions* class_;	/**< Class functions. */
	void** ptr;						/**< Pointer to the sorted triangle struct. */
	unsigned int count;				/**< Number of trinagles. */
	unsigned int inputs;			/**< Number of inputs for vertex info. */
	unsigned int* indices;			/**< Index numbers. */
	int vertices_offset;			/**< The vertexs offset in the interlaces index */
	FloatArray* vertices;			/**< The vertices cordinates */
	int normals_offset;				/**< The normals offset in the interlaces index */
	FloatArray* normals;			/**< The normals cordinates */
	int texcoords_offset;			/**< The textcoords offset in the interlaces index */
	FloatArray* texcoords;			/**< The texture cordinates */
	Image* image;					/**< The texture image */
} UnsortedTriangles;

/**
 * A bunch of unsorted polygons.
 */
typedef struct UnsortedPolygons {
	const ClassFunctions* class_;	/**< Class functions. */
	void** ptr;						/**< Pointer to the sorted polygon struct. */
	unsigned int count;				/**< Number of polygons */
	unsigned int inputs;			/**< Number of inputs */
} UnsortedPolygons;

/**
 * Triangle data for a mesh.
 */
typedef struct Triangles {
	const ClassFunctions* class_;	/**< Class functions. */
	unsigned int count;				/**< Number of trinagles. */
	FloatArray* vertices;			/**< The vertex cordinates. */
	FloatArray* normals;			/**< The normal cordinates. */
	FloatArray* texcoords;			/**< The texture cordinates. */
	Image* image;					/**< Texture image. */
} Triangles;

/** 
 * Contains model mesh data.
 **/
typedef struct Mesh {
	const ClassFunctions* class_;	/**< Class functions. */
	Triangles* triangles;			/**< Triangle model data. */
} Mesh;

/**
 * A COLLADA scene node.
 */
typedef struct SceneNode {
	const ClassFunctions* class_;	/**< Class functions. */

	Mesh* mesh;						/**< Mesh model. */

	float translate[3];				/**< Spacial cordinates. */
	float scale[3];					/**< Model scale. */

	struct List* rotations;			/**< Rotations to apply. */

	struct SceneNode* next;			/**< Next sibbling node in tree. */
	struct SceneNode* prev;			/**< Previous sibbling node in the tree. */
	struct SceneNode* child;		/**< Head child node */
	struct SceneNode* parent;		/**< The parent node */
} SceneNode;


void Model_0Model(Model* model);
Model* Model_Model();

void ModelTempory_0ModelTempory(ModelTempory* tempory);
ModelTempory* ModelTempory_ModelTempory();

void Rotate_0Rotate(Rotate* rotate);
Rotate* Rotate_Rotate();

void SceneNode_0SceneNode(SceneNode *node);
SceneNode* SceneNode_SceneNode();
void SceneNodeDebugInfo(SceneNode* node);

void Mesh_0Mesh(Mesh *mesh);
Mesh* Mesh_Mesh();

void FloatArray_0FloatArray(FloatArray* array);
FloatArray* FloatArray_FloatArray(int count);

void UnsortedTriangles_0UnsortedTriangles(UnsortedTriangles* triangles);
UnsortedTriangles* UnsortedTriangles_UnsortedTriangles(int count);
int UnsortedTrianglesAllocateIndices(UnsortedTriangles* triangles);

void Image_0Image(Image* image);
Image* Image_Image(char* filename);
Image* Image_FindByName(List* images, char* filename);
Image* Image_Add(List* images, char* filename, int refs);

void Triangles_0Triangles(Triangles* triangles);
Triangles* Triangles_Triangles(int count);


#endif
