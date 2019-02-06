#include "Scene.h"

Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
		
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_LIGHTING);								// Enable lighting
	glEnable(GL_COLOR_MATERIAL);						// Enable material colour
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Initialise blend function
	glEnable(GL_TEXTURE_2D);							// Enable textures on polygons
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// Set mode for texture application

	// Other OpenGL / render setting should be applied here.
	tram.load("Models/tram.obj", NULL, "models/tram.mtl");
	crowbar.load("Models/Crowbar.obj", NULL, NULL);

	// Initialise variables
	textureSetup();										// Set up some default textures
	selectedTexMode = "None";							// Output selected default texture filtering option to screen.

	cameraPointer = &freeCamera;						// Set default camera to free camera
	selectedCamera = "Free Camera";						// Output selected default camera to screen.
	tramCamera.setPosition(Vector3(tramX, 5.f, -5.f));	// Set tramCamera position.
	doorCamera.setPosition(Vector3(0.f, 5.f, -40.f));	// Set doorCamera position.
	freeCamera.update();								// Update freeCamera variables
	tramCamera.update();								// Update tramCamera variables
	doorCamera.update();								// Update doorCamera variables

	shape.genDiscData(1.f, 24.f);							// Genereate disc data
	shape.genSphereData(1.f, 20.f);							// Genereate sphere data
	shape.genCylinderData(1.f, 24.f);						// Genereate cylinder data
	shape.genTorusData(0.325f, 24.f);						// Genereate torus data
	shape.genTramRailData(1.f, 20.f);						// Genereate tram rail data
	shape.genWall(1.f, 20.f);								// Genereate wall data
}

void Scene::update(float dt)
{
	// Controls the movement of the tram.
	tramMovement(dt);

	// Set tramCamera position.
	tramCamera.setPosition(Vector3(tramX, 5.f, -5.f));

	// Enable/Disable Wireframe Mode
	wireframeMode();

	// Move the camera via keyboard controls.
	cameraMovement(dt);

	// Update scene related variables.
	cameraRotation();

	// Switch between different cameras in the scene.
	cameraSelection();

	// Swap between texture filtering modes.
	texFilterMode();

	// Calculate FPS for output
	calculateFPS();

	// Controls the opening/closing of the door.
	doorControls(dt);

	// Allows user to reset everything.
	reset();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(cameraPointer->getPosition().getX(), cameraPointer->getPosition().getY(), cameraPointer->getPosition().getZ(), 
		cameraPointer->getLookAt().getX(), cameraPointer->getLookAt().getY(), cameraPointer->getLookAt().getZ(), 
		cameraPointer->getUp().getX(), cameraPointer->getUp().getY(), cameraPointer->getUp().getZ());

	//Set up lights
	lightingSetup();

	// Render geometry/scene here -------------------------------------
	
	skyboxSetup();

	renderScene();

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	glDisable(GL_LIGHTING);	// Disable lighting to prevent issues with text discolouration.
	renderTextOutput();
	glEnable(GL_LIGHTING);	// Re-enable lighting to prevent issues with scene lights.
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 1000.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Shows use of the stencil buffer to create a reflection of a given shape.
void Scene::stencilBufferExample()
{
	// Turn off writing to the frame buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Enable stencil test
	glEnable(GL_STENCIL_TEST);

	// Set stencil function to always pass
	glStencilFunc(GL_ALWAYS, 1, 1);

	// Set the stencil opertaion to replace values when the test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Disable depth test
	glDisable(GL_DEPTH_TEST);

	// Draw wall object
	glPushMatrix();
		glTranslatef(-12.f, 0.f, -55.f);
		glScalef(1.2f, 1.2f, 1.0f);
		shape.renderPlane(NULL);		// Reflection
	glPopMatrix();
	

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Turn on rendering to the frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// Set stencil function to test if the value is '1'
	glStencilFunc(GL_EQUAL, 1, 1);

	// Set the stencil operation to keep all values
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Reflected objects
	glPushMatrix();
		glColor3f(1.f, 1.f, 1.f);
		glScalef(1.0f, 1.0f, -1.0f);
		glTranslatef(tramX, 2.965f, 100.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		tram.render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.f, -37.f);
		renderDoor();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.f, -20.f);
		renderDoorRoom();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.f, -94.5f);
		renderRail();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.f, -40.f);
		renderDoorLocks();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.f, -110.f);
		glRotatef(180.f, 1.f, 0.f, 0.f);
		renderWalkway();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(9.1f, 1.5f, -65.f);
		glRotatef(-45.f, 0.f, 0.f, 1.f);
		glScalef(0.05f, 0.05f, 0.05f);
		crowbar.render();
	glPopMatrix();

	// Disable stencil test
	glDisable(GL_STENCIL_TEST);

	// Enable alpha blending
	glEnable(GL_BLEND);

	// Disable lighting
	glDisable(GL_LIGHTING);

	// Set colour of floor object
	glColor4f(0.4f, 0.4f, 0.5f, 0.8f);

	glPushMatrix();
		glTranslatef(-12.f, 0.f, -55.f);
		glScalef(1.2f, 1.2f, 1.0f);
		shape.renderPlane(NULL);				// Reflection Plane
	glPopMatrix();

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Disable blend
	glDisable(GL_BLEND);

	// Real objects
	glPushMatrix();
		glTranslatef(0.f, 0.f, -3.0f);
		renderDoor();
	glPopMatrix();

	glPushMatrix();
		renderDoorRoom();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.f, -2.5f);
		renderDoorLocks();
	glPopMatrix();

	glPushMatrix();
		renderWalkway();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(9.1f, 1.5f, -45.f);
		glRotatef(-45.f, 0.f, 0.f, 1.f);
		glScalef(0.05f, 0.05f, 0.05f);
		crowbar.render();
	glPopMatrix();
}

