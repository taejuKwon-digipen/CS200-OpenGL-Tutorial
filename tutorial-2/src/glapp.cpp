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

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
std::vector<GLApp::GLModel> GLApp::models;
std::vector<GLApp::GLViewport> GLApp::vps;

void GLApp::init() {
	srand(glfwGetTimerValue() * 7);
	
	// Part 1: clear colorbuffer with RGBA value in glClearColor ...
	glClearColor(1.f, 1.f, 1.f, 1.f);		// Part 2: use the entire window as viewport ...
	GLint w{ GLHelper::width }, h{ GLHelper::height };

	// Part 3: initialize VAO and create shader program
	//mdl.setup_shdrpgm();
	
	GLApp::models.push_back(GLApp::points_model(20,20, "../shaders/tutorial-2-new.vert", "../shaders/tutorial-2-new.frag"));
	GLApp::models.push_back(GLApp::lines_model(40, 40,"../shaders/tutorial-2-new.vert","../shaders/tutorial-2-new.frag"));
	GLApp::models.push_back(GLApp::trifans_model(30, "../shaders/tutorial-2-new.vert","../shaders/tutorial-2-new.frag"));
	GLApp::models.push_back(GLApp::tristrips_model(5,10, "../shaders/tutorial-2-new.vert", "../shaders/tutorial-2-new.frag"));
	// Part 4: This is related to Task 1 where OpenGL context
	
	vps.push_back(GLViewport{ 0,h/2, w/2, h/2 });
	vps.push_back(GLViewport{ w/2, h/2, w/2, h/2 });
	vps.push_back(GLViewport{ 0, 0, w/2, h/2 });
	vps.push_back(GLViewport{ w/2, 0, w/2, h/2 });

	GLHelper::print_specs();
}


void GLApp::update(double delta_time) {
}

void GLApp::draw() {

	std::string stitle{ GLHelper::title };
	glfwSetWindowTitle(GLHelper::ptr_window, stitle.c_str());
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(vps[0].x, vps[0].y, vps[0].width, vps[0].height);
	GLApp::models[0].draw();

	glViewport(vps[1].x, vps[1].y, vps[1].width, vps[1].height);
	GLApp::models[1].draw();

	glViewport(vps[2].x, vps[2].y, vps[2].width, vps[2].height);
	GLApp::models[2].draw();

	glViewport(vps[3].x, vps[3].y, vps[3].width, vps[3].height);
	GLApp::models[3].draw();
}
void GLApp::cleanup() {
  // empty for now
}

void GLApp::GLModel::setup_shdrpgm(std::string vtx_shdr, std::string frg_shdr) {
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vtx_shdr));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frg_shdr));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
}

void create_xvals(int slices) {
	int const count{ slices + 1 }; // number of values
	std::vector<float> xpos(count);
	for (int i{ 0 }; i <= slices; ++i) {
		float x = (2.f * i) / static_cast<float>(slices) - 1.f;
		xpos[i] = x;
	}
}

GLApp::GLModel GLApp::points_model(int slices,int stacks, std::string vtx_shdr, std::string frg_shdr) {
	
	int const count{ (stacks + 1) * (slices + 1) };
	std::vector<glm::vec2> pos_vtx(count);
	float const u{ 2.f / static_cast<float>(slices) };
	float const v{ 2.f / static_cast<float>(stacks) };
	for (int row{ 0 }, index{ 0 }; row <= stacks; ++row) {
		for (int col{ 0 }; col <= slices; ++col) {
			pos_vtx[index++] = glm::vec2(u * static_cast<float>(col) - 1.f, v * static_cast<float>(row) - 1.f);
		}
	}
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	glBindVertexArray(0);

	GLApp::GLModel mdl;
	mdl.vaoid = vao_hdl;
	mdl.primitive_type = GL_POINTS;
	mdl.setup_shdrpgm(vtx_shdr, frg_shdr);
	mdl.draw_cnt = pos_vtx.size();
	mdl.primitive_cnt = pos_vtx.size();
	return mdl;
}
GLApp::GLModel GLApp::lines_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr)
{
	int const count{ ((stacks + 1) * 2) + ((slices +1)*2) };
	std::vector<glm::vec2> pos_vtx(count);
	float const u{ 2.f / static_cast<float>(slices) };
	float const v{ 2.f / static_cast<float>(stacks) };

	int index{ 0 };
	
	for (int col{ 0 }; col <= slices; ++col) {
		float x{ u * static_cast<float>(col) - 1.f };
		pos_vtx[index++] = glm::vec2(x, -1.f);
		pos_vtx[index++] = glm::vec2(x, 1.f);
	}

	for (int row{ 0 }; row <= stacks; ++row)
	{
		float y{ v * static_cast<float>( row) -1.f };
		pos_vtx[index++] = glm::vec2(-1.f, y);
		pos_vtx[index++] = glm::vec2(1.f, y);
	}

	// set up VAO as in GLApp::points_model
	
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	glBindVertexArray(0);

	GLApp::GLModel mdl;
	mdl.vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
	mdl.primitive_type = GL_LINES;
	mdl.setup_shdrpgm(vtx_shdr, frg_shdr);
	mdl.draw_cnt = count; // number of vertices
	mdl.primitive_cnt = count / 2; // number of GL_LINES
	return mdl;
}

