#include <stdio.h>
#include "rcbc_render_gl.h"
#include "console.h"
#include <GL/gl.h>
#include <GL/glut.h>

int RCBC_GL_Init() {
	logit("Initilizing GL render...");
	return 0;
}

void RCBC_GL_Draw_Node(RCBCNode* node) {
	LLNode* itr;

	glTranslatef(node->translate[0], node->translate[2], node->translate[1]);
	glPushMatrix();
		glScalef(node->scale[0], node->scale[2], node->scale[1]);

		for(itr = node->rotations; itr; itr=itr->next) {
			RCBCNode_Rotate* rotation = itr->data;
			glRotatef(rotation->angle, rotation->x, rotation->z, rotation->y);
		}
		
		RCBCMesh* mesh = node->mesh;
		RCBC_Triangles* triangles;
		if(!mesh || !(triangles = mesh->triangles)) {
			glColor3f(1.0f, 0.0f, 0.0f);
			glutWireSphere(1.0f, 5, 5);
		} else {
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, triangles->vertices->values);
			if(triangles->normals) {
				glNormalPointer(GL_FLOAT, 0, triangles->normals->values);
			} else {

			}

			if(triangles->texcoords) {	
				glTexCoordPointer(2, GL_FLOAT, 0, triangles->texcoords->values);
			} else {

			}

			glDrawArrays(GL_TRIANGLES, 0, triangles->count * 3);

			glDisableClientState(GL_VERTEX_ARRAY);
		}

	glPopMatrix();
}

void RCBC_GL_Draw_Nodes(RCBCNode* node) {
/*	RCBCNode* head = thing->visual_scene;
	RCBCNode* current = head;*/

	while(node) {
		glPushMatrix();
		RCBC_GL_Draw_Node(node);
		/*child = current->child;

		while(child) {
			RCBC_GL_Draw_Node(current);
			child = child->child;
		}*/
		if(node->child) {
			RCBC_GL_Draw_Node(node->child);
		}
		glPopMatrix();
		node = node->next;
	}
}

int RCBC_GL_Draw(RCBCThing* thing) {
	RCBCNode* head = thing->visual_scene;
	/*RCBCNode* current = head;

	while(current) {
		RCBC_GL_Draw_Node(current);
		while(current->child) {
			current = current->child;
			RCBC_GL_Draw_Node(current);
		}

		if(current->next) {
			current = current->next;
		} else {
			current = current->parent;
		}
	}*/
	RCBC_GL_Draw_Nodes(head);
	return 0;
}
