#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL.h>

#include "rcbc_render_gl.h"
#include "console.h"

#define PI 3.14152f

/* Initilize the GL render... (nothing is required for this) */
int RCBC_GL_Init() {
	LOG("Initilizing GL render...");

	return 0;
}

/* Draws a node */
void RCBC_GL_Draw_Node(RCBCNode* node) {
	LLNode* itr;

	/* Translate node */
	glTranslatef(node->translate[0], node->translate[1], node->translate[2]);
	glPushMatrix();

	/* Apply rotations... */
	for(itr = node->rotations; itr; itr=itr->next) {
		RCBCNode_Rotate* rotation = itr->data;
		glRotatef(rotation->angle, rotation->x, rotation->y, rotation->z);
	}

	/* Scale */
	glScalef(node->scale[0], node->scale[1], node->scale[2]);
	
	RCBCMesh* mesh = node->mesh;
	RCBC_Triangles* triangles;
	if(!mesh || !(triangles = mesh->triangles)) { /* If there is node mesh data*/
		#warning TODO: Dont draw a red sphere...
		glColor3f(1.0f, 0.0f, 0.0f);
		glutWireSphere(1.0f, 5, 5); /* Draw a red sphere */
	} else {
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, triangles->vertices->values);
		if(triangles->normals) {
			glNormalPointer(GL_FLOAT, 0, triangles->normals->values);
		} else {
			#warning TODO: Clear normal pointer so no normals are used...
		}

		if(triangles->texcoords) {	
			glTexCoordPointer(2, GL_FLOAT, 0, triangles->texcoords->values);
		} else {
			#warning TODO: Clear texcoord pointer so no texcoords are used...
		}
		
		#warning TODO: Find out what these do exactly...
		//glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glClientActiveTextureARB(GL_TEXTURE0_ARB);

		/* Check for a image for this mesh */
		if(triangles->image) {
			if(triangles->image->id == 0) { /* If the image hasn't been loaded, we do it now */
				triangles->image->id = SOIL_load_OGL_texture(
					triangles->image->filename,
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				);
				
				/* If the image failed to load, we throw an error and set it to -1 so it won't load again */
				if(triangles->image->id == 0) {
					triangles->image->id = -1;
					ERROR("Failed to load texture: '%s', %s", triangles->image->filename, SOIL_last_result());
				}
			}
		}

		if(triangles->image->id != 0) {
			glBindTexture(GL_TEXTURE_2D, triangles->image->id);
		}

		/* Draw it, yay! */
		glDrawArrays(GL_TRIANGLES, 0, triangles->count * 3);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	glPopMatrix();
}

/* Draws all the nodes and their children... */
void RCBC_GL_Draw_Nodes(RCBCNode* node) {

	while(node) {
		glPushMatrix();
		RCBC_GL_Draw_Node(node);

		if(node->child) {
			RCBC_GL_Draw_Node(node->child);
		}
		glPopMatrix();
		node = node->next;
	}
}

/* Draws a model */
int RCBC_GL_Draw(RCBC_Model* thing) {
	RCBCNode* head = thing->visual_scene;

	/* Draw axis */
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
	return 0;
}
