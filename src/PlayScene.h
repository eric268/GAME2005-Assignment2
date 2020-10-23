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

	void beginSimulation();
	void resetSceneSettings();
	void resetCrateSettings();
	void checkGuiChangs();
	float getPPM();
	void drawRamp();

	float calculateDistanceOnGround();

	bool noFrictionSelected();

	float m_rampHeightTemp;
	float m_rampHeightPrevious;
	float m_rampPositionTracker;
	float m_rampWidthPrevious;
	float m_widthTemp;

	float m_distance;
	float m_rampHeight;
	float m_rampWidth;
	float m_rampPos;
	float m_rampPosPrev;
	float m_rampPosTemp;
	float m_lootBoxOffset;

	float m_crateMass;
	float m_crateMassTemp;
	float m_crateMassPrev;

	bool m_steelFriction;
	bool m_iceFriction;
	bool m_glassFriction;
	bool m_grassFriction;

	bool m_steelChecked;
	bool m_iceChecked;
	bool m_glassChecked;
	bool m_grassChecked;

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
	Label* m_pMaxSpeed;
	Button* m_pBackButton;
	Button* m_pNextButton;
	Target* m_pLootCrate;
	Label* m_pFrictionCoefficient;



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
	float PPMPrev;
	const SDL_Color black = { 0,0,0,255 };
	const SDL_Color white = { 255,255,255, 255 };
	const SDL_Color red = { 255,0,0,255 };
};

#endif /* defined (__PLAY_SCENE__) */