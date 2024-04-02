#pragma once
#include "Andromeda/Editor/EditorWindow.h"
#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And{

class GameInfoWindow : public EditorWindow{

  public:
  GameInfoWindow();
  ~GameInfoWindow();
  NON_COPYABLE_CLASS(GameInfoWindow);
  NON_MOVABLE_CLASS(GameInfoWindow);

  virtual void Show() override;


  private:
  
};

}