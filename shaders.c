#warning ['TODO']: This isn't used... (Also the OpenGL 1.1 windows uses doesn't support it without extentions).
#if 0

#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>

#include "console.h"

void ShaderProgram_0ShaderProgram(ShaderProgram* sp) {
	if(sp->vert) {
		glDetachShader(sp->program, sp->vert);
		glDeleteShader(sp->vert);
	}
	if(sp->frag) {
		glDetachShader(sp->program, sp->frag);
		glDeleteShader(sp->frag);
	}

	if(sp->program) {
		glDeleteProgram(sp->program);
	}

	free(sp);
}

static const ClassFunctions ShaderProgram_c = {
	(void*)ShaderProgram_0ShaderProgram
};

ShaderProgram* ShaderProgram_ShaderProgram() {
	ALLOCATE(ShaderProgram, sp);
	sp->class_ = &ShaderProgram_c;
	
}

int load_file(const char* filename, char**data) {
	int size = 0;
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		#warning ['TODO']: Print perror message...
		ERROR("Failed to open file '%s'...", filename);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	*data = (char*)malloc(size+1);
	if(!*data) {
		#warning ['TODO']: Print perror message...
		ERROR("Failed to allocate space for data...");
		fclose(fp);
		return -2;
	}

	int bytes = fread(*data, sizeof(char), size, fp);
	if(size != bytes) {
		#warning ['TODO']: Print perror message...
		ERROR("Failed to read data...");
		free(*data);
		*data = NULL;
		fclose(fp);
		return -3;
	}
	fclose(fp);
	(*data)[size] = '\0';
	return size;
}

GLuint load_shader(char* filename, int type) {
	char* source;
	int result = load_file(filename, &source);
	if(result < 0) {
		ERROR("Failed to load shader...");
		return 0;
	}
	
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	

	free(source);
	return shader;
}

ShaderProgram* load_phong_shader() {
	ShaderProgram* sp = NEW(ShaderProgram);
	sp->vert = load_shader("phong.vert", GL_VERTEX_SHADER);
	sp->frag = load_shader("phong.frag", GL_FRAGMENT_SHADER);
	
	sp->program = glCreateProgram();
	if(!sp->vert || !sp->frag || !sp->program) {
		DELETE(sp);
		return NULL;
	}
		
	glAttachShader(sp->program, sp->vert);
	glAttachShader(sp->program, sp->frag);
	
	glLinkProgram(sp->program);
	
	return sp;
}
#endif
