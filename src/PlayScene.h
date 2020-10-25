#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
// PlayScene.h -> header
#include "Target.h"
#include "Util.h"
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
	// PlayScene.h -> public:
	void setPosition(float p) { position = p; }
	void setWidth(float w) { width = w; }
	void setHeight(float h) { height = h; }
	// PlayScene.h -> public:
	void drawRamp();
	// PlayScene.h -> public: Label key statistics
		float getForce();
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	// PlayScne.h -> private:
	Target* m_pCrateLoot;
	float width = 400.0f;
	float height = 300.0f;
	float theta = atan(height / width) * 180 / 3.14;
	float position = 100.0f;
	glm::vec2 vertice1 = { position,400.0f };
	glm::vec2 vertice2 = { vertice1.x,vertice1.y - height };
	glm::vec2 vertice3 = { vertice1.x + width,vertice1.y };
};

#endif /* defined (__PLAY_SCENE__) */