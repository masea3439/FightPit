#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <vector>

namespace cst
{
    extern float frameTime;
    extern float acceleration;
    enum playerState {still, walk, jump, air, dash, dive};
}

struct AnimationFrames
{
    AnimationFrames(int y, int width, int height, int frames);
    std::vector<sf::IntRect> m_Frames;
};

class GameObject
{
private:
    std::vector<int> m_FramesPerAnimation;
    std::vector<AnimationFrames> m_Animations;
    void updateFrame(float dt);

protected:
    int m_State;
    int m_Frame;
    void updateSpriteRect();
    void resetFrame();

public:
    GameObject(float x, float y, int width, int height, std::vector<int> framesPerAnimation);
    sf::Sprite m_Sprite;
    void update(float dt);
};


class Player: public GameObject
{
private:
    float m_VelX;
    float m_VelY;
    void checkBorderCollision();
public:
    Player(float x, float y, int width, int height, std::vector<int> framesPerAnimation);
    void processInput();
    void update(float dt);
};



#endif // GAMEOBJECT_H_INCLUDED
