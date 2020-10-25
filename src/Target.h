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
	// Target.h -> public:
	void setAngle(double a) { angle = a; }
	float getMass() { return mass; }
	void setMass(float m) { mass = m; }
	float getCokf() { return cokf; }
	void setCokf(float c) { cokf = c; }
	// Target.h -> public: Move according to the problem
	float getFlatAcceleration() { return -9.8 * cokf; }
	// Target.h -> public:
	void toMove(float theta);
	// Target.h -> public: Allow user to Change variables
	void MoveManipulate();
private:
	void m_move();
	void m_checkBounds();
	void m_reset();
	// Target.h -> private:
	double angle = 0.0f;
	float mass = 12.8;
	float cokf = 0.42;
};


#endif /* defined (__TARGET__) */