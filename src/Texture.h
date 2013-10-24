// Texture class created by Juliane Reschke and used with permission

#ifndef __TEXTURE_H
#define __TEXTURE_H

#define GLEW_STATIC
#include <FreeImage.h>
#include <string>

class Texture
{
public:

	Texture(const std::string& filename);
	void bind() const;
	void unbind() const;
	unsigned getHandle();
	~Texture(void);

private:
	FIBITMAP *_bitmap;
	unsigned _handle;
	

};

#endif

