// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include "Camera.h"
#include "Shape.h"
#include "Model.h"
#include "Shadow.h"

class Scene{

public:
	Scene(Input *in);
	// Main render function
	void render();
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);	

protected:
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	// draw primitive functions
	// Resets everything in the scene.
	void reset();
	// Skybox cube created around the cameras position.
	void skyboxSetup();
	// Set up default values for lighting
	void lightingSetup();
	// Movement controls for the camera.
	void cameraMovement(float dt);
	// Rotation for the camera, based on mouse movement.
	void cameraRotation();
	// Selection of different cameras.
	void cameraSelection();
	// Set up textures.
	void textureSetup();
	// Set up specular material values.
	void specularMaterials();
	// Enable/Disable Wireframe mode
	void wireframeMode();
	// Allows the user to switch between texture filtering modes.
	void texFilterMode();
	// Render chosen scene.
	void renderScene();
	// Move tram.
	void tramMovement(float dt);
	// Open/Close door.
	void doorControls(float dt);
	// Renders the lights spheres.
	void renderLightSpheres();
	// Renders the entire tram rail.
	void renderRail();
	// Renders the tram.
	void renderTram();
	// Renders the door.
	void renderDoor();
	// Renders the door room.
	void renderDoorRoom();
	// Renders the walkway.
	void renderWalkway();
	// Renders the left dock.
	void renderLeftDock();
	// Renders the right dock.
	void renderRightDock();
	// Renders the walls/floor.
	void renderEnclosure();
	// Renders the door locks.
	void renderDoorLocks();
	// Planar Shadow
	void planarShadow();
	// Stencil Buffer example
	void stencilBufferExample();

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter, mouse coordinate, selected texture filtering option and selected camera output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];
	char textureText[40];
	char cameraText[40];
	string selectedTexMode, selectedCamera;

	//variables
	float tramX = 0.0f, doorLockX = 0.0f, doorLock2X = 0.0f;
	float bottomDoorY = 0.0f, topDoorY = 6.0f;
	float angle = 0.0f, angle2 = 0.0f;
	bool wireframe = false;
	float shadowMatrixArray[16];
	GLuint doorTopTexture, doorTopTextureFlipped, doorBottomTexture, doorBottomTextureFlipped, grateTexture, hazardTexture, wallTexture;
	Camera freeCamera, tramCamera, doorCamera, *cameraPointer;
	Shape shape;
	Model tram, crowbar;
	Shadow shadowMatrix;
	Vector3 doorLight1Pos, doorLight2Pos, tramLight1Pos, tramLight2Pos, dockLight1Pos, dockLight2Pos;
	GLfloat sceneLightPosition[3] = { 0,0,0 };
};

#endif