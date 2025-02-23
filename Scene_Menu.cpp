#include <iostream>
#include "Vec2.h"
#include "Scene.h"
#include "Action.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Components.h"
#include "GameEngine.h"
#include "EntityManager.h"

Scene_Menu::Scene_Menu (GameEngine* g) {
  m_game = g;
  init("");
}

void Scene_Menu::init(const std::string& path) {
  registerAction(sf::Keyboard::Key::W, "UP");
  registerAction(sf::Keyboard::Key::S, "DOWN");
  registerAction(sf::Keyboard::Key::Enter, "SELECT");
  registerAction(sf::Keyboard::Key::Escape, "QUIT");

  auto font = m_game->assets().getFont("Absender");

  addOption(true, "Level_I", "assets/levels/Level_1.txt", *font);
  addOption(false, "Exit", "EXIT", *font);
}

void Scene_Menu::addOption(
  bool selected,
  const std::string& text,
  const std::string& value,
  const sf::Font& font
) {
  Vec2 pos = Vec2(50, m_numOptions * 100);

  sf::Text option = sf::Text(font);
  option.setString(text);
  option.setCharacterSize(100);
  option.setFillColor(selected ? sf::Color::White : sf::Color::Black);
  
  option.setPosition({ 
    pos.x, 
    pos.y,
  });

  m_options.insert_or_assign(m_numOptions, option);

  m_numOptions++;
}

void Scene_Menu::doAction (const Action& action) {
  if (action.type() == "END") {
    if (action.name() == "UP") {
      if (m_selected > 0) {
        m_options.at(m_selected).setFillColor(sf::Color::Black);
        
        m_selected -= 1;

        m_options.at(m_selected).setFillColor(sf::Color::White);
      }
    
    } else if (action.name() == "DOWN") {
      if ((m_selected + 1) < m_numOptions) {
        m_options.at(m_selected).setFillColor(sf::Color::Black);
        
        m_selected += 1;

        m_options.at(m_selected).setFillColor(sf::Color::White);
      }
    } else if (action.name() == "SELECT") {
      auto selected = m_options.at(m_selected);

      if (selected.getString() == "Exit") {
        m_game->quit();
      
      } else {
        m_game->changeScene(
          "PLAY",
          std::make_shared<Scene_Play>(m_game, "assets/levels/" + selected.getString() + ".txt")
        );
      }
    
    } else if (action.name() == "QUIT") {
      m_game->quit();
    }
  }
}

void Scene_Menu::sRender () {
  auto size = m_game->window().getSize();
  m_game->window().clear(sf::Color::Magenta);
  sf::View view(sf::Vector2f(size.x / 2, size.y / 2), sf::Vector2f(size.x, size.y));

  m_game->window().setView(view);

  for (auto it = m_options.begin(); it != m_options.end(); it++) {
    m_game->window().draw(it->second);
  }

  m_game->window().display();
}

void Scene_Menu::update () {
  m_entities.update();

  sRender();
}
