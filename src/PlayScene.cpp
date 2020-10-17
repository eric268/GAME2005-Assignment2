#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Renderer.h"
#include <iomanip>

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	//Background
	TextureManager::Instance()->draw("Background1", 400, 300, 0, 255, true);
	drawDisplayList();
	drawRamp();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	m_updateUI();
	updateDisplayList();


	//Printing Labels
	m_pVelocityLabel->setText("Velocity: " + std::to_string(Util::magnitude(m_pThermalDetonator->getRigidBody()->velocity)/PPM));
	m_pDistanceLabel->setText("Distance = m");
	m_pMassLabel->setText("Weight = " + std::to_string(m_pThermalDetonator->getMass()) + "kg");
	m_pAccelerationLabel->setText("Acceleration = " + std::to_string(Util::magnitude(m_pThermalDetonator->getRigidBody()->acceleration)) + "m/s^2");
	m_pForceLabel->setText("Force = " + std::to_string(m_pThermalDetonator->calculateForce()) + "N");
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	auto& io = ImGui::GetIO();
	auto wheel = 0;

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_BACKQUOTE:
				m_displayUI = (m_displayUI) ? false : true;
				break;
			}
			{
				const int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			default:
				break;
			}
			{
				const int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		default:
			break;
		}
	}
	io.DeltaTime = 1.0f / 60.0f;
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	m_ImGuiKeyMap();
	m_ImGuiSetStyle();
}


