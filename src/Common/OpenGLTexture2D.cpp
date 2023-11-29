#include "Common/OpenGLTexture2D.h"

#include "Backends/OpenGL/OpenGL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
{
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	uint8* pixels = stbi_load(path.c_str(), (int*)&m_Info.width, (int*)&m_Info.height, (int*)&m_Channels, STBI_default);

	if (pixels) {
		m_IsLoaded = true;
		m_Path = path;
		assert(m_Channels == 4 || m_Channels == 3 && "TODO: Handle texture extra channels");
		if (m_Channels == 3) {
			m_Info.format = TextureFormat_RGB8;
			m_InternalFormat = GL_RGB8;
			m_Format = GL_RGB;
		}
		if (m_Channels == 4) {
			m_Info.format = TextureFormat_RGBA8;
			m_InternalFormat = GL_RGBA8;
			m_Format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Info.width, m_Info.height, 0, m_Format, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		m_Info.mipmaps = true;
		//ISDEV_LOG();
		printf("Image %s loaded %u\n", path.c_str(), m_Id);
	}
	else {
		printf("Failed to load texture %s\n", path.c_str());
		//LOG_ERROR();
	}

	stbi_image_free(pixels);

	WAIT_GPU_LOAD();
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	if (m_Id)
	{
		printf("Texture destroyed %u\n", m_Id);
		glDeleteTextures(1, &m_Id);
	}
}

void OpenGLTexture2D::set_data(void* data, uint32 size)
{

}

void OpenGLTexture2D::bind(uint32 slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

bool OpenGLTexture2D::is_loaded() const
{
	return m_IsLoaded;
}

uint32 OpenGLTexture2D::get_id() const
{
	return m_Id;
}

uint32 OpenGLTexture2D::get_width() const
{
	return m_Info.width;
}

uint32 OpenGLTexture2D::get_height() const
{
	return m_Info.height;
}

const std::string& OpenGLTexture2D::get_path() const
{
	return m_Path;
}

void OpenGLTexture2D::draw_in_imgui(int id)
{
	if (ImGui::Begin(("Texture(" + std::to_string(id) + ")").c_str()))
	{
		ImGui::Text((m_Path + " (" + std::to_string(m_Id) + ")").c_str());
		ImGui::Image((void*)(intptr_t)m_Id, ImGui::GetWindowSize());
	}
	ImGui::End();
}