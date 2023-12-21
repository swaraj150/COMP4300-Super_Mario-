#include "GameEngine.h"
#include "Assets.h"
#include "SceneMenu.h"
//#include "ScenePlay.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <iostream>
GameEngine::GameEngine(const std::string& path) {
	init(path);
};

void GameEngine::init(const std::string& parameter) {
	// Your implementation here
	m_assets.loadFromFile(parameter);
	m_window.create(sf::VideoMode(1200, 760), "Definitely Not Mario");
	//m_window.create(sf::VideoMode(1200, 760), "Definitely Not Mario", sf::Style::Fullscreen);
	m_window.setFramerateLimit(60);
	changeScene(MENU, std::make_shared<SceneMenu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene() {
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning() {
	return m_running && m_window.isOpen();
}


void GameEngine::run() {
	while (isRunning()) {
		
		update();
	}
}

void GameEngine::sUserInput() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			quit();
		}
		
		if (event.type == sf::Event::KeyPressed || event.type==sf::Event::KeyReleased) {
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) continue;
			eActionTypes actionType = event.type == sf::Event::KeyPressed ? START : END;
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

void GameEngine::changeScene(eGameEngineScenes scenes, const std::shared_ptr<Scene>& scene, bool endCurrentScene) {
	m_currentScene = scenes;
	if (scene) {
		std::cout << "PLAY Scene\n";
		m_sceneMap[scenes] = scene;
	}
}
sf::RenderWindow& GameEngine::window() {
	// Your implementation here
	return m_window;
}

void GameEngine::update() {
	sUserInput();
	m_sceneMap.at(m_currentScene)->update();
	m_currentFrames++;
}

const Assets& GameEngine::getAssets() const { return m_assets; }

void GameEngine::quit() { m_running = false; }

std::shared_ptr<Scene> GameEngine::getScene(eGameEngineScenes scenes) {
	return m_sceneMap[scenes];
}
