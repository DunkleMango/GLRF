#pragma once
#include <glm/glm.hpp>
#include "Texture.hpp"

class Material {
public:
	glm::vec3 albedo = glm::vec3(1.0f);
	glm::vec3 normal = glm::vec3();
	float roughness = 0.0f;
	float metallic = 0.0f;
	float ao = 0.0f;
	float height = 1.0f;
	bool useTextureAlbedo = false;
	bool useTextureNormal = false;
	bool useTextureRoughness = false;
	bool useTextureMetallic = false;
	bool useTextureAo = false;
	bool useTextureHeight = false;
	Texture textureAlbedo;
	Texture textureNormal;
	Texture textureRoughness;
	Texture textureMetallic;
	Texture textureAo;
	Texture textureHeight;

	float height_scale = 1.0f;

	void loadTextures(std::string name, std::string separator, std::string fileType);
	void bindTextures(unsigned int textureUnitsBegin);
};