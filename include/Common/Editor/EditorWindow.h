#pragma once
#include <vector>
#include <string>


namespace And{

class Editor;

  class EditorWindow{

    public:
    EditorWindow();
    EditorWindow(const EditorWindow&) = delete;
    EditorWindow(EditorWindow&&) = delete;
    virtual ~EditorWindow();

    friend class Editor;

    protected:

    bool m_is_open;
    Editor* m_Editor;
    std::string m_title;

    private:

    void SetEditor(Editor*);
    virtual void Show() = 0;


  };
}