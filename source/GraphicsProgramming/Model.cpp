// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "model.h"

bool Model::load(char* modelFilename, char* textureFilename, char* mtlFilename)
{
	bool result;

	// Load in the model data,
	result = loadModel(modelFilename);
	if (!result)
	{
#ifdef _DEBUG
		MessageBox(NULL,"Model failed to load", "Error", MB_OK);
#else
		MessageBox(NULL, L"Model failed to load", L"Error", MB_OK);
#endif
		return false;
	}

	// Load the texture for this model.
	loadTexture(textureFilename);

	// Load the MTL file for the model. (If applicable)
	//loadMTL(mtlFilename);

	return true;
}

void Model::render()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glEnableClientState(GL_VERTEX_ARRAY);				// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);				// Enable normal arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);		// Enable texture co-ord arrays

	glVertexPointer(3, GL_FLOAT, 0, vertex.data());				// Pointer to vertex array
	glNormalPointer(GL_FLOAT, 0, normals.data());				// Pointer to normals array
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());		// Pointer to texture co-ords array

	glDrawArrays(GL_TRIANGLES, 0, (vertex.size() / 3));

	glDisableClientState(GL_VERTEX_ARRAY);				// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);				// Disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);		// Disable texture co-ords arrays
}


// Modified from a mulit-threaded version by Mark Ropper.
bool Model::loadModel(char* filename)
{
	vector<Vector3> verts;
	vector<Vector3> norms;
	vector<Vector3> texCs;
	vector<unsigned int> faces;
	int line = 0;
	int activeMaterial = 0;
	char name[80];
	std::map<std::string, int>::const_iterator iter;
	
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}
	while (true)
	{
		char lineHeader[128];

		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
				line++;
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				texCs.push_back(uv);
				line++;
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
				line++;
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
																			&face[3], &face[4], &face[5],
																			&face[6], &face[7], &face[8]);
				if (matches != 9)
				{
					// Parser error, or not triangle faces
					return false;
				}
				
				for (int i = 0; i < 9; i++)
				{
					faces.push_back(face[i]);
				}
				line++;
			}
			else if (strcmp(lineHeader, "usemtl") == 0) // MTL specific texture
			{
				// parse the material name
				fscanf(file, "%s", &name);
				if (std::find(materialNames.begin(), materialNames.end(), name) == materialNames.end())
				{
					materialNames.push_back(name);
				}
				line++;
			}
		}
	}

	// Store the vertex, normal and texCoord data read in from the .obj file.
	for (int i = 0; i < faces.size(); i += 3)
	{
		if (faces[i + 1] > 0)
		{
			texCoords.push_back(texCs[faces[i + 1] - 1].x);	// Get x pos from texCs vector at pos face[i]
			texCoords.push_back(texCs[faces[i + 1] - 1].y);	// Get y pos from texCs vector at pos face[i]

			vertex.push_back(verts[faces[i] - 1].x);	// Get x pos from verts vector at pos face[i]
			vertex.push_back(verts[faces[i] - 1].y);	// Get y pos from verts vector at pos face[i]
			vertex.push_back(verts[faces[i] - 1].z);	// Get z pos from verts vector at pos face[i]

			normals.push_back(norms[faces[i + 2] - 1].x);	// Get x pos from norms vector at pos face[i]
			normals.push_back(norms[faces[i + 2] - 1].y);	// Get y pos from norms vector at pos face[i]
			normals.push_back(norms[faces[i + 2] - 1].z);	// Get z pos from norms vector at pos face[i]
		}
	}
	
	// Once data has been sorted clear read data (which has been copied and are not longer needed).
	verts.clear();
	norms.clear();
	texCs.clear();
	faces.clear();

	return true;
}

void Model::loadTexture(char* filename)
{
	if (filename != NULL)
	{
		texture = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
		);

		//check for an error during the load process
		if (texture == 0)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
	}

}

