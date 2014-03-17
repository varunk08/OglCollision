#ifndef _PROCEDURAL_SPHERE_
#define _PROCEDURAL_SPHERE_

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/random.hpp"
class ProceduralSphere
{
protected:
	std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
	std::vector<GLfloat> colors;
	GLuint _vertexBuffer;
	GLuint _colorBuffer;
	mat4 modelMatrix;
	vec3 _position;
	vec3 _velocity;
public:
	ProceduralSphere();
	void CreateSphere(float radius, unsigned int rings, unsigned int sectors);
	void Draw();
	void SetVelocity(vec3 velocity);
	void SetPosition(vec3 newPos);
	void Update();
	void TranslateSphere(glm::vec3 translation);
	mat4 GetModelTransform();
	~ProceduralSphere();

private:
	void InitBuffers(float radius, unsigned int rings, unsigned int sectors);

};

ProceduralSphere::ProceduralSphere(){}
void ProceduralSphere::CreateSphere(float radius, unsigned int rings, unsigned int sectors)
{
	//init physical properties
	SetPosition(glm::vec3(0));
	SetVelocity(glm::sphericalRand(0.001f));
	//init transforms
	modelMatrix = glm::mat4(1);

	InitBuffers(radius, rings, sectors);
	
}
void ProceduralSphere::Update()
{
	vec3 old = _position;
	_position = _position + _velocity;
	if(_position.x > 1 || _position.x < -1)
	{
		_velocity.x *= -1;
		_position = old + _velocity;
	}
	if(_position.y > 1 || _position.y < -1)
	{
		_velocity.y *= -1;
		_position = old + _velocity;
	}
	if(_position.z > 1 || _position.z < -1)
	{
		_velocity.z *= -1;
		_position = old + _velocity;
	}
	TranslateSphere(_velocity);
}
void ProceduralSphere::SetPosition(vec3 newPos)
{
	_position = newPos;
}
void ProceduralSphere::SetVelocity(vec3 velocity)
{
	_velocity = velocity;
}
void ProceduralSphere::TranslateSphere(glm::vec3 translation)
{
	modelMatrix = glm::translate(modelMatrix,translation);
}
mat4 ProceduralSphere::GetModelTransform()
{
	return modelMatrix;
}
void ProceduralSphere::InitBuffers(float radius, unsigned int rings, unsigned int sectors)
{
	float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
		colors.resize(rings * sectors * 3);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
		std::vector<GLfloat>::iterator c_itr = colors.begin();
        for(r = 0; r < rings; r++){
			for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;

				float red = 1.0f, blue = 0.0f, green = 0.0f;
				*c_itr++ = red;
				*c_itr++ = green;
				*c_itr++ = blue;
			}
		}

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings-1; r++) {
			for(s = 0; s < sectors-1; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
			}
		}

		glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);
}
void ProceduralSphere::Draw()
{
	glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
                glVertexAttribPointer(
                        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                );

                // 2nd attribute buffer : colors
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
                glVertexAttribPointer(
                        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                        3,                                // size
                        GL_FLOAT,                         // type
                        GL_FALSE,                         // normalized?
                        0,                                // stride
                        (void*)0                          // array buffer offset
                );

                // Draw the triangle !
                //glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

				glDrawElements(GL_QUADS,indices.size(),GL_UNSIGNED_SHORT,&indices[0]);

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);


}
ProceduralSphere::~ProceduralSphere()
{
}

#endif _PROCEDURAL_SPHERE_