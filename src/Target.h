#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	//Math related functions
	float Target::checkDistance(GameObject* pGameObject);
	float calculateTheta();
	float calculateForceGX();
	float calculateForceK();

	float calculateNetForce(float,float);
	void calculateAcceleration(float,float);
	float calculateForce();
	
	//Getters
	glm::vec2 getDirection();
	glm::vec2 getEnemyPosition();
	bool getBeginSimulation();
	float getDistanceToEnemy();
	float getSpeedThrown();
	float getGravity();
	float getForce();
	float getTheta();
	float getMass();
	float getFrictionCoefficient();
	glm::vec2 getStartingPosition();
	glm::vec2 getRampEndPosition();

	float getRampSlope();
	
	//Setters
	void setDirX(float);
	void setDirY(float);
	void setRampEndPosition(glm::vec2);
	void setStartingPosition(glm::vec2);
	void setDirection(glm::vec2);
	void setEnemyPosition(glm::vec2);
	void setBeginSimulation(bool);
	void setSpeedThrown(float);
	void setForce(float);
	void setTheta(float);
	void setMass(float);
	void setRampSlope(float);
	void setFrictionCoefficient(float);

private:
	glm::vec2 m_direction;
	glm::vec2 m_enemyPosition;
	glm::vec2 m_startingPosition;
	glm::vec2 m_rampEndPosition;

	float m_rotateTarget;
	float m_rampSlope;
	float m_frictionCoefficient;
	float m_acceleraton;
	float m_speedThrown;
	float m_force;
	float m_theta;
	float m_mass;
	bool m_beginSimulation;
	void m_move();
	void m_checkBounds();
	void m_reset();

	const float PPM = 50.0f;
	const float m_gravity = 9.8f;
};


#endif /* defined (__TARGET__) */