#include "Enemy.h"
namespace cst
{
    enum knightAttack {k_idle, k_charge};
    enum knightState {k_idle_stand, k_charge_prep, k_charge_attack, k_charge_sit};

}

Knight::Knight(float x, float y, std::vector<AnimationInfo> animationInfoList): GameObject(x, y, animationInfoList)
, m_StateOrigins{sf::Vector2f((float)animationInfoList[0].width/2.0f, (float)animationInfoList[0].height/2.0f)}
{
    m_Sprite.setOrigin(m_StateOrigins[cst::k_idle_stand]);
    m_Attack = cst::k_idle;
}

void Knight::update(const float &dt)
{
    GameObject::update(dt);
    switch(m_Attack) {
        case cst::k_idle: idle(dt); break;
        case cst::k_charge: charge(dt); break;
    }
}

void Knight::idle(const float &dt)
{

}

void Knight::charge(const float &dt)
{

}
