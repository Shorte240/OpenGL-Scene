#include "shape.h"
#define PI 3.14159265

// Vertex array to allow rendering of a cube.
extern float cubeVerts[] = {	-1.0, -1.0, -1.0,			// Vertex #0
							1.0, -1.0, -1.0,		// Vertex #1
							1.0,  1.0, -1.0,		// Vertex #2
							-1.0,  1.0, -1.0,			// Vertex #3
							-1.0, -1.0, 1.0,		// Vertex #4
							1.0, -1.0, 1.0,			// Vertex #5
							1.0, 1.0, 1.0,			// Vertex #6
							-1.0, 1.0, 1.0			// Vertex #7
						};

// Normal array to allow lighting of a cube.
extern float norms[] = { 0.0, 0.0, 1.0,		//0
						0.0, 0.0, 1.0,		//1
 						0.0, 0.0, 1.0,		//2
						0.0, 0.0, 1.0,		//3
						};		

// TexCoords array to allow texturing of a cube.
extern float texcoords[]= {	0.0, 0.0, 		//0
							0.0, 1.0, 		//1
 							1.0, 1.0, 		//2
							1.0, 0.0, 		//3
							};

// Indices array to allow rendering of a cube without the vertex's being in the correct order.
extern GLubyte cubeIndices[] = { 1, 0, 2,		//front
							2, 0, 3,
							4, 5, 7,		//back
							7, 5, 6,
							0, 4, 3,		//left
							3, 4, 7,
							5, 1, 6,		//right
							6, 1, 2,
							7, 6, 3,		//up
							3, 6, 2,
							1, 0, 5,		//down
							5, 0, 4};

// Render function utilising dereferencing of single array elements.
void Shape::render1()
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, cubeVerts);			// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, norms);				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);		// Pointer to texture co-ords array

	glBegin(GL_QUADS);
	glArrayElement(0);
	glArrayElement(1);
	glArrayElement(2);
	glArrayElement(3);
	glEnd();

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a disc.
void Shape::renderDisc()
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, discVertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, discNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, discTexCoords.data());		// Pointer to texture co-ords array

	glDrawArrays(GL_TRIANGLE_FAN, 0, discVertex.size());

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a sphere.
void Shape::renderSphere()
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, sphereVertex.data());			// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, sphereNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, sphereTexCoords.data());		// Pointer to texture co-ords array

	glDrawArrays(GL_QUADS, 0, sphereVertex.size());

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a cylinder.
void Shape::renderCylinder()
{
	renderDisc();										// Add disc to front of cylinder
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, cylinderVertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, cylinderNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, cylinderTexCoords.data());		// Pointer to texture co-ords array

	glDrawArrays(GL_QUADS, 0, cylinderVertex.size());

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
	glTranslatef(0, 0, cylinderSeg);					// Add disc to end of cylinder
	renderDisc();
}

// Renders, textures and allows lighting of a torus.
void Shape::renderTorus()
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, torusVertex.data());			// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, torusNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, torusTexCoords.data());		// Pointer to texture co-ords array

	glDrawArrays(GL_QUADS, 0, torusVertex.size());

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Render function utilising dereferencing of all array elements and implementation of an index array.
void Shape::render3()
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, cubeVerts);			// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, norms);				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);		// Pointer to texture co-ords array

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a tram rail.
void Shape::renderTramRail(GLuint texture, GLuint texture2)
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, tramRailVertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, tramRailNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, tramRailTexCoords.data());		// Pointer to texture co-ords array

	// Essentially the same as renderPlane although with the rotations and limitations on the Vertex data,
	// It folds the plan over and restricts rendering of only a cuboid.
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture2);
	glScalef(1.0f, 0.1f, 0.1f);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Back face/Face 1
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Bottom face/Face 2
	glTranslatef(0, 10, -10);
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Front face/Face 3
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Top face/Face 4
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a tram dock.
void Shape::renderTramDock(GLuint texture)
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, tramRailVertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, tramRailNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, tramRailTexCoords.data());		// Pointer to texture co-ords array

	// Essentially the same as renderTramRail although implemented as only uses one texture.
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glScalef(1.0f, 0.1f, 0.1f);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Back face/Face 1
	glRotatef(90, 1, 0, 0);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Bottom face/Face 2
	glTranslatef(0, 10, -10);
	glRotatef(90, 1, 0, 0);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Front face/Face 3
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());	// Top face/ Face 4
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a flat plane.
void Shape::renderPlane(GLuint texture)
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, tramRailVertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, tramRailNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, tramRailTexCoords.data());		// Pointer to texture co-ords array

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_QUADS, 0, tramRailVertex.size());
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Renders, textures and allows lighting of a wall with a hole in it.
void Shape::renderWall(GLuint texture)
{
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, wallVertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, wallNormals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, wallTexCoords.data());		// Pointer to texture co-ords array

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_QUADS, 0, wallVertex.size());
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}

