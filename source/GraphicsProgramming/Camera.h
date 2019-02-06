#pragma once
// Camera class. Configures a basic camera.
// update (for updating camera variables).

// Include <math.h> to allow use of cosf/sinf.
#include<math.h>
#include "Vector3.h"

class Camera
{

public:
	Camera();
	~Camera();

	// Update function receives delta time from parent (used for frame independent updating).
	void update();

	// Getters
	Vector3 getPosition() { return position; };
	Vector3 getLookAt() { return lookAt; };
	Vector3 getForward() { return forward; };
	Vector3 getUp() { return up; };
	float getYaw() { return Yaw; };
	float getPitch() { return Pitch; };
	float getRoll() { return Roll; };

	// Setters
	void setYaw(float x) { Yaw += x; };
	void setPitch(float x) { Pitch += x; };
	void setRoll(float x) { Roll = x; };
	void setPosition(Vector3 pos) { position = pos; };
	void setLookAt(Vector3 look) { lookAt = look; };
	void setForward(Vector3 forwar) { forward = forwar; };

	// Reset all camera variables.
	void reset();

	// Moving functions (With Keyboard)
	void moveForward(float dt);
	void moveBackwards(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);
	void moveUp(float dt);
	void moveDown(float dt);

private:
	// Variables
	Vector3 position = Vector3(0, 0, 6);
	Vector3 forward, lookAt, up, right;
	float Yaw, Pitch, Roll;
};

