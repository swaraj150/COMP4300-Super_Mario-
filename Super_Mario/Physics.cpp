#include "Physics.h"

//Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
//	Vec2 delta = { std::abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x),
//		std::abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y) };
//	float ox = a->getComponent<CBoundingBox>().size.x / 2.0f + b->getComponent<CBoundingBox>().size.x / 2.0f + delta.x;
//	float oy = a->getComponent<CBoundingBox>().size.y / 2.0f + b->getComponent<CBoundingBox>().size.y / 2.0f + delta.y;
//	return Vec2(ox, oy);
//}
//
//Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
//	Vec2 delta = { std::abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x),
//		std::abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y) };
//	float ox = a->getComponent<CBoundingBox>().size.x / 2.0f + b->getComponent<CBoundingBox>().size.x / 2.0f + delta.x;
//	float oy = a->getComponent<CBoundingBox>().size.y / 2.0f + b->getComponent<CBoundingBox>().size.y / 2.0f + delta.y;
//	return Vec2(ox, oy);
//}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // TODO: return the overlap rectangle size of the bounding boxes of entity a
    // and b
    Vec2 aPos = { a->getComponent<CTransform>().pos };
    Vec2 bPos = { b->getComponent<CTransform>().pos };
    Vec2 aSize = a->getComponent<CBoundingBox>().size;
    Vec2 bSize = b->getComponent<CBoundingBox>().size;

    Vec2 delta = { abs(aPos.x - bPos.x), abs(aPos.y - bPos.y) };
    float overlapX = (aSize.x / 2 + bSize.x / 2) - delta.x;
    float overlapY = (aSize.y / 2 + bSize.y / 2) - delta.y;

    return { overlapX, overlapY };
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a,
    std::shared_ptr<Entity> b) {
    // TODO: return the previous overlap rectangle size of the bounding boxes of
    // entity a and b
    //       previous overlap the entity's previous position
    Vec2 aPos = { a->getComponent<CTransform>().prevPos };
    Vec2 bPos = { b->getComponent<CTransform>().prevPos };
    Vec2 aSize = a->getComponent<CBoundingBox>().size;
    Vec2 bSize = b->getComponent<CBoundingBox>().size;

    Vec2 delta = { abs(aPos.x - bPos.x), abs(aPos.y - bPos.y) };
    float overlapX = (aSize.x / 2 + bSize.x / 2) - delta.x;
    float overlapY = (aSize.y / 2 + bSize.y / 2) - delta.y;

    return { overlapX, overlapY };
}
