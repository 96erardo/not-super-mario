#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Vec2.h"

class Component {
  public:
    bool has = false;
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

class CTransform: public Component {
  public:
    Vec2  pos   = { 0,0 };
    Vec2  vel   = { 0,0 };
    Vec2  scale = { 1,1 };
    int   angle = 0;

    CTransform () {};
    CTransform (Vec2 p, Vec2 v, int a):
      pos(p),
      vel(v),
      angle(a)
    {}
};

class CInput: public Component {
  public:
    bool left = false;
    bool right = false;
    bool jump = false;
    bool shoot = false;

    CInput () {};
};

class CAnimation: public Component {
  public:
    Animation animation;

    CAnimation (): animation(Animation(sf::Texture())) {};
    CAnimation (Animation& a): animation(a) {};
};

class CState: public Component {
  public:
    std::string state = "none";

    CState () {}
    CState (const std::string& s): state(s) {};
};

#endif