// Shows an example of a planar shadow using a model.
void Scene::planarShadow()
{
	glBindTexture(GL_TEXTURE_2D, NULL);

	GLfloat wallVerts[] = { -30.f, 30.f, -34.9f, // top left
							-30.f, -30.f, -34.9f, // bottom left
							30.f, -30.f, -34.9f, // bottom right
							30.f, 30.f, -34.9f };// top right

	// Generate shadow matrix
	shadowMatrix.generateShadowMatrix(shadowMatrixArray, sceneLightPosition, wallVerts);

	// Turn off writing to the frame buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Enable stencil test
	glEnable(GL_STENCIL_TEST);

	// Set stencil function to always pass
	glStencilFunc(GL_ALWAYS, 1, 1);

	// Set the stencil opertaion to replace values when the test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 0.8f);
	glVertex3f(-30.f, 30.f, -34.9f);
	glVertex3f(-30.f, -30.f, -34.9f);
	glVertex3f(30.f, -30.f, -34.9f);
	glVertex3f(30.f, 30.f, -34.9f);
	glEnd();

	// Disable depth test
	glDisable(GL_STENCIL_TEST);

	// Render shadow
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);


	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

	glColor3f(0.1f, 0.1f, 0.1f);	// Shadow's colour
	glPushMatrix();
		glMultMatrixf((GLfloat *)shadowMatrixArray);
		renderRail();
		glTranslatef(tramX, 2.965f, -5.0f);
		glRotatef(90, 0.f, 1.f, 0.f);
		tram.render();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 1.0f, 1.0f); // Model colour
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	// render object
	glPushMatrix();
		glTranslatef(tramX, 2.965f, -5.0f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		tram.render();
	glPopMatrix();

	glPushMatrix();
		renderRail();
	glPopMatrix();
}

// Resets all variables to default values within the scene.
void Scene::reset()
{
	// Reset camera variables.
	if (input->isKeyDown('r'))
	{
		cameraPointer->reset();
		doorCamera.setPosition(Vector3(0.f, 5.f, -40.f));
		cameraPointer->update();
		tramX = 0.0f, doorLockX = 0.0f, doorLock2X = 0.0f;
		bottomDoorY = 0.0f, topDoorY = 6.0f;
		angle = 0.0f, angle2 = 0.0f;
		wireframe = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		input->SetKeyUp('r');
	}
}

