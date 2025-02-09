#include <iostream>
#include "Vec2.h"
#include "Scene.h"
#include "Action.h"
#include "Scene_Menu.h"
#include "Components.h"
#include "GameEngine.h"
#include "EntityManager.h"

Scene_Menu::Scene_Menu (GameEngine* g) {
  m_game = g;
  init();
}

void Scene_Menu::init() {
  registerAction(sf::Keyboard::Key::W, "UP");
  registerAction(sf::Keyboard::Key::S, "DOWN");
  registerAction(sf::Keyboard::Key::Enter, "SELECT");

  if (!m_font.openFromFile("assets/font/absender1.ttf")) {
    std::cerr << "Error on loading font\n";
  }

  addOption(0, true, "Level I", "assets/levels/Level_1.txt", m_font);
  addOption(1, false, "Level II", "assets/levels/Level_2.txt", m_font);
  addOption(2, false, "Exit", "EXIT", m_font);
}

void Scene_Menu::addOption(
  int index,
  bool selected,
  const std::string& text,
  const std::string& value,
  const sf::Font& font
) {
  auto entity = m_entities.addEntity("Option");

  entity->cTransform = std::make_shared<CTransform>(
    Vec2(50, index * 100),
    Vec2(0, 0),
    0
  );

  entity->cText = std::make_shared<CText>(sf::Text(font));
  entity->cText->text.setString(text);
  entity->cText->text.setCharacterSize(100);
  entity->cText->text.setFillColor(selected ? sf::Color::White : sf::Color::Black);
  
  entity->cText->text.setPosition({ 
    entity->cTransform->pos.x, 
    entity->cTransform->pos.y,
  });

  entity->cMenuItem = std::make_shared<CMenuItem>(
    index,
    selected,
    text
  );

  m_numOptions++;
}

void Scene_Menu::doAction (const Action& action) {
  std::cout << action.name() << " " << action.type() << "\n";

  if (action.type() == "END") {
    if (action.name() == "UP") {
      if (m_selected > 0) {
        m_entities.getEntities().at(m_selected)->cText->text.setFillColor(sf::Color::Black);
        
        m_selected -= 1;

        m_entities.getEntities().at(m_selected)->cText->text.setFillColor(sf::Color::White);
      }
    
    } else if (action.name() == "DOWN") {
      if ((m_selected + 1) < m_numOptions) {
        m_entities.getEntities().at(m_selected)->cText->text.setFillColor(sf::Color::Black);
        
        m_selected += 1;

        m_entities.getEntities().at(m_selected)->cText->text.setFillColor(sf::Color::White);
      }
    }

    std::cout << m_selected << "\n";
  }
}

void Scene_Menu::sRender () {
  m_game->window().clear(sf::Color::Magenta);

  for (auto entity : m_entities.getEntities()) {
    m_game->window().draw(entity->cText->text);
  }

  m_game->window().display();
}

void Scene_Menu::update () {
  m_entities.update();

  sRender();
}
