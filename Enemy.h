#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include "GameObject.h"

class Knight: public GameObject
{
private:
    const sf::Vector2f m_StateOrigins[3][2];
    const Player& player;
    int m_Attack;
    void updateOrigin();
    void idle(const float &dt);
    void charge(const float &dt);
public:
    Knight(float x, float y, std::vector<AnimationInfo> animationInfoList, const Player &player);
    void update(const float &dt);
};

#endif // ENEMY_H_INCLUDED
