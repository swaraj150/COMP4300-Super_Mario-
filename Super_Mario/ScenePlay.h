#pragma once
#include "Scene.h"
#include"Common.h"
#include "Physics.h"
class ScenePlay :public Scene {
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
    };

    struct EnemyConfig {
        float X, Y, SPEED = 2;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;

    const Vec2 m_gridSize = { 64, 64 };
    sf::Text m_gridText;
    Physics m_physics;

    void init(const std::string& levelPath);
    void loadLevel(const std::string& filePath);
    //void loadMap(const std::string& filePath);

    void update() override;
    void onEnd() override;

    void spawnPlayer();
    void spawnBullet(const std::shared_ptr<Entity>& entity);
    void spawnEnemy();

    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

    void sMovement();
    void sLifespan();
    void sCollision();
    void sDoAction(const Action& action) override;
    void sAnimation();
    void sRender() override;
    
public:
    ScenePlay(GameEngine* gameEngine, const std::string& levelPath);

};