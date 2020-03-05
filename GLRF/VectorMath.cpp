#include "VectorMath.h"

glm::vec3 calculateTangent(VertexFormat p1, VertexFormat p2, VertexFormat p3) {
	glm::vec3 edge1 = p2.position - p1.position;
	glm::vec3 edge2 = p3.position - p1.position;
	glm::vec2 deltaUV1 = p2.uv - p1.uv;
	glm::vec2 deltaUV2 = p3.uv - p1.uv;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	return glm::vec3(tangent);
}

void calculateAndSetTangents(std::vector<VertexFormat> * vertices, GLenum drawType) {
	if (drawType == GL_TRIANGLES) calculateAndSetTangents_GL_TRIANGLES(vertices);
}

void calculateAndSetTangents_GL_TRIANGLES(std::vector<VertexFormat> * vertices) {
	for (unsigned int i = 0; i < vertices->size(); i += 3) {
		glm::vec3 tangent = calculateTangent(vertices->at(i), vertices->at(i + 1), vertices->at(i + 2));

		vertices->at(i).tangent = tangent;
		vertices->at(i + 1).tangent = tangent;
		vertices->at(i + 2).tangent = tangent;
	}
}

glm::vec3 dehomogenizeVec4(glm::vec4 homogeneous_input) {
	return glm::vec3(homogeneous_input) / homogeneous_input.w;
}

glm::vec3 getRandomNormalizedVector() {
	return glm::normalize(glm::vec3(getRandomFloat(), getRandomFloat(), getRandomFloat()));
}

glm::vec3 findNonLinearNormalizedVector(glm::vec3 v) {
	glm::vec3 w = getRandomNormalizedVector();
	glm::vec3 result = w - (glm::dot(v, w) / glm::dot(v, v)) * v;
	return (glm::length(result) == 0.0f) ? findNonLinearNormalizedVector(v) : glm::normalize(result);
}

float getRandomFloat() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}