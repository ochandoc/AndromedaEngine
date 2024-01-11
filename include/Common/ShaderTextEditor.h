#pragma once

class ShaderTextEditor
{
public:
  ShaderTextEditor(const char* Path);

  ~ShaderTextEditor();

  void do_something();

  bool can_reload();

private:
  std::unique_ptr<struct ShaderTextEditorData> m_Data;
  bool m_just_changed;
};