#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"



#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

using namespace std;

GLFWwindow *window;

Program program;
std::vector<float> posBuff;  // vertex position in object space
std::vector<float> norBuff;  // vertex normal
std::vector<float> texBuff;  // vertex texture coordinates
Program Gouraud, Phong, Silhouette;
vector <Program> programVec  = {Gouraud, Phong, Silhouette};
Program *currentProgram = &programVec[0];
// variables used in shader1
glm::vec4 worldPosition, worldNormal;

glm::vec3 eye(0.0f, 0.0f, 4.0f);

struct materialStruct {
	glm::vec3 ka, kd, ks;  // 3 different vec 3 declaration (empty)
	float s;
} materials[NUM_MATERIALS];  // materials array is of length 3: stores material parameters

int currentMaterial = 0;

struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
} lights[NUM_LIGHTS]; // lights array is of length 2: stores lighting information



lightStruct *currentLight = lights;  // points to the current light in lightStruct

void Display()
{		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 inverseTranspose = glm::inverse(glm::transpose(modelMatrix));

	program.Bind();  // activate shader programs so that they can be used for drawing
	// sends uniform data to vertex and fragment
	program.SendUniformData(inverseTranspose, "inverseTranspose");
	// program.SendUniformData(worldPosition, "worldPosition");
	// program.SendUniformData(worldNormal, "worldNormal");
	// send initalized variables
	program.SendUniformData(eye, "eye");
	program.SendUniformData(modelMatrix, "model");
	program.SendUniformData(viewMatrix, "view");
	program.SendUniformData(projectionMatrix, "projection");
	// send lights data
	program.SendUniformData(lights[0].position, "lights[0].position");
	program.SendUniformData(lights[0].color, "lights[0].color");
	program.SendUniformData(lights[1].position, "lights[1].position");
	program.SendUniformData(lights[1].color, "lights[1].color");
	// send materials data
	if (currentMaterial == 0) {
		program.SendUniformData(materials[0].ka, "ka");
		program.SendUniformData(materials[0].kd, "kd");
		program.SendUniformData(materials[0].ks, "ks");
		program.SendUniformData(materials[0].s, "s");
	} else if (currentMaterial == 1) {
		program.SendUniformData(materials[1].ka, "ka");
		program.SendUniformData(materials[1].kd, "kd");
		program.SendUniformData(materials[1].ks, "ks");
		program.SendUniformData(materials[1].s, "s");
	} else if (currentMaterial == 2) {
		program.SendUniformData(materials[2].ka, "ka");
		program.SendUniformData(materials[2].kd, "kd");
		program.SendUniformData(materials[2].ks, "ks");
		program.SendUniformData(materials[2].s, "s");
	}

	
	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);  // displays the triangles on the screen
	program.Unbind();  // deactivates the shader

}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case '1':  // Gouraud
		program.SetShadersFileName("../shaders/shader1.vert", "../shaders/shader1.frag");
		program.Init();
		program.SendAttributeData(posBuff, "vPositionModel"); 
		program.SendAttributeData(norBuff, "vNormalModel");
		break;
	case '2':  // Phong
		program.SetShadersFileName("../shaders/shader2.vert", "../shaders/shader2.frag");
		program.Init();
		program.SendAttributeData(posBuff, "vPositionModel"); 
		program.SendAttributeData(norBuff, "vNormalModel");
		break;
	case '3':  // Sillhouette
		program.SetShadersFileName("../shaders/shader3.vert", "../shaders/shader3.frag");
		program.Init();
		program.SendAttributeData(posBuff, "vPositionModel"); 
		program.SendAttributeData(norBuff, "vNormalModel");
		break;
	case 'l': // move forwards
		if (currentLight == &lights[0]) {
			currentLight = &lights[1];
		} else {
			currentLight = &lights[0];
		}
		break;
	case 'L':  // move backwards
		if (currentLight == &lights[1]) {
			currentLight = &lights[0];
		} else {
			currentLight = &lights[1];
		}
		break;
	case 'm':  // shader forward
		if (currentMaterial == 0 || currentMaterial == 1) {
			currentMaterial += 1;
		} else if (currentMaterial == 2) {
			currentMaterial = 0;
		}
		break;
	case 'M':  // shader backward 
		if (currentMaterial == 2 || currentMaterial == 1) {
			currentMaterial -= 1;
		} else if (currentMaterial == 0) {
			currentMaterial = 2;
		}
		break;
	case 'x':  // positive x light direction
		if (currentLight == &lights[0]) {
			lights[0].position[0]+= 0.2;
		} else if (currentLight == &lights[1]) {
			lights[1].position[0]+= 0.2;
		}
		break;
	case 'X':  // negative x light direction
		if (currentLight == &lights[0]) {
			lights[0].position[0] -= 0.2;
		} else if (currentLight == &lights[1]) {
			lights[1].position[0] -= 0.2;
		}
		break;
	case 'y':  // positive y light direction
		if (currentLight == &lights[0]) {
			lights[0].position[1] += 0.2;
		} else if (currentLight == &lights[1]) {
			lights[1].position[1] += 0.2;
		}
		break;
	case 'Y':  // negative y light direction
		if (currentLight == &lights[0]) {
			lights[0].position[1] -= 0.2;
		} else if (currentLight == &lights[1]) {
			lights[1].position[1] -= 0.2;
		}
		break;
	case 'z':  // positive z light direction
		if (currentLight == &lights[0]) {
			lights[0].position[2] += 0.2;
		} else if (currentLight == &lights[1]) {
			lights[1].position[2] += 0.2;
		}
		break;
	case 'Z':  // negative z light direction
		if (currentLight == &lights[0]) {
			lights[0].position[2] -= 0.2;
		} else if (currentLight == &lights[1]) {
			lights[1].position[2] -= 0.2;
		}
		break;
	default:
		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment4 - Seth Johnston", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");
	// program.SetShadersFileName(vertshader, fragshader);  // sets the address of the vertex and fragment shader files so that we can load and compile them
	program.SetShadersFileName("../shaders/shader1.vert", "../shaders/shader1.frag");
	program.Init();  // loads, compiles, and links shader files so that they can be executed
	program.SendAttributeData(posBuff, "vPositionModel");  // sends position of each vertex which is stored in posbuff to the vertex program under the name of vPositionModel
	program.SendAttributeData(norBuff, "vNormalModel");  // sends the nomral of each vertex which is stored in norBuff to the vertex program under the name of vNormalModel
}

int main()
{	
	// Material1
	materials[0].ka = {0.2, 0.2, 0.2};
	materials[0].kd = {0.8, 0.7, 0.7};
	materials[0].ks = {1.0, 1.0, 1.0};
	materials[0].s = 10.0;
	// Material2
	materials[1].ka = {0.0, 0.2, 0.2};
	materials[1].kd = {0.5, 0.7, 0.2};
	materials[1].ks = {0.1, 1.0, 0.1};
	materials[1].s = 100.0;
	// Material3
	materials[2].ka = {0.2, 0.2, 0.2};
	materials[2].kd = {0.1, 0.3, 0.9};
	materials[2].ks = {0.1, 0.1, 0.1};
	materials[2].s = 1.0;
	// Light1
	lights[0].position = {0.0, 0.0, 3.0};  // need to change this to world coordinate
	lights[0].color = {0.5, 0.5, 0.5};
	// Light2
	lights[1].position = {0.0, 3.0, 0.0};  // need to change this to world coordinate
	lights[1].color = {0.2, 0.2, 0.2};
	Init();


	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}