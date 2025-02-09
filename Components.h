#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "Vec2.h"
#include "SFML/Graphics.hpp"

class CTransform {
  public:
    Vec2 pos;
    Vec2 vel;
    int  angle;

    CTransform (Vec2 p, Vec2 v, int a):
      pos(p),
      vel(v),
      angle(a)
    {}
};

class CText {
  public:
    sf::Text text;

    CText (sf::Text t): text(t) {}
};

class CMenuItem {
  public:
    int         index;
    bool        selected;
    std::string value;

    CMenuItem (int i, bool s, const std::string& v):
      index(i), 
      selected(s),
      value(v)
    {}
};

#endif