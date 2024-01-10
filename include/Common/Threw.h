#pragma once


namespace And{

  template <typename T>
  class Threw{
    public:
      Threw(const char* file_name, const T& t){
        FILE *f;
        std::string name = std::string(file_name) + ".and";
        f = fopen(name.c_str(), "wb");

        if(f == NULL){
          printf("\n*** Error writing file ***\n");
          //return false;
        }else{
          fwrite(&t, sizeof(T), 1, f);
          fclose(f);
        }
      }
      
      Threw(const char* file_name, const T& t, const char* extension){
        FILE *f;
        std::string name = std::string(file_name) + extension;
        f = fopen(name.c_str(), "wb");

        if(f == NULL){
          printf("\n*** Error writing file ***\n");
          //return false;
        }else{
          fwrite(&t, sizeof(T), 1, f);
          fclose(f);
        }
      }


      Threw(const Threw&) = delete;
      Threw(Threw&&) = delete;
      ~Threw(){}


    private:

  };


}