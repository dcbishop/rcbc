#ifndef _RCBC_SHADERS_H
#define _RCBC_SHADERS_H

#include <GL/gl.h>
#include "ooc.h"

typedef struct ShaderProgram {
	const ClassFunctions* class_;
	GLuint shader;
	GLuint program;
} ShaderProgram;

void ShaderProgram_0ShaderProgram(ShaderProgram* sp);
ShaderProgram* ShaderProgram_ShaderProgram();

int load_file(const char* filename, char**data);

ShaderProgram* load_phong_vert_shader();
ShaderProgram* load_phong_frag_shader();

#endif
