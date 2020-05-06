#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLRF/VertexFormat.hpp>
#include <GLRF/Material.hpp>
#include <GLRF/IdManager.hpp>

namespace GLRF {
	struct MeshData;
	class SceneObject;
	class SceneMesh;
	template <typename T> class SceneNode;
}

struct GLRF::MeshData {
	std::vector<VertexFormat> vertices;
	std::optional<std::vector<GLuint>> indices = std::nullopt;
	void unionize(MeshData& other) {
		size_t current_vertices_size = this->vertices.size();
		size_t current_indices_size = this->indices.value().size();
		size_t other_vertices_size = other.vertices.size();
		size_t other_indices_size = other.indices.value().size();

		bool this_has_indices = this->indices.has_value();
		bool other_has_indices = other.indices.has_value();

		if (this_has_indices == other_has_indices) {
			this->vertices.reserve(current_vertices_size + other_vertices_size);
			std::copy(other.vertices.begin(), other.vertices.end(), std::back_inserter(this->vertices));

			if (this_has_indices && other_has_indices) {
				this->indices.value().reserve(current_indices_size + other_indices_size);
				std::transform(other.indices.value().begin(), other.indices.value().end(), std::back_inserter(this->indices.value()),
					[current_vertices_size](GLuint x) -> GLuint { return x + current_vertices_size; });
			}
		}
	}
};

/**
 * @brief A 3d object.
 * 
 * The data is stored here and should not be copied.
 * Instead, a new SceneNode should be created that only points to the mesh, thus reducing memory usage.
 */
class GLRF::SceneObject {
public:
	/**
	 * @brief Draws the object with the current shader.
	 * 
	 */
	virtual void draw() = 0;

	/**
	 * @brief Returns the Material object.
	 * 
	 * @return Material the material of the object
	 */
	virtual Material getMaterial() { return this->material; }

	/**
	 * @brief Sets the Material object.
	 * 
	 * @param material the new material for the object
	 */
	virtual void setMaterial(Material material) { this->material = material; }
private:
	Material material;
};

/**
 * @brief A mesh of vertices that resembles a 3d object.
 * 
 * The data is stored here and should not be copied.
 * Instead, a new SceneNode should be created that only points to the mesh, thus reducing memory usage.
 */
class GLRF::SceneMesh : public virtual SceneObject {
public:
	/**
	 * @brief Construct a new SceneMesh object.
	 * 
	 * @param vertices the vertices that define the structure of the mesh
	 * @param drawType the OpenGL draw type that specifies how the mesh will be rendered - e.g. GL_STATIC_DRAW
	 * @param material the material that defines the appearance of the mesh
	 */
	SceneMesh(MeshData data, GLenum drawType, Material material = Material());

	/**
	 * @brief Updates the vertex data and the draw type of the mesh.
	 * 
	 * @param vertices the new vertices that will replace the old vertices
	 * @param drawType the new value for the OpenGL draw type
	 */
	void update(MeshData data, GLenum drawType);

	/**
	 * @brief Updates the vertex data of the mesh.
	 * 
	 * @param vertices the new vertices that will replace the old vertices
	 */
	void update(MeshData data);

	/**
	 * @brief Draws the mesh with the current shader.
	 * 
	 */
	void draw();
private:
	GLuint VBO, VAO, EBO;
	GLenum drawType;
	GLenum geometryType;
	MeshData data;
};

/**
 * @brief A lightweight reference / instance of a specific object that buffers changes to the meshs local coordinate system.
 * 
 */
template <typename T>
class GLRF::SceneNode {
public:
	const IdSpaceSize id;

	/**
	 * @brief Construct a new SceneNode object.
	 * 
	 * @param object the object that will be referenced
	 */
	SceneNode(std::shared_ptr<T> object) : id(IdManager::getInstance().getNodeId()) {
		this->object = object;
	}

	/**
	 * @brief Get the object that this node refers to.
	 * 
	 * @return std::shared_ptr<T> a shared pointer to the object of this node
	 */
	std::shared_ptr<T> getObject() {
		return this->object;
	}

	friend bool operator==(const SceneNode &n1, const SceneNode &n2) {
		return n1.id == n2.id;
	}

	friend bool operator!=(const SceneNode &n1, const SceneNode &n2) {
		return !(n1 == n2);
	}

	/**
	 * @brief Sets the position of this node. Use 'move' for relative displacement.
	 * 
	 * @param position the position to be set
	 */
	void setPosition(glm::vec3 position)  {
		this->position = position;
	}

	/**
	 * @brief Sets the rotation of this node. Use 'rotate' for relative rotation.
	 * 
	 * @param rotation the rotation to be set
	 */
	void setRotation(glm::mat4 rotation)  {
		this->rotation = rotation;
	}

	/**
	 * @brief Moves the node along a vector. Use 'setPosition' for absolute displacement.
	 * 
	 * @param offset the vector to move along at
	 */
	void move(glm::vec3 offset)  {
		this->position += offset;
	}

	/**
	 * @brief Rotates the node around an axis. Use 'setRotation' for absolute rotation.
	 * 
	 * @param axis the axis to rotate around
	 * @param angle the angle in degrees
	 */
	void rotateDeg(glm::vec3 axis, float angle) {
		this->rotation = glm::rotate(this->rotation, glm::radians(angle), axis);
	}

	/**
	 * @brief Rotates the node around an axis. Use 'setRotation' for absolute rotation.
	 * 
	 * @param axis the axis to rotate around
	 * @param angle the angle in radians
	 */
	void rotateRad(glm::vec3 axis, float angle) {
		this->rotation = glm::rotate(this->rotation, angle, axis);
	}

	/**
	 * @brief Returns the position vector.
	 * 
	 * @return glm::vec3 the position vector
	 */
	glm::vec3 getPosition() { return this->position; }

	/**
	 * @brief Returns the rotation vector.
	 * 
	 * @return glm::vec3 the rotation vector
	 */
	glm::mat4 getRotation() { return this->rotation; }

	/**
	 * @brief Calculates and returns the model matrix.
	 * 
	 * @return glm::vec3 the model matrix
	 */
	glm::mat4 calculateModelMatrix() {
		return glm::translate(glm::mat4(1.f), this->position) * this->rotation;
	}
private:
	std::shared_ptr<T> object = nullptr;
	glm::vec3 position = glm::vec3(0.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
};