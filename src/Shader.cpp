

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "GL/glew.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif


static char* textFileRead(const char *fileName) {
	char* text = 0;



	if (fileName != 0) {
		FILE *file = fopen(fileName, "rt");

		if (file != 0) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
		else {
			std::cerr<< "File not found"<<std::endl<<fileName<<std::endl;
#ifdef __APPLE__
            exit(0);
#else
            glutExit();
#endif
		}
	}
	return text;
}

static void validateShader(unsigned shader, const char* file = 0) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;

	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	if (length > 0) {
		std::cerr << "Shader " << shader << " (" << (file?file:"") << ") compile Log: " << buffer << std::endl;
	}
}


Shader::Shader(){
};

unsigned Shader::loadShader(const char *fileName, GLenum shaderType){

	unsigned shader_id = 0;
	FILE* file;
	long file_size = -1;
	const char* glsl_source;

	glsl_source = textFileRead(fileName);

	shader_id = glCreateShader(shaderType);

	glShaderSource(shader_id, 1, &glsl_source, 0);
	glCompileShader(shader_id);
	validateShader(shader_id);

	return shader_id;
}
