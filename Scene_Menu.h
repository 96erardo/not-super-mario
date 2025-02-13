#ifndef SCENE_MENU_H
#define SCENE_MENU_H
#include <SFML/Graphics/Font.hpp>
#include "Scene.h"

class Scene_Menu: public Scene {
  int      m_selected = 0;
  size_t   m_numOptions = 0;

  void init (const std::string& path) override;
  void sRender () override;
  void addOption (
    int index, 
    bool selected,
    const std::string& text, 
    const std::string& value, 
    const sf::Font& font
  );

  public:
    Scene_Menu(GameEngine* g);
    
    void update () override;
    void doAction (const Action& action) override;
};

#endif