#include "Physics.h"
#include <iostream>

Vec2 Physics::gridToMidPixel(Vec2& position, std::shared_ptr<Entity> entity)
{
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
    Vec2 collideePreviousPosition = gridToMidPixel(collidee->m_previousPosition, collidee);
    Vec2 colliderPosition = gridToMidPixel(collider->m_position, collider);
    
    Vec2 overlap = getOverlap(collideePosition, collidee->m_boundingBox / 2, colliderPosition, collider->m_boundingBox / 2);

    if (overlap.x > 0 && overlap.y > 0)
    {
        collidee->m_overlap = overlap;
        return true;
    }

    return false;
}

short int Physics::resolveOverlap(const std::shared_ptr<Entity>& collidee, const std::shared_ptr<Entity>& collider)
{   
    Vec2 colliderPosition = gridToMidPixel(collider->m_position, collider);
    Vec2 collideePreviousPosition = gridToMidPixel(collidee->m_previousPosition, collidee);
    Vec2 previousOverlap = getOverlap(collideePreviousPosition, collidee->m_boundingBox / 2, colliderPosition, collider->m_boundingBox / 2);

    if (previousOverlap.y > 0)
    {
        if (collideePreviousPosition.x < colliderPosition.x)
        {
            //std::cout << "LEFT" << std::endl;
            collidee->m_position.x -= collidee->m_overlap.x;
        }
        else
        {
            //std::cout << "RIGHT" << std::endl;
            collidee->m_position.x += collidee->m_overlap.x;
        }
    }
    else if (previousOverlap.x > 0)
    {
        if (collideePreviousPosition.y < colliderPosition.y)
        {
            //std::cout << "TOP" << std::endl;
            collidee->m_position.y -= collidee->m_overlap.y;
        }
        else
        {
            //std::cout << "BOTTOM" << std::endl;
            collidee->m_position.y += collidee->m_overlap.y;
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
