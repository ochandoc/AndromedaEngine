#pragma once
#include "Andromeda/Graphics/IndexBuffer.h"
#include "OpenGL.h"

namespace And {

	class OpenGLIndexBuffer : public IndexBuffer{

	public:
		static std::shared_ptr<OpenGLIndexBuffer> CreateShared(const std::vector<uint32>& Indices);

		std::vector<unsigned int>& GetBuffer() { return m_indices; }
		inline uint64 GetNumIndices() const { return m_NumIndices; }

		void BindEBO() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); }

	private:
		std::vector<unsigned int> m_indices;
		uint64 m_NumIndices;
		unsigned int m_ebo;
	};
}