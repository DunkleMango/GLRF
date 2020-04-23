#pragma once
#include <vector>
#include <algorithm>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.hpp>
#include <Camera.hpp>
#include <SceneObject.hpp>
#include <SceneLight.hpp>
#include <VectorMath.hpp>

namespace GLRF {
	class Scene;
}

/**
 * @brief A 3d space can contain objects, lights and cameras.
 * 
 */
class GLRF::Scene {
public:
	/**
	 * @brief Construct a new Scene object.
	 * 
	 * @param camera the new active camera
	 */
	Scene(std::shared_ptr<Camera> camera);

	/**
	 * @brief Construct a new Scene object.
	 * 
	 * A default camera will be created and set as active.
	 */
	Scene();

	/**
	 * @brief Adds a mesh object at the given position with the given rotation to the scene.
	 * 
	 * @param node the node of the mesh that will be added to the scene
	 */
	void addObject(SceneNode<SceneMesh> node);

	/**
	 * @brief Adds a point lightsource to the scene.
	 * 
	 * @param light the point lightsource that will be added to the scene
	 */
	void addObject(SceneNode<PointLight> light);

	/**
	 * @brief Adds a directional lightsource to the scene.
	 * 
	 * @param light the directional lightsource that will be added to the scene
	 */
	void addObject(SceneNode<DirectionalLight> light);

	/**
	 * @brief Adds a camera to the scene. The camera will not become active.
	 * 
	 * @param camera the camera the will be added to the scene
	 */
	void addObject(std::shared_ptr<Camera> camera);

	/**
	 * @brief Sets the given camera as the active camera.
	 * 
	 * @param camera the camera that will be added to the scene
	 * 
	 * If the camera is new, it will also be added to the scene.
	 */
	void setActiveCamera(std::shared_ptr<Camera> camera);

	/**
	 * @brief Draws all objects of the scene with the given shader.
	 * 
	 * @param shader the shader to draw the scenes objects with
	 */
	void draw(Shader & shader);

	/**
	 * @brief Processes keyboard inputs for the scene.
	 * 
	 * @param window the window that receives the current inputs
	 */
	void processInput(GLFWwindow * window);

	/**
	 * @brief Processes mouse inputs for the scene.
	 * 
	 * @param xOffset the offset of the mouse position in pixels along the x-axis of the mouse since the last measurement
	 * @param yOffset the offset of the mouse position in pixels along the y-axis of the mouse since the last measurement
	 */
	void processMouse(float xOffset, float yOffset);
private:
	std::vector<SceneNode<SceneMesh>> meshNodes;
	std::vector<SceneNode<PointLight>> pointLights;
	std::vector<SceneNode<DirectionalLight>> directionalLights;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
};