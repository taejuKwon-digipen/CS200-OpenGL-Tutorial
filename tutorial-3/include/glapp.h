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
#include <list>

#include "glslshader.h"

struct GLApp {
	static void init();
	static void update(double delta_time);
	static void draw();
	static void cleanup();

	static std::vector<GLSLShader> shdrpgms;
	using VPSS = std::vector<std::pair<std::string, std::string>>;
	static void init_shdrpgms_cont(GLApp::VPSS const&);
	
	struct GLModel {
		GLenum primitive_type; // which OpenGL primitive to be rendered?
		GLuint primitive_cnt;
		GLuint vaoid; // handle to VAO
		GLuint draw_cnt;
	};
	static std::vector<GLModel> models; // singleton
	static GLApp::GLModel box_model(); // center(0,0) , 1x1 NDC Box
	static GLApp::GLModel mystery_model();
	static void init_models_cont();
	
	struct GLObject {
		
		glm::vec2 position;
		glm::vec2 scaling;
		GLfloat angle_speed, angle_disp;
		glm::mat3 mdl_to_ndc_xform;
		GLuint mdl_ref;
		GLuint shd_ref;
		void init();
		void draw() const;
		void update(GLdouble delta_time);
	};
	static std::list<GLApp::GLObject> objects; // singleton
	
	//struct GLViewport {
	//	GLint x, y;
	//	GLsizei width, height;
	//};
	//static std::vector<GLViewport> vps; // container for viewports
	static GLApp::GLModel points_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);
	static GLApp::GLModel lines_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);
	static GLApp::GLModel trifans_model(int slices, std::string vtx_shdr, std::string frg_shdr);
	static GLApp::GLModel tristrips_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);

};


#endif /* GLAPP_H */
