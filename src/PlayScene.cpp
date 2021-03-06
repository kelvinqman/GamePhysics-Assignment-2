#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	// PlayScene.cpp -> draw()
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	// PlayScene.cpp -> draw()
	drawRamp();
}

void PlayScene::update()
{
	updateDisplayList();
	// PlayScene.cpp -> update()
	updateVertices();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);
	m_pPlaneSprite->getTransform()->position.x = -200;

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_pPlayer->getTransform()->position.x = -200;
	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 500.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);

	addChild(m_pInstructionsLabel);
	// PlayScene.cpp -> start() Add background image
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
	// PlayScene.cpp -> start() 
	m_pCrateLoot = new Target;
	addChild(m_pCrateLoot);
	m_pCrateLoot->getTransform()->position.x = position - sin(theta) * 20;
	m_pCrateLoot->getTransform()->position.y = 400 - height - cos(theta) * 20;
	m_pCrateLoot->setAngle(theta);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// PlayScene.cpp -> GUI_Function()
	if (ImGui::Button("Move according problem"))
	{
		position = 100.0f;
		width = 400.0f;
		height = 300.0f;
		updateTheta();
		m_pCrateLoot->setAngle(theta);
		m_pCrateLoot->setMass(12.8);
		m_pCrateLoot->setCokf(0.42);
		updateCrate();
		m_pCrateLoot->toMove(theta);
	}

	ImGui::Separator();

	// PlayScene.cpp -> GUI_Function()
	std::string strm = " " + std::to_string(m_pCrateLoot->getMass());
	const char* cstrm = strm.c_str();
	ImGui::LabelText("Mass", cstrm);
	std::string strf = " " + std::to_string(getForce());
	const char* cstrf = strf.c_str();
	ImGui::LabelText("Force", cstrf);
	std::string str1 = " X: " + std::to_string(m_pCrateLoot->getTransform()->position.x)
		+ " Y: " + std::to_string(m_pCrateLoot->getTransform()->position.y);
	const char* cstr1 = str1.c_str();
	ImGui::LabelText("Position", cstr1);
	std::string str2 = " X: " + std::to_string(m_pCrateLoot->getRigidBody()->velocity.x)
		+ " Y: " + std::to_string(m_pCrateLoot->getRigidBody()->velocity.y);
	const char* cstr2 = str2.c_str();
	ImGui::LabelText("Velocity", cstr2);
	std::string str3 = " X: " + std::to_string(m_pCrateLoot->getRigidBody()->acceleration.x)
		+ " Y: " + std::to_string(m_pCrateLoot->getRigidBody()->acceleration.y);
	const char* cstr3 = str3.c_str();
	ImGui::LabelText("Acceleration", cstr3);
	
	// PlayScene.cpp -> GUI_Function()
	static float posTemp[1] = { position };
	if (ImGui::SliderFloat("Position", posTemp, 0.0f, 800.0f)) {
		setPosition(posTemp[0]);
		updateCrate();
	}
	static float wTemp[1] = { width };
	if (ImGui::SliderFloat("Width", wTemp, 0.0f, 300.0f)) {
		setWidth(wTemp[0]);
		updateCrate();
	}
	static float hTemp[1] = { height };
	if (ImGui::SliderFloat("Height", hTemp, 0.0f, 300.0f)) {
		setHeight(hTemp[0]);
		updateCrate();
	}
	static float mTemp[1] = { m_pCrateLoot->getMass() };
	if (ImGui::SliderFloat("Mass", mTemp, 0.0f, 100.0f)) {
		m_pCrateLoot->setMass(mTemp[0]);
	}
	static float cTemp[1] = { m_pCrateLoot->getCokf() };
	if (ImGui::SliderFloat("Coefficient of kinetic friction", cTemp, 0.0f, 20.0f)) {
		m_pCrateLoot->setCokf(cTemp[0]);
	}
	if (ImGui::Button("Moving with manipulcated variables")) {
		updateCrate();
		m_pCrateLoot->MoveManipulate();
	}
	ImGui::SameLine();
	if (ImGui::Button("reset")) {
		updateCrate();
	}
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
// PlayScene.cpp -> member function
void PlayScene::drawRamp()
{
	// Bottom line
	Util::DrawLine(vertice1, vertice3, glm::vec4(0.0f, 100.0f, 100.0f, 0.0f));
	// Left line
	Util::DrawLine(vertice1, vertice2, glm::vec4(0.0f, 100.0f, 100.0f, 0.0f));
	// Ramp line
	Util::DrawLine(vertice2, vertice3, glm::vec4(0.0f, 100.0f, 100.0f, 0.0f));
}

// PlayScene.cpp -> memeber function
float PlayScene::getForce()
{
	if (m_pCrateLoot->getTransform()->position.y < 400 - 20)
		return m_pCrateLoot->getMass() * (-9.8) * (-sin(theta / 180 * 3.14));
	else
		return m_pCrateLoot->getMass() * (-9.8) * m_pCrateLoot->getCokf();
}
// PlayScene.cpp -> memeber function
void PlayScene::updateVertices()
{
	vertice1 = { position,400.0f };
	vertice2 = { vertice1.x,vertice1.y - height };
	vertice3 = { vertice1.x + width,vertice1.y };
}

void PlayScene::updateCrate()
{
	updateTheta();
	float temp = theta / 180 * 3.14;
	m_pCrateLoot->getTransform()->position.x = position + sin(temp) * 20;
	m_pCrateLoot->getTransform()->position.y = 400 - height - cos(temp) * 20;
	m_pCrateLoot->setAngle(theta);
}