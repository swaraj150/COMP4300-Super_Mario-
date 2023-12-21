#include "Entity.h"


Entity::Entity(eLevelEntitiesType& id, eLevelEntities& tag) :m_id(id), m_tag(tag) {};
Entity::Entity() {};

void Entity::destroy() {
	m_Active = false;
}

bool Entity::isActive() const{
	return m_Active;
}
eLevelEntitiesType Entity::id() const{
	return m_id;
}
eLevelEntities Entity::tag() const {
	return m_tag;
}