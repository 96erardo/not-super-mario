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
  registerAction(sf::Keyboard::Key::Space, "SHOOT");
  registerAction(sf::Keyboard::Key::Escape, "ESCAPE");

  std::string command;
  std::ifstream file(path);

  while (file >> command) {
    if (command == "Background") {
      int r, g, b;

      file >> r >> g >> b;

      m_background = sf::Color(r, g, b);

    } else if (command == "Dec") {
      std::string name;
      float x, y;

      file >> name >> x >> y;

      auto entity = m_entities.addEntity("Dec");
      auto animation = m_game->assets().getAnimation(name);

      entity->addComponent<CAnimation>(*animation);
      auto transform = entity->addComponent<CTransform>(gridToMidPixel(x, y, entity), Vec2(0,0), 0);

      if (transform.pos.x + (animation->getSize().x / 2) > m_worldWidth) {
        m_worldWidth = transform.pos.x + (animation->getSize().x / 2);
      }
    } else if (command == "Tile") {
      std::string name;
      float x, y;

      file >> name >> x >> y;

      auto entity = m_entities.addEntity("Tile");
      auto animation = m_game->assets().getAnimation(name);

      entity->addComponent<CAnimation>(*animation);
      auto transform = entity->addComponent<CTransform>(gridToMidPixel(x, y, entity), Vec2(0,0), 0);
      
      entity->addComponent<CBoundingBox>(
        Vec2(
          entity->getComponent<CAnimation>().animation.getSize().x,
          entity->getComponent<CAnimation>().animation.getSize().y
        )
      );

      if (transform.pos.x + (animation->getSize().x / 2) > m_worldWidth) {
        m_worldWidth = transform.pos.x + (animation->getSize().x / 2);
      }
    } else if (command == "Player") {
      file >> m_playerConfig.X 
        >> m_playerConfig.Y 
        >> m_playerConfig.CX 
        >> m_playerConfig.CY 
        >> m_playerConfig.SPEED 
        >> m_playerConfig.JUMP
        >> m_playerConfig.MAXSPEED
        >> m_playerConfig.GRAVITY
        >> m_playerConfig.WEAPON;

        spawnPlayer();
    }
  }
}

void Scene_Play::spawnPlayer () {
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

void Scene_Play::spawnBullet () {
  auto entity = m_entities.addEntity("Bullet");
  auto animation = m_game->assets().getAnimation("Bullet");
  Vec2 size = m_player->getComponent<CAnimation>().animation.getSize();
  float dir = m_player->getComponent<CTransform>().scale.x;

  entity->addComponent<CAnimation>(*animation);
  
  entity->addComponent<CTransform>(
    Vec2(
      m_player->getComponent<CTransform>().pos.x + (dir * (size.x / 2)),
      m_player->getComponent<CTransform>().pos.y
    ),
    Vec2(dir * 15, 0),
    0
  );

  entity->addComponent<CLifespan>(
    180
  );
}

void Scene_Play::spawnExplosion (std::shared_ptr<Entity> entity) {
  auto animation = m_game->assets().getAnimation("Explosion");
  animation->setEndable();

  entity->removeComponent<CBoundingBox>();
  entity->removeComponent<CAnimation>();

  entity->addComponent<CAnimation>(*animation);
}

void Scene_Play::spawnHitCoin (std::shared_ptr<Entity> entity) {
  auto coin = m_entities.addEntity("CoinTemp");
  auto animation = m_game->assets().getAnimation("CoinTemp");
  auto position = entity->getComponent<CTransform>().pos;
  auto size = entity->getComponent<CAnimation>().animation.getSize();

  coin->addComponent<CTransform>(
    Vec2(position.x, position.y - (size.y / 2)),
    Vec2(0, -20),
    0
  );

  coin->addComponent<CGravity>(0.75);
  coin->addComponent<CAnimation>(*animation);
  coin->addComponent<CLifespan>(50);
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

    if (action.name() == "SHOOT") {
      if (m_player->getComponent<CInput>().canShoot) {
        spawnBullet();
        m_player->getComponent<CInput>().canShoot = false;
      }
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
      m_player->getComponent<CInput>().canJump = false;
    }

    if (action.name() == "SHOOT") {
      m_player->getComponent<CInput>().canShoot = true;
    }
  }
}

void Scene_Play::sGravity () {
  for (auto entity: m_entities.getEntities()) {
    if (entity->hasComponent<CGravity>()) {
      entity->getComponent<CTransform>().vel += Vec2(0, entity->getComponent<CGravity>().value);
    }
  }
}

