#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vec2.h"
#include "Entity.h"

class Physics {
  public:
    Vec2 GetOverlap (std::shared_ptr<Entity>, std::shared_ptr<Entity>) const; 
    Vec2 GetPreviousOverlap (std::shared_ptr<Entity>, std::shared_ptr<Entity>) const; 
};

#endif