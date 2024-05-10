#pragma once

#include "OpenGL.h"
#include "Andromeda/Graphics/Texture.h"

namespace And {
	class OpenGLSkyBoxTexture : public SkyboxTexture {


	public:

		static std::shared_ptr<OpenGLSkyBoxTexture> CreateShared(const std::vector<std::string>& paths);

		OpenGLSkyBoxTexture(const OpenGLSkyBoxTexture&);
		OpenGLSkyBoxTexture(OpenGLSkyBoxTexture&&);

		void Activate(unsigned int Slot);

		inline GLuint GetTexture() const { return m_texture_id; }
		//inline ID3D11ShaderResourceView* GetView() const { return m_View.Get(); }
		//inline ID3D11SamplerState* GetSampler() const { return m_Sampler.Get(); }


	private:
		GLuint m_texture_id;
		//ComPtr<ID3D11ShaderResourceView> m_View;
		//ComPtr<ID3D11SamplerState> m_Sampler;

		OpenGLSkyBoxTexture();
	};
};