// Sets up the skybox.
void Scene::skyboxSetup()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	glColor3f(0.15f, 0.15f, 0.15f); // 0.125 is close to wall colour

	glPushMatrix();
		glTranslatef(cameraPointer->getPosition().x, cameraPointer->getPosition().y, cameraPointer->getPosition().z);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		// Front face
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(0.5f, 0.5f, -0.5f);
	
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
	
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
	
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		// Back face
		glTexCoord2f(0.75f, 0.25f);
		glVertex3f(0.5f, 0.5f, 0.5f);
	
		glTexCoord2f(1.f, 0.25f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
	
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
	
		glTexCoord2f(0.75f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		// Left face
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
	
		glTexCoord2f(0.0f, 0.25f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
	
		glTexCoord2f(0.0f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
	
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);

		// Right face
		glTexCoord2f(0.75f, 0.25f);
		glVertex3f(0.5f, 0.5f, 0.5f);
	
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(0.5f, 0.5f, -0.5f);
	
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
	
		glTexCoord2f(0.75f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		// Top face
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(0.5f, 0.5f, -0.5f);
	
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
	
		glTexCoord2f(0.25f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
	
		glTexCoord2f(0.5f, 0.0f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		// Bottom face
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
	
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
	
		glTexCoord2f(0.25f, 0.75f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
	
		glTexCoord2f(0.5f, 0.75f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glEnd();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	glPopMatrix();
}

// Set up some lighting variables.
void Scene::lightingSetup()
{
	
#pragma region Door Light 1 (Spot)  

	// Door Light 1 (Spot)
	glPushMatrix();
		glRotatef(angle, 0.f, 1.f, 0.f);
		GLfloat Light_Ambient[] = { 0.4f, 0.0f, 0.0f, 0.0f };
		GLfloat Light_Diffuse[] = { 1.0f, 0.0f, 0.0f, 0.0f };
		GLfloat Light_Position[] = { -12.0f, 12.5f, -34.75f, 1.0f };
		doorLight1Pos.x = Light_Position[0];
		doorLight1Pos.y = Light_Position[1];
		doorLight1Pos.z = Light_Position[2];
		GLfloat spot_Direction[] = { 0.0f, 0.0f, -1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.f);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_Direction);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.f);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.f);
	glPopMatrix();

#pragma endregion Top left door light

#pragma region Door Light 2 (Spot) 

	// Door Light 2 (Spot)
	glPushMatrix();
		glRotatef(-angle, 0.f, 1.f, 0.f);
		GLfloat Light_Ambient2[] = { 0.4f, 0.0f, 0.0f, 0.0f };
		GLfloat Light_Diffuse2[] = { 1.0f, 0.0f, 0.0f, 0.0f };
		GLfloat Light_Position2[] = { 12.0f, 12.5f, -34.75f, 1.0f };
		doorLight2Pos.x = Light_Position2[0];
		doorLight2Pos.y = Light_Position2[1];
		doorLight2Pos.z = Light_Position2[2];
		GLfloat spot_Direction2[] = { 0.0f, 0.0f, -1.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, Light_Ambient2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Diffuse2);
		glLightfv(GL_LIGHT1, GL_POSITION, Light_Position2);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.f);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_Direction2);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.f);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.f);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.f);
	glPopMatrix();

#pragma endregion Top right door light

#pragma region Tram Light Front (Spot)

	// Tram Light Front (Spot)
	glPushMatrix();
		GLfloat Light_Ambient3[] = { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat Light_Diffuse3[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat Light_Position3[] = { tramX - 6.5f, 3.5f, -5.0f, 1.0f };
		tramLight1Pos.x = Light_Position3[0];
		tramLight1Pos.y = Light_Position3[1];
		tramLight1Pos.z = Light_Position3[2];
		GLfloat spot_Direction3[] = { -1.0f, 0.0f, 0.0f };
		glLightfv(GL_LIGHT2, GL_AMBIENT, Light_Ambient3);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, Light_Diffuse3);
		glLightfv(GL_LIGHT2, GL_POSITION, Light_Position3);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.f);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_Direction3);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 20.f);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.f);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.f);
	glPopMatrix();