void PlayScene::start()
{
	rampHeight = 3;
	rampWidth = 4;
	rampPos = 0.5;
	//Ramp position variables
	adjustRampPosition = glm::vec2(50.0f, 500);
	adjustRampWidth = glm::vec2(adjustRampPosition.x + (rampWidth*PPM), adjustRampPosition.y);
	adjustRampHeight = glm::vec2(adjustRampPosition.x, adjustRampPosition.y - (rampHeight*PPM));
	
	//Ramp values according to PPM


	//Setting trackers for changes in sliders
	rampPosPrev = rampPos;
	rampHeightPrevious = rampHeight;
	rampWidthPrevious = rampWidth;
	rampPositionTracker = adjustRampPosition.x;

	m_updateUI();

	//Setting up background
	TextureManager::Instance()->load("../Assets/textures/Background1.jpg", "Background1");

	//Target Sprite(Thermal Detonator)
	m_pThermalDetonator = new Target();
	addChild(m_pThermalDetonator);

	//Texture sets position to center of box so offset it 
	lootBoxOffset = m_pThermalDetonator->getHeight() / 2;
	m_pThermalDetonator->getTransform()->position = glm::vec2(adjustRampPosition.x, adjustRampHeight.y - lootBoxOffset);

	

	// Label
	const SDL_Color white = { 255,255,255, 255 };
	const SDL_Color red = { 255,0,0,255 };

	m_pDistanceLabel = new Label("Distance", "Consolas", 20, white, glm::vec2(596.0f, 40.0f));
	m_pDistanceLabel->setParent(this);
	addChild(m_pDistanceLabel);

	m_pAccelerationLabel = new Label("Acceleration: ", "Consolas", 20, white, glm::vec2(632.0f, 80.0f));
	m_pAccelerationLabel->setParent(this);
	addChild(m_pAccelerationLabel);

	m_pVelocityLabel = new Label("Velocity: ", "Consolas", 20, white, glm::vec2(599.0f, 120.0f));
	m_pVelocityLabel->setParent(this);
	addChild(m_pVelocityLabel);

	m_pMassLabel = new Label("Mass: ", "Consolas", 20, white, glm::vec2(583.0f, 160.0f));
	m_pMassLabel->setParent(this);
	addChild(m_pMassLabel);

	m_pForceLabel = new Label("Force: ", "Consolas", 20, white, glm::vec2(571.0f, 200.0f));
	m_pForceLabel->setParent(this);
	addChild(m_pForceLabel);

	m_pPPM = new Label("Scale: "+std::to_string(PPM), "Consolas", 15, white, glm::vec2(741.0f, 580.0f));
	m_pPPM->setParent(this);
	addChild(m_pPPM);

	m_pCannotHitTrooper = new Label(" ", "Consolas", 25, red, glm::vec2(400.0f, 350.0f));
	m_pCannotHitTrooper->setParent(this);
	addChild(m_pCannotHitTrooper);

	resetSceneSettings();
}
void PlayScene::setGuidSlidePlaceholders()
{
}
void PlayScene::resetSceneSettings()
{
}
void PlayScene::checkGuiChangs()
{
	if (ImGui::Button("Begin Simulation"))
	{
		m_pThermalDetonator->setBeginSimulation(true);
		m_pThermalDetonator->setDirX(rampWidth);
		m_pThermalDetonator->setDirY(rampHeight);
		m_pThermalDetonator->calculateTheta();
		m_pThermalDetonator->calculateAcceleration(m_pThermalDetonator->calculateForceGX(), 0);


	}
	ImGui::SameLine();
	if (ImGui::Button("Reset Scene"))
	{
		m_pThermalDetonator->getTransform()->position = glm::vec2(adjustRampPosition.x, adjustRampHeight.y - lootBoxOffset);
		m_pThermalDetonator->setBeginSimulation(false);
		m_pThermalDetonator->getRigidBody()->velocity = glm::vec2(0.0f);
		m_pThermalDetonator->getRigidBody()->acceleration = glm::vec2(0.0f);
		m_pThermalDetonator->setRotateTarget(0.0f);
		m_pThermalDetonator->setOnGround(false);
	}
	
	//if (ImGui::SliderFloat("Speed Thrown", &GuiSliderPlaceholders[2], 80, 200))
	//{
	//	m_pThermalDetonator->setSpeedThrown(GuiSliderPlaceholders[2]);
	//}
	//if (ImGui::SliderFloat("Gravity", &GuiSliderPlaceholders[3], 5, 20))
	//{
	//	m_pThermalDetonator->setGravity(GuiSliderPlaceholders[3]);
	//}
	//if (ImGui::SliderFloat("Mass", &GuiSliderPlaceholders[4], 0.1, 10))
	//{
	//	m_pThermalDetonator->setMass(4);
	//}
	//if (ImGui::SliderFloat("Angle (theta)", &GuiSliderPlaceholders[5], 0, 90))
	//{
	//	m_pThermalDetonator->setTheta(GuiSliderPlaceholders[5]);

	//}
	if (ImGui::SliderFloat("Adjust ramp Position ", &rampPos, 0, 4));
	{
		if (rampPos != rampPosPrev)
		{
			rampPosTemp = rampPos * PPM - rampPosPrev * PPM;
			adjustRampPosition.x += rampPosTemp;
			adjustRampWidth.x += rampPosTemp;
			rampPosPrev = rampPos;
			m_pThermalDetonator->getTransform()->position.x = adjustRampPosition.x;
			m_pThermalDetonator->getTransform()->position.y = adjustRampHeight.y - lootBoxOffset;

		}
		adjustRampHeight.x = adjustRampPosition.x;
		if (adjustRampPosition.x != rampPositionTracker)
		{
			rampPositionTracker = adjustRampPosition.x;
		}
	}
	if (ImGui::SliderFloat("Adjust ramp Width ", &rampWidth, 1, 7));
	{
		if (rampWidth != rampWidthPrevious)
		{
			widthTemp = rampWidth * PPM - rampWidthPrevious * PPM;
			adjustRampWidth.x += widthTemp;
			rampWidthPrevious = rampWidth;
		}
		//adjustRampWidth.x = adjustRampWidth.x + rampWidth * 50;
	}
	if (ImGui::SliderFloat("Adjust ramp Height ", &rampHeight, 1, 10));
	{
		if (rampHeight != rampHeightPrevious)
		{
			rampHeightTemp = (rampHeight *PPM) - (rampHeightPrevious* PPM);
			adjustRampHeight.y -= rampHeightTemp;
			m_pThermalDetonator->getTransform()->position.y = adjustRampHeight.y - lootBoxOffset;
			rampHeightPrevious = rampHeight;

		}
	}
}

void PlayScene::drawRamp()
{
	Util::DrawLine(adjustRampPosition, adjustRampWidth);

	Util::DrawLine(adjustRampPosition, adjustRampHeight);

	Util::DrawLine(adjustRampHeight,adjustRampWidth);

	//Ground for degugging purposes
	Util::DrawLine(glm::vec2(0,500), glm::vec2(1000, 500));
}
// ImGui functions ***********************************************

void PlayScene::m_ImGuiKeyMap()
{
	auto& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
}

void PlayScene::m_ImGuiSetStyle()
{
	auto& style = ImGui::GetStyle();

	style.Alpha = 0.8f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void PlayScene::m_updateUI()
{
	// Prepare Window Frame
	ImGui::NewFrame();

	ImGui::Begin("Settings ", NULL, ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_MenuBar);

	// set window to top getLeft corner
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	/*************************************************************************************************/
	/* MENU                                                                                          */
	/*************************************************************************************************/

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Exit", NULL, &m_exitApp);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (m_exitApp)
	{
		TheGame::Instance()->quit();
	}
	if (m_displayAbout)
	{
		ImGui::Begin("Projectile Tragectory", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::Text("Authors:");
		ImGui::Text("Eric Galway & Ian Chapman ");
		ImGui::End();
	}
	//ImGui::SameLine();
	checkGuiChangs();

	// Main Window End
	ImGui::End();
}

