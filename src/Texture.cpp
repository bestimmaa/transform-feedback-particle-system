#include "Texture.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <glErrorUtil.h>

#include "GL/glew.h"





Texture::Texture(const std::string& filename):
	_bitmap(0),_handle(0)
{
	_bitmap = FreeImage_Load(FreeImage_GetFileType(filename.c_str()),filename.c_str());
	if(!_bitmap){
		std::cout << std::endl <<  "Image " << filename << " did not load";
		return;
	}
	
	std::cout << "Texture " << filename << " loaded :-)";
	std::cout << std::endl << "        width:  " << FreeImage_GetWidth(_bitmap);
	std::cout << std::endl << "        height: " << FreeImage_GetHeight(_bitmap);
	
	
	unsigned char* pixeldata = FreeImage_GetBits(_bitmap);
	
	CheckErrorsGL("Texture::Texture: befor GL_TEXTURE_2D");


	glEnable(GL_TEXTURE_2D);
	CheckErrorsGL("Texture::Texture: after glEnable(GL_TEXTURE_2D)");
    glActiveTexture(GL_TEXTURE0);
	CheckErrorsGL("Texture::Texture: after glActiveTexture(GL_TEXTURE0)");

	
	CheckErrorsGL("Texture::Texture: befor glGenTexture");


	glGenTextures(1, &_handle);
	glBindTexture(GL_TEXTURE_2D, _handle);
	glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGB,
				FreeImage_GetWidth(_bitmap),
				FreeImage_GetHeight(_bitmap),
				0,
				GL_BGR,
				GL_UNSIGNED_BYTE,
				pixeldata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	

	CheckErrorsGL("Texture::Texture: after glGenTexture");

	std::cout << std::endl << "      handle: " << _handle;
	//std::cout << std::endl << "      data:   " << FreeImage_GetBits(_bitmap);
	

}

void Texture::bind() const{
	
	glBindTexture(GL_TEXTURE_2D, _handle);

}

void Texture::unbind() const{
	
	glBindTexture(GL_TEXTURE_2D, 0);

}

unsigned Texture::getHandle(){

	return _handle;
}


Texture::~Texture(void)
{
	FreeImage_Unload(_bitmap);
}