#pragma endregion Front tram light

#pragma region Tram Light Back (Spot)

	// Tram Light Back (Spot)
	glPushMatrix();
		GLfloat Light_Ambient4[] = { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat Light_Diffuse4[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat Light_Position4[] = { tramX + 6.65f, 3.5f, -5.0f, 1.0f };
		tramLight2Pos.x = Light_Position4[0];
		tramLight2Pos.y = Light_Position4[1];
		tramLight2Pos.z = Light_Position4[2];
		GLfloat spot_Direction4[] = { 1.0f, 0.0f, 0.0f };
		glLightfv(GL_LIGHT3, GL_AMBIENT, Light_Ambient4);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, Light_Diffuse4);
		glLightfv(GL_LIGHT3, GL_POSITION, Light_Position4);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90.f);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_Direction4);
		glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 20.f);
		glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.f);
		glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.f);
	glPopMatrix();

#pragma endregion Back tram light

#pragma region Tram Dock Light Left (Point)

	// Tram Dock Light Left (Point)
	glPushMatrix();
		GLfloat Light_Ambient5[] = { 0.4f, 0.4f, 0.0f, 1.0f };
		GLfloat Light_Diffuse5[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		GLfloat Light_Position5[] = { -49.8f, 6.0f, -5.0f, 1.0f };
		dockLight1Pos.x = Light_Position5[0];
		dockLight1Pos.y = Light_Position5[1];
		dockLight1Pos.z = Light_Position5[2];
		glLightfv(GL_LIGHT4, GL_AMBIENT, Light_Ambient5);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, Light_Diffuse5);
		glLightfv(GL_LIGHT4, GL_POSITION, Light_Position5);
		glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.25f);
		glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.05f);
		glEnable(GL_LIGHT4);
	glPopMatrix();

#pragma endregion Left dock light

#pragma region Tram Dock Light Right (Point)

	// Tram Dock Light Right (Point)
	glPushMatrix();
		GLfloat Light_Ambient6[] = { 0.4f, 0.4f, 0.0f, 1.0f };
		GLfloat Light_Diffuse6[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		GLfloat Light_Position6[] = { 49.8f, 6.0f, -5.0f, 1.0f };
		dockLight2Pos.x = Light_Position6[0];
		dockLight2Pos.y = Light_Position6[1];
		dockLight2Pos.z = Light_Position6[2];
		glLightfv(GL_LIGHT5, GL_AMBIENT, Light_Ambient6);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, Light_Diffuse6);
		glLightfv(GL_LIGHT5, GL_POSITION, Light_Position6);
		glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.25f);
		glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.05f);
		glEnable(GL_LIGHT5);
	glPopMatrix();

#pragma endregion Right dock light

#pragma region Scene lighting (Point)

	// Scene lighting (Point)
	glPushMatrix();
		GLfloat Light_Ambient7[] = { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat Light_Diffuse7[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat Light_Specular7[] = { 0.5f, 0.5f, 0.5f, 1.f };
		GLfloat Light_Position7[] = { 0.f, 9.0f, 24.f, 1.0f };
		sceneLightPosition[0] = Light_Position7[0];
		sceneLightPosition[1] = Light_Position7[1];
		sceneLightPosition[2] = Light_Position7[2];
		glLightfv(GL_LIGHT6, GL_AMBIENT, Light_Ambient7);
		glLightfv(GL_LIGHT6, GL_DIFFUSE, Light_Diffuse7);
		glLightfv(GL_LIGHT6, GL_POSITION, Light_Position7);
		glLightfv(GL_LIGHT6, GL_SPECULAR, Light_Specular7);
		glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.2f);
		glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.f);
		glEnable(GL_LIGHT6);
	glPopMatrix();

#pragma endregion Lights up the entire scene a small amount

}

