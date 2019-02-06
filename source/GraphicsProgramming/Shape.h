#ifndef _SHAPE_H
#define _SHAPE_H

#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "Vector3.h"

class Shape
{

	public:
		// Render fuction using dereferencing method 1.
		void render1();
		// Renders a disc using dereferencing method 2 (Accessing full arrays).
		void renderDisc();
		// Renders a sphere using dereferencing method 2 (Accessing full arrays).
		void renderSphere();
		// Renders a cylinder using dereferencing method 2 (Accessing full arrays).
		void renderCylinder();
		// Renders a torus using dereferencing method 2 (Accessing full arrays).
		void renderTorus();
		// Render function using dereferencing method 3.
		void render3();

		// Renders a tram rail using dereferencing method 2 (Accessing full arrays).
		void renderTramRail(GLuint texture, GLuint texture2);
		// Renders a tram dock using dereferencing method 2 (Accessing full arrays).
		void renderTramDock(GLuint texture);
		// Renders a large plane using dereferencing method 2 (Accessing full arrays).
		void renderPlane(GLuint texture);
		// Render wall with a hole in it using dereferencing method 2 (Accessing full arrays).
		void renderWall(GLuint texture);

		// Generates all required data for rendering a disc.
		void genDiscData(float radius, float segments);
		// Generates all required data for rendering a sphere.
		void genSphereData(float radius, float segments);
		// Generates all required data for rendering a cylinder.
		void genCylinderData(float radius, float segments);
		// Generates all required data for rendering a torus.
		void genTorusData(float radius, float segments);

		// Generates all required data for rendering a tram rail.
		void genTramRailData(float radius, float segments);
		// Generates all required data for rendering a wall with a hole in it.
		void genWall(float radius, float segments);

	private:
		// Variable used to translate a disc to "cap" a cylinder.
		float cylinderSeg;
		// Vertex and Normal vector which store the info required for shape rendering and lighting.
		std::vector<Vector3> discVertex, discNormals, sphereVertex, sphereNormals, cylinderVertex, cylinderNormals, torusVertex, torusNormals, tramRailVertex, tramRailNormals, wallVertex, wallNormals;
		// TexCoords vector which store the info required for shape texturing.
		std::vector<float> discTexCoords, sphereTexCoords, cylinderTexCoords, torusTexCoords, tramRailTexCoords, wallTexCoords;
};
#endif 
