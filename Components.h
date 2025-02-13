#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Vec2.h"

class CTransform {
  public:
    Vec2 pos;
    Vec2 vel;
    int  angle;

    CTransform (Vec2 p): 
      pos(p), 
      vel({0,0}),
      angle(0) 
    {}

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

class CAnimation {
  public:
    Animation animation;

    CAnimation (Animation& a): animation(a) {}
};

#endif