// Move the camera around the scene via keyboard controls.
void Scene::cameraMovement(float dt)
{
	// Camera movement controls.
	if (cameraPointer == &freeCamera)	// Restrict movement to freeCamera only.
	{
		// Move Right.
		if (input->isKeyDown('d'))
		{
			cameraPointer->moveRight(dt);
		}
		// Move Forwards.
		if (input->isKeyDown('w'))
		{
			cameraPointer->moveForward(dt);
		}
		// Move Left.
		if (input->isKeyDown('a'))
		{
			cameraPointer->moveLeft(dt);
		}
		// Move Backwards.
		if (input->isKeyDown('s'))
		{
			cameraPointer->moveBackwards(dt);
		}

		// Move camera up(y-axis, rotation independant).
		if (input->isKeyDown(32))	// Spacebar
		{
			cameraPointer->moveUp(dt);
		}
		// Move camera down(y-axis, rotation independant).
		if (input->isKeyDown('c'))
		{
			cameraPointer->moveDown(dt);
		}
	}
}

// Alters the cameras rotation based on the location of the mouse.
void Scene::cameraRotation()
{
	if (cameraPointer == &freeCamera || cameraPointer == &tramCamera)	// Restrict camera rotation to freeCamera and tramCamera.
	{
		//Mouse rotation for the camera
		cameraPointer->setYaw(((input->getMouseX() - (width / 2.f)) / 10.f));
		cameraPointer->setPitch(-1.f * ((input->getMouseY() - (height / 2.f)) / 10.f));
		cameraPointer->update();
	}
	glutWarpPointer(width / 2, height / 2);
}

// Allows user to switch between the different cameras.
void Scene::cameraSelection()
{
	if (input->isKeyDown('7'))
	{
		// Select freeCamera.
		cameraPointer = &freeCamera;
		selectedCamera = "Free Camera";
	}
	if (input->isKeyDown('8'))
	{
		// Select tramCamera.
		cameraPointer = &tramCamera;
		selectedCamera = "Tram Camera";
	}
	if (input->isKeyDown('9'))
	{
		// Select doorCamera.
		cameraPointer = &doorCamera;
		selectedCamera = "Door Camera";
	}
}

