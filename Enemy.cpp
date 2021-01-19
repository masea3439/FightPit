#include "Enemy.h"

namespace kni
{
    enum attack {k_idle, k_charge};
    enum state {k_idle_stand, k_charge_prep, k_charge_attack, k_charge_sit};
    const float chargeVelocity = 500.0f;
    const float recoilVelocity = 250.0f;
    const float jumpVelocity = -250.0f;
    const float hitboxHeight = 90.0f;
    const float hitboxLength = 45.0f;
    const float knightGravityAcceleration = 500.0f;
}

Knight::Knight(float x, float y, std::vector<AnimationInfo> animationInfoList, const Player& player): GameObject(x, y, animationInfoList)
, m_StateOrigins{{sf::Vector2f(15.0f, 22.0f), sf::Vector2f(37.0f, 22.0f)}, {sf::Vector2f(37.0f, 43.0f), sf::Vector2f(37.0f, 43.0f)},
                 {sf::Vector2f(44.0f, 22.0f), sf::Vector2f(18.0f, 22.0f)}, {sf::Vector2f(37.0f, 43.0f), sf::Vector2f(37.0f, 43.0f)}}
, player(player)
{
    m_VelX = 0;
    m_VelY = 0;
    updateOrigin();
    m_Attack = kni::k_idle;
    m_Hitbox = sf::FloatRect(x-kni::hitboxLength/2, y-kni::hitboxHeight/2, kni::hitboxLength, kni::hitboxHeight);
}

void Knight::update(const float &dt)
{
    GameObject::update(dt);
    switch(m_Attack) {
        case kni::k_idle: idle(dt); break;
        case kni::k_charge: charge(dt); break;
    }
    float shiftX = m_VelX * dt;
    float shiftY = m_VelY * dt;
    m_Sprite.move(shiftX, shiftY);
    m_Hitbox.left += shiftX;
    m_Hitbox.top += shiftY;
    if (m_Hitbox.top + kni::hitboxHeight > 360.0f) {
        m_Sprite.setPosition(m_Sprite.getPosition().x, 360.0f-kni::hitboxHeight/2);
        m_Hitbox.top = 360.0f-kni::hitboxHeight;
        m_VelY = 0;
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
        m_Attack = kni::k_charge;
        m_State = kni::k_charge_prep;
        updateOrigin();
        resetFrame();
    }
}

void Knight::charge(const float &dt)
{
    static float chargeTime = 0.0f;
    switch(m_State) {
        case kni::k_charge_prep:
            if (chargeTime < 0.65f){
                chargeTime += dt;
            } else {
                m_State = kni::k_charge_attack;
                if (m_Facing == cst::right) {
                    m_VelX = kni::chargeVelocity;
                } else {
                    m_VelX = -kni::chargeVelocity;
                }
                updateOrigin();
                resetFrame();
            }
            break;
        case kni::k_charge_attack:
            if (m_Hitbox.left <= 0.0f || m_Hitbox.left+m_Hitbox.width >= 640.0f) {
                chargeTime = 0.0f;
                if (m_Hitbox.left <= 0.0f) {
                    m_VelX = kni::recoilVelocity;
                } else {
                    m_VelX = -kni::recoilVelocity;
                }
                m_VelY = kni::jumpVelocity;
                m_State = kni::k_charge_sit;
                updateOrigin();
                resetFrame();
            }
            break;
        case kni::k_charge_sit:
            if (chargeTime < 1.0f){
                m_VelY += kni::knightGravityAcceleration * dt;
                chargeTime += dt;
            } else {
                chargeTime = 0.0f;
                m_VelX = 0;
                m_Attack = kni::k_idle;
                m_State = kni::k_idle_stand;
                updateOrigin();
                resetFrame();
            }
            break;
    }
}
