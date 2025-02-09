#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "Components.h"

typedef std::tuple<
  CTransform
> ComponentTuple; 

class Entity {
  size_t         m_id     = 0;
  std::string    m_tag    = "default";
  bool           m_active = true;
  
  public:
    Entity (size_t id, const std::string& tag);

    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CText> cText;
    std::shared_ptr<CMenuItem> cMenuItem;

    size_t id () const;
    std::string tag () const;
    bool isActive () const;
    void destroy ();
};

#endif