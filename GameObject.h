#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <vector>

namespace cst
{
    extern const float gravityAcceleration;
    extern const float horizontalAcceleration;
    extern const float walkVelocity;
    extern const float jumpVelocity;
    extern const float hJumpVelocity;
    extern const float abilityTime;
    extern const float abilityCooldown;
    extern const float dashVelocity;
    extern const float hDashVelocity;
    enum playerState {still, walk, jump, air, dash, dive};
    enum direction {left, right};
}

struct AnimationInfo
{
    int x;
    int y;
    int width;
    int height;
    int frames;
    float frameTime;
};

struct AnimationFrames
{
    AnimationFrames(AnimationInfo animationInfo);
    std::vector<sf::IntRect> m_RightFrames;
    std::vector<sf::IntRect> m_LeftFrames;
};

class GameObject
{
private:
    std::vector<AnimationInfo> m_AnimationInfoList;
    std::vector<AnimationFrames> m_Animations;
    float m_ElapsedTime;
    void updateFrame(const float &dt);

protected:
    int m_State;
    int m_Frame;
    int m_Facing;
    void updateSpriteRect();
    void resetFrame();

public:
    GameObject(float x, float y, std::vector<AnimationInfo> animationInfoList);
    sf::Sprite m_Sprite;
    void update(const float &dt);
};


class Player: public GameObject
{
private:
    float m_VelX;
    float m_VelY;
    float m_abilityTimeElapsed;
    float m_abilityCooldownElapsed;
    void checkBorderCollision();
public:
    Player(float x, float y, std::vector<AnimationInfo> animationInfoList);
    void processInput(const float &dt);
    void update(const float &dt);
};



#endif // GAMEOBJECT_H_INCLUDED
