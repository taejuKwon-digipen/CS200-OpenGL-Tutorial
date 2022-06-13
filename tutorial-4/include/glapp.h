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

	static void init_scene(std::string scene_filename);
	
	static std::map<std::string, GLSLShader> shdrpgms;

	static void init_shdrpgms(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr);

	struct GLModel {
		GLenum primitive_type; // which OpenGL primitive to be rendered?
		GLuint primitive_cnt;
		GLuint vaoid; // handle to VAO
		GLuint draw_cnt;

		void init(std::string model_name); // read mesh data from file ...
		void release(); // return buffers back to GPU ...
	};
	
	static std::map<std::string, GLModel> models;  // singleton

	static void init_models_cont();
	
	struct GLObject {
		//GLfloat angle_disp, angle_speed; // removed in tutorial 4
		glm::vec2 orientation; // orientation.x = angle_disp and orientation.y = angle_speed
		glm::vec2 scaling;
		glm::vec2 position;
		glm::mat3 mdl_to_ndc_xform;
		std::map<std::string, GLApp::GLModel>::iterator mdl_ref;
		std::map<std::string, GLSLShader>::iterator shd_ref;

		glm::vec3 color;
		glm::mat3 mdl_xform; // model transformation
		
		void init();
		void draw() const;
		void update(GLdouble delta_time);
	};
	
	static std::map<std::string, GLObject> objects; // singleton

	struct Camera2D {
		GLObject* pgo; // pointer to game object that embeds camera
		glm::vec2 right, up;
		glm::mat3 view_xform;

		// window parameters ...
		GLint height{ 1000 };
		GLfloat ar;
		glm::mat3 camwin_to_ndc_xform;
		glm::mat3 world_to_ndc_xform;

		// window change parameters ...
		GLint min_height{ 500 }, max_height{ 2000 };
		
		// height is increasing if 1 and decreasing if -1
		GLint height_chg_dir{ 1 };
		
		// increments by which window height is changed per Z key press
		GLint height_chg_val{ 5 };
		
		// camera's speed when button U is pressed
		GLfloat linear_speed{ 2.f };
		
		// keyboard button press flags
		//GLboolean camtype_flag{ GL_FALSE }; // button V
		GLboolean zoom_flag{ GL_FALSE }; // button Z
		//GLboolean left_turn_flag{ GL_FALSE }; // button H
		//GLboolean right_turn_flag{ GL_FALSE }; // button K
		//GLboolean move_flag{ GL_FALSE }; // button U
		// you can implement these functions as you wish ...
		void init(GLFWwindow* window , GLObject* ptr);
		void update(GLFWwindow* window );
	};
	// define object of type Camera2D ...
	static  Camera2D cam;
};


#endif /* GLAPP_H */
