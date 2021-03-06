#include "common_macros.h"
#include "GLAttribute.h"

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

GLAttribute::GLAttribute()
    : location_(-1), size_(0), type_(GL_FLOAT)
{
	name_[0] = '\0';
}

GLAttribute::GLAttribute(GLuint program, GLuint index)
    : location_(-1), size_(0), type_(GL_FLOAT)
{
	GLsizei length;
	glGetActiveAttrib(program, index, MaxNameLength, &length, &size_, &type_, name_);
	location_ = glGetAttribLocation(program, name_);

	if (location_ == -1)
		LOGW_X("Attribute location not found for attribute \"%s\" (%u) in shader program %u", name_, index, program);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

GLenum GLAttribute::basicType() const
{
	switch (type_)
	{
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
			return GL_FLOAT;
		case GL_INT:
		case GL_INT_VEC2:
		case GL_INT_VEC3:
		case GL_INT_VEC4:
			return GL_INT;
		default:
			LOGW_X("No available case to handle type: %u", type_);
			return type_;
	}
}

int GLAttribute::numComponents() const
{
	switch (type_)
	{
		case GL_FLOAT:
			return 1;
		case GL_FLOAT_VEC2:
			return 2;
		case GL_FLOAT_VEC3:
			return 3;
		case GL_FLOAT_VEC4:
			return 4;
		case GL_INT:
			return 1;
		case GL_INT_VEC2:
			return 2;
		case GL_INT_VEC3:
			return 3;
		case GL_INT_VEC4:
			return 4;
		default:
			LOGW_X("No available case to handle type: %u", type_);
			return 0;
	}
}

}
