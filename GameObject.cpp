#include "GameObject.h"

namespace cst
{
    const float gravityAcceleration = 1000.0f;
    const float horizontalAcceleration = 1500.0f;
    const float walkVelocity = 250.0f;
    const float jumpVelocity = -500.0f;
    const float hJumpVelocity = jumpVelocity / 2.0f;
    const float abilityTime = 0.3f;
    const float abilityCooldown = 0.5f;
    const float dashVelocity = 900.0f;
    const float hDashVelocity = dashVelocity / 2.0f;
}

AnimationFrames::AnimationFrames(AnimationInfo animationInfo)
{
    m_RightFrames.reserve(animationInfo.frames);
    m_LeftFrames.reserve(animationInfo.frames);
    for (int i = 0; i < animationInfo.frames; i++)
    {
        m_RightFrames[i] = sf::IntRect(animationInfo.x + i*animationInfo.width, animationInfo.y, animationInfo.width, animationInfo.height);
        m_LeftFrames[i] = sf::IntRect(animationInfo.x + i*animationInfo.width + animationInfo.width, animationInfo.y, -animationInfo.width, animationInfo.height);
    }
}

GameObject::GameObject(float x, float y, std::vector<AnimationInfo> animationInfoList)
{
    m_Sprite.setPosition(x, y);
    m_Sprite.setScale(2.0f, 2.0f);
    m_AnimationInfoList = animationInfoList;
    m_Animations.reserve(animationInfoList.size());
    for (unsigned int i = 0; i < animationInfoList.size(); i++)
    {
        m_Animations[i] = AnimationFrames(animationInfoList[i]);
    }
    m_ElapsedTime = 0.0f;
    m_State = 0;
    m_Frame = 0;
    m_Facing = cst::right;
    updateSpriteRect();
}

void GameObject::updateSpriteRect()
{
    if (m_Facing == cst::right)
    {
        m_Sprite.setTextureRect(m_Animations[m_State].m_RightFrames[m_Frame]);
    } else {
        m_Sprite.setTextureRect(m_Animations[m_State].m_LeftFrames[m_Frame]);
    }

}

void GameObject::updateFrame(const float &dt)
{
    m_ElapsedTime += dt;
    if (m_ElapsedTime >= m_AnimationInfoList[m_State].frameTime)
    {
        m_ElapsedTime -= m_AnimationInfoList[m_State].frameTime;
        m_Frame += 1;
        if (m_Frame >= m_AnimationInfoList[m_State].frames)
        {
            m_Frame = 0;
        }
        updateSpriteRect();
    }
}

void GameObject::resetFrame()
{
    m_ElapsedTime = 0.0f;
    m_Frame = 0;
    updateSpriteRect();
}

void GameObject::update(const float &dt)
{
    updateFrame(dt);
}

Player::Player(float x, float y, std::vector<AnimationInfo> animationInfoList):
    GameObject(x, y, animationInfoList)
{
    m_VelX = 0.0f;
    m_VelY = 0.0f;
    m_abilityTimeElapsed = 0.0f;
    m_abilityCooldownElapsed = cst::abilityCooldown;
}

void Player::checkBorderCollision()
{
    if (m_Sprite.getPosition().x < 0)
    {
        m_Sprite.setPosition(0.0f, m_Sprite.getPosition().y);
        if (m_State == cst::dash || m_State == cst::dive)
        {
            m_VelX = cst::walkVelocity;
            m_VelY = cst::hJumpVelocity;
            m_State = cst::air;
            resetFrame();
        } else {
            m_VelX = 0.0f;
        }
    } else if (m_Sprite.getPosition().x + m_Sprite.getLocalBounds().width * m_Sprite.getScale().x > 640.0f) {

        m_Sprite.setPosition(640.0f - m_Sprite.getLocalBounds().width * m_Sprite.getScale().x, m_Sprite.getPosition().y);
        if (m_State == cst::dash || m_State == cst::dive)
        {
            m_VelX = -cst::walkVelocity;
            m_VelY = cst::hJumpVelocity;
            m_State = cst::air;
            resetFrame();
        } else {
            m_VelX = 0.0f;
        }
    }

    if ((m_State == cst::jump || m_State == cst::air || m_State == cst::dive) && (m_Sprite.getPosition().y + m_Sprite.getLocalBounds().height * m_Sprite.getScale().y > 360.0f))
    {
        if (m_State == cst::dive)
        {
            m_VelY = cst::hJumpVelocity;
            m_State = cst::air;
        } else {
            m_VelX = 0.0f;
            m_VelY = 0.0f;
            m_State = cst::still;
        }
        m_Sprite.setPosition(m_Sprite.getPosition().x, 360.0f - m_Sprite.getLocalBounds().height * m_Sprite.getScale().y);
        resetFrame();
    }
}

