#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "SceneCamera.h"
#include "SceneObject.h"
#include "SceneLight.h"
#include "VectorMath.h"

class Scene {
public:
	Scene(SceneCamera * camera);
	Scene();
	void addObject(SceneMesh * mesh, glm::vec3 position, glm::mat4 rotation);
	void addObject(PointLight * light);
	void addObject(DirectionalLight * light);
	void addObject(SceneCamera * camera);
	void setActiveCamera(SceneCamera * camera);
	void draw(Shader shader);
	void processInput(GLFWwindow * window);
	void processMouse(float xOffset, float yOffset);
private:
	std::vector<SceneMeshNode> meshNodes;
	std::vector<PointLight*> pointLights;
	DirectionalLight * directionalLight = nullptr;
	std::vector<SceneCamera*> cameras;
	SceneCamera * activeCamera = nullptr;
};