void Scene_Play::sMovement () {
  auto& input = m_player->getComponent<CInput>();
  auto& transform = m_player->getComponent<CTransform>();

  transform.vel.x = 0;

  if (input.left == true) {
    transform.vel.x = -m_playerConfig.SPEED;
    transform.scale = { -1, 1 };
  }

  if (input.right == true) {
    transform.vel.x = m_playerConfig.SPEED;
    transform.scale = { 1, 1 };
  }

  if (input.jump == true && input.canJump == true) {
    transform.vel.y = std::max(-m_playerConfig.MAXSPEED, transform.vel.y + m_playerConfig.JUMP);

    if (abs(transform.vel.y) == m_playerConfig.MAXSPEED) {
      input.canJump = false;
    }
  }

  for (auto entity : m_entities.getEntities()) {
    auto& transform = entity->getComponent<CTransform>();
    transform.prevPos = transform.pos;
    transform.pos += transform.vel;
  }

  transform.pos = Vec2(
    std::min(
      std::max(
        32.f, 
        transform.pos.x
      ),
      m_worldWidth - 32.f
    ),
    transform.pos.y
  );

  if (transform.pos.y > m_game->window().getSize().y) {
    m_player->destroy();
    spawnPlayer();
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
        if (m_player->getComponent<CTransform>().prevPos.x < entity->getComponent<CTransform>().pos.x) {
          // Comming from the left
          m_player->getComponent<CTransform>().pos -= Vec2(overlap.x, 0);
        } else {
          // Comming from the right
          m_player->getComponent<CTransform>().pos += Vec2(overlap.x, 0);
        }
      } else if (prevOverlap.x > 0) {
        m_player->getComponent<CTransform>().vel.y = 0;
        
        if (m_player->getComponent<CTransform>().prevPos.y < entity->getComponent<CTransform>().pos.y) {
          // Comming from the top
          m_player->getComponent<CTransform>().pos -= Vec2(0, overlap.y);
          m_player->getComponent<CState>().state = "Stand";

          if (m_player->getComponent<CInput>().jump == false) {
            m_player->getComponent<CInput>().canJump = true;
          }
        } else {
          // Comming from bellow
          m_player->getComponent<CTransform>().pos += Vec2(0, overlap.y);

          if (entity->getComponent<CAnimation>().animation.getName() == "Brick") {
            spawnExplosion(entity);
          
          } else if (entity->getComponent<CAnimation>().animation.getName() == "Coin") {
            auto animation = m_game->assets().getAnimation("CoinHit");
            
            entity->removeComponent<CAnimation>();            
            entity->addComponent<CAnimation>(*animation);

            spawnHitCoin(entity);
          }
        }
      }
    }
  }
}

void Scene_Play::sLifespan () {
  for (auto entity: m_entities.getEntities()) {
    if (entity->hasComponent<CLifespan>()) {
      entity->getComponent<CLifespan>().value -= 1;

      if (entity->getComponent<CLifespan>().value == 0) {
        entity->destroy();
      }
    }  
  }
}

void Scene_Play::sState () {
  if (
    m_player->getComponent<CTransform>().vel.y != 0 ||
    (m_player->getComponent<CTransform>().vel.y == 0 && m_player->getComponent<CState>().state == "Jump")
  ) {
    m_player->getComponent<CState>().state = "Jump";
  
  } else if (
    m_player->getComponent<CTransform>().vel.x != 0 
  ) {
    m_player->getComponent<CState>().state = "Run";
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

    if (animation.animation.hasEnded()) {
      entity->destroy();
    }
  }
}

void Scene_Play::sRender () {
  m_game->window().clear(m_background);

  Vec2 pos = m_player->getComponent<CTransform>().pos;
  auto size = m_game->window().getSize();

  float centerX = std::min(std::max((size.x / 2.0f), pos.x), m_worldWidth - (size.x / 2));
  float centerY = std::min((size.y / 2.0f), pos.y);

  sf::View view(sf::Vector2f(centerX, centerY), sf::Vector2f(size.x, size.y));

  m_game->window().setView(view);

  for (auto entity : m_entities.getEntities()) {
    m_game->window().draw(entity->getComponent<CAnimation>().animation.getSprite());
  }

  m_game->window().display();
}

void Scene_Play::update () {
  m_entities.update();

  sGravity();
  sMovement();
  sAnimation();
  sCollision();
  sLifespan();
  sState();
  sRender();
}
