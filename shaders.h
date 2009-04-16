#ifndef _RCBC_SHADERS_H
#define _RCBC_SHADERS_H

#include <GL/gl.h>
#include "ooc.h"

/**
 * A program with vertex and fragment shaders.
 */
typedef struct ShaderProgram {
	const ClassFunctions* class_;	/**< Class functions. */
	GLuint vert;					/**< The vertex shader id */
	GLuint frag;					/**< The fragment shader id */
	GLuint program;					/**< The compiled/linked shader program id */
} ShaderProgram;

void ShaderProgram_0ShaderProgram(ShaderProgram* sp);
ShaderProgram* ShaderProgram_ShaderProgram();

int load_file(const char* filename, char**data);
GLuint load_shader(char* filename, int type);
ShaderProgram* load_phong_shader();

#endif
