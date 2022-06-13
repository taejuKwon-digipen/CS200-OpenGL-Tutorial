/*!
@file    glapp.cpp
@author  pghali@digipen.edu
@date    10/11/2016

This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and to extract function pointers
to OpenGL implementations.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glapp.h>
#include <glhelper.h>
#include <glslshader.h>
#include <iostream>
#include <random>
#include <glm/gtc/type_ptr.hpp>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
std::vector<GLApp::GLModel> GLApp::models;
//std::vector<GLApp::GLViewport> GLApp::vps;
std::vector<GLSLShader> GLApp::shdrpgms;
std::list<GLApp::GLObject> GLApp::objects;

void GLApp::init() {

	// Part 1: clear colorbuffer with RGBA value in glClearColor ...
	glClearColor(1.f, 1.f, 1.f, 1.f);
	GLint w{ GLHelper::width }, h{ GLHelper::height };

	// Part 2: use the entire window as viewport ..
	glViewport(0, 0, w, h);

	GLApp::VPSS shdr_file_names{ std::make_pair<std::string, std::string>("../shaders/tutorial-3.vert", "../shaders/tutorial-3.frag") };

	GLApp::init_shdrpgms_cont(shdr_file_names);
	GLApp::init_models_cont();
	GLHelper::print_specs();

	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
}


void GLApp::update(double delta_time) {
	static bool Click = false;
	static bool Press = false;
	
	int state = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_P);
	
	int state2 = glfwGetMouseButton(GLHelper::ptr_window, GLFW_MOUSE_BUTTON_LEFT);
	
	static bool max = false;
	//spawning
	if (state2 == GLFW_PRESS && Click == false)
	{
		std::cout << "hello world" << std::endl;
		Click = true;
		GLObject a;

		 int sub = objects.size();
		
		if (max == false)
		{
			if( sub == 0)
			{
				a.init();
				objects.push_back(a);
			}
			else
			{
				for (int i = 0; i < sub; i++)
				{
					a.init();
					objects.push_back(a);
				}
			}
			if (objects.size() >= 32768)
			{
				max = true;
			}
		}
		
		else if (max == true)
		{
			for (int i = 0; i < sub/2; i++)
			{
				objects.pop_front();
			}
			if (sub <= 1)
			{
				max = false;
			}
		}
	}
	
	if (state2 == GLFW_RELEASE && Click == true)
	{
		Click = false;
	}
	//press p -

	static int i = 0;
	
	if (state == GLFW_PRESS && Press == false)
	{
		i++;
		if( i == 1)
		{
			glLineWidth(10);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else if ( i == 2)
		{
			glPointSize(10);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		} else if ( i == 3)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			i = 0;
		}
		Press = true;
	}
	if(state == GLFW_RELEASE && Press ==true)
	{
		Press = false;
	}

	for (auto& x : GLApp::objects) {
		x.update(delta_time);
	}
	std::cout << " object size: " << objects.size()<< " - " << max << std::endl;
}

void GLApp::draw() {
	std::string stitle{ GLHelper::title };
	glfwSetWindowTitle(GLHelper::ptr_window, stitle.c_str());
	glClear(GL_COLOR_BUFFER_BIT);
	
	for (auto const& x : GLApp::objects) {
		x.draw(); // call member function GLObject::draw()
	}

}

void GLApp::cleanup() {
	// empty for now
}

void create_xvals(int slices) {
	int const count{ slices + 1 }; // number of values
	std::vector<float> xpos(count);
	for (int i{ 0 }; i <= slices; ++i) {
		float x = (2.f * i) / static_cast<float>(slices) - 1.f;
		xpos[i] = x;
	}
}
GLApp::GLModel GLApp::box_model()
{
	int const count{ 4 };
	std::vector<glm::vec2> pos_vtx(count);
	std::vector<glm::vec3> clr_vtx(count); // new for task 4b
	std::vector<GLushort> idx_vtx(4);

	idx_vtx = { 2,0,3,1 };
	pos_vtx = { {-0.5,-0.5}, {0.5, -0.5}, {-0.5,0.5}, {0.5,0.5} };

	for (int col = 0; col < count; ++col) {

		clr_vtx[col] = glm::vec3(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
	}

	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

	GLint max_vtx_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vtx_attribs);
	std::cout << "Maximum vertex attributes: " << max_vtx_attribs << '\n';
	// encapsulate information about contents of VBO and VBO handle
	// to another object called VAO

	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);

	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	//position

	glEnableVertexArrayAttrib(vao_hdl, 1);
	glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 1, 1);
	//color

	GLApp::GLModel mdl;

	GLuint ebo_hdl;

	glCreateBuffers(1, &ebo_hdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_vtx.size(), reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);

	mdl.primitive_type = GL_TRIANGLE_STRIP;
	mdl.vaoid = vao_hdl;
	/*mdl.setup_shdrpgm(vtx_shdr, frg_shdr);*/
	mdl.draw_cnt = (GLuint)idx_vtx.size(); // number of vertices
	mdl.primitive_cnt = count / 2; // number of GL_TRIANGLE_FAN
	return mdl;
}

