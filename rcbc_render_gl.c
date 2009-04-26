#include <stdio.h>
#define DO_OPENGL
#ifndef _SKIPGL
#include <GL/gl.h>
#include <IL/il.h>
#endif

#include "rcbc_render_gl.h"
#include "console.h"
#include "shaders.h"

#define PI 3.14152f

/**
 * Initilize the GL render and DevIL library...
 */
int RCBC_GL_Init() {
#ifndef _SKIPGL
	LOG("Initilizing GL render...");

	// Ensure DevIL is correct version...
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
			ERROR("DevIL version is different...exiting!\n");
//			BREAK();
			return 1;
	}

	// Initilize DevIL
	ilInit();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(10000.0f);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
#endif
	return 0;
}

#warning ['TODO']: Optimize me :)
/**
 * Draws a SceneNode.
 * @param node The node to draw.
 */
void RCBC_GL_Draw_Node(SceneNode* node) {
	#ifndef _SKIPGL
	DEBUG_V("Entering function...");

	if(!node) {return;}
	ListNode* itr;

#warning ['TODO']: This probably shouldnt be here
	/*static ShaderProgram* phong = NULL;
	if(!phong) {
		phong = load_phong_shader();
	}
	
	if(phong) {
		glUseProgram(phong->program);
	}*/
	
	
	// Translate node
	glTranslatef(node->translate[0], node->translate[1], node->translate[2]);
	glPushMatrix();

	// Apply rotations...
	for(itr = node->rotations->first; itr; itr=itr->next) {
		Rotate* rotation = itr->data;
		glRotatef(rotation->angle, rotation->x, rotation->y, rotation->z);
	}

	// Scale
	glScalef(node->scale[0], node->scale[1], node->scale[2]);

	#warning ['TODO']: Load this onto the graphics card for increased speed...
	// Mesh data
	Mesh* mesh = node->mesh;
	Triangles* triangles;
	if(!mesh || !(triangles = mesh->triangles)) { // If there is node mesh data
		//#warning ['TODO']: Dont draw a red sphere...
		//glColor3f(1.0f, 0.0f, 0.0f);
		//glutWireSphere(1.0f, 5, 5); // Draw a red sphere*/
	} else {
		// Bind vertex data...
		if(triangles->vertices) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, triangles->vertices->values);
		} else {
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		
		// Bind normal data...
		if(triangles->normals) {
			glNormalPointer(GL_FLOAT, 0, triangles->normals->values);
			glEnableClientState(GL_NORMAL_ARRAY);
		} else {
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		// Bind texture cordinates...
		if(triangles->texcoords) {	
			glTexCoordPointer(2, GL_FLOAT, 0, triangles->texcoords->values);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		} else {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		// Check for a image for this mesh
		if(triangles->image) {
			// If the image hasn't been loaded, we do it now
			if(triangles->image->id == 0) {
				/*triangles->image->id = SOIL_load_OGL_texture(
					triangles->image->filename,
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				);*/
				
				// Use DevIL to load the image and bind it to the texture id.
				ILuint texid = 0;
				ilGenImages(1, &texid);
				ilBindImage(texid);
				if(ilLoadImage(triangles->image->filename)) {
					glGenTextures(1, &triangles->image->id);
					glBindTexture(GL_TEXTURE_2D, triangles->image->id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
						ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
						ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
				}
				ilDeleteImages(1, &texid);
				
				// If the image failed to load, we throw an error and set it to -1 so it won't load again
				if(triangles->image->id == 0) {
					triangles->image->id = -1;
					//ERROR("Failed to load texture: '%s', %s", triangles->image->filename, SOIL_last_result());
					ERROR("Failed to load texture: '%s', %s", triangles->image->filename, "");
					//ERROR("DevIL says: '%d'", ilGetError());
				}
				DEBUG_M("Loaded texture: '%s'", triangles->image->filename);
			}
			
			// If we have a valid texture id, bind it
			if(triangles->image->id != 0) {
				glBindTexture(GL_TEXTURE_2D, triangles->image->id);
				glEnable(GL_TEXTURE_2D);
			}
		} else { // If this mesh doesn't use a texture
			glDisable(GL_TEXTURE_2D);
			//return;
		}
		
		#warning ['TODO']: Debugging
		glEnable ( GL_LIGHTING ) ;
		glEnable(GL_LIGHT0);
		//glEnable(GL_COLOR_MATERIAL);
		//glColor3f(1.0f, 0.0f, 0.0f);
		//glDisable(GL_TEXTURE_2D);
		
		// Draw it, yay!
		glDrawArrays(GL_TRIANGLES, 0, triangles->count * 3);
	}

	glPopMatrix();
#endif
}

/**
 * Draws all the nodes and their children...
 * @param node The root node to draw.
 */
void RCBC_GL_Draw_Nodes(SceneNode* node) {
#ifndef _SKIPGL
	DEBUG_V("Entering function...");
	while(node) {
		glPushMatrix();
		RCBC_GL_Draw_Node(node);

		if(node->child) {
			RCBC_GL_Draw_Node(node->child);
		}
		glPopMatrix();
		node = node->next;
	}
#endif
}

/**
 * Draws a model.
 * @return 0 or crash
 */
int RCBC_GL_Draw(Model* model) {
#ifndef _SKIPGL
	DEBUG_V("Entering function...");
	SceneNode* head = model->visual_scene;
	#warning ['TODO']: Remove debug...
	// Draw axis
	/*glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();*/

	RCBC_GL_Draw_Nodes(head);
#endif
	return 0;
}