void Player::processInput(const float &dt)
{
    const float xAxisPos = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    const float yAxisPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
    const float zAxisPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);

    if (zAxisPos <= -20 && (m_State != cst::dash || m_State != cst::dive) && m_abilityCooldownElapsed >= cst::abilityCooldown) {
        if (yAxisPos <= 50)
        {
            if (m_Facing == cst::right)
            {
                m_VelX = cst::dashVelocity;
            } else {
                m_VelX = -cst::dashVelocity;
            }
            m_VelY = 0;
            m_State = cst::dash;
        } else {
            if (m_Facing == cst::right)
            {
                m_VelX = cst::hDashVelocity;
            } else {
                m_VelX = -cst::hDashVelocity;
            }
            m_VelY = cst::hDashVelocity;
            m_State = cst::dive;
        }
        m_abilityTimeElapsed = 0.0f;
        m_abilityCooldownElapsed = 0.0f;
        resetFrame();
    }

    if (xAxisPos >= 40)
    {
        if (m_State == cst::still)
        {
            m_VelX = cst::walkVelocity;
            m_State = cst::walk;
            resetFrame();
        } else if (m_State == cst::jump || m_State == cst::air) {
            if (m_VelX < cst::walkVelocity)
                m_VelX += cst::horizontalAcceleration * dt;
            if (m_VelX > cst::walkVelocity)
                m_VelX = cst::walkVelocity;
        }
        if (m_State == cst::still || m_State == cst::walk || m_State == cst::jump || m_State == cst::air)
        {
            m_Facing = cst::right;
            resetFrame();
        }
    } else if (xAxisPos <= -40) {
        if (m_State == cst::still || m_State == cst::walk)
        {
            m_VelX = -cst::walkVelocity;
            m_State = cst::walk;
            resetFrame();
        } else if (m_State == cst::jump || m_State == cst::air) {
            if (m_VelX > -cst::walkVelocity)
                m_VelX -= cst::horizontalAcceleration * dt;
            if (m_VelX < -cst::walkVelocity)
                m_VelX = -cst::walkVelocity;
        }
        if (m_State == cst::still || m_State == cst::walk || m_State == cst::jump || m_State == cst::air)
        {
            m_Facing = cst::left;
            resetFrame();
        }
    } else {
        if (m_State == cst::walk)
        {
            m_VelX = 0.0f;
            m_State = cst::still;
            resetFrame();
        } else if (m_State == cst::jump || m_State == cst::air) {
            if (std::fabs(m_VelX) <= cst::horizontalAcceleration * dt)
            {
                m_VelX = 0;
            } else if (m_VelX < 0) {
                m_VelX += cst::horizontalAcceleration * dt;
            } else {
                m_VelX -= cst::horizontalAcceleration * dt;
            }
        }
    }
    if (sf::Joystick::isButtonPressed(0, 0) && (m_State == cst::still || m_State == cst::walk))
    {
        m_VelY = cst::jumpVelocity;
        m_State = cst::jump;
        resetFrame();
    }
}

void Player::update(const float &dt)
{
    GameObject::update(dt);
    if (m_State != cst::dash && m_State != cst::dive && m_abilityCooldownElapsed < cst::abilityCooldown)
    {
        m_abilityCooldownElapsed += dt;
    }
    if (m_State == cst::jump || m_State == cst::air)
    {
        m_Sprite.move(m_VelX * dt, m_VelY * dt);
        m_VelY += cst::gravityAcceleration * dt;
    } else {
        m_Sprite.move(m_VelX * dt, m_VelY * dt);
        if (m_State == cst::dash)
        {
            m_abilityTimeElapsed += dt;
            if (m_abilityTimeElapsed >= cst::abilityTime)
            {
                m_VelX = 0.0f;
                m_State = cst::air;
                resetFrame();
            }
        }
    }
    checkBorderCollision();
}