GLApp::GLModel GLApp::mystery_model()
{
	int const count{ 11 };
	std::vector<glm::vec2> pos_vtx(count);
	std::vector<glm::vec3> clr_vtx(17); // new for task 4b
	std::vector<GLushort> idx_vtx(17);

	idx_vtx = {0,1,2,3,4,5,6,7,8,9,10,1};
	pos_vtx = { {0.0,0.0} ,{0.0,-0.2},{0.4,-0.5},{0.25,-0.05},{0.5,0.2},{0.15,0.2},{0.0,0.5},{-0.15,0.2},{-0.5, 0.2},{-0.25,-0.05},{-0.4,-0.5} };

	for (int col = 0; col < count; ++col) {

		clr_vtx[col] = glm::vec3(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
	}

	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

	GLint max_vtx_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vtx_attribs);
	std::cout << "Maximum vertex attributes: " << max_vtx_attribs << '\n';
	// encapsulate information about contents of VBO and VBO handle
	// to another object called VAO

	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);

	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	//position

	glEnableVertexArrayAttrib(vao_hdl, 1);
	glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 1, 1);
	//color

	GLApp::GLModel mdl;

	GLuint ebo_hdl;

	glCreateBuffers(1, &ebo_hdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_vtx.size(), reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);*/
	mdl.primitive_type = GL_TRIANGLE_FAN;
	mdl.vaoid = vao_hdl;
	mdl.draw_cnt = (GLuint)idx_vtx.size(); // number of vertices
	mdl.primitive_cnt = count / 2; // number of GL_TRIANGLE_FAN
	return mdl;
}


void GLApp::init_models_cont() {
	GLApp::models.push_back(GLApp::box_model());
	GLApp::models.push_back(GLApp::mystery_model());
}


void GLApp::init_shdrpgms_cont(GLApp::VPSS const& vpss) {
	for (auto const& x : vpss) {
		std::vector<std::pair<GLenum, std::string>> shdr_files;
		shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, x.first));
		shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, x.second));
		GLSLShader shdr_pgm;
		shdr_pgm.CompileLinkValidate(shdr_files);
		shdrpgms.push_back(shdr_pgm);
	}
}

void GLApp::GLObject::init()
{
	std::default_random_engine dre(std::random_device{}());
	static std::uniform_real_distribution<float> urdf(-1.0, 1.0);
	double const WorldRange{ 2000.0 };
	mdl_ref = static_cast<int>(urdf(dre) + 1);
	shd_ref = 0;
	
	scaling = { urdf(dre) * 175.0 + 225, urdf(dre) * 175.0 + 225 };
	position = glm::vec2(urdf(dre) * WorldRange, urdf(dre) * WorldRange);
	angle_disp = urdf(dre) * 360.f; // current orientation
	angle_speed = urdf(dre) * 30.f; // degrees per frame
}

void GLApp::GLObject::draw() const
{
	// Part 1: Install the shader program used by this object to
	// render its model using GLSLShader::Use()
	GLApp::shdrpgms[shd_ref].Use();
	// Part 2: Bind the object's VAO handle using glBindVertexArray
	glBindVertexArray(GLApp::models[mdl_ref].vaoid);

	GLint uniform_var_loc1 = glGetUniformLocation(GLApp::shdrpgms[shd_ref].GetHandle(), "uModel_to_NDC");
	if (uniform_var_loc1 >= 0)
	{
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mdl_to_ndc_xform));
	}
	else {
		std::cout << "Uniform variable doesn't exist!!!\n";
		std::exit(EXIT_FAILURE);
	}
	glDrawElements(models[mdl_ref].primitive_type, models[mdl_ref].draw_cnt, GL_UNSIGNED_SHORT, NULL);
}

void GLApp::GLObject::update(GLdouble delta_time)
{
	double PI = 3.141592;

	glm::mat3 H = glm::mat3(scaling.x, 0, 0, 0, scaling.y, 0, 0, 0, 1);
	angle_disp += angle_speed * (GLfloat)delta_time;; //current oriantation
	double angle_radian = angle_disp * (PI / 180);
	glm::mat3 R = glm::mat3(cos(angle_radian), sin(angle_radian), 0, -sin(angle_radian), cos(angle_radian), 0, 0, 0, 1);
	glm::mat3 T = glm::mat3(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
	glm::mat3 H2 = glm::mat3(1 / 2000.0, 0, 0, 0, 1 / 2000.0, 0, 0, 0, 1);

	mdl_to_ndc_xform = (H2 * T * R * H);

}