// Sets up textures to be used in the scene.
void Scene::textureSetup()
{
	doorTopTexture = SOIL_load_OGL_texture
	(
		"gfx/doorTop.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	doorBottomTexture = SOIL_load_OGL_texture
	(
		"gfx/doorBottom.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	doorTopTextureFlipped = SOIL_load_OGL_texture
	(
		"gfx/doorTopFlipped.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	doorBottomTextureFlipped = SOIL_load_OGL_texture
	(
		"gfx/doorBottomFlipped.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	grateTexture = SOIL_load_OGL_texture
	(
		"gfx/grate.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	hazardTexture = SOIL_load_OGL_texture
	(
		"gfx/hazard.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	wallTexture = SOIL_load_OGL_texture
	(
		"gfx/wall.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

// Sets up specular material values to be used in the scene.
void Scene::specularMaterials()
{
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_specular_low[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat no_shininess[] = { 0.0f };
	GLfloat low_shininess[] = { 5.f };
	GLfloat high_shininess[] = { 100.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_low);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
}

// Allows wireframe to be turned on/off.
void Scene::wireframeMode()
{
	if (wireframe && input->isKeyDown('f'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		input->SetKeyUp('f');
		wireframe = false;
	}
	if (!wireframe && input->isKeyDown('f'))
	{
		wireframe = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		input->SetKeyUp('f');
	}
}

// Allows user to switch between texture filtering modes.
void Scene::texFilterMode()
{
	/*
		GL_NEAREST					// Point Sampling
		GL_LINEAR					// Bilinear
		GL_NEAREST_MIPMAP_NEAREST	// Point sampling on single mip level
		GL_LINEAR_MIPMAP_NEAREST	// Bilinear on nearest mipmap level
		GL_NEAREST_MIPMAP_LINEAR	// Point sampling on two mip level and blends
		GL_LINEAR_MIPMAP_LINEAR		// Trilinear
	*/
	if (input->isKeyDown('1'))
	{
		// Point Sampling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// Far/Minify
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// Near/Magnify
		selectedTexMode = "Point Sampling";
	}
	else if (input->isKeyDown('2'))
	{
		// Bilinear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Far/Minify
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Near/Magnify
		selectedTexMode = "Bilinear";
	}
	else if (input->isKeyDown('3'))
	{
		// Trilinear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Far/Minify
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Near/Magnify
		selectedTexMode = "Trilinear";
	}
	else if (input->isKeyDown('4'))
	{
		// Point/Trilinear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Far/Minify
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// Near/Magnify
		selectedTexMode = "Point/Trilinear";
	}	
}

// Renders the scene.
void Scene::renderScene()
{
	renderEnclosure();

	//renderLightSpheres();

	stencilBufferExample();

	planarShadow();

	renderLeftDock();

	renderRightDock();
}

// Allows user to move the tram forwards/backwards.
void Scene::tramMovement(float dt)
{
	// Move tram backwards
	if (input->isKeyDown('k'))
	{
		if (tramX < 40.0f)
		{
			tramX += 1.f*dt;
			glEnable(GL_LIGHT3);
		}
	}
	// Move tram forwards
	else if (input->isKeyDown('i'))
	{
		if (tramX > -40.0f)
		{
			tramX -= 1.f*dt;
			glEnable(GL_LIGHT2);
		}
	}
	else if (!input->isKeyDown('i') && !input->isKeyDown('k'))
	{
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
	}
}

// Allows the user to open/close the door.
void Scene::doorControls(float dt)
{
	// Close doors
	if (input->isKeyDown('q'))
	{
		if (bottomDoorY < 0.0f && topDoorY > 6.0f)
		{
			bottomDoorY += 0.25f*dt;
			topDoorY -= 0.25f*dt;
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			angle += 25.0f * dt;
		}
		if (doorLockX >= 0.f && doorLock2X <= 0.f && bottomDoorY >= 0.0f && topDoorY >= 5.9f)
		{
			doorLockX -= 1.0f*dt;
			doorLock2X += 1.0f*dt;
			angle2 += 50.f*dt;
		}
		
	}
	// Opens doors
	else if (input->isKeyDown('e'))
	{
		if (doorLockX >= 22.f && doorLock2X <= -22.f && bottomDoorY > -5.9f && topDoorY < 11.9f)
		{
			bottomDoorY -= 0.25f*dt;
			topDoorY += 0.25f*dt;
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			angle += 25.0f * dt;
		}
		if (doorLockX < 22.f && doorLock2X > -22.f)
		{
			doorLockX += 1.0f*dt;
			doorLock2X -= 1.0f*dt;
			angle2 -= 50.f*dt;
		}
	}
	if (!input->isKeyDown('q') && !input->isKeyDown('e'))
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
}

// Renders spheres at each lights location to allow lights to be seen easier.
void Scene::renderLightSpheres()
{
	// Render a sphere at top left of door (LIGHT_0) position
	glPushMatrix();
	glColor3f(1.f, 0.f, 0.f);
	glTranslatef(doorLight1Pos.x, doorLight1Pos.y, doorLight1Pos.z);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();

	// Render a sphere at top right of door (LIGHT_1) position
	glPushMatrix();
	glColor3f(1.f, 0.f, 0.f);
	glTranslatef(doorLight2Pos.x, doorLight2Pos.y, doorLight2Pos.z);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();

	// Render a sphere at front of tram (LIGHT_2) position
	glPushMatrix();
	glColor3f(1.f, 0.8f, 1.f);
	glTranslatef(tramLight1Pos.x, tramLight1Pos.y, tramLight1Pos.z);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();

	// Render a sphere at back of tram (LIGHT_3) position
	glPushMatrix();
	glColor3f(1.f, 0.8f, 1.f);
	glTranslatef(tramLight2Pos.x, tramLight2Pos.y, tramLight2Pos.z);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();

	// Render a sphere at left dock (LIGHT_4) position
	glPushMatrix();
	glColor3f(1.f, 0.8f, 1.f);
	glTranslatef(dockLight1Pos.x, dockLight1Pos.y, dockLight1Pos.z);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();

	// Render a sphere at right dock (LIGHT_5) position
	glPushMatrix();
	glColor3f(1.f, 0.8f, 1.f);
	glTranslatef(dockLight2Pos.x, dockLight2Pos.y, dockLight2Pos.z);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();

	// Render a sphere at scene lights location (LIGHT_6) position
	glPushMatrix();
	glColor3f(1.f, 1.0f, 1.f);
	glTranslatef(sceneLightPosition[0], sceneLightPosition[1], sceneLightPosition[2]);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glPopMatrix();
}

// Renders the trams rail.
void Scene::renderRail()
{
	// Render rail 0
	glPushMatrix();
	glTranslatef(30.f, 10.99f, -5.5f);
	shape.renderTramRail(hazardTexture, hazardTexture);
	glPopMatrix();

	// Render rail 1
	glPushMatrix();
	glTranslatef(10.f, 10.99f, -5.5f);
	shape.renderTramRail(hazardTexture, hazardTexture);
	glPopMatrix();

	// Render rail 2
	glPushMatrix();
	glTranslatef(-10.f, 10.99f, -5.5f);
	shape.renderTramRail(hazardTexture, hazardTexture);
	glPopMatrix();

	// Render rail 3
	glPushMatrix();
	glTranslatef(-30.f, 10.99f, -5.5f);
	shape.renderTramRail(hazardTexture, hazardTexture);
	glPopMatrix();

	// Render rail 4
	glPushMatrix();
	glTranslatef(-50.f, 10.99f, -5.5f);
	shape.renderTramRail(hazardTexture, hazardTexture);
	glPopMatrix();
}

// Renders the tram.
void Scene::renderTram()
{
	// Render tram
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(tramX, 2.965f, -5.0f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glColor3f(1.0f, 1.0f, 1.0f);
	tram.render();
	glPopMatrix();
}

// Renders the door.
void Scene::renderDoor()
{
	// Door Top
	glPushMatrix();
	glTranslatef(-12.f, topDoorY, -36.05f);
	glScalef(1.2f, 6.0f, 1.0f);
	shape.renderTramRail(doorTopTexture, doorTopTextureFlipped);
	glPopMatrix();

	// Door Bottom
	glPushMatrix();
	glTranslatef(-12.f, bottomDoorY, -36.05f);
	glScalef(1.2f, 6.0f, 1.0f);
	shape.renderTramRail(doorBottomTexture, doorBottomTextureFlipped);
	glPopMatrix();
}

// Renders the room behind the door.
void Scene::renderDoorRoom()
{
	// Render Door room
	glPushMatrix();
	glTranslatef(12.f, 0.f, -55.f);
	glRotatef(-90.f, 0.f, 1.f, 0.f);
	glScalef(1.0f, 12.0f, 24.0f);
	shape.renderTramDock(wallTexture);
	glPopMatrix(); 
}

// Renders the walkway.
void Scene::renderWalkway()
{
	glEnable(GL_BLEND);
	// Render walkway part 1
	glPushMatrix();
	glTranslatef(-18.0f, 0.0f, -35.0f);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glScalef(1.8f, 1.0f, 1.0f);
	shape.renderPlane(grateTexture);
	glPopMatrix();

	// Render walkway part 2
	glPushMatrix();
	glTranslatef(-12.0f, 0.0f, -25.0f);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glScalef(1.2f, 1.75f, 1.0f);
	shape.renderPlane(grateTexture);
	glPopMatrix();
	glDisable(GL_BLEND);
}

// Renders the left tram dock.
void Scene::renderLeftDock()
{
	// Render left room
	glPushMatrix();
		glTranslatef(-50.f, 0.f, -17.0f);
		glScalef(1.0f, 12.0f, 24.0f);
		shape.renderTramDock(wallTexture);
		glPushMatrix();
			glScalef(1.0f, 0.1f, 0.05f);
			glRotatef(-90.f, 0.f, 1.f, 0.f);
			shape.renderPlane(wallTexture);
		glPopMatrix();
	glPopMatrix();
}

// Renders the right tram dock.
void Scene::renderRightDock()
{
	// Render right room
	glPushMatrix();
		glTranslatef(30.f, 0.f, -17.0f);
		glScalef(1.0f, 12.0f, 24.0f);
		shape.renderTramDock(wallTexture);
		glPushMatrix();
			glTranslatef(20.f, 0.f, 1.f);
			glRotatef(-270.f, 0.f, 1.f, 0.f);
			glScalef(0.05f, 0.1f, 1.f);
			shape.renderPlane(wallTexture);
		glPopMatrix();
	glPopMatrix();
}

// Renders the walls and floor of the scene.
void Scene::renderEnclosure()
{
	// Render bottom plane
	glPushMatrix();
		glColor3f(0.f, 0.f, 0.f);
		glTranslatef(-30.0f, -30.0f, -35.0f);
		glRotatef(90.f, 1.f, 0.f, 0.f);
		glScalef(3.0f, 6.0f, 1.0f);
		shape.renderPlane(NULL);
	glPopMatrix();

	// Render back wall
	glPushMatrix();
		glColor3f(0.6f, 0.6f, 0.6f);
		glTranslatef(-30.0f, -30.0f, -35.0f);
		glScalef(3.0f, 6.0f, 1.0f);
		shape.renderWall(wallTexture);
	glPopMatrix();

	// Render left wall
	glPushMatrix();
		glColor3f(0.6f, 0.6f, 0.6f);
		glTranslatef(-30.0f, -30.0f, 25.0f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		glScalef(3.0f, 6.0f, 1.0f);
		shape.renderWall(wallTexture);
	glPopMatrix();

	// Render right wall
	glPushMatrix();
		glColor3f(0.6f, 0.6f, 0.6f);
		glTranslatef(30.0f, -30.0f, -35.f);
		glRotatef(270.f, 0.f, 1.f, 0.f);
		glScalef(3.0f, 6.0f, 1.0f);
		shape.renderWall(wallTexture);
	glPopMatrix();
}

// Renders the cylinders, discs and torus's in front of the door (which resemble door locks).
void Scene::renderDoorLocks()
{
	specularMaterials();

	glBindTexture(GL_TEXTURE_2D, NULL);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslatef(doorLock2X - 12.f, 2.f, -34.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		shape.renderCylinder();
		glPushMatrix();
			glTranslatef(0.325f, -1.f, -doorLock2X - 23.f);
			glRotatef(90.f, 0.f, 1.f, 0.f);
			glRotatef(angle2, 0.f, 0.f, 1.f);
			shape.renderDisc();
			glPushMatrix();
				glTranslatef(0.f, 0.f, -0.325f);
				glRotatef(angle2, 0.f, 0.f, 1.f);
				shape.renderTorus();
			glPopMatrix();
			glTranslatef(0.f, 0.f, -0.65f);
			shape.renderDisc();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(doorLockX - 12.f, 10.f, -34.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		shape.renderCylinder();
		glPushMatrix();
			glTranslatef(0.325f, 1.f, -doorLockX - 1.f);
			glRotatef(90.f, 0.f, 1.f, 0.f);
			glRotatef(angle2, 0.f, 0.f, 1.f);
			shape.renderDisc();
				glPushMatrix();
					glTranslatef(0.f, 0.f, -0.325f);
					glRotatef(angle2, 0.f, 0.f, 1.f);
					shape.renderTorus();
				glPopMatrix();
			glTranslatef(0.f, 0.f, -0.65f);
			shape.renderDisc();
		glPopMatrix();
	glPopMatrix();
}

// Calculates FPS.
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 1.f, 1.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 1.f, 1.f, fps);
	sprintf_s(textureText, "Texture Mode: %s", selectedTexMode.c_str());
	displayText(-1.f, 0.84f, 1.f, 1.f, 1.f, textureText);
	sprintf_s(cameraText, "Selected Camera: %s", selectedCamera.c_str());
	displayText(-1.f, 0.78f, 1.f, 1.f, 1.f, cameraText);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
