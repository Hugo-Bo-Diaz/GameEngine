
#include <glad/glad.h>
#include "TexLoader.h"
#include "../stb/stb_image.h"
#include "../Resources.h"
#include "../../editor/ExtraWindows/SpriteShowcaseWindow.h"

Texture* TextureLoader::LoadTexture(const char* filename)
{
	void* pixels;

	int size_x, size_y;
	int nchannels;
	int stride;

	stbi_set_flip_vertically_on_load(false);
	pixels = stbi_load(filename, &size_x, &size_y, &nchannels, 0);
	if (pixels)
	{
		stride = size_x * nchannels;
	}
	else
	{
		//couldn't load file
		return nullptr;
	}

	Texture* newtex = new Texture();

	newtex->nchannels = nchannels;
	newtex->path = filename;
	newtex->size_x = size_x;
	newtex->size_y = size_y;
	newtex->stride = stride;
	newtex->nchannels = nchannels;

	GLenum internalFormat = GL_RGB8;
	GLenum dataFormat = GL_RGB;

	switch (nchannels)
	{
	case 3: dataFormat = GL_RGB;
			internalFormat = GL_RGB8; 
			break;
	case 4: dataFormat = GL_RGBA;
			internalFormat = GL_RGBA8; 
			break;
	default:
		delete newtex; 
		return nullptr; 
		break;
	}

	
	glGenTextures(1, &newtex->bufferID);
	glBindTexture(GL_TEXTURE_2D, newtex->bufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, newtex->size_x, newtex->size_y, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//I made it so that GL_REPEAT is the default result instead of clamping to edge
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);



	stbi_image_free(pixels);

	return newtex;
}

WinTexture* TextureLoader::LoadWinTexture(const char*filename)
{
	int size_x, size_y;
	int nchannels;

	unsigned char* image_data;

	stbi_set_flip_vertically_on_load(false);
	image_data = stbi_load(filename, &size_x, &size_y, &nchannels,3);
	if (!image_data)
	{
		//couldn't load file
		return nullptr;
	}

	WinTexture* ret = new WinTexture();

	ret->image_buffer = new unsigned char[size_x * size_y * nchannels];

	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			ret->image_buffer[((size_y - 1 - i) * size_x + j) * nchannels + 0] = image_data[(i * size_x + j) * 3 + 2];
			ret->image_buffer[((size_y - 1 - i) * size_x + j) * nchannels + 1] = image_data[(i * size_x + j) * 3 + 1];
			ret->image_buffer[((size_y - 1 - i) * size_x + j) * nchannels + 2] = image_data[(i * size_x + j) * 3 + 0];
		}
	}

	ret->info.biSize = sizeof(ret->info);
	ret->info.biPlanes = 1;
	ret->info.biBitCount = nchannels * 8;
	ret->info.biWidth = size_x;
	ret->info.biHeight = size_y;
	ret->info.biSizeImage = size_x*size_y*nchannels;

	ret->size_x = size_x;
	ret->size_y = size_y;

	delete[] image_data;


	return ret;
}