// Generates all vertex's, normals and texture coordinates required to render a disc.
void Shape::genDiscData(float radius, float segments)
{
	float r = radius;
	float d = 2 * r;

	Vector3 dVerts;	// Central point of disc
	discVertex.push_back(dVerts);
	discTexCoords.push_back(0.5);	// Tex Co-ord in centre of disc - U
	discTexCoords.push_back(0.5);	// Tex Co-ord in centre of disc - V
	Vector3 dNorms = Vector3(0, 0, 1); // Normal in positive Z
	discNormals.push_back(dNorms);

	for (int i = 0; i < segments + 1; i++)
	{
		float theta = (i*(2 * PI)) / (segments);
		dVerts.x = r*cos(theta);
		dVerts.y = r*sin(theta);
		dVerts.z = 0;
		discVertex.push_back(dVerts);

		float u = (cos(theta) / d) + 0.5;	// Tex Co-ord - U
		float v = (sin(theta) / d) + 0.5;	// Tex Co-ord - V
		discTexCoords.push_back(u);
		discTexCoords.push_back(v);

		Vector3 dNorms = Vector3(0, 0, 1); // Normal in positive Z
		discNormals.push_back(dNorms);
	}
}

// Generates all vertex's, normals and texture coordinates required to render a sphere.
void Shape::genSphereData(float radius, float segments)
{
	float r = radius;
	float d = 2 * r;

	Vector3 sVerts;
	Vector3 sNorms;

	for (int lon = 0; lon < segments; lon++)
	{
		float delta = (lon*PI) / (segments);			// Angle of longitude

		for (int lat = 0; lat < segments; lat++)
		{
			float theta = (lat*2 * PI) / (segments);	// Angle of latitude
			sVerts.x = (r*cos(theta))*(sin(delta));
			sVerts.y = r*cos(delta);
			sVerts.z = (r*sin(theta))*(sin(delta));
			sphereVertex.push_back(sVerts);
			sNorms = sVerts;
			sNorms.x = sNorms.x / r;
			sNorms.y = sNorms.y / r;
			sNorms.z = sNorms.z / r;
			sphereNormals.push_back(sNorms);

			lon++;
			delta = (lon*PI) / (segments);
			sVerts.x = (r*cos(theta))*(sin(delta));
			sVerts.y = r*cos(delta);
			sVerts.z = (r*sin(theta))*(sin(delta));
			sphereVertex.push_back(sVerts);
			sNorms = sVerts;
			sNorms.x = sNorms.x / r;
			sNorms.y = sNorms.y / r;
			sNorms.z = sNorms.z / r;
			sphereNormals.push_back(sNorms);

			lat++;
			theta = (lat * 2 * PI) / (segments);
			sVerts.x = (r*cos(theta))*(sin(delta));
			sVerts.y = r*cos(delta);
			sVerts.z = (r*sin(theta))*(sin(delta));
			sphereVertex.push_back(sVerts);
			sNorms = sVerts;
			sNorms.x = sNorms.x / r;
			sNorms.y = sNorms.y / r;
			sNorms.z = sNorms.z / r;
			sphereNormals.push_back(sNorms);

			lon--;
			delta = (lon*PI) / (segments);
			sVerts.x = (r*cos(theta))*(sin(delta));
			sVerts.y = r*cos(delta);
			sVerts.z = (r*sin(theta))*(sin(delta));
			sphereVertex.push_back(sVerts);
			sNorms = sVerts;
			sNorms.x = sNorms.x / r;
			sNorms.y = sNorms.y / r;
			sNorms.z = sNorms.z / r;
			sphereNormals.push_back(sNorms);
			lat--;

			float u1 = 0 + (lat * (1 / (segments)));
			float v1 = 0 + (lon * (1 / (segments)));
			float u2 = u1;
			float v2 = v1 + (1 / (segments));
			float u3 = u2 + (1 / (segments));
			float v3 = v2;
			float u4 = u3;
			float v4 = v1;
			sphereTexCoords.push_back(u1);
			sphereTexCoords.push_back(v1);
			sphereTexCoords.push_back(u2);
			sphereTexCoords.push_back(v2);
			sphereTexCoords.push_back(u3);
			sphereTexCoords.push_back(v3);
			sphereTexCoords.push_back(u4);
			sphereTexCoords.push_back(v4);
		}
	}
}

