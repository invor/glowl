#ifndef VertexLayout_hpp
#define VertexLayout_hpp

#include <glad/glad.h>

/**
* \struct VertexLayout
*
* \brief Container for vertex layout descritions. Used by Mesh class.
*
* \author Michael Becher
*/
struct VertexLayout
{
	struct Attribute
	{
		Attribute(GLint size, GLenum type, GLboolean normalized, GLsizei offset)
			: size(size), type(type), normalized(normalized), offset(offset) {}

		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei offset;
	};

	VertexLayout() : byte_size(0), attributes() {}
	VertexLayout(GLsizei byte_size, const std::vector<Attribute>& attributes)
		: byte_size(byte_size), attributes(attributes) {}
	VertexLayout(GLsizei byte_size, std::vector<Attribute>&& attributes)
		: byte_size(byte_size), attributes(attributes) {}

	GLsizei byte_size;
	std::vector<Attribute> attributes;
};

inline
bool operator==(VertexLayout::Attribute const& lhs, VertexLayout::Attribute const& rhs)
{
	return lhs.normalized == rhs.normalized && lhs.offset == rhs.offset && lhs.size == rhs.size && lhs.type == rhs.type;
}

inline
bool operator==(VertexLayout const& lhs, VertexLayout const& rhs)
{
	bool rtn = (lhs.byte_size == rhs.byte_size);
	
	if (lhs.attributes.size() == rhs.attributes.size())
	{
		for (size_t i = 0; i < lhs.attributes.size(); ++i)
		{
			rtn &= (lhs.attributes == rhs.attributes);
		}
	}
	else
	{
		rtn = false;
	}

	return rtn;
}


static constexpr size_t computeByteSize(GLenum value_type)
{
	size_t retval = 0;

	switch (value_type)
	{
	case GL_BYTE:
		retval = 1;
		break;
	case GL_SHORT:
		retval = 2;
		break;
	case GL_INT:
		retval = 4;
		break;
	case GL_FIXED:
		retval = 4;
		break;
	case GL_FLOAT:
		retval = 4;
		break;
	case GL_HALF_FLOAT:
		retval = 2;
		break;
	case GL_DOUBLE:
		retval = 8;
		break;
	case GL_UNSIGNED_BYTE:
		retval = 1;
		break;
	case GL_UNSIGNED_SHORT:
		retval = 2;
		break;
	case GL_UNSIGNED_INT:
		retval = 4;
		break;
	case GL_INT_2_10_10_10_REV:
		retval = 4;
		break;
	case GL_UNSIGNED_INT_2_10_10_10_REV:
		retval = 4;
		break;
	case GL_UNSIGNED_INT_10F_11F_11F_REV:
		retval = 4;
		break;
	default:
		break;
	}

	return retval;
}

static size_t computeAttributeByteSize(VertexLayout::Attribute attrib_desc)
{
	return computeByteSize(attrib_desc.type) * attrib_desc.size;
}

#endif // !VertexLayout_hpp
