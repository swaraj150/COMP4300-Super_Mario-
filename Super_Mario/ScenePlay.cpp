#include "ScenePlay.h"
#include "Common.h"
#include<cassert>
#include "SceneMenu.h"
ScenePlay::ScenePlay(GameEngine* game, const std::string& path) :Scene(game), m_levelPath(path) {
	// std::cout << "ScenePlay constructor" << std::endl;
	init(m_levelPath);
}

void ScenePlay::init(const std::string& path) {
	registerAction(sf::Keyboard::P, PAUSE);
	registerAction(sf::Keyboard::Escape, QUIT);
	registerAction(sf::Keyboard::T, TOGGLE_TEXTURE);
	registerAction(sf::Keyboard::G, TOGGLE_GRID);
	registerAction(sf::Keyboard::C, TOGGLE_COLLISION);
	registerAction(sf::Keyboard::Up, UP);
	registerAction(sf::Keyboard::Left, LEFT);
	registerAction(sf::Keyboard::Right, RIGHT);
	registerAction(sf::Keyboard::Space,FIRE);

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->getAssets().getFont(PIXEL));
	// std::cout << "ScenePlay init" << std::endl;

	loadLevel(path);
}

Vec2 ScenePlay::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) {
	// Calculate the center position using the entity's animation size
	float x = gridX + (m_gridSize.x - entity->getComponent<CAnimation>().animation.getSize().x) / 2;
	float y = (m_game->window().getSize().y - gridY) - (m_gridSize.y + entity->getComponent<CAnimation>().animation.getSize().y) / 2;
	return Vec2(x, y);
	/*float x, y;
	x = gridX + entity->getComponent<CAnimation>().animation.getSize().x / 2;
	y = (m_game->window().getSize().y - gridY) - entity->getComponent<CAnimation>().animation.getSize().y / 2;*/
	//return Vec2(x, y);
}



void ScenePlay::loadLevel(const std::string& filePath) {
	// std::cout << "ScenePlay loadlevel" << std::endl;

	m_entityManager = EntityManager();// reset the entity manager every time we load a level

	std::ifstream file(filePath);
	//eLevelEntitiesType levelEntitiesType;
	eLevelEntities levelEntities;
	eAnimationTypes animationTypes;
	std::string str;
	file >> str;

	while (file.good() && str != " ") {
		// std::cout << "ScenePlay while" << std::endl;
		//file >> str;
		//Intstr = std::stoi(str);
		//levelEntities = static_cast<eLevelEntities>(Intstr);
		//std::cout << "str " << str << "\n";

		switch (toascii(str[0]) - '0')
		{
		case eLevelEntitiesType::TILE: {
			//std::cout << "TILE\n";
			std::string name;
			size_t gx, gy;
			file >> name >> gx >> gy;
			//std::cout << "name " << name << "gx "<<gx<<"gy "<<gy<<"\n";
			levelEntities = static_cast<eLevelEntities>(std::stoi(name));
			animationTypes = static_cast<eAnimationTypes>(levelEntities);
			// std::cout << animationTypes << "\n";
			if (levelEntities < eLevelEntities::LAST && levelEntities >= 0) {
				auto entity = m_entityManager.addEntity(TILE, levelEntities);
				entity->addComponent<CTransform>(gridToMidPixel((float)64 * gx, (float)64 * gy, entity));
				entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
				entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(animationTypes), true);
				entity->addComponent<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
			}
			else {
				std::cerr << "unknown entity" << std::endl;
				getline(file, str);
			}
			break;
		}
		case eLevelEntitiesType::DEC: {
			// std::cout << "DEC\n";
			std::string name;
			size_t gx, gy;
			file >> name >> gx >> gy;
			//std::cout << "name " << name << "gx " << gx << "gy " << gy << "\n";

			levelEntities = static_cast<eLevelEntities>(std::stoi(name));
			animationTypes = static_cast<eAnimationTypes>(levelEntities);
			if (levelEntities < eLevelEntities::LAST && levelEntities >= 0) {
				auto entity = m_entityManager.addEntity(DEC, levelEntities);
				entity->addComponent<CTransform>(gridToMidPixel((float)64 * gx, (float)64 * gy, entity));
				entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
				entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(animationTypes), true);
			}
			else {
				std::cerr << "unknown entity" << std::endl;
				getline(file, str);
			}
			break;

		}
		case eLevelEntitiesType::DYNAMIC: {
			//std::cout << "DYNAMIC\n";
			std::string name;
			file >> name;
			levelEntities = static_cast<eLevelEntities>(std::stoi(name));
			if (levelEntities == PLAYER) {
				file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY >> m_playerConfig.SPEED
					>> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY;
			}
			else if (levelEntities == GOOMBA) {
				file >> m_enemyConfig.X >> m_enemyConfig.Y >> m_playerConfig.SPEED;
				spawnEnemy();
					
			}
			break;

		}
		default:

			break;
		}

		file >> str;
	}

	spawnPlayer();
	// std::cout << m_player->getComponent<CTransform>().pos.x << m_player->getComponent<CTransform>().pos.y << std::endl;


}

