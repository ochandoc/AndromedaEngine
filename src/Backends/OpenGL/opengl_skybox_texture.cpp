#include "Backends/OpenGL/opengl_skybox_texture.h"
#include "Andromeda/Graphics/Texture.h"
#include "stb_image.h"

namespace And {

std::shared_ptr<OpenGLSkyBoxTexture> And::OpenGLSkyBoxTexture::CreateShared(const std::vector<std::string>& paths){
	
	
	unsigned int texture_id;
	unsigned char* data;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	for (int i = 0; i < paths.size(); i++) {
		


		int width, height, nrChannels;
		int TextureChannels = 0;
		stbi_set_flip_vertically_on_load_thread(1);
		data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, STBI_default);

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		}else {
			printf("\n *** Failed to load cube map texture *** \n");
			stbi_image_free(data);
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	std::shared_ptr<OpenGLSkyBoxTexture> sky_box = std::make_shared<OpenGLSkyBoxTexture>(OpenGLSkyBoxTexture{});
	sky_box->m_texture_id = (GLuint)texture_id;

	return sky_box;
}

void OpenGLSkyBoxTexture::Activate(unsigned int Slot) {
	glActiveTexture(GL_TEXTURE0 + Slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_id);
}

OpenGLSkyBoxTexture::OpenGLSkyBoxTexture(const OpenGLSkyBoxTexture& other){
	this->m_texture_id = other.m_texture_id;
}

OpenGLSkyBoxTexture::OpenGLSkyBoxTexture(OpenGLSkyBoxTexture&& other){
	this->m_texture_id = other.m_texture_id;
}

OpenGLSkyBoxTexture::OpenGLSkyBoxTexture() {}

}
