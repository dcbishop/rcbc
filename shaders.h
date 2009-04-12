#ifndef _RCBC_SHADERS_H
#define _RCBC_SHADERS_H

#include <GL/gl.h>
#include "ooc.h"

typedef struct ShaderProgram {
	const ClassFunctions* class_;
	GLuint vert;
	GLuint frag;
	GLuint program;
} ShaderProgram;

void ShaderProgram_0ShaderProgram(ShaderProgram* sp);
ShaderProgram* ShaderProgram_ShaderProgram();

int load_file(const char* filename, char**data);
GLuint load_shader(char* filename, int type);
ShaderProgram* load_phong_shader();

#endif
