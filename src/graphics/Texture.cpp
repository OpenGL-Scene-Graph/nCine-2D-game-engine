#include <cstdlib> // for exit()
#define NCINE_INCLUDE_OPENGL
#include "common_headers.h"

#include "Texture.h"
#include "ITextureLoader.h"
#include "GLTexture.h"
#include "ServiceLocator.h"

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

Texture::Texture()
	: Object(TEXTURE_TYPE), glTexture_(new GLTexture(GL_TEXTURE_2D)), width_(0), height_(0),
	  mipMapLevels_(1), isCompressed_(false), hasAlphaChannel_(false)
{

}


Texture::Texture(const char *filename)
	: Object(TEXTURE_TYPE, filename), glTexture_(new GLTexture(GL_TEXTURE_2D)),
	  width_(0), height_(0), mipMapLevels_(1), isCompressed_(false), hasAlphaChannel_(false)
{
	glTexture_->bind();

	ITextureLoader *pTexLoader = ITextureLoader::createFromFile(filename);
	load(*pTexLoader);
	delete pTexLoader;
}

Texture::Texture(const char *filename, int width, int height)
	: Object(TEXTURE_TYPE, filename), glTexture_(new GLTexture(GL_TEXTURE_2D)),
	  width_(0), height_(0), isCompressed_(false), hasAlphaChannel_(false)
{
	glTexture_->bind();

	ITextureLoader *pTexLoader = ITextureLoader::createFromFile(filename);
	load(*pTexLoader, width, height);
	delete pTexLoader;
}

Texture::Texture(const char *filename, Vector2i size)
	: Object(TEXTURE_TYPE, filename), glTexture_(new GLTexture(GL_TEXTURE_2D)),
	  width_(0), height_(0), isCompressed_(false), hasAlphaChannel_(false)
{
	glTexture_->bind();

	ITextureLoader *pTexLoader = ITextureLoader::createFromFile(filename);
	load(*pTexLoader, size.x, size.y);
	delete pTexLoader;
}

Texture::~Texture()
{
	delete glTexture_;
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void Texture::setFiltering(TextureFiltering filter)
{
	GLenum glFilter = GL_NEAREST;
	switch (filter)
	{
		case NEAREST:					glFilter = GL_NEAREST; break;
		case LINEAR:					glFilter = GL_LINEAR; break;
		case NEAREST_MIPMAP_NEAREST:	glFilter = GL_NEAREST_MIPMAP_NEAREST; break;
		case LINEAR_MIPMAP_NEAREST:		glFilter = GL_LINEAR_MIPMAP_NEAREST; break;
		case NEAREST_MIPMAP_LINEAR:		glFilter = GL_NEAREST_MIPMAP_LINEAR; break;
		case LINEAR_MIPMAP_LINEAR:		glFilter = GL_LINEAR_MIPMAP_LINEAR; break;
		default:						glFilter = GL_NEAREST; break;
	}

	glTexture_->bind();
	glTexture_->texParameteri(GL_TEXTURE_MAG_FILTER, glFilter);
	glTexture_->texParameteri(GL_TEXTURE_MIN_FILTER, glFilter);
}

void Texture::setWrap(TextureWrap wrap)
{
	GLenum glWrap = GL_CLAMP_TO_EDGE;
	switch (wrap)
	{
		case CLAMP_TO_EDGE:				glWrap = GL_CLAMP_TO_EDGE; break;
		case MIRRORED_REPEAT:			glWrap = GL_MIRRORED_REPEAT; break;
		case REPEAT:					glWrap = GL_REPEAT; break;
		default:						glWrap = GL_CLAMP_TO_EDGE; break;
	}

	glTexture_->bind();
	glTexture_->texParameteri(GL_TEXTURE_WRAP_S, glWrap);
	glTexture_->texParameteri(GL_TEXTURE_WRAP_T, glWrap);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void Texture::load(const ITextureLoader& texLoader)
{
	load(texLoader, texLoader.width(), texLoader.height());
}

void Texture::load(const ITextureLoader& texLoader, int width, int height)
{
	const IGfxCapabilities& gfxCaps = theServiceLocator().gfxCapabilities();
	if (width > gfxCaps.value(IGfxCapabilities::MAX_TEXTURE_SIZE) ||
		height > gfxCaps.value(IGfxCapabilities::MAX_TEXTURE_SIZE))
	{
		LOGF("Texture size is bigger than device maximum");
		exit(EXIT_FAILURE);
	}

	glTexture_->texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexture_->texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (texLoader.mipMapCount() > 1)
	{
		glTexture_->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexture_->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#ifndef __ANDROID__
		// To prevent artifacts if the MIP map chain is not complete
		glTexture_->texParameteri(GL_TEXTURE_MAX_LEVEL, texLoader.mipMapCount());
#endif
	}
	else
	{
		glTexture_->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexture_->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	const TextureFormat &texFormat = texLoader.texFormat();

	int levelWidth = width;
	int levelHeight = height;
	for (int i = 0; i < texLoader.mipMapCount(); i++)
	{
		if (texFormat.isCompressed())
		{
			glTexture_->compressedTexImage2D(i, texFormat.internalFormat(), levelWidth, levelHeight,
				texLoader.dataSize(i), texLoader.pixels(i));
		}
		else
		{
			glTexture_->texImage2D(i, texFormat.internalFormat(), levelWidth, levelHeight,
				texFormat.format(), texFormat.type(), texLoader.pixels(i));
		}

		levelWidth /= 2;
		levelHeight /= 2;
	}

	width_ = width;
	height_ = height;
	mipMapLevels_ = texLoader.mipMapCount();
	isCompressed_ = texFormat.isCompressed();
	hasAlphaChannel_ = texFormat.hasAlpha();
}

}