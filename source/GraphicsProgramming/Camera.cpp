#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::update()
{
	float cosR, cosP, cosY;		//temp values for sin/cos
	float sinR, sinP, sinY;
	// Roll, Pitch, and Yaw are all variables stored by the camera
	// Handle rotation
	// Only want to calculate these values once, when rotation changes, not every frame.
	cosY = cosf(Yaw*3.1415f / 180.f);
	cosP = cosf(Pitch*3.1415f / 180.f);
	cosR = cosf(Roll*3.1415f / 180.f);
	sinY = sinf(Yaw*3.1415f / 180.f);
	sinP = sinf(Pitch*3.1415f / 180.f);
	sinR = sinf(Roll*3.1415f / 180.f);
	//This using the parametric equation of a sphere

	// Calculate the three vectors to put into gluLookat
	// Look direction, position and the up vector
	// This function could also calculate the right vector

	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	// Look at Point
	// To calculate add Forward Vector to Camera position.
	lookAt = forward + position;

	//Up Vector
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	// Side Vector(Right)
	// This is a cross product between forward and up vector.
	right = forward.cross(up);
}

// Resets the camera to default values.
void Camera::reset()
{
	position = Vector3(0, 0, 6);
	Yaw = 0, Pitch = 0, Roll = 0;
	lookAt = forward + position;
}

// Moves the camera forwards (negative z direction) based on where it is looking.
void Camera::moveForward(float dt)
{
	position.add(forward, dt);
	lookAt = forward + position;
}

// Moves the camera backwards (positive z direction) based on where it is looking.
void Camera::moveBackwards(float dt)
{
	position.subtract(forward, dt);
	lookAt = forward + position;
}

// Moves the camera left (negative x direction) based on where it is looking.
void Camera::moveLeft(float dt)
{
	position.subtract(right, dt);
	lookAt = forward + position;
}

// Moves the camera right (positive x direction) based on where it is looking.
void Camera::moveRight(float dt)
{
	position.add(right, dt);
	lookAt = forward + position;
}

// Moves the camera up (positive y direction). This is rotation independent.
void Camera::moveUp(float dt)
{
	position.y += 1.0f * dt;
	lookAt = forward + position;
}

// Moves the camera down (negative y direction). This is rotation independent.
void Camera::moveDown(float dt)
{
	position.y -= 1.0f * dt;
	lookAt = forward + position;
}
