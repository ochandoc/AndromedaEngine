#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/Resources/ResourceGenerator.h"

namespace And
{
	class ResourceManager;
}

enum TextureFormat
{
	TextureFormat_RGB8,
	TextureFormat_RGBA8,
};

struct TextureCreationInfo
{
	uint32 width = 1;
	uint32 height = 1;
	TextureFormat format = TextureFormat_RGBA8;
	bool mipmaps = true;
};

class OpenGLTexture2D
{
private:
	OpenGLTexture2D();

public:
	OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
	OpenGLTexture2D(OpenGLTexture2D&& other) { m_Id = other.m_Id; other.m_Id = 0; };

	~OpenGLTexture2D();

	OpenGLTexture2D& operator =(const OpenGLTexture2D&) = delete;
	OpenGLTexture2D& operator =(OpenGLTexture2D&& other) { if (this != &other) { std::swap(m_Id, other.m_Id); } return *this; }

	static OpenGLTexture2D* Make(const std::string& path);

	void set_data(void* data, uint32 size);

	void bind(uint32 slot) const;

	bool is_loaded() const;

	uint32 get_id() const;

	uint32 get_width() const;
	uint32 get_height() const;

	const std::string& get_path() const;

	void draw_in_imgui(int id);

	friend class And::ResourceManager;
	friend class TextureGenerator;
private:
	bool m_IsLoaded;
	uint32 m_Id;
	uint32 m_InternalFormat;
	uint32 m_Format;
	uint32 m_Channels;
	TextureCreationInfo m_Info;
	std::string m_Path;
};

class TextureGenerator : public And::ResourceGenerator<OpenGLTexture2D>
{
public:
	TextureGenerator()	
	{
		m_Default = std::shared_ptr<OpenGLTexture2D>(OpenGLTexture2D::Make("missing_texture.png"));
	}

	virtual ~TextureGenerator()
	{

	}

	virtual std::shared_ptr<OpenGLTexture2D> operator()(const std::string& Path) override
	{
		OpenGLTexture2D* Tex = OpenGLTexture2D::Make(Path);
		if (Tex)
		{
			return std::shared_ptr<OpenGLTexture2D>(Tex);
		}
		return m_Default;
	}

	virtual uint64 GenerateId(const std::string& Path) override
	{
		return std::hash<std::string>{}(Path);
	}

	virtual std::shared_ptr<OpenGLTexture2D> GetDefault() override
	{
		return m_Default;
	}
private:
	std::shared_ptr<OpenGLTexture2D> m_Default;
};
