#include <iostream>
#include <fstream>
#include "Vec2.h"
#include "Action.h"
#include "Entity.h"
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

      entity->addComponent<CAnimation>(*animation);
      entity->addComponent<CTransform>(gridToMidPixel(x, y, entity), Vec2(0,0), 0);
      
      entity->addComponent<CBoundingBox>(
        Vec2(
          entity->getComponent<CAnimation>().animation.getSize().x,
          entity->getComponent<CAnimation>().animation.getSize().y
        )
      );
    
    } else if (command == "Player") {
      file >> m_playerConfig.X 
        >> m_playerConfig.Y 
        >> m_playerConfig.CX 
        >> m_playerConfig.CY 
        >> m_playerConfig.SPEED 
        >> m_playerConfig.JUMP
        >> m_playerConfig.MAXSPEED
        >> m_playerConfig.GRAVITY;

      auto entity = m_entities.addEntity("Player");
      auto animation = m_game->assets().getAnimation("Stand");

      entity->addComponent<CAnimation>(*animation);
      entity->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, entity), Vec2(0,0), 0);
      entity->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
      entity->addComponent<CGravity>(m_playerConfig.GRAVITY);
      entity->addComponent<CInput>();
      entity->addComponent<CState>("Stand");

      m_player = entity;
    }
  }
}

Vec2 Scene_Play::gridToMidPixel (float gridX, float gridY, std::shared_ptr<Entity> entity) {
  float gridSize = 64;
  Vec2 size = entity->getComponent<CAnimation>().animation.getSize();

  float x = (gridX * gridSize) + (size.x / 2);
  float y = abs((768 - (gridY * gridSize) - (size.y / 2)));

  return Vec2(x, y);
}

void Scene_Play::doAction (const Action& action) {
  if (action.type() == "START") {
    if (action.name() == "LEFT") {
      m_player->getComponent<CInput>().left = true;
    }
    
    if (action.name() == "RIGHT") {
      m_player->getComponent<CInput>().right = true;
    }

    if (action.name() == "UP") {
      m_player->getComponent<CInput>().jump = true;
    }
  } else if (action.type() == "END") {
    if (action.name() == "ESCAPE") {
      m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
    }

    if (action.name() == "LEFT") {
      m_player->getComponent<CInput>().left = false;
    }
    
    if (action.name() == "RIGHT") {
      m_player->getComponent<CInput>().right = false;
    }

    if (action.name() == "UP") {
      m_player->getComponent<CInput>().jump = false;
    }
  }
}

void Scene_Play::sMovement () {
  auto& input = m_player->getComponent<CInput>();
  auto& transform = m_player->getComponent<CTransform>();
  m_player->getComponent<CState>().state = "Stand";
  Vec2 vel = Vec2(0, transform.vel.y) + Vec2(0, m_player->getComponent<CGravity>().value);

  if (input.left == true) {
    vel.x = -m_playerConfig.SPEED;
    transform.scale = { -1, 1 };
    m_player->getComponent<CState>().state = "Run";
  }

  if (input.right == true) {
    vel.x = m_playerConfig.SPEED;
    transform.scale = { 1, 1 };
    m_player->getComponent<CState>().state = "Run";
  }

  if (input.jump == true && input.canJump == true) {
    vel += Vec2(0, m_playerConfig.JUMP);
    input.canJump = false;
  }

  transform.vel = vel;

  for (auto entity : m_entities.getEntities()) {
    auto& transform = entity->getComponent<CTransform>();
    transform.prevPos = transform.pos;
    transform.pos += transform.vel;
  }
}

void Scene_Play::sCollision () {
  for (auto entity: m_entities.getEntities("Tile")) {
    if (!entity->hasComponent<CBoundingBox>()) {
      continue;
    }

    Vec2 overlap = m_physics.GetOverlap(m_player, entity);

    if (overlap.x > 0 && overlap.y > 0) {
      Vec2 prevOverlap = m_physics.GetPreviousOverlap(m_player, entity);

      if (prevOverlap.y > 0) {
        m_player->getComponent<CTransform>().pos -= Vec2(overlap.x, 0);
      } else if (prevOverlap.x > 0) {
        m_player->getComponent<CTransform>().vel.y = 0;
        m_player->getComponent<CTransform>().pos -= Vec2(0, overlap.y);

        if (m_player->getComponent<CTransform>().pos.y < entity->getComponent<CTransform>().pos.y) {
          m_player->getComponent<CInput>().canJump = true;
        }
      }
    }
  }
}

void Scene_Play::sAnimation () {
  for (auto entity : m_entities.getEntities()) {
    if (
      entity->hasComponent<CState>() && 
      (entity->getComponent<CState>().state != entity->getComponent<CAnimation>().animation.getName())
    ) {
      entity->addComponent<CAnimation>(
        *m_game->assets().getAnimation(entity->getComponent<CState>().state)
      );
    }

    auto& animation = entity->getComponent<CAnimation>();
    auto& transform = entity->getComponent<CTransform>();

    animation.animation.getSprite().setPosition({
      transform.pos.x,
      transform.pos.y
    });

    animation.animation.getSprite().setScale({
      transform.scale.x * 4,
      transform.scale.y * 4
    });

    animation.animation.update();
  }
}

void Scene_Play::sRender () {
  m_game->window().clear(m_background);

  for (auto entity : m_entities.getEntities()) {
    m_game->window().draw(entity->getComponent<CAnimation>().animation.getSprite());
  }

  m_game->window().display();
}

void Scene_Play::update () {
  m_entities.update();

  sMovement();
  sCollision();
  sAnimation();
  sRender();
}
