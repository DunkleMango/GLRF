#include <GLRF/Material.hpp>

using namespace GLRF;

template<typename T>
MaterialProperty<T>::MaterialProperty() {
	this->value_default = (T)0.f;
	this->texture = std::nullopt;
}

template<typename T>
void MaterialProperty<T>::loadTexture(std::string library, std::string texture_name, std::string separator, std::string property_name, std::string fileType)
{
	this->texture = std::shared_ptr<Texture>(new Texture(library, texture_name + separator + property_name + period + fileType));
}

template<typename T>
void MaterialProperty<T>::loadTexture(std::string texture_name, std::string separator, std::string property_name, std::string fileType)
{
	this->texture = std::shared_ptr<Texture>(new Texture(texture_name + separator + property_name + period + fileType));
}

Material::Material() {
	this->albedo = MaterialProperty<glm::vec3>();
	this->normal = MaterialProperty<glm::vec3>();
	this->roughness = MaterialProperty<float>();
	this->metallic = MaterialProperty<float>();
	this->ao = MaterialProperty<float>();
	this->height = MaterialProperty<float>();
	
	this->normal.value_default = glm::vec3(0.f, 0.f, 1.f);
	this->roughness.value_default = 0.f;
	this->metallic.value_default = 0.f;
	this->ao.value_default = 1.f;
	this->height.value_default = 1.f;
	this->height_scale = 1.0f;
}

void Material::loadTextures(std::string library, std::string name, std::string separator, std::string fileType)
{
	this->albedo.loadTexture(library, name, separator, "albedo", fileType);
	this->normal.loadTexture(library, name, separator, "normal", fileType);
	this->roughness.loadTexture(library, name, separator, "roughness", fileType);
	this->metallic.loadTexture(library, name, separator, "metallic", fileType);
	this->ao.loadTexture(library, name, separator, "ao", fileType);
	this->height.loadTexture(library, name, separator, "height", fileType);
}

void Material::loadTextures(std::string name, std::string separator, std::string fileType)
{
	this->albedo.loadTexture(name, separator, "albedo", fileType);
	this->normal.loadTexture(name, separator, "normal", fileType);
	this->roughness.loadTexture(name, separator, "roughness", fileType);
	this->metallic.loadTexture(name, separator, "metallic", fileType);
	this->ao.loadTexture(name, separator, "ao", fileType);
	this->height.loadTexture(name, separator, "height", fileType);
}

void Material::bindTextures(unsigned int textureUnitsBegin)
{
	if (this->albedo.texture.has_value())		this->albedo.texture.value()->bind(GL_TEXTURE0 + textureUnitsBegin);
	if (this->normal.texture.has_value())		this->normal.texture.value()->bind(GL_TEXTURE0 + textureUnitsBegin + 1);
	if (this->roughness.texture.has_value())	this->roughness.texture.value()->bind(GL_TEXTURE0 + textureUnitsBegin + 2);
	if (this->metallic.texture.has_value())		this->metallic.texture.value()->bind(GL_TEXTURE0 + textureUnitsBegin + 3);
	if (this->ao.texture.has_value())			this->ao.texture.value()->bind(GL_TEXTURE0 + textureUnitsBegin + 4);
	if (this->height.texture.has_value())		this->height.texture.value()->bind(GL_TEXTURE0 + textureUnitsBegin + 5);
}