// Generates all vertex's, normals and texture coordinates required to render a cylinder.
void Shape::genCylinderData(float radius, float segments)
{
	float r = radius;
	cylinderSeg = segments;
	float d = 2 * r;

	Vector3 cVerts;
	Vector3 cNorms;

	for (int column = 0; column < segments; column++)
	{
		for (int stacks = 0; stacks < segments; stacks++)
		{
			float theta = (stacks * 2 * PI) / (segments);	// Angle of latitude
			cVerts.x = r*cos(theta);
			cVerts.y = r*sin(theta);
			cVerts.z = column;
			cylinderVertex.push_back(cVerts);
			cNorms = cVerts;
			cNorms.x = cNorms.x / r;
			cNorms.y = cNorms.y / r;
			cNorms.z = cNorms.z / r;
			cylinderNormals.push_back(cNorms);

			stacks++;
			theta = (stacks * 2 * PI) / (segments);
			cVerts.x = r*cos(theta);
			cVerts.y = r*sin(theta);
			cVerts.z = column;
			cylinderVertex.push_back(cVerts);
			cNorms = cVerts;
			cNorms.x = cNorms.x / r;
			cNorms.y = cNorms.y / r;
			cNorms.z = cNorms.z / r;
			cylinderNormals.push_back(cNorms);
			stacks--;

			stacks++;
			theta = (stacks * 2 * PI) / (segments);
			cVerts.x = r*cos(theta);
			cVerts.y = r*sin(theta);
			column++;
			cVerts.z = column;
			cylinderVertex.push_back(cVerts);
			cNorms = cVerts;
			cNorms.x = cNorms.x / r;
			cNorms.y = cNorms.y / r;
			cNorms.z = cNorms.z / r;
			cylinderNormals.push_back(cNorms);

			stacks--;
			theta = (stacks * 2 * PI) / (segments);
			cVerts.x = r*cos(theta);
			cVerts.y = r*sin(theta);
			cVerts.z = column;
			cylinderVertex.push_back(cVerts);
			cNorms = cVerts;
			cNorms.x = cNorms.x / r;
			cNorms.y = cNorms.y / r;
			cNorms.z = cNorms.z / r;
			cylinderNormals.push_back(cNorms);
			column--;

			float u1 = 1 - (stacks * (1 / (segments)));
			float v1 = 0 + (column/segments);
			float u2 = u1;
			float v2 = v1 + (1 / (segments));
			float u3 = u2 - (1 / (segments));
			float v3 = v2;
			float u4 = u3;
			float v4 = v1;
			cylinderTexCoords.push_back(u1);
			cylinderTexCoords.push_back(v1);
			cylinderTexCoords.push_back(u4);
			cylinderTexCoords.push_back(v4);
			cylinderTexCoords.push_back(u3);
			cylinderTexCoords.push_back(v3);
			cylinderTexCoords.push_back(u2);
			cylinderTexCoords.push_back(v2);
		}
	}
}