void ScenePlay::spawnPlayer() {
	m_player = m_entityManager.addEntity(eLevelEntitiesType::DYNAMIC, eLevelEntities::PLAYER);
	m_player->addComponent<CGravity>().gravity = m_playerConfig.GRAVITY;
	m_player->addComponent<CAnimation>(
		m_game->getAssets().getAnimation(eAnimationTypes::STAND), true);
	m_player->addComponent<CTransform>(
		gridToMidPixel((float)64 * m_playerConfig.X, (float)64 * m_playerConfig.Y, m_player));
	m_player->addComponent<CBoundingBox>(
		Vec2(m_playerConfig.CX, m_playerConfig.CY));
	m_player->getComponent<CTransform>().scale.x =
		64 / m_player->getComponent<CAnimation>().animation.getSize().x;
	m_player->getComponent<CTransform>().scale.y =
		64 / m_player->getComponent<CAnimation>().animation.getSize().y;


	//this code sets the scale of the player's transform based on the ratio of a desired 64*64 size. The result is used to scale the player's rendering size in the game world.
}

void ScenePlay::spawnBullet(const std::shared_ptr<Entity>& entity) {
	Vec2 entityPos = entity->getComponent<CTransform>().pos;
	(entity->getComponent<CTransform>().scale.x < 0) ? entityPos.x -= 64
		: entityPos.x += 64;
	// TODO: this should spawn a bullet at the given entity, going in the
	// direction the entity is facing
	auto bullet = m_entityManager.addEntity(DYNAMIC, eLevelEntities::BULLET);
	bullet->addComponent<CAnimation>(
		m_game->getAssets().getAnimation(eAnimationTypes::BUSTER), true);
	bullet->addComponent<CTransform>(entityPos);
	bullet->addComponent<CBoundingBox>(Vec2(bullet->getComponent<CAnimation>().animation.getSize().x / 2.0, bullet->getComponent<CAnimation>().animation.getSize().y / 2.0));
	bullet->getComponent<CTransform>().velocity.x =
		entity->getComponent<CTransform>().scale.x * m_playerConfig.SPEED * 6;
	bullet->addComponent<CLifeSpan>(1.8231);
}

void ScenePlay::spawnEnemy() {
	// TODO(due: Friday): implement all the spawn enemy stuff
	// outh use the goomba animation and be added as Tile entity
	// see the current implementation of how the goomba is built
	// form the loadLevel method
	auto enemy = m_entityManager.addEntity(DYNAMIC, GOOMBA);
	enemy->addComponent<CAnimation>(m_game->getAssets().getAnimation(aniGOOMBA), true);
	enemy->addComponent<CTransform>(
		gridToMidPixel(m_enemyConfig.X * 64, m_enemyConfig.Y * 64, enemy));
	enemy->getComponent<CTransform>().prevPos =
		enemy->getComponent<CTransform>().pos;
	enemy->addComponent<CBoundingBox>(
		enemy->getComponent<CAnimation>().animation.getSize());
	enemy->getComponent<CTransform>().velocity.x = m_enemyConfig.SPEED;
}

