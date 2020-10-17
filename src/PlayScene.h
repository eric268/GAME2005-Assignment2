#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Enemy.h"
#include "Target.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	float GuiSliderPlaceholders[6];

	void setGuidSlidePlaceholders();
	void resetSceneSettings();
	void checkGuiChangs();

	void drawRamp();

	float rampHeightTemp;
	float rampHeightPrevious;
	float rampPositionTracker;
	float rampWidthPrevious;
	float widthTemp;
	
	float rampHeight;
	float rampWidth;
	float rampPos;
	float rampPosPrev;
	float rampPosTemp;
	float lootBoxOffset;

private:
	glm::vec2 m_mousePosition;
	glm::vec2 adjustRampWidth;
	glm::vec2 adjustRampHeight;
	glm::vec2 adjustRampPosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Enemy* m_pEnemy[3];
	
	Label* m_pDistanceLabel;
	Label* m_pMassLabel;
	Label* m_pVelocityLabel;
	Label* m_pAccelerationLabel;
	Label* m_pForceLabel;
	Label* m_pPPM;
	Label* m_pCannotHitTrooper;
	
	Button* m_pBackButton;
	Button* m_pNextButton;
	Target* m_pThermalDetonator;



	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateUI();

	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	// ImGui button variables
	//ImVec4 m_manhattanButtonColour;
	//ImVec4 m_euclideanButtonColour;

	// ImGui visibility variables

	//Pixel per meter
	float PPM = 25.0f;
};

#endif /* defined (__PLAY_SCENE__) */