// Generates all vertex's, normals and texture coordinates required to render a torus.
void Shape::genTorusData(float radius, float segments)
{
	float r = radius;	// Minor radius
	float R = 2 * r;	// Major radius

	Vector3 tVerts;
	Vector3 tNorms;

	for (int column = 0; column < segments; column++)
	{
		float delta = (column*2*PI) / (segments);			// Angle of longitude
		for (int stacks = 0; stacks < segments; stacks++)
		{
			float theta = (stacks * 2 * PI) / (segments);	// Angle of latitude
			tVerts.x = ((R + (r*cos(theta)))*(cos(delta)));
			tVerts.y = ((R + (r*cos(theta)))*(sin(delta)));
			tVerts.z = (r*sin(theta));
			torusVertex.push_back(tVerts);
			tNorms = tVerts;
			tNorms.x = tNorms.x / r;
			tNorms.y = tNorms.y / r;
			tNorms.z = tNorms.z / r;
			torusNormals.push_back(tNorms);

			column++;
			delta = (column * 2 * PI) / (segments);
			tVerts.x = ((R + (r*cos(theta)))*(cos(delta)));
			tVerts.y = ((R + (r*cos(theta)))*(sin(delta)));
			tVerts.z = (r*sin(theta));
			torusVertex.push_back(tVerts);
			tNorms = tVerts;
			tNorms.x = tNorms.x / r;
			tNorms.y = tNorms.y / r;
			tNorms.z = tNorms.z / r;
			torusNormals.push_back(tNorms);

			stacks++;
			theta = (stacks * 2 * PI) / (segments);
			tVerts.x = ((R + (r*cos(theta)))*(cos(delta)));
			tVerts.y = ((R + (r*cos(theta)))*(sin(delta)));
			tVerts.z = (r*sin(theta));
			torusVertex.push_back(tVerts);
			tNorms = tVerts;
			tNorms.x = tNorms.x / r;
			tNorms.y = tNorms.y / r;
			tNorms.z = tNorms.z / r;
			torusNormals.push_back(tNorms);

			column--;
			delta = (column * 2 * PI) / (segments);
			tVerts.x = ((R + (r*cos(theta)))*(cos(delta)));
			tVerts.y = ((R + (r*cos(theta)))*(sin(delta)));
			tVerts.z = (r*sin(theta));
			torusVertex.push_back(tVerts);
			tNorms = tVerts;
			tNorms.x = tNorms.x / r;
			tNorms.y = tNorms.y / r;
			tNorms.z = tNorms.z / r;
			torusNormals.push_back(tNorms);
			stacks--;

			float u1 = 0 + (column / segments);
			float v1 = 1 - (stacks * (1 / (segments)));
			float u2 = u1;
			float v2 = v1 - (1 / (segments));
			float u3 = u2 + (1 / (segments));
			float v3 = v2;
			float u4 = u3;
			float v4 = v1;
			torusTexCoords.push_back(u1);
			torusTexCoords.push_back(v1);
			torusTexCoords.push_back(u4);
			torusTexCoords.push_back(v4);
			torusTexCoords.push_back(u3);
			torusTexCoords.push_back(v3);
			torusTexCoords.push_back(u2);
			torusTexCoords.push_back(v2);
		}
	}
}