GLApp::GLModel GLApp::trifans_model(int slices, std::string vtx_shdr, std::string frg_shdr) {

	int const count{ (slices + 1) };
	std::vector<glm::vec2> pos_vtx(count);
	std::vector<glm::vec3> clr_vtx(slices);
	std::vector<GLushort> idx_vtx(count);

	pos_vtx[0] = glm::vec2(0.f, 0.f);

	float Theta = 6.28 / slices;

	for (int i = 0; i < slices; ++i)
	{
		idx_vtx[i] = i;
		clr_vtx[i] = glm::vec3(static_cast<float>(rand())/RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
		
		float angle = (i *Theta);

		float x = cos(angle);
		float y = sin(angle);

		pos_vtx[i+1] = glm::vec2{ x,y };
	}

	idx_vtx[count - 1] = 1;
	
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

	mdl.primitive_type = GL_TRIANGLE_FAN;
	mdl.vaoid = vao_hdl;
	mdl.setup_shdrpgm(vtx_shdr, frg_shdr);
	mdl.draw_cnt = idx_vtx.size(); // number of vertices
	mdl.primitive_cnt = slices ; // number of GL_TRIANGLE_FAN
	return mdl;
}

GLApp::GLModel GLApp::tristrips_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr)
{
	int const count{ (stacks + 1) * (slices + 1) };
	int const count2{ ((slices + 1) * 2) * stacks + (stacks - 1) * 2 };
	std::vector<glm::vec2> pos_vtx(count);
	std::vector<glm::vec3> clr_vtx(count); // new for task 4b
	std::vector<GLushort> idx_vtx(count2);

	float const u{ 2.f / static_cast<float>(slices) };
	float const v{ 2.f / static_cast<float>(stacks) };

	int a = 0;
	for (int i = 0; i < stacks; i++)
	{
		if (i != 0)
		{
			idx_vtx[a++] = (i + 1) * (slices + 1);
		}
		for (int j = 0; j <= slices; j++)
		{
			idx_vtx[a++] = (i + 1) * (slices  + 1) + j;
			idx_vtx[a++] = idx_vtx[a-1] - (slices+1);
		}
		if (i!=stacks-1)
		{
			idx_vtx[a++] = idx_vtx[a - 1];
		}
	}
	a = 0;
	for (int row = 0; row <= stacks; ++row) {

		for (int col = 0; col <= slices; ++col) {
			pos_vtx[a] = glm::vec2(u * static_cast<float>(col) - 1.f, v * static_cast<float>(row) - 1.f);
			clr_vtx[a++] = glm::vec3(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
		}
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
	mdl.setup_shdrpgm(vtx_shdr, frg_shdr);
	mdl.draw_cnt = idx_vtx.size(); // number of vertices
	mdl.primitive_cnt = slices; // number of GL_TRIANGLE_FAN
	return mdl;
}


void GLApp::GLModel::draw() {
	
	shdr_pgm.Use();
	glBindVertexArray(vaoid);
	if (primitive_type == GL_POINTS) {
		glPointSize(10.f);
		glVertexAttrib3f(1, 1.f, 0.0f, 0.f); // red color for points
		glDrawArrays(GL_POINTS, 0, draw_cnt);
		glPointSize(1.f);
	}
	else if (primitive_type == GL_LINES) {
		glLineWidth(3.f);
		glVertexAttrib3f(1, 0.f, 0.f, 1.f); // blue color for lines
		glDrawArrays(GL_LINES, 0, draw_cnt);
		glLineWidth(1.f);
	}
	else if (primitive_type == GL_TRIANGLE_FAN) {
		glLineWidth(3.f);
		glDrawElements(primitive_type, draw_cnt, GL_UNSIGNED_SHORT, NULL);
	}
	else if (primitive_type == GL_TRIANGLE_STRIP) {
		glLineWidth(3.f);
		glDrawElements(primitive_type, draw_cnt, GL_UNSIGNED_SHORT, NULL);
	}
	glBindVertexArray(0);
	shdr_pgm.UnUse();
}








