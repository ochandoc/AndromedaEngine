#include "Backends/OpenGL/OpenGLTexture2D.h"

#include "Backends/OpenGL/OpenGL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"


namespace And
{
	static int32 GetOpenGLInternalFormat(ETextureFormat Format)
	{
		switch (Format)
		{
		case And::ETextureFormat::RGB8:
			return GL_RGB8;
			break;
		case And::ETextureFormat::RGBA8:
			return GL_RGBA8;
			break;
		case And::ETextureFormat::RGB16F:
			return GL_RGB16F;
			break;
		case And::ETextureFormat::RGBA16F:
			return GL_RGBA16F;
			break;
		case And::ETextureFormat::RGB32F:
			return GL_RGB32F;
			break;
		case And::ETextureFormat::RGBA32F:
			return GL_RGBA32F;
			break;
		case And::ETextureFormat::Depth:
			return GL_DEPTH_COMPONENT;
			break;
		}

		return 0;
	}

	static uint32 GetOpenGLFormat(ETextureFormat Format)
	{
		switch (Format)
		{
		case And::ETextureFormat::RGB8:
			return GL_RGB;
			break;
		case And::ETextureFormat::RGBA8:
			return GL_RGBA;
			break;
		case And::ETextureFormat::RGB16F:
			return GL_RGB;
			break;
		case And::ETextureFormat::RGBA16F:
			return GL_RGBA;
			break;
		case And::ETextureFormat::RGB32F:
			return GL_RGB;
			break;
		case And::ETextureFormat::RGBA32F:
			return GL_RGBA;
			break;
		case And::ETextureFormat::Depth:
			return GL_DEPTH_COMPONENT;
			break;
		}

		return 0;
	}

	static uint32 GetOpenGLFormatType(ETextureFormat Format)
	{
		switch (Format)
		{
		case And::ETextureFormat::RGB8:
			return GL_UNSIGNED_BYTE;
			break;
		case And::ETextureFormat::RGBA8:
			return GL_UNSIGNED_BYTE;
			break;
		case And::ETextureFormat::RGB16F:
			return GL_FLOAT;
			break;
		case And::ETextureFormat::RGBA16F:
			return GL_FLOAT;
			break;
		case And::ETextureFormat::RGB32F:
			return GL_FLOAT;
			break;
		case And::ETextureFormat::RGBA32F:
			return GL_FLOAT;
			break;
		case And::ETextureFormat::Depth:
			return GL_FLOAT;
			break;
		}

		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D() : m_Id(0), m_LoadedFromFile(false)
	{

	}

	OpenGLTexture2D::OpenGLTexture2D(ENoInit) : OpenGLTexture2D()
	{

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		if (m_Id != 0)
		{
			glDeleteTextures(1, &m_Id);
		}
	}

	std::shared_ptr<OpenGLTexture2D> OpenGLTexture2D::Make(const TextureCreationInfo& CreationInfo)
	{
		uint64 TextureSize = CreationInfo.Width * CreationInfo.Height;
		if (TextureSize == 0) return std::shared_ptr<OpenGLTexture2D>();

		std::shared_ptr<OpenGLTexture2D> Texture(new OpenGLTexture2D);
		Texture->m_CreationInfo = CreationInfo;

		glGenTextures(1, &Texture->m_Id);
		glBindTexture(GL_TEXTURE_2D, Texture->m_Id);

		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GetOpenGLInternalFormat(CreationInfo.Format), CreationInfo.Width, CreationInfo.Height, 0, GetOpenGLFormat(CreationInfo.Format), GetOpenGLFormatType(CreationInfo.Format), NULL);

		WAIT_GPU_LOAD();
		glBindTexture(GL_TEXTURE_2D, 0);
		return Texture;
	}

	std::shared_ptr<OpenGLTexture2D> OpenGLTexture2D::Make(const std::string& Path)
	{
		TextureCreationInfo CreationInfo;
		int TextureChannels = 0;
		uint8* pixels = stbi_load(Path.c_str(), (int*)&CreationInfo.Width, (int*)&CreationInfo.Height, (int*)&TextureChannels, STBI_default);

		std::shared_ptr<OpenGLTexture2D> Texture;
		if (pixels)
		{
			if (TextureChannels == 3) CreationInfo.Format = ETextureFormat::RGB8;
			if (TextureChannels == 4) CreationInfo.Format = ETextureFormat::RGBA8;

			CreationInfo.Mipmaps = true;


			Texture = OpenGLTexture2D::Make(CreationInfo);

			uint64 TextureSize = CreationInfo.Width * CreationInfo.Height;
			Texture->UploadData(pixels, TextureSize);

			Texture->m_LoadedFromFile = true;
			Texture->m_Path = Path;

			WAIT_GPU_LOAD();
			stbi_image_free(pixels);
		}

		return Texture;
	}

	void OpenGLTexture2D::Activate(uint32 Slot) const
	{
		glActiveTexture(GL_TEXTURE0 + Slot);
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}

	void OpenGLTexture2D::UploadData(void* Data, uint64 Size)
	{
		uint32 dataFormat = 0;

		if (m_CreationInfo.Format == ETextureFormat::RGB8) dataFormat = GL_RGB;
		if (m_CreationInfo.Format == ETextureFormat::RGBA8) dataFormat = GL_RGBA;

		glTextureSubImage2D(m_Id, 0, 0, 0, m_CreationInfo.Width, m_CreationInfo.Height, dataFormat, GL_UNSIGNED_BYTE, Data);
	}

	bool OpenGLTexture2D::IsLoadedFromFile() const
	{
		return m_LoadedFromFile;
	}

	const char* OpenGLTexture2D::GetPath() const
	{
		if (!m_LoadedFromFile) return nullptr;
		return m_Path.c_str();
	}
}