/* !
@file    glapp.h
@author  pghali@digipen.edu
@date    10/11/2016

This file contains the declaration of namespace GLApp that encapsulates the
functionality required to implement an OpenGL application including
compiling, linking, and validating shader programs
setting up geometry and index buffers,
configuring VAO to present the buffered geometry and index data to
vertex shaders,
configuring textures (in later labs),
configuring cameras (in later labs),
and transformations (in later labs).
*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLAPP_H
#define GLAPP_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glhelper.h>
#include <array>
#include "glslshader.h"

struct GLApp {
	static void init();
	static void update(double delta_time);
	static void draw();
	static void cleanup();

	struct GLModel {
		GLenum primitive_type; 
		GLSLShader shdr_pgm;
		GLuint vaoid; 
		GLuint idx_elem_cnt; 
		void setup_vao();
		void setup_shdrpgm();
		void draw();
	};
	struct Vertex {
		glm::vec2 Position;
		glm::vec3 Color;
		glm::vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
	};
	static std::vector<Vertex> vertices;
	static GLModel mdl;
};


#endif /* GLAPP_H */
