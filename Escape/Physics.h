#pragma once
#include <memory>
#include "Entity.h"

Vec2 gridToMidPixel(Vec2& position, std::shared_ptr<Entity> entity);
Vec2 getOverlap(const Vec2& collideePos, const Vec2& collideeHalf, const Vec2& colliderPos, const Vec2& colliderHalf);
bool isCollision(const std::shared_ptr<Entity>& collidee, const std::shared_ptr<Entity>& collider);
int resolveOverlap(const std::shared_ptr<Entity>& collidee, const std::shared_ptr<Entity>& collider);
bool isLineIntersect(Vec2& a, Vec2& b, Vec2& c, Vec2& d);