void ScenePlay::sRender() {
	// color the background darker, so you know that the game is paused
	//std::cout << "Rendering initiated\n";

	if (!m_paused) {
		m_game->window().clear(sf::Color(87, 174, 209));
	}
	else {
		m_game->window().clear(sf::Color(50, 50, 150));
	}

	// set the viewport of the window to be centered on the player if its far
	// enough right
	auto& pPos = m_player->getComponent<CTransform>().pos;
	float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
	sf::View view = m_game->window().getView();
	view.setCenter(windowCenterX,
		m_game->window().getSize().y - view.getCenter().y);
	m_game->window().setView(view);

	// draw all Entity textures / animations
	if (m_drawTextures) {
		for (auto& e : m_entityManager.getEntities()) {

			auto& transform = e->getComponent<CTransform>();
			if (e->hasComponent<CAnimation>()) {
				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}
	if (m_drawGrid) {
		// Draw vertical grid lines
		for (float x = 0; x <= width(); x += m_gridSize.x)
			drawLine(Vec2(x, height()), Vec2(x, 0));

		// Draw horizontal grid lines
		for (float y = height(); y >= 0; y -= m_gridSize.y)
			drawLine(Vec2(0, y), Vec2(width(), y));

		// Draw grid text
		for (int i = 0; i <= width(); i += m_gridSize.x) {
			for (int j = height(); j >= 0; j -= m_gridSize.y) {
				m_gridText.setPosition(i, j);
				m_gridText.setCharacterSize(12);
				m_gridText.setString("(" + std::to_string((int)i / static_cast<int>(m_gridSize.x)) + "," +
					std::to_string((int)(height() - j) / static_cast<int>(m_gridSize.y) - 1) + ")");
				m_game->window().draw(m_gridText);
			}
		}
	}
	if (m_drawCollision) {
		for (auto& e : m_entityManager.getEntities()) {
			if (e->hasComponent<CBoundingBox>()) {
				auto& box = e->getComponent<CBoundingBox>();
				auto& transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.size.x / 2, box.size.y / 2));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}
	
	m_game->window().display();
}


void ScenePlay::update() {
	m_entityManager.update();

	if (!m_player->isActive()) {

		spawnPlayer();

	}

	sMovement();
	sCollision();
	sLifespan();
	sAnimation();
	sRender();
	
}

void ScenePlay::sMovement() {
	//m_player->getComponent<CTransform>().velocity = {0.0,0.0};
	//Vec2 playerVelocity(0, 0);
	Vec2 playerVelocity(0, m_player->getComponent<CTransform>().velocity.y);
	float g = m_playerConfig.GRAVITY;
	if (!m_player->getComponent<CInput>().up && !m_player->getComponent<CInput>().right && !m_player->getComponent<CInput>().left && !m_player->getComponent<CInput>().right) {
		m_player->getComponent<CState>().state = STANDING;
		m_player->addComponent<CAnimation>(
			m_game->getAssets().getAnimation(eAnimationTypes::STAND), true);
		if (m_player->getComponent<CInput>().shoot == true) {
			m_player->addComponent<CAnimation>(
				m_game->getAssets().getAnimation(eAnimationTypes::SHOT), true);
		}
	}
	
	if (m_player->getComponent<CInput>().up) {
		playerVelocity.y = -m_playerConfig.JUMP;
		if (m_player->getComponent<CTransform>().pos.y <=
			m_player->getComponent<CTransform>().aimPos) {
			m_player->getComponent<CInput>().up = false;
			m_player->getComponent<CTransform>().velocity.y = m_playerConfig.JUMP;
			m_player->getComponent<CState>().notGrounded = true;
		}
	}

	if (m_player->getComponent<CInput>().left) {
		playerVelocity.x = -m_playerConfig.SPEED;
	}
	if (m_player->getComponent<CInput>().right) {
		playerVelocity.x = m_playerConfig.SPEED;
	}
	m_player->getComponent<CTransform>().velocity = playerVelocity;
	for (auto& e : m_entityManager.getEntities()) {
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		//The line should be placed at the end of the frame update or simulation step, just before you start processing the next frame. This ensures that prevPos is updated after all calculations involving the current frame have been performed.
		if (e->hasComponent<CGravity>() && e->getComponent<CState>().notGrounded == true) {
			e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
			if (e->getComponent<CTransform>().velocity.y > m_playerConfig.MAXSPEED) {
				e->getComponent<CTransform>().velocity.y = m_playerConfig.MAXSPEED;
			}
		}
		e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;

	}
	if (m_player->getComponent<CTransform>().pos.y > height() * 2.5)
		spawnPlayer();
		//loadLevel();

}

void ScenePlay::onEnd() {
	// TODO: when the scene ends, change back to the MENU scene
	//       use m_game->changeState(correct params)
	m_game->changeScene(MENU, m_game->getScene(MENU));
}

void ScenePlay::sAnimation() {
	for (auto& e : m_entityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			e->getComponent<CAnimation>().animation.update();
		}
		if (e->getComponent<CAnimation>().animation.hasEnded() && !e->getComponent<CAnimation>().repeat)
			e->destroy();
	}
}
void ScenePlay::sCollision() {


	for (auto& d : m_entityManager.getEntities(DYNAMIC)) {
		d->getComponent<CState>().notGrounded = true;
		for (auto& e : m_entityManager.getEntities()) {

			if (d->getComponent<CTransform>().prevPos == e->getComponent<CTransform>().prevPos)
				continue;// that is needed for not iterate on the same dynamic entity
			if (e->id() == DEC)
				continue;// of course we wont itherate on the decoration entities
			Vec2 overlap = m_physics.GetOverlap(d, e);
			if (overlap.x > 0 && overlap.y > 0) {//collision detected

				//std::cout << prevOverlap.x << std::endl;
				if (m_physics.GetPreviousOverlap(d,e).x > 0) {// collision in y direction (on tile)
					if (d->getComponent<CTransform>().pos.y > d->getComponent<CTransform>().prevPos.y) {//top
						//std::cout << "from top collision\n";
						d->getComponent<CInput>().jump = true;
						d->getComponent<CState>().notGrounded = false;
						d->getComponent<CTransform>().velocity.y = 0;
						d->getComponent<CTransform>().pos.y -= overlap.y;
						if (d->hasComponent<CAnimation>() && d->getComponent<CAnimation>().animation.getName() == JUMP) {
							std::cout << "Grounded\n";
							d->addComponent<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
							d->getComponent<CState>().state = RUNNING;
							//d->removeComponent<CAnimation>();
						}


					}
					else {
						d->getComponent<CTransform>().pos.y += overlap.y;
						d->getComponent<CState>().notGrounded = true;
						d->getComponent<CInput>().jump = false;
						if (e->tag() == BRICK) {
							e->addComponent<CAnimation>(
								m_game->getAssets().getAnimation(eAnimationTypes::EXPLOSION),
								false);
							e->getComponent<CBoundingBox>().size = { 0, 0 };
							d->getComponent<CInput>().up = false;
							d->getComponent<CTransform>().velocity.y = d->getComponent<CGravity>().gravity;
						}
						if (e->tag() == QUESTION) {
							if (e->getComponent<CAnimation>().animation.getName() == AniQUESTIONBLOCK) {
								auto entity=m_entityManager.addEntity(DEC, COIN);
								Vec2 p = e->getComponent<CTransform>().pos;
								entity->addComponent<CTransform>(p + Vec2(0,-64));
								entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
								entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(AniCOIN), false);
							}
							e->addComponent<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::AniQUESTION),true);
							d->getComponent<CInput>().up = false;
							d->getComponent<CTransform>().velocity.y = d->getComponent<CGravity>().gravity;
						}
						if (e->tag() == GOOMBA) {
							std::cout << "PLAYER COLLIDED FROM TOp\n";
							e->addComponent<CAnimation>(
								m_game->getAssets().getAnimation(eAnimationTypes::aniGOODEATH),
								false);

							e->getComponent<CBoundingBox>().size = { 0, 0 };
							e->getComponent<CTransform>().has = false;

						}
						

					}
				}
				else {// x idrection
					if (m_physics.GetPreviousOverlap(d,e).y > 0) {// collision in x direction (on tile)
						
						if(e->id()==TILE){
							//if (d->getComponent<CTransform>().pos.x > d->getComponent<CTransform>().prevPos.x) {// from left
							//	d->getComponent<CTransform>().velocity.x = 0;
							//	d->getComponent<CTransform>().pos.x -= overlap.x;


							//}
							//else {// from right
							//	//std::cout << "from right collision\n";
							//	d->getComponent<CTransform>().velocity.x = 0;
							//	d->getComponent<CTransform>().pos.x += overlap.x;


							//}
							d->getComponent<CTransform>().pos.x += 
								(d->getComponent<CTransform>().pos.x > d->getComponent<CTransform>().prevPos.x) ? (-1) * overlap.x : overlap.x;
							//d->getComponent<CTransform>().velocity.x = 0;
							if (d->tag() == BULLET) {
								d->destroy();
								if (e->tag() == BRICK) {
									e->addComponent<CAnimation>(
										m_game->getAssets().getAnimation(eAnimationTypes::EXPLOSION),
										false);
									e->getComponent<CBoundingBox>().size = { 0, 0 };
								}

							}
							if (d->tag() == GOOMBA) {
								std::cout<<"GOOMBA\n";
								d->getComponent<CTransform>().velocity.x *= -1;


							}
						}
						else {
							e->destroy();
							d->destroy();
							spawnEnemy();
						}
						/*if (e->id() == TILE) {
							d->getComponent<CTransform>().pos.x -=
								(d->getComponent<CTransform>().pos.x - d->getComponent<CTransform>().prevPos.x) /
								(std::abs(d->getComponent<CTransform>().pos.x - d->getComponent<CTransform>().prevPos.x)) *
								overlap.x;

							if (d->tag() == BULLET)
								d->destroy();

							if (d->tag() == GOOMBA)
								d->getComponent<CTransform>().velocity.x *= -1;

							break;
						}
						else {
							
						}*/
						
					}
				}
			}
		}
	}


}
void ScenePlay::sLifespan() {
	for (auto e : m_entityManager.getEntities()) {
		if (e->hasComponent<CLifeSpan>())
			if (e->getComponent<CLifeSpan>().lifeSpan <
				e->getComponent<CLifeSpan>().clock.getElapsedTime().asSeconds())
				e->destroy();
	}
}
void ScenePlay::sDoAction(const Action& action) {
	if (action.type() == START) {
		if (action.name() == UP) {
			if (m_player->getComponent<CInput>().jump) {
				m_player->getComponent<CInput>().up = true;
				m_player->getComponent<CState>().state = JUMPING;
				m_player->addComponent<CAnimation>(
					m_game->getAssets().getAnimation(eAnimationTypes::JUMP), true);
				m_player->getComponent<CTransform>().aimPos =
					m_player->getComponent<CTransform>().pos.y - 170.0;
				m_player->getComponent<CInput>().jump = false;
				m_player->getComponent<CState>().notGrounded = true;
			}
		}
		if (action.name() == LEFT) {
			m_player->getComponent<CInput>().left = true;
			m_player->getComponent<CState>().state = RUNNING;
			m_player->getComponent<CInput>().jump = false;

			if (m_player->hasComponent<CAnimation>()) {
				
				if (m_player->getComponent<CAnimation>().animation.getName() != RUN) {
					m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
				}
			}
			else {
				m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
			}
			(m_player->getComponent<CTransform>().scale.x > 0) ? m_player->getComponent<CTransform>().scale.x *= -1 : 1;


		}
		if (action.name() == RIGHT) {
			m_player->getComponent<CInput>().right = true;
			m_player->getComponent<CState>().state = RUNNING;

			if (m_player->hasComponent<CAnimation>()) {
				
				if (m_player->getComponent<CAnimation>().animation.getName() != RUN) {
					m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
				}
			}
			else {
				m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
			}




			m_player->getComponent<CInput>().jump = false;
			(m_player->getComponent<CTransform>().scale.x < 0) ? m_player->getComponent<CTransform>().scale.x *= -1 : 1;
		}


		else if (action.name() == TOGGLE_GRID) {
			m_drawGrid = !m_drawGrid;
		}
		else if (action.name() == TOGGLE_COLLISION) {
			m_drawCollision = !m_drawCollision;
		}
		else if (action.name() == TOGGLE_TEXTURE) {
			m_drawTextures = !m_drawTextures;
		}
		else if (action.name() == QUIT) {
			m_game->changeScene(MENU, std::make_shared<SceneMenu>(m_game));
		}
		else if (action.name() == FIRE) {
			std::cout << "SHOTS FIRED\n";
			m_player->getComponent<CInput>().shoot = true;
			m_player->addComponent<CAnimation>(
				m_game->getAssets().getAnimation(eAnimationTypes::SHOT), true);
			spawnBullet(m_player);
			m_player->getComponent<CState>().state = SHOOTING;
		}

	}
	else if (action.type() == END) {
		if (action.name() == UP) {
			if (m_player->getComponent<CInput>().up) {
				m_player->getComponent<CInput>().up = false;
				m_player->getComponent<CTransform>().velocity.y = 0;
				m_player->getComponent<CState>().notGrounded = true;
				

			}
		}
		else if (action.name() == LEFT) {
			m_player->getComponent<CInput>().left = false;
		}
		else if (action.name() == RIGHT) {
			m_player->getComponent<CInput>().right = false;
		}
		else if (action.name() == FIRE) {
			m_player->getComponent<CInput>().shoot = false;
			m_player->getComponent<CState>().state = STANDING;
		}
	}
}