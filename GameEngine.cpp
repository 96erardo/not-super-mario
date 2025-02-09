#include <SFML/Graphics.hpp>
#include <map>
#include "Action.h"
#include "GameEngine.h"
#include "Scene_Menu.h"

GameEngine::GameEngine (const std::string& config) {
  init();
};

void GameEngine::init() {
  m_window = sf::RenderWindow(
    sf::VideoMode({1280, 768}), 
    "Super Mario",
    sf::Style::Default,
    sf::State::Windowed
  );

  m_window.setFramerateLimit(60);

  changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

void GameEngine::update() {
  sUserInput();

  currentScene()->update();
}

void GameEngine::run() {
  while (m_window.isOpen()) {
    update();
  }
}

void GameEngine::changeScene (const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene) {
  if (endCurrentScene) {
    m_scenes.erase(m_currentScene);
  }

  m_currentScene = sceneName;
  m_scenes.insert_or_assign(sceneName, scene);
}

std::shared_ptr<Scene> GameEngine::currentScene () {
  return m_scenes[m_currentScene];
}

sf::RenderWindow& GameEngine::window () {
  return m_window;
}

void GameEngine::sUserInput () {
  while (const std::optional event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
        m_window.close();
    
    }
    
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (currentScene()->getActionMap().find(keyPressed->code) == currentScene()->getActionMap().end()) {
        continue;
      }

      currentScene()->doAction(Action(currentScene()->getActionMap().at(keyPressed->code), "START"));
    
    }
    
    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
      if (currentScene()->getActionMap().find(keyReleased->code) == currentScene()->getActionMap().end()) {
        continue;
      }

      std::cout << currentScene(); 

      currentScene()->doAction(Action(currentScene()->getActionMap().at(keyReleased->code), "END"));
    }
  }
}