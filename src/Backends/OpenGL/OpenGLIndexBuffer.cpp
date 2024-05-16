#include "Backends/OpenGL/OpenGLIndexBuffer.h"
#include "OpenGL.h"
#include "Andromeda/Graphics/Vertex.h"


namespace And {

	std::shared_ptr<OpenGLIndexBuffer> And::OpenGLIndexBuffer::CreateShared(const std::vector<uint32>& Indices){

		unsigned int ebo;

		glGenBuffers(1, &ebo);

		assert(ebo >= 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)(Indices.size() * sizeof(uint32)), &Indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		std::shared_ptr<OpenGLIndexBuffer> index_buffer(new OpenGLIndexBuffer);
		index_buffer->m_indices = Indices;
		index_buffer->m_NumIndices = Indices.size();
		index_buffer->m_ebo = ebo;


		return index_buffer;
	}
}
