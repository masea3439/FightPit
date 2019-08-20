#include "GameObject.h"

namespace cst
{
    float frameTime = 0.5f;
    float acceleration = 0.5f;
}

AnimationFrames::AnimationFrames(int y, int width, int height, int frames)
{
    m_Frames.reserve(frames);
    for (int i = 0; i < frames; i++)
    {
        m_Frames[i] = sf::IntRect(i*width, y, width, height);
    }
}

GameObject::GameObject(float x, float y, int width, int height, std::vector<int> framesPerAnimation)
{
    m_Sprite.setPosition(x, y);
    m_FramesPerAnimation = framesPerAnimation;
    m_Animations.reserve(framesPerAnimation.size());
    for (unsigned int i = 0; i < framesPerAnimation.size(); i++)
    {
        m_Animations[i] = AnimationFrames(height*i ,width, height, framesPerAnimation[i]);
    }
    m_State = 0;
    m_Frame = 0;
    updateSpriteRect();
}

void GameObject::updateSpriteRect()
{
    m_Sprite.setTextureRect(m_Animations[m_State].m_Frames[m_Frame]);
}

void GameObject::updateFrame(float dt)
{
    static float elapsedTime = 0.0f;
    elapsedTime += dt;
    if (elapsedTime >= cst::frameTime)
    {
        elapsedTime -= cst::frameTime;
        m_Frame += 1;
        if (m_Frame >= m_FramesPerAnimation[m_State])
        {
            m_Frame = 0;
        }
        updateSpriteRect();
    }
}

void GameObject::resetFrame()
{
    m_Frame = 0;
    updateSpriteRect();
}

void GameObject::update(float dt)
{
    updateFrame(dt);
}

Player::Player(float x, float y, int width, int height, std::vector<int> framesPerAnimation):
    GameObject(x, y, width, height, framesPerAnimation)
{
    m_VelX = 0.0f;
    m_VelY = 0.0f;
}

void Player::checkBorderCollision()
{
    if (m_Sprite.getPosition().x < 0)
    {
        m_VelX = 0.0f;
        m_Sprite.setPosition(0.0f, m_Sprite.getPosition().y);
        if (m_State == cst::dash || m_State == cst::dive)
        {
            m_State = cst::air;
        }
    } else if (m_Sprite.getPosition().x + m_Sprite.getScale().x > 640.0f) {
        m_VelX = 0.0f;
        m_Sprite.setPosition(640.0f - m_Sprite.getScale().x, m_Sprite.getPosition().y);
        if (m_State == cst::dash || m_State == cst::dive)
        {
            m_State = cst::air;
        }
    }

    if ((m_State == cst::jump || m_State == cst::air) && (m_Sprite.getPosition().y + m_Sprite.getScale().y > 360.0f))
    {
        m_VelX = 0.0f;
        m_VelY = 0.0f;
        m_Sprite.setPosition(m_Sprite.getPosition().x, 360.0f - m_Sprite.getScale().y);
        m_State = cst::still;
        m_Frame = 0;
        updateSpriteRect();
    }
}

void Player::processInput()
{
    float xAxisPos = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    float yAxisPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
    if (xAxisPos >= 40) {
        if (m_State == cst::still)
        {
            m_VelX = 60.0f;
            m_State = cst::walk;
            resetFrame();
        }
    } else if (xAxisPos <= -40) {
        if (m_State == cst::still || m_State == cst::walk)
        {
            m_VelX = -60.0f;
            m_State = cst::walk;
            resetFrame();
        }
    } else {
        if (m_State == cst::walk)
        {
            m_VelX = 0.0f;
            m_State = cst::still;
            resetFrame();
        }
    }
}

void Player::update(float dt)
{
    GameObject::update(dt);
    if (m_State == cst::jump || m_State == cst::air)
    {
        m_Sprite.move(m_VelX * dt, m_VelY * dt);
        m_VelY += cst::acceleration * dt;
    } else {
        m_Sprite.move(m_VelX * dt, m_VelY * dt);
    }
    checkBorderCollision();
}

