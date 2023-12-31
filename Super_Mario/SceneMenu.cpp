#include "SceneMenu.h"
#include "Assets.h"
#include "Components.h"
#include "GameEngine.h"
#include "ScenePlay.h"

SceneMenu::SceneMenu(GameEngine* game) : Scene(game) { init(""); }

void SceneMenu::init(const std::string& menuConfig) {
    m_game->window().setKeyRepeatEnabled(false);
    m_menuText.setFont(m_game->getAssets().getFont(eFontTypes::PIXEL));
    m_title = "Not Mario";
    m_menuStrings.emplace_back("Level  1");
    m_levelPaths.emplace_back("./bin/levels/level1.txt");
    m_menuStrings.emplace_back("Level  2");
    m_levelPaths.emplace_back("./bin/levels/level1.txt");
    m_menuStrings.emplace_back("Level  3");
    m_levelPaths.emplace_back("./bin/levels/level1.txt");
}

void SceneMenu::update() {
    sRender();
    m_entityManager.update();
    sUserInput();
}

void SceneMenu::sUserInput() {
    sf::Event event{};
    while (m_game->window().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_game->quit();
        }
        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Escape: {
                m_game->quit();
                break;
            }
            case sf::Keyboard::W: {
                if (m_selectedMenuIndex > 0) {
                    m_selectedMenuIndex--;
                }
                else {
                    m_selectedMenuIndex = m_menuStrings.size() - 1;
                }
                break;
            }
            case sf::Keyboard::S: {
                m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
                break;
            }
            case sf::Keyboard::D: {
                m_game->changeScene(eGameEngineScenes::PLAY,
                    std::make_shared<ScenePlay>(
                        m_game, m_levelPaths.at(m_selectedMenuIndex)));
                break;
            }
            default: {
                break;
            }
            }
        }
    }
}

void SceneMenu::sRender() {
    m_game->window().clear(sf::Color(100, 27, 0));
    // drawing the game title in the top-left of the screen
    m_menuText.setString(m_title);
    m_menuText.setCharacterSize(32);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(sf::Vector2f(m_game->window().getSize().x / 2 -
        m_menuText.getCharacterSize() * 3,
        10));
    m_game->window().draw(m_menuText);
    // draw all the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++) {
        m_menuText.setString(m_menuStrings.at(i));
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White
            : sf::Color::Black);
        m_menuText.setPosition(sf::Vector2f(m_game->window().getSize().x / 2 -
            m_menuText.getCharacterSize() * 2,
            250 + i * 72));
        m_game->window().draw(m_menuText);
    }

    m_menuText.setString(
        "Left :  A        S :  Down        Up : W        D : Do");
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setCharacterSize(12);
    m_menuText.setPosition(
        sf::Vector2f(m_menuText.getCharacterSize(),
            height() - m_menuText.getCharacterSize() * 2));
    m_game->window().draw(m_menuText);

    m_game->window().display();
}

void SceneMenu::onEnd() {}

void SceneMenu::sDoAction(const Action& action) {}
