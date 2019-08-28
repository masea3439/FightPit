#ifndef HITDETECTION_H_INCLUDED
#define HITDETECTION_H_INCLUDED
#include <vector>
#include <SFML/Graphics.hpp>

struct Hitbox {
    Hitbox(std::vector<sf::Vector2f> points);
    std::vector<sf::Vector2f> m_Vertices;
    std::vector<sf::Vector2f> m_Normals;
};

#endif // HITDETECTION_H_INCLUDED
