#pragma once
#include "Andromeda/Graphics/Geometry.h"


namespace And {

	class OpenGLGeometry : public Geometry {

		public:
			std::shared_ptr<OpenGLGeometry> load(std::string filename);
			OpenGLGeometry(const OpenGLGeometry&);
			OpenGLGeometry(OpenGLGeometry&&);
			~OpenGLGeometry();



		private:
			OpenGLGeometry();


			unsigned int m_vbo;
			unsigned int m_vao;
			unsigned int m_ebo;
		

	};
}