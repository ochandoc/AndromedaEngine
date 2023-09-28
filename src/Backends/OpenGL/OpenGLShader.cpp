#include "OpenGLShader.h"

#include <vector>
#include "GL/glew.h"
#include <iostream>
#include <fstream>


namespace And{

  OpenGLShader::OpenGLShader(std::vector<ShaderInfo> S_info){

    unsigned int program = glCreateProgram();
    unsigned int id;
    
    for(ShaderInfo& shader : S_info){
      switch (shader.type){
      case Shader_Vertex:
        id = glCreateShader(GL_VERTEX_SHADER);        
      break;
      case Shader_Fragment:
        id = glCreateShader(GL_FRAGMENT_SHADER);
      break;
      case Shader_Geometry:
        
      break;
      case Shader_Teselation:
        
      break;
      
      default:
      break;
      }


      // Compilamos todos los shaders y hacemos el attach del program al shader
      const char* src = shader.file.c_str();
      printf("%s",src);
      glShaderSource(id, 1, &src, nullptr);
      glCompileShader(id);
      glAttachShader(program, id);
    }

    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(program);
    glValidateProgram(program);

    int succes;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &succes);
    printf("%d", succes);

    glUseProgram(program);
   
    // glDeleteShader(shader)

  }

  OpenGLShader::~OpenGLShader(){
      printf("cagaste");
  }

  int OpenGLShader::LoadShaderFromFile(char** path){

  }

  //static const char *kLoadFilesErrors[] = {"Can't open the file\0", "Not enough memory\0", "The data si not been loaded correctly\0"};
  
  bool LoadShaderFromFile(const char* filename, char** data, size_t& size) {
    // Abre el archivo en modo binario
    std::ifstream file(filename, std::ios::binary);
    
    if (!file) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return false;
    }
    
    // Obtiene el tamaño del archivo
    file.seekg(0, std::ios::end);
    size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    
    // Asigna memoria para almacenar los datos del archivo
    *data = new char[size];
    
    // Lee los datos del archivo en el buffer
    file.read(*data, size);
    
    // Cierra el archivo
    file.close();
    
    return true;
}

  void OpenGLShader::CreateShader(){

  }

  

}