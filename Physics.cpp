#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap (std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) const {
  float dx = abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x);
  float dy = abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y);
  float xo = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x;
  float yo = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y;

  return Vec2(xo - dx, yo - dy);
}

Vec2 Physics::GetPreviousOverlap (std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) const {
  float dx = abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x);
  float dy = abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y);
  float xo = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x;
  float yo = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y;
  
  return Vec2(xo - dx, yo - dy);
}
