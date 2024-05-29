#include "Backends/OpenGL/OpenGLVertexBuffer.h"

namespace And {


    std::shared_ptr<OpenGLVertexBuffer> OpenGLVertexBuffer::CreateShare(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
    {

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        unsigned int VBO;
        glGenBuffers(1, &VBO);

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        assert(EBO >= 0);



        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)(Indices.size() * sizeof(uint32)), &Indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Position
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))); // Normal
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float))); // UV



        // Desbindeamos
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        std::shared_ptr<OpenGLVertexBuffer> vertex_buffer(new OpenGLVertexBuffer);
        vertex_buffer->m_vbo = VBO;
        vertex_buffer->m_vao = VAO;
        vertex_buffer->m_ebo = EBO;
        vertex_buffer->m_NumVertices = Vertices.size();
        vertex_buffer->m_NumIndices = (unsigned int)Indices.size();

        WAIT_GPU_LOAD();


        return vertex_buffer;
    }
}
