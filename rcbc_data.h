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
 * @see Model_Model()
 * @see Model_0Model()
 */
typedef struct Model {
	const ClassFunctions* class_;	/**< Class functions. */
	struct SceneNode* visual_scene;	/**< The COLLADA visual scene */
	struct List* geometries;		/**< All the geometry mesh data */
} Model;

void Model_0Model(Model* model);
Model* Model_Model();




/**
 * A throwaway struct for holding tempory values while loading a model.
 * After processing the COLLADA model into OpenGL these need to be cleaned.
 * @see ModelTempory_ModelTempory()
 * @see ModelTempory_0ModelTempory()
 */
typedef struct ModelTempory {
	const ClassFunctions* class_;	/**< Class functions. */
	Model* model;					/**< The model */
	List* sources;					/**< Sources of data to hookup */
	//List* image_sources;			/**< Sources of image data to hookup */
	List* sinks;					/**< A sink in which to plug the source */
	List* unsorted;					/**< Unsorted Triangles that need sorting */
	List* images;					/**< The list of texture images */
	
	List* freeme;					/**< misc data that needs a free() */
	List* deleteme;					/**< misc data that needs a DELETE() */
	int up_axis;					/**< The COLLADA axis direction */
} ModelTempory;

void ModelTempory_0ModelTempory(ModelTempory* tempory);
ModelTempory* ModelTempory_ModelTempory();




/**
 * Contains rotation information for a COLLADA scene node.
 * @see Rotate_Rotate()
 * @see Rotate_0Rotate()
 **/
typedef struct Rotate {
	const ClassFunctions* class_;	/**< Class functions. */
	float x;						/**< OpenGL X rotation */
	float y;						/**< OpenGL Y rotation */
	float z;						/**< OpenGL Z rotation */
	float angle;					/**< OpenGL rotation angle */
} Rotate;

void Rotate_0Rotate(Rotate* rotate);
Rotate* Rotate_Rotate();




/**
 * An array of floats.
 * @see FloatArray_FloatArray()
 * @see FloatArray_0FloatArray()
 */
typedef struct FloatArray {
	const ClassFunctions* class_;	/**< Class functions. */
	int count;						/**< Number of floats in the array */
	GLfloat* values;				/**< Pointer to the actual floats. */
} FloatArray;

void FloatArray_0FloatArray(FloatArray* array);
FloatArray* FloatArray_FloatArray(int count);



/** 
 * Contains a filename for a image, an id for opengl texture binding 
 * and the number of refrences to this image.
 * @see Image_Image()
 * @see Image_0Image()
 * @see Image_FindByName()
 * @see Image_Add()
 */
typedef struct Image {
	const ClassFunctions* class_;	/**< Class functions. */
	int id;							/**< Unique ID used by image library */
	char* filename;					/**< The image filename */
	int refs;						/**< Number of times this image is in use */
} Image;

Image* Image_Image(char* filename);
void Image_0Image(Image* image);
Image* Image_FindByName(List* images, char* filename);
Image* Image_Add(List* images, char* filename, int refs);



/**
 * Contains unprocessed triangle data as it is read from the COLLADA
 * file.
 * @see UnsortedTriangles_UnsortedTriangles()
 * @see UnsortedTriangles_0UnsortedTriangles()
 * @see UnsortedTrianglesAllocateIndices()
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

UnsortedTriangles* UnsortedTriangles_UnsortedTriangles(int count);
void UnsortedTriangles_0UnsortedTriangles(UnsortedTriangles* triangles);
int UnsortedTrianglesAllocateIndices(UnsortedTriangles* triangles);



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
 * @see Triangles_Triangles()
 * @see Triangles_0Triangles()
 */
typedef struct Triangles {
	const ClassFunctions* class_;	/**< Class functions. */
	unsigned int count;				/**< Number of trinagles. */
	FloatArray* vertices;			/**< The vertex cordinates. */
	FloatArray* normals;			/**< The normal cordinates. */
	FloatArray* texcoords;			/**< The texture cordinates. */
	Image* image;					/**< Texture image. */
} Triangles;

void Triangles_0Triangles(Triangles* triangles);
Triangles* Triangles_Triangles(int count);



/** 
 * Contains model mesh data.
 * @see Mesh_Mesh()
 * @see Mesh_0Mesh()
 **/
typedef struct Mesh {
	const ClassFunctions* class_;	/**< Class functions. */
	Triangles* triangles;			/**< Triangle model data. */
} Mesh;

void Mesh_0Mesh(Mesh *mesh);
Mesh* Mesh_Mesh();


/**
 * A COLLADA scene node.
 * @see SceneNode_SceneNode()
 * @see SceneNode_0SceneNode()
 * @see SceneNodeDebugInfo()
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

void SceneNode_0SceneNode(SceneNode *node);
SceneNode* SceneNode_SceneNode();
void SceneNodeDebugInfo(SceneNode* node);


#endif /* _RCBC_DATA_DEF */
