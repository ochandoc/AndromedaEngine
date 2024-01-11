#pragma once

class ShaderTextEditor
{
public:
  ShaderTextEditor(const char* Path);

  ~ShaderTextEditor();

  void do_something();

private:
  std::unique_ptr<struct ShaderTextEditorData> m_Data;
};