// Generates all vertex's, normals and texture coordinates required to render a tram rail.
void Shape::genTramRailData(float radius, float segments)
{
	float r = radius;
	float d = 2 * r;

	Vector3 trVerts;
	Vector3 trNorms;

	for (int column = 0; column < segments; column++)
	{
		for (int stacks = 0; stacks < (segments / 2); stacks++)
		{
			trVerts.x = column;
			trVerts.y = stacks;
			trVerts.z = 0;
			tramRailVertex.push_back(trVerts);
			trNorms.x = 0 / r;
			trNorms.y = 0 / r;
			trNorms.z = 1 / r;
			tramRailNormals.push_back(trNorms);

			stacks++;
			trVerts.x = column;
			trVerts.y = stacks;
			trVerts.z = 0;
			tramRailVertex.push_back(trVerts);
			trNorms.x = 0 / r;
			trNorms.y = 0 / r;
			trNorms.z = 1 / r;
			tramRailNormals.push_back(trNorms);
			stacks--;

			stacks++;
			column++;
			trVerts.x = column;
			trVerts.y = stacks;
			trVerts.z = 0;
			tramRailVertex.push_back(trVerts);
			trNorms.x = 0 / r;
			trNorms.y = 0 / r;
			trNorms.z = 1 / r;
			tramRailNormals.push_back(trNorms);

			stacks--;
			trVerts.x = column;
			trVerts.y = stacks;
			trVerts.z = 0;
			tramRailVertex.push_back(trVerts);
			trNorms.x = 0 / r;
			trNorms.y = 0 / r;
			trNorms.z = 1 / r;
			tramRailNormals.push_back(trNorms);
			column--;

			float u1 = 0 + (column * (1 / (segments)));
			float v1 = 0 + (stacks * (1 / (segments / 2)));
			float u2 = u1;
			float v2 = v1 + (1 / (segments/2));
			float u3 = u2 + (1 / (segments));
			float v3 = v2;
			float u4 = u3;
			float v4 = v1;
			tramRailTexCoords.push_back(u1);
			tramRailTexCoords.push_back(v1);
			tramRailTexCoords.push_back(u2);
			tramRailTexCoords.push_back(v2);
			tramRailTexCoords.push_back(u3);
			tramRailTexCoords.push_back(v3);
			tramRailTexCoords.push_back(u4);
			tramRailTexCoords.push_back(v4);
		}
	}
}

void Shape::genWall(float radius, float segments)
{
	float r = radius;
	float d = 2 * r;

	Vector3 wVerts;
	Vector3 wNorms;

	for (int column = 0; column < segments; column++)
	{
		for (int stacks = 0; stacks < segments / 2; stacks++)
		{
			wVerts.x = column;
			wVerts.y = stacks;
			if (column >= 6 && column <= 14 && stacks >= 5 && stacks <= 6)
			{
				wVerts.x = 14;
				wVerts.y = stacks;
			}
			wVerts.z = 0;
			wallVertex.push_back(wVerts);
			wNorms.x = 0 / r;
			wNorms.y = 0 / r;
			wNorms.z = 1 / r;
			wallNormals.push_back(wNorms);

			stacks++;
			wVerts.x = column;
			wVerts.y = stacks;
			if (column >= 6 && column <= 14 && stacks >= 6 && stacks <= 7)
			{
				wVerts.x = 14;
				wVerts.y = stacks;
			}
			wVerts.z = 0;
			wallVertex.push_back(wVerts);
			wNorms.x = 0 / r;
			wNorms.y = 0 / r;
			wNorms.z = 1 / r;
			wallNormals.push_back(wNorms);
			stacks--;

			stacks++;
			column++;
			wVerts.x = column;
			wVerts.y = stacks;
			if (column >= 7 && column <= 14 && stacks >= 6 && stacks <= 7)
			{
				wVerts.x = 14;
				wVerts.y = stacks;
			}
			wVerts.z = 0;
			wallVertex.push_back(wVerts);
			wNorms.x = 0 / r;
			wNorms.y = 0 / r;
			wNorms.z = 1 / r;
			wallNormals.push_back(wNorms);

			stacks--;
			wVerts.x = column;
			wVerts.y = stacks;
			if (column >= 7 && column <= 14 && stacks >= 5 && stacks <= 6)
			{
				wVerts.x = 14;
				wVerts.y = stacks;
			}
			wVerts.z = 0;
			wallVertex.push_back(wVerts);
			wNorms.x = 0 / r;
			wNorms.y = 0 / r;
			wNorms.z = 1 / r;
			wallNormals.push_back(wNorms);
			column--;

			float u1 = 0 + (stacks * (1 / (segments / 2)));
			float v1 = 0 + (column * (1 / (segments)));
			float u2 = u1;
			float v2 = v1 + (1 / (segments));
			float u3 = u2 + (1 / (segments / 2));
			float v3 = v2;
			float u4 = u3;
			float v4 = v1;
			wallTexCoords.push_back(u1);
			wallTexCoords.push_back(v1);
			wallTexCoords.push_back(u4);
			wallTexCoords.push_back(v4);
			wallTexCoords.push_back(u3);
			wallTexCoords.push_back(v3);
			wallTexCoords.push_back(u2);
			wallTexCoords.push_back(v2);
		}
	}
}


