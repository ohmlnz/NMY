#include "Physics.h"
#include <iostream>

Vec2 Physics::gridToMidPixel(Vec2& position, std::shared_ptr<Entity> entity)
{
    // TODO: replace 30 by const for grid size
    float positionX = position.x + entity->m_half.x;
    float positionY = position.y + entity->m_half.y;
    return Vec2(positionX, positionY);
}

Vec2 Physics::getOverlap(const Vec2& collideePos, const Vec2& collideeHalf, const Vec2& colliderPos, const Vec2& colliderHalf)
{
    Vec2 delta(abs(collideePos.x - colliderPos.x), abs(collideePos.y - colliderPos.y));
    Vec2 overlap((collideeHalf.x + colliderHalf.x) - delta.x, (collideeHalf.y + colliderHalf.y) - delta.y);
    return overlap;
}

bool Physics::isCollision(const std::shared_ptr<Entity>& collidee, const std::shared_ptr<Entity>& collider)
{   
    Vec2 collideePosition = gridToMidPixel(collidee->m_position, collidee);
    Vec2 colliderPosition = gridToMidPixel(collider->m_position, collider);
    Vec2 overlap = getOverlap(collideePosition, collidee->m_half, colliderPosition, collider->m_half);

    if (overlap.x > 0 && overlap.y > 0)
    {
        collidee->m_overlap = overlap;
        return true;
    }

    return false;
}

short int Physics::resolveOverlap(const std::shared_ptr<Entity>& collidee, const std::shared_ptr<Entity>& collider)
{   
    const Vec2& entityOnePrevPos = collidee->m_previousPosition;
    const Vec2& entityTwoPos = collider->m_position;

    const Vec2& entityOneHalf = collidee->m_half;
    const Vec2& entityTwoHalf = collider->m_half;

    auto previousOverlap = getOverlap(entityOnePrevPos, entityOneHalf, entityTwoPos, entityTwoHalf);
    auto currentOverlap = collidee->m_overlap;

    Vec2& entityOnePosition = collidee->m_position;
    
    if (previousOverlap.y > previousOverlap.x)
    {
       if (entityOnePrevPos.x < entityTwoPos.x)
       {
           entityOnePosition.x -= currentOverlap.x;
           return 0;
       }
       else
       {
           entityOnePosition.x += currentOverlap.x;
           return 1;
       }
    }
    else
    {
        if (entityOnePrevPos.y < entityTwoPos.y)
        {
            entityOnePosition.y -= currentOverlap.y;
            return 2;
        }
        else
        {
            entityOnePosition.y += currentOverlap.y;
            return 3;
        }
    }

    return 0;
}

bool Physics::isLineIntersect(Vec2& a, Vec2& b, Vec2& c, Vec2& d)
{
    Vec2 r = (b - a);
    Vec2 s = (d - c);
    float rxs = r * s;
    Vec2 cma = c - a;
    float t = cma * s / rxs;
    float u = cma * r / rxs;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        return true;
    }

    return false;
}
