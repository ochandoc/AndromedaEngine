#include "Common/OpenGLTexture2D.h"

#include "Backends/OpenGL/OpenGL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"

#include "Common/ResourceManager.h"

OpenGLTexture2D::OpenGLTexture2D()
{

}

OpenGLTexture2D::~OpenGLTexture2D()
{
	if (m_Id)
	{
		AND_LOG(ResourceManagerLog, And::Info, "Texture {} destroyed", m_Path.c_str());
		glDeleteTextures(1, &m_Id);
	}
}

OpenGLTexture2D* OpenGLTexture2D::Make(const std::string& path)
{
	bool Loaded = false;
	OpenGLTexture2D* Tex = new OpenGLTexture2D;
	glGenTextures(1, &Tex->m_Id);
	glBindTexture(GL_TEXTURE_2D, Tex->m_Id);

	glTextureParameteri(Tex->m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(Tex->m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(Tex->m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(Tex->m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	uint8* pixels = stbi_load(path.c_str(), (int*)&Tex->m_Info.width, (int*)&Tex->m_Info.height, (int*)&Tex->m_Channels, STBI_default);

	if (pixels)
	{
		Tex->m_IsLoaded = true;
		Tex->m_Path = path;
		assert(Tex->m_Channels == 4 || Tex->m_Channels == 3 && "TODO: Handle texture extra channels");
		if (Tex->m_Channels == 3)
		{
			Tex->m_Info.format = TextureFormat_RGB8;
			Tex->m_InternalFormat = GL_RGB8;
			Tex->m_Format = GL_RGB;
		}
		if (Tex->m_Channels == 4)
		{
			Tex->m_Info.format = TextureFormat_RGBA8;
			Tex->m_InternalFormat = GL_RGBA8;
			Tex->m_Format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, Tex->m_InternalFormat, Tex->m_Info.width, Tex->m_Info.height, 0, Tex->m_Format, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		Tex->m_Info.mipmaps = true;
		//ISDEV_LOG();

		Loaded = true;
		AND_LOG(ResourceManagerLog, And::Info, "Image {} loaded", path.c_str());
	}
	else
	{
		Loaded = false;
		AND_LOG(ResourceManagerLog, And::Error, "Failed to load texture {}", path.c_str());
	}

	stbi_image_free(pixels);

	WAIT_GPU_LOAD();
	if (!Loaded)
	{
		delete Tex;
		Tex = nullptr;
	}

	return Tex;
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