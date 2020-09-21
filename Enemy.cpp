#include "Enemy.h"
namespace cst
{
    enum knightAttack {k_idle, k_charge};
    enum knightState {k_idle_stand, k_charge_prep, k_charge_attack, k_charge_sit};
}

Knight::Knight(float x, float y, std::vector<AnimationInfo> animationInfoList, const Player& player): GameObject(x, y, animationInfoList)
, m_StateOrigins{{sf::Vector2f(15.0f, 22.0f), sf::Vector2f(37.0f, 22.0f)}, {sf::Vector2f(37.0f, 43.0f), sf::Vector2f(37.0f, 43.0f)}, {sf::Vector2f(44.0f, 22.0f), sf::Vector2f(18.0f, 22.0f)}}
, player(player)
{
    updateOrigin();
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

void Knight::updateOrigin()
{
    m_Sprite.setOrigin(m_StateOrigins[m_State][m_Facing]);
}

void Knight::idle(const float &dt)
{
    static float idleTime = 0.0f;
    idleTime += dt;

    if (idleTime < 3.0f){
        if (player.m_Sprite.getPosition().x + player.m_Sprite.getLocalBounds().width * player.m_Sprite.getScale().x / 2.0f < m_Sprite.getPosition().x)
        {
            if(m_Facing != cst::left)
            {
                m_Facing = cst::left;
                updateOrigin();
                resetFrame();
            }
        } else {
            if(m_Facing != cst::right)
            {
                m_Facing = cst::right;
                updateOrigin();
                resetFrame();
            }
        }
    } else {
        idleTime = 0.0f;
        m_Attack = cst::k_charge;
        m_State = cst::k_charge_prep;
        updateOrigin();
        resetFrame();
    }
}

void Knight::charge(const float &dt)
{
    static float chargeTime = 0.0f;
    switch(m_State) {
        case cst::k_charge_prep:
            if (chargeTime < 0.65f){
                chargeTime += dt;
            } else {
                m_State = cst::k_charge_attack;
                updateOrigin();
                resetFrame();
            }
            break;
    }
}
