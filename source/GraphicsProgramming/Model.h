// Model class, represents a loaded OBJ model
// handles the loading/processing of the model file. Also loads and stores its own texture
// NOT complete, for students to complete. Model data requires being sorted and the render function needs completed.
#ifndef _MODEL_H_
#define _MODEL_H_

// INCLUDES //
#include <glut.h>
#include <fstream>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace std;

// MY CLASS INCLUDES //
#include <vector>
#include <map>
#include <string>
#include "Vector3.h"
#include "SOIL.h"

class Model
{

public:

	bool load(char* modelFilename, char* textureFilename, char* mtlFilename);
	void render();

private:

	void loadTexture(char*);
	void loadMTL(char*);
	bool loadModel(char*);

	int m_vertexCount;
	GLuint texture;

	vector<float> vertex, normals, texCoords;

	struct Material {
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float shininess;        // [0 = min shininess, 1 = max shininess]
		float alpha;            // [0 = fully transparent, 1 = fully opaque]

		std::string name;
		std::string colorMapFilename;
		std::string bumpMapFilename;
	};

	vector<string> materialNames;
	std::map<std::string, int> m_materialCache;
	int m_numberOfMaterials;
	vector<Material> m_materials;

};

#endif