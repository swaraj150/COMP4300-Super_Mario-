#pragma once


#include "Common.h"
#include "Scene.h"
//#include "ScenePlay.h"
#include <deque>
#include <map>
#include <memory>

class SceneMenu : public Scene {

public:
	EntityManager m_entityManager;
	std::string m_title;
	std::vector<std::string> m_menuStrings;
	std::vector<std::string> m_levelPaths;
	sf::Text m_menuText;
	size_t m_selectedMenuIndex = 0;

	void init(const std::string& menuConfig);
	void update() override;
	void sUserInput();
	void sRender() override;
	void onEnd() override;
	void sDoAction(const Action& action) override;

public:
	explicit SceneMenu(GameEngine* game);
};
