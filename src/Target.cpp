#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	// Target.cpp -> Target() Change image of target
	TextureManager::Instance()->load("../Assets/textures/tile.png", "tile");

	const auto size = TextureManager::Instance()->getTextureSize("tile");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	// Target.cpp -> draw()
	TextureManager::Instance()->draw("tile", x, y, angle, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	// Target.cpp -> m_move()
	if (getTransform()->position.y > 400 - 20)
	{
		getRigidBody()->velocity.y = 0;
		getRigidBody()->acceleration.y = 0;
		getRigidBody()->acceleration.x = getFlatAcceleration();
		angle = 0.0;
		if (getRigidBody()->velocity.x <= 0) {
			getRigidBody()->velocity = glm::vec2(0, 0);
			getRigidBody()->acceleration = glm::vec2(0, 0);
		}
	}
	float deltaTime = 1.0f / 20.0f;
	getRigidBody()->velocity += (getRigidBody()->acceleration) * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
// Target.cpp -> member function
void Target::toMove(float theta)
{
	getRigidBody()->acceleration.x = cos(36.87 / 180 * 3.14) * 5.88;
	getRigidBody()->acceleration.y = sin(36.87 / 180 * 3.14) * 5.88;
}