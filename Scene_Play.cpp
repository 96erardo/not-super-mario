#include <iostream>
#include <fstream>
#include "Vec2.h"
#include "Action.h"
#include "Components.h"
#include "GameEngine.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"

Scene_Play::Scene_Play (GameEngine* g, const std::string& path) {
  m_game = g;
  init(path);
}

void Scene_Play::init(const std::string& path) {
  registerAction(sf::Keyboard::Key::W, "UP");
  registerAction(sf::Keyboard::Key::S, "DOWN");
  registerAction(sf::Keyboard::Key::A, "LEFT");
  registerAction(sf::Keyboard::Key::D, "RIGHT");
  registerAction(sf::Keyboard::Key::Escape, "ESCAPE");

  std::string command;
  std::ifstream file(path);

  while (file >> command) {
    if (command == "Background") {
      int r, g, b;

      file >> r >> g >> b;

      m_background = sf::Color(r, g, b);

    } else if (command == "Tile") {
      std::string name;
      float x, y;

      file >> name >> x >> y;

      auto entity = m_entities.addEntity("Tile");
      auto animation = m_game->assets().getAnimation(name);

      entity->cAnimation = std::make_shared<CAnimation>(*animation);
      
      Vec2 pos = gridToMidPixel(x, y, entity);

      entity->cTransform = std::make_shared<CTransform>(pos);
      entity->cAnimation->animation.getSprite().setPosition({ pos.x, pos.y });
    }
  }
}

Vec2 Scene_Play::gridToMidPixel (float gridX, float gridY, std::shared_ptr<Entity> entity) {
  float gridSize = 64;
  Vec2 size = entity->cAnimation->animation.getSize();

  float x = (gridX * gridSize) + (size.x / 2);
  float y = abs((768 - (gridY * gridSize) - (size.y / 2)));

  return Vec2(x, y);
}

void Scene_Play::doAction (const Action& action) {
  if (action.type() == "END") {
    if (action.name() == "ESCAPE") {
      m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
    }
  }
}

void Scene_Play::sRender () {
  m_game->window().clear(m_background);

  for (auto entity : m_entities.getEntities()) {
    m_game->window().draw(entity->cAnimation->animation.getSprite());
  }

  m_game->window().display();
}

void Scene_Play::update () {
  m_entities.update();

  sRender();
}
