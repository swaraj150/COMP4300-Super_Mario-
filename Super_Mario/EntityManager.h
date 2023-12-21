#pragma once
#include "Common.h"
#include "Entity.h"
typedef std::vector<std::shared_ptr<Entity>> EntityVec;
class EntityManager {
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	std::map<eLevelEntitiesType, EntityVec> m_entityMap;
	size_t m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);
public:
	EntityManager();

	void update();
	std::shared_ptr<Entity> addEntity(eLevelEntitiesType entities_type, eLevelEntities entities);

	EntityVec& getEntities();
	EntityVec& getEntities(eLevelEntitiesType type);
};