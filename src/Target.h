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
	float getSpeedThrown();
	float getGravity();
	float getForce();
	float getTheta();
	float getMass();
	float getFrictionCoefficient();
	float getRotateTarget();
	float getPPM();
	float getMaxSpeed();
	bool getBeginSimulation();
	bool getOnGround();

	//Setters
	void setDirX(float);
	void setDirY(float);
	void setDirection(glm::vec2);
	void setBeginSimulation(bool);
	void setSpeedThrown(float);
	void setForce(float);
	void setTheta(float);
	void setMass(float);
	void setFrictionCoefficient(float);
	void setRotateTarget(float);
	void setOnGround(bool);
	void setPPM(float);
	void setMaxSpeed(float);
	void setAtMaxSpeed(bool);
	void setDeccelerationCalculated(bool);

private:
	glm::vec2 m_direction;
	float m_rotateTarget;
	float m_frictionCoefficient;
	float m_acceleraton;
	float m_speedThrown;
	float m_force;
	float m_theta;
	float m_mass;
	float m_PPM;
	float m_MaxSpeed;
	bool m_beginSimulation;
	bool m_onGround;
	bool m_deccelerationCalculated;
	bool m_atMaxSpeed;

	void m_move();
	void m_checkBounds();
	void m_reset();

	//Constants
	const float m_gravity = 9.8f;
};


#endif /* defined (__TARGET__) */