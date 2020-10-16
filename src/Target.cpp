#include "Target.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Util.h"
#include <iostream>
#include <cmath>
#define PI 3.14159265

Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/LB1.png","LB1");

	const auto size = TextureManager::Instance()->getTextureSize("LB1");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(250.0f, 250.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	m_direction = glm::vec2(4.0f, 3.0f);
	m_acceleraton = 0;
	m_theta = 0.0f;
	m_speedThrown = 95.0f;
	m_mass = 12.8f;
	m_beginSimulation = false;
	m_force = 0.0f;
	m_rampSlope = 0;
	m_startingPosition = getTransform()->position;
	m_rampEndPosition = glm::vec2(0.0f);
	m_frictionCoefficient = 0.42f;
	m_rotateTarget = 0;

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
	TextureManager::Instance()->draw("LB1", x, y, m_rotateTarget, 255, true);

}

void Target::update()
{
	m_move();

}
float Target::checkDistance(GameObject* pGameObject) {
	int x = getTransform()->position.x - pGameObject->getTransform()->position.x;
	int y = getTransform()->position.y - pGameObject->getTransform()->position.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}

float Target::calculateTheta()
{
	 m_theta = atan(m_direction.y/m_direction.x)*180/PI;
	 return m_theta;
}

float Target::calculateForceGX()
{
	float temp = m_theta;
	temp = temp * PI / 180;
	std::cout << m_mass * m_gravity * sin(temp);
	return m_mass * m_gravity * sin(temp);
}

float Target::calculateForceK()
{
	float natrualForce = m_mass * m_gravity;
	return -1 * (m_frictionCoefficient * natrualForce);
}

float Target::calculateNetForce(float Fx, float Fk)
{
	return Fx + Fk;
}

void Target::calculateAcceleration(float forceGX, float forceK)
{
	m_acceleraton = calculateNetForce(forceGX,forceK) / m_mass;
}


float Target::calculateForce()
{
	m_force = m_mass * getRigidBody()->acceleration.y;
	return m_force;
}


glm::vec2 Target::getDirection()
{
	return m_direction;
}

void Target::setDirX(float dirX)
{
	m_direction.x = dirX;
}

void Target::setDirY(float dirY)
{
	m_direction.y = dirY;
}

void Target::setRampEndPosition(glm::vec2 rampEndPosition)
{
	m_rampEndPosition = rampEndPosition;
}

void Target::setStartingPosition(glm::vec2 startingPosition)
{
	m_startingPosition = startingPosition;
}

void Target::setDirection(glm::vec2 direction)
{
	m_direction = direction;
}

glm::vec2 Target::getEnemyPosition()
{
	return m_enemyPosition;
}

void Target::setEnemyPosition(glm::vec2 enemyPosition)
{
	m_enemyPosition = enemyPosition;
}

bool Target::getBeginSimulation()
{
	return m_beginSimulation;
}

void Target::setBeginSimulation(bool beginSimulation)
{
	m_beginSimulation = beginSimulation;
}

float Target::getDistanceToEnemy()
{
	return 1;
}


float Target::getSpeedThrown()
{
	return m_speedThrown;
}

void Target::setSpeedThrown(float speedThrown)
{
	m_speedThrown = speedThrown;
}

float Target::getGravity()
{
	return m_gravity;
}

float Target::getForce()
{
	return m_force;
}

void Target::setForce(float force)
{
	m_force = force;
}

float Target::getTheta()
{
	return m_theta;
}

void Target::setTheta(float theta)
{
	m_theta = theta;
}

float Target::getMass()
{
	return m_mass;
}

float Target::getFrictionCoefficient()
{
	return m_frictionCoefficient;
}

glm::vec2 Target::getStartingPosition()
{
	return m_startingPosition;
}

glm::vec2 Target::getRampEndPosition()
{
	return m_rampEndPosition;
}

float Target::getRampSlope()
{
	return m_rampSlope;
}

void Target::setMass(float mass)
{
	m_mass = mass;
}

void Target::setRampSlope(float rampSlope)
{
	m_rampSlope = rampSlope;
}

void Target::setFrictionCoefficient(float frictionCoefficient)
{
	m_frictionCoefficient = frictionCoefficient;
}


void Target::clean()
{
}

void Target::m_move()
{
	const float deltaTime = 1.0f / 60.0f;

	if (getTransform()->position.y >=500.0f - getHeight()/2 && m_beginSimulation)
	{
		m_rotateTarget = 0;
		
		if (Util::magnitude(getRigidBody()->velocity) <= 5.0f)
		{
			getTransform()->position.y = 500.0f - getWidth() / 2;
			getRigidBody()->velocity = glm::vec2(0.0f);
			getRigidBody()->acceleration = glm::vec2(0.0f);
			m_beginSimulation = false;
		}
		else
		{
			getTransform()->position.y = 500.0f - getHeight() / 2;
			calculateAcceleration(0, calculateForceK());
			getRigidBody()->acceleration = Util::normalize(m_direction) * ((m_acceleraton)*(PPM*deltaTime));
			getRigidBody()->velocity += getRigidBody()->acceleration;
		}


	}
	 else if (m_beginSimulation)
	{
		m_rotateTarget = m_theta;
		 getRigidBody()->acceleration = Util::normalize(m_direction) * ((m_acceleraton) * (PPM *deltaTime));

		 getRigidBody()->velocity += getRigidBody()->acceleration;
	}

	 glm::vec2 pos = getTransform()->position;
	 pos.x += getRigidBody()->velocity.x * deltaTime;
	 pos.y += getRigidBody()->velocity.y * deltaTime;

	 getTransform()->position = pos;


}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
