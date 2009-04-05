#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>

#include "console.h"

void ShaderProgram_0ShaderProgram(ShaderProgram* sp) {
	glDetachShader(sp->program, sp->shader);

	glDeleteShader(sp->shader);
	glDeleteProgram(sp->program);

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

ShaderProgram* load_shader(char* filename, int type) {
	ShaderProgram* sp = NEW(ShaderProgram);

	char* source;
	int result = load_file("phong.vert", &source);
	if(result < 0) {
		ERROR("Failed to load shader...");
		return NULL;
	}

	sp->shader = glCreateShader(type);
	sp->program = glCreateProgram();

	glShaderSource(sp->shader, 1, &source, NULL);
	glCompileShader(sp->shader);

	glAttachShader(sp->program, sp->shader);
	glLinkProgram(sp->program);

	free(source);
	return sp;
}

ShaderProgram* load_phong_vert_shader() {
	return load_shader("phong.vert", GL_VERTEX_SHADER);
}

ShaderProgram* load_phong_frag_shader() {
	return load_shader("phong.frag", GL_FRAGMENT_SHADER);
}