void Model::loadMTL(char * filename)
{
	// parse the mtl file name
	
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
#ifdef _DEBUG
		MessageBox(NULL, "MTL File failed to load", "Error", MB_OK);
#else
		MessageBox(NULL, L"MTL File failed to load", L"Error", MB_OK);
#endif
	}

	Material *pMaterial = 0;
	int illum = 0;
	int numMaterials = 0;
	char lineHeader[128];

	// Count the number of materials in the MTL file.
	while (true)
	{
		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "newmtl") == 0) // MTL specific texture
			{
				++numMaterials;
			}
			else if (strcmp(lineHeader, "Ns") == 0) // Ns
			{
				fscanf(file, "%d\n", &pMaterial->shininess);

				// Wavefront .MTL file shininess is from [0,1000].
				// Scale back to a generic [0,1] range.
				pMaterial->shininess /= 1000.0f;
			}
			else if (strcmp(lineHeader, "Ka") == 0) // Ka
			{
				fscanf(file, "%f %f %f",
					&pMaterial->ambient[0],
					&pMaterial->ambient[1],
					&pMaterial->ambient[2]);
				pMaterial->ambient[3] = 1.0f;
			}
			else if (strcmp(lineHeader, "Kd") == 0) // Kd
			{
				fscanf(file, "%f %f %f",
					&pMaterial->diffuse[0],
					&pMaterial->diffuse[1],
					&pMaterial->diffuse[2]);
				pMaterial->diffuse[3] = 1.0f;
			}
			else if (strcmp(lineHeader, "Ks") == 0) // Ks
			{
				fscanf(file, "%f %f %f",
					&pMaterial->specular[0],
					&pMaterial->specular[1],
					&pMaterial->specular[2]);
				pMaterial->specular[3] = 1.0f;
			}
			else if (strcmp(lineHeader, "d") == 0) // d
			{
				fscanf(file, "%f", &pMaterial->alpha);
			}
			else if (strcmp(lineHeader, "i") == 0) // i
			{
				fscanf(file, "%d", &illum);

				if (illum == 1)
				{
					pMaterial->specular[0] = 0.0f;
					pMaterial->specular[1] = 0.0f;
					pMaterial->specular[2] = 0.0f;
					pMaterial->specular[3] = 1.0f;
				}
			}
			else if (strcmp(lineHeader, "map_Kd") == 0) // map_Kd
			{
				fgets(lineHeader, sizeof(lineHeader), file);
				sscanf(lineHeader, "%s %s", lineHeader, lineHeader);
				pMaterial->colorMapFilename = lineHeader;
			}
			else if (strcmp(lineHeader, "n") == 0) // newmtl
			{
				fgets(lineHeader, sizeof(lineHeader), file);
				sscanf(lineHeader, "%s %s", lineHeader, lineHeader);

				pMaterial = &m_materials[numMaterials];
				pMaterial->ambient[0] = 0.2f;
				pMaterial->ambient[1] = 0.2f;
				pMaterial->ambient[2] = 0.2f;
				pMaterial->ambient[3] = 1.0f;
				pMaterial->diffuse[0] = 0.8f;
				pMaterial->diffuse[1] = 0.8f;
				pMaterial->diffuse[2] = 0.8f;
				pMaterial->diffuse[3] = 1.0f;
				pMaterial->specular[0] = 0.0f;
				pMaterial->specular[1] = 0.0f;
				pMaterial->specular[2] = 0.0f;
				pMaterial->specular[3] = 1.0f;
				pMaterial->shininess = 0.0f;
				pMaterial->alpha = 1.0f;
				pMaterial->name = lineHeader;
				pMaterial->colorMapFilename.clear();
				pMaterial->bumpMapFilename.clear();

				m_materialCache[pMaterial->name] = numMaterials;
				++numMaterials;
			}
		}
	}

	m_numberOfMaterials = numMaterials;
	numMaterials = 0;
	m_materials.resize(m_numberOfMaterials);

	//// Load the materials in the MTL file.
	//while (true)
	//{
	//	if (strcmp(lineHeader, "Ns") == 0) // Ns
	//	{
	//		fscanf(file, "%f", &pMaterial->shininess);

	//		// Wavefront .MTL file shininess is from [0,1000].
	//		// Scale back to a generic [0,1] range.
	//		pMaterial->shininess /= 1000.0f;
	//	}
	//	else if (strcmp(lineHeader, "Ka") == 0) // Ka
	//	{
	//		fscanf(file, "%f %f %f",
	//			&pMaterial->ambient[0],
	//			&pMaterial->ambient[1],
	//			&pMaterial->ambient[2]);
	//		pMaterial->ambient[3] = 1.0f;
	//	}
	//	else if (strcmp(lineHeader, "Kd") == 0) // Kd
	//	{
	//		fscanf(file, "%f %f %f",
	//			&pMaterial->diffuse[0],
	//			&pMaterial->diffuse[1],
	//			&pMaterial->diffuse[2]);
	//		pMaterial->diffuse[3] = 1.0f;
	//	}
	//	else if (strcmp(lineHeader, "Ks") == 0) // Ks
	//	{
	//		fscanf(file, "%f %f %f",
	//			&pMaterial->specular[0],
	//			&pMaterial->specular[1],
	//			&pMaterial->specular[2]);
	//		pMaterial->specular[3] = 1.0f;
	//	}
	//	else if (strcmp(lineHeader, "d") == 0) // d
	//	{
	//		fscanf(file, "%f", &pMaterial->alpha);
	//	}
	//	else if (strcmp(lineHeader, "i") == 0) // i
	//	{
	//		fscanf(file, "%d", &illum);

	//		if (illum == 1)
	//		{
	//			pMaterial->specular[0] = 0.0f;
	//			pMaterial->specular[1] = 0.0f;
	//			pMaterial->specular[2] = 0.0f;
	//			pMaterial->specular[3] = 1.0f;
	//		}
	//	}
	//	else if (strcmp(lineHeader, "map_Kd") == 0) // map_Kd
	//	{
	//		fgets(lineHeader, sizeof(lineHeader), file);
	//		sscanf(lineHeader, "%s %s", lineHeader, lineHeader);
	//		pMaterial->colorMapFilename = lineHeader;
	//	}
	//	else if (strcmp(lineHeader, "n") == 0) // newmtl
	//	{
	//		fgets(lineHeader, sizeof(lineHeader), file);
	//		sscanf(lineHeader, "%s %s", lineHeader, lineHeader);

	//		pMaterial = &m_materials[numMaterials];
	//		pMaterial->ambient[0] = 0.2f;
	//		pMaterial->ambient[1] = 0.2f;
	//		pMaterial->ambient[2] = 0.2f;
	//		pMaterial->ambient[3] = 1.0f;
	//		pMaterial->diffuse[0] = 0.8f;
	//		pMaterial->diffuse[1] = 0.8f;
	//		pMaterial->diffuse[2] = 0.8f;
	//		pMaterial->diffuse[3] = 1.0f;
	//		pMaterial->specular[0] = 0.0f;
	//		pMaterial->specular[1] = 0.0f;
	//		pMaterial->specular[2] = 0.0f;
	//		pMaterial->specular[3] = 1.0f;
	//		pMaterial->shininess = 0.0f;
	//		pMaterial->alpha = 1.0f;
	//		pMaterial->name = lineHeader;
	//		pMaterial->colorMapFilename.clear();
	//		pMaterial->bumpMapFilename.clear();

	//		m_materialCache[pMaterial->name] = numMaterials;
	//		++numMaterials;
	//	}
	//}
}



