#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>

#include "rcbc.h"
#include "console.h"

#define PI 3.1415
#define Y_LOCK 0.1f /* Stops flipping the cube by rotating to high */

/* Zoom settings (mouse wheel) */
#define ZOOM_STEP 0.25f /* Amount to zoom for each wheel nodule */
#define ZOOM_MIN 0.5f
#define ZOOM_MAX 5.0f

#define COLOUR_WHITE_3F 1.0f, 1.0f, 1.0f
#define COLOUR_CUBE COLOUR_WHITE_3F
#define BOX_SIZE 1.0f

struct globals {
	unsigned int height;
	unsigned int width;
	RCBCThing* thing;

	int mouse_x;
	int mouse_y;

	float cam_x;
	float cam_y;
	float cam_z;
	float cam_zoom;

	float cam_rot_x;
	float cam_rot_y;
	float cam_rot_x_temp;
	float cam_rot_y_temp;
	float cam_fov;
};

struct globals g;

/* Position camera around box using some voodoo math */
void cameraPosition() {
   /* http://en.wikipedia.org/wiki/Spherical_coordinates */
   float theta = g.cam_rot_x+g.cam_rot_x_temp;
   float phi = g.cam_rot_y+g.cam_rot_y_temp;
   
   g.cam_x = g.cam_zoom * (cos(theta*(PI/180))) * (sin(phi*(PI/180)));
   g.cam_y = g.cam_zoom * (cos(phi*(PI/180)));
   g.cam_z = g.cam_zoom * (sin(theta*(PI/180))) * (sin(phi*(PI/180)));
   
   gluLookAt( g.cam_x, g.cam_y, g.cam_z, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0.0f );
}

/* Set 3D perspective mode */
void setPerspective() {
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(g.cam_fov, (GLfloat)g.width / (GLfloat)g.height, 0.1f, 10000.0f);
   glMatrixMode(GL_MODELVIEW);
}

void displayFunc() {
	static int last_render_time = 0;
	static int last_fps_time = 0;
	static int frame = 0;
	int fps;
	int current_time = glutGet(GLUT_ELAPSED_TIME);

	/* Limit framerate */
	if( !( (current_time - last_render_time) >= 1000/60) ) {
		return;
	}

	/* Calculate FPS */
	frame++;
	if(current_time - last_fps_time > 1000) {
		fps = frame*1000.0f/(current_time-last_fps_time);
		last_fps_time = current_time;
		frame = 0;
		logit("FPS: %d", fps);
	}

	last_render_time = current_time;

	setPerspective();
	glLoadIdentity();
	cameraPosition();

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClearDepth(10000.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// glutWireCube(1.0f);
	RCBC_Render(g.thing);

	glFlush();
	glutSwapBuffers();

	int error;
	if ((error = glGetError()))
		errorit("GLUT: %s", gluErrorString(error));
}

void reshapeFunc(int width, int height) {
   if (height==0) {
      height=1;
   }
   g.height = height;
   g.width = width;
   glViewport(0, 0, width, height);

   setPerspective();
}

/* Handle mouse roation */
void motionFunc(int x, int y) {
   int x_diff, y_diff;

   x_diff = x - g.mouse_x;
   y_diff = y - g.mouse_y;

   g.cam_rot_x_temp = ((float)x_diff / g.width) * 100;
   g.cam_rot_y_temp = -((float)y_diff / g.height) * 100;
   
   /* Stop flipping over the top of the cube */
   if(g.cam_rot_y_temp + g.cam_rot_y < Y_LOCK) {
      g.cam_rot_y_temp = Y_LOCK-g.cam_rot_y;
   } else if(g.cam_rot_y_temp + g.cam_rot_y > 180.0f - Y_LOCK) {
      g.cam_rot_y_temp = (180.0f-Y_LOCK)-g.cam_rot_y;
   }
}

/* When mouse clicked */
void mouseFunc(int button, int state, int x, int y) {
   switch(button) {
      case(0):
         if(!state) {   /* Save the current x,y cords when starting to drag */
            g.mouse_x = x;
            g.mouse_y = y;
         } else { /* Store the temp rotation as permenate when finished */
            g.cam_rot_x+=g.cam_rot_x_temp;
            g.cam_rot_y+=g.cam_rot_y_temp;
            g.cam_rot_x_temp = 0.0f;
            g.cam_rot_y_temp = 0.0f;
         }
         break;
      case(4): /* Zoom out with mouse wheel */
         g.cam_zoom+=ZOOM_STEP;
         if(g.cam_zoom>ZOOM_MAX)
            g.cam_zoom=ZOOM_MAX;
         logit("Zoom at %f.", g.cam_zoom);
         break;
      case(3): /* Zoom in with mouse wheel */
         g.cam_zoom-=ZOOM_STEP;
         if(g.cam_zoom<ZOOM_MIN)
            g.cam_zoom=ZOOM_MIN;
         logit("Zoom at %f.", g.cam_zoom);
         break;
   }
}

void idleFunc() {
   glutPostRedisplay();
}

int main(int argc, char** argv) {
	RCBC_Init();
	g.thing = RCBC_LoadFile("samples/cube on plane.dae");

	g.height = 600;
	g.width = 800;

	logit("Initilizing RCRB Viewer GLUT...");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	/* Create window */
	glutInitWindowSize(g.width, g.height);
	glutCreateWindow("RCBC Viewer");

	glutDisplayFunc(&displayFunc);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(&reshapeFunc);
	glutMouseFunc(&mouseFunc);
	glutMotionFunc(&motionFunc);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(10000.0f);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	g.mouse_x = 0;
	g.mouse_y = 0;

	g.cam_zoom = 2.5f;
	g.cam_rot_x = 25.0f;
	g.cam_rot_y = 75.0f;
	g.cam_rot_x_temp = 0.0f;
	g.cam_rot_y_temp = 0.0f;

	g.cam_fov = 45.0f;

	glutMainLoop();
	
	exit(EXIT_SUCCESS);
}