#include "HitDetection.h"

Hitbox::Hitbox(std::vector<sf::Vector2f> points){
    m_Vertices = points;
    m_Normals.reserve(points.size());
    for (unsigned int i=0; i < points.size()-1; i++)
    {
        m_Normals[i] = sf::Vector2f(points[i+1].y - points[i].y, points[i].x - points[i+1].x);
    }
    m_Normals[points.size()-1] = sf::Vector2f(points[0].y - points[points.size()-1].y, points[points.size()-1].x - points[0].x);
}
