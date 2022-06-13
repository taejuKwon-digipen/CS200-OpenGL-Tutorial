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
std::map<std::string, GLSLShader> GLApp::shdrpgms;
std::map<std::string, GLApp::GLModel> GLApp::models;
std::map<std::string, GLApp::GLObject> GLApp::objects;
GLApp::Camera2D GLApp::cam;


//GLApp::GLModel model;
GLSLShader shader;

void GLApp::init() {

	// Part 1: clear colorbuffer with RGBA value in glClearColor ...
	glClearColor(1.f, 1.f, 1.f, 1.f);
	GLint w{ GLHelper::width }, h{ GLHelper::height };

	// Part 2: use the entire window as viewport ..
	glViewport(0, 0, w, h);
	GLApp::init_scene("../scenes/tutorial-4.scn");
	GLApp::cam.init(GLHelper::ptr_window, &GLApp::objects.at("Camera"));

	GLApp::init_models_cont();
	GLHelper::print_specs();
}


void GLApp::update(double delta_time) {

	GLApp::cam.update(GLHelper::ptr_window);
	for (auto& x : GLApp::objects) {
		x.second.update(delta_time); // call member function GLObject::draw()
	}
}

void GLApp::draw() {
	std::string stitle{ GLHelper::title };
	glfwSetWindowTitle(GLHelper::ptr_window, stitle.c_str());
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto const& x : GLApp::objects)
	{
		x.second.draw(); // call member function GLObject::draw()
	}
	// render camera afer everything else has been rendered
	// otherwise, the black triangle will be occluded by other objects
}

void GLApp::cleanup() {
	// empty for now
}

void GLApp::init_scene(std::string scene_filename)
{
	std::ifstream ifs{ scene_filename, std::ios::in };

	if (!ifs) {
		std::cout << "ERROR: Unable to open scene file: " << scene_filename << "\n";
		exit(EXIT_FAILURE);
	}

	ifs.seekg(0, std::ios::beg);
	std::string line;
	getline(ifs, line); // first line is count of objects in scene
	std::istringstream line_sstm{ line };
	int obj_cnt;
	line_sstm >> obj_cnt; // read count of objects in scene

	while (obj_cnt--) { // read each object's parameters
		getline(ifs, line); // 1st parameter: model's name
		std::istringstream line_modelname{ line };
		std::string model_name;
		line_modelname >> model_name;

		if (models.find(model_name) == models.end())
		{
			GLModel model;
			model.init(model_name);
			models.insert(std::pair <std::string, GLModel>(model_name, model));
		}

		getline(ifs, line); //2st - object's name
		std::istringstream out_name{ line };
		std::string obj;
		out_name >> obj;

		getline(ifs, line);
		std::istringstream out_file{ line };
		std::string sdr_prg, ver, frag;
		out_file >> sdr_prg >> ver >> frag;
		if (shdrpgms.find(sdr_prg) == shdrpgms.end())
		{
			init_shdrpgms(sdr_prg, ver, frag);
		}

		getline(ifs, line); //3st - RGB parameters
		std::istringstream out_rgb{ line };
		GLObject object;
		float r, g, b;
		out_rgb >> r >> g >> b;
		object.color = { r,g,b };

		getline(ifs, line); //4st - scaling factors to be applied on object's model
		std::istringstream out_scale{ line };
		double x, y;
		out_scale >> x >> y;
		object.scaling = { x,y };

		getline(ifs, line); //5st - orientation factors
		std::istringstream out_ori{ line };
		double o1, o2;
		out_ori >> o1 >> o2;
		object.orientation = { o1,o2 };

		getline(ifs, line); //6st - object's position in game world
		std::istringstream out_position{ line };
		double x1, y1;
		out_position >> x1 >> y1;
		object.position = { x1, y1 };  //game world라서 애매하노

		object.mdl_ref = models.find(model_name);
		object.shd_ref = shdrpgms.find(sdr_prg);

		objects.insert(std::pair<std::string, GLObject>(obj, object));
	}
};

void GLApp::GLModel::init(std::string model_name)
{

	std::vector<glm::vec2> pos_vtx;
	std::vector<GLushort> idx_vtx;

	if (model_name == "square")
	{
		std::ifstream ifs1{ "../meshes/" + model_name + ".msh", std::ios::in };
		std::string line;

		getline(ifs1, line);//object name

		std::istringstream line_sstm{ line };

		while (!ifs1.eof())
		{
			std::string line;
			getline(ifs1, line);
			std::istringstream line_sstm{ line };
			char type;
			line_sstm >> type;
			if (type == 'v')
			{
				float x, y;
				line_sstm >> x;
				line_sstm >> y;
				pos_vtx.push_back(glm::vec2(x, y));
			}
			else if (type == 't')
			{
				primitive_type = GL_TRIANGLES;
				GLushort x;
				while (line_sstm >> x)
				{
					idx_vtx.push_back(x);
				}
			};
		}
	}
	else if (model_name == "triangle")
	{
		std::ifstream ifs2{ "../meshes/" + model_name + ".msh", std::ios::in };
		std::string line;


		getline(ifs2, line);//object name
		std::istringstream line_sstm{ line };


		while (!ifs2.eof())
		{
			std::string line;
			getline(ifs2, line);
			std::istringstream line_sstm{ line };
			char type;
			line_sstm >> type;
			if (type == 'v')
			{
				float x, y;
				line_sstm >> x;
				line_sstm >> y;
				pos_vtx.push_back(glm::vec2(x, y));

			}
			else if (type == 't')
			{
				primitive_type = GL_TRIANGLE_FAN;
				GLushort x;
				while (line_sstm >> x)
				{
					idx_vtx.push_back(x);
				}
			};
		}

	}
	else if (model_name == "circle")
	{
		std::ifstream ifs3{ "../meshes/" + model_name + ".msh", std::ios::in };
		std::string line;
		getline(ifs3, line);//object name

		std::istringstream line_sstm{ line };


		while (!ifs3.eof())
		{
			std::string line;
			getline(ifs3, line);
			std::istringstream line_sstm{ line };
			char type;
			line_sstm >> type;
			if (type == 'v')
			{
				float x, y;
				line_sstm >> x;
				line_sstm >> y;
				pos_vtx.push_back(glm::vec2(x, y));

			}
			else if (type == 'f')
			{
				primitive_type = GL_TRIANGLE_STRIP;
				GLushort x;
				while (line_sstm >> x)
				{
					idx_vtx.push_back(x);
				}
			}
		}
	}

	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl,sizeof(glm::vec2) * pos_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0,sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * static_cast<GLuint>(idx_vtx.size()),reinterpret_cast<GLvoid*>(idx_vtx.data()),GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);

	vaoid = vao_hdl;
	draw_cnt = static_cast<GLuint>(idx_vtx.size()); // number of vertices

}
void create_xvals(int slices) {
	int const count{ slices + 1 }; // number of values
	std::vector<float> xpos(count);
	for (int i{ 0 }; i <= slices; ++i) {
		float x = (2.f * i) / static_cast<float>(slices) - 1.f;
		xpos[i] = x;
	}
}

void GLApp::init_models_cont() {
}

void GLApp::GLObject::init()
{
	//std::default_random_engine dre(std::random_device{}());
	//static std::uniform_real_distribution<float> urdf(-1.0, 1.0);
	double const WorldRange{ 2000.0 };
}

void GLApp::GLObject::draw() const
{
	// load shader program in use by this object
	shd_ref->second.Use();
	// bind VAO of this object's model
	glBindVertexArray(mdl_ref->second.vaoid);

	// uniform variable uModelToNDC
	GLint uniform_var_loc1 = glGetUniformLocation(GLApp::shdrpgms[shd_ref->first].GetHandle(), "uModelToNDC");
	if (uniform_var_loc1 >= 0)
	{
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mdl_to_ndc_xform));
	}
	else {
		std::cout << "Uniform variable doesn't exist!!!\n";
		std::exit(EXIT_FAILURE);
	}

	GLint color = glGetUniformLocation(GLApp::shdrpgms[shd_ref->first].GetHandle(), "uColor");
	if (uniform_var_loc1 >= 0 && color >= 0)
	{
		glUniform3f(color, this->color.r, this->color.g, this->color.b);
	}
	else if (uniform_var_loc1 >= 0 && color >= 0)
	{
		std::cout << "Uniform variable doesn't exist!!!\n";
		std::exit(EXIT_FAILURE);
	}

	glDrawElements(mdl_ref->second.primitive_type, mdl_ref->second.draw_cnt, GL_UNSIGNED_SHORT, NULL);

	shd_ref->second.UnUse();
}

void GLApp::GLObject::update(GLdouble delta_time)
{
	glm::mat3 scale_matrix
	{
		scaling.x,0,0,
		0,scaling.y,0,
		0,0,1
	};
	double PI = 3.14159265359;
	orientation.x += orientation.y * (GLfloat)delta_time;

	glm::mat3 rotation_matrix
	{
		cos(orientation.x * (float)PI / (float)180),sin(orientation.x * (float)PI / (float)180),0,
	   -sin(orientation.x * (float)PI / (float)180),cos(orientation.x * (float)PI / (float)180),0,
		0,0,1
	};

	glm::mat3 trans_matrix
	{
		1,0,0,
		0,1,0,
	   position.x,position.y,1
	};
	mdl_xform = trans_matrix * rotation_matrix * scale_matrix;
	mdl_to_ndc_xform = cam.world_to_ndc_xform * mdl_xform;

}

void GLApp::init_shdrpgms(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr) {
	std::vector<std::pair<GLenum, std::string>> shdr_files{
	std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
	std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
	};
	GLSLShader shdr_pgm;
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
	// add compiled, linked, and validated shader program to
	// std::map container GLApp::shdrpgms
	GLApp::shdrpgms[shdr_pgm_name] = shdr_pgm;
}


void GLApp::Camera2D::init(GLFWwindow* window, GLObject* ptr)
{
	double PI = 3.141592;
	// assign address of object of type GLApp::GLObject with
	// name "Camera" in std::map container GLApp::objects ...
	pgo = ptr;

	GLsizei fb_width, fb_height;
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
	ar = static_cast<GLfloat>(fb_width) / fb_height;

	// compute camera's up and right vectors ...
	//right and up is must be orthogonal vector
	//up vector = (-sin theta, cos theta)

	double angle_radian_x = pgo->orientation.x * (PI / 180);
	up = glm::vec2{ -sin(angle_radian_x), cos(angle_radian_x) };
	right = glm::vec2{ cos(angle_radian_x), sin(angle_radian_x) };

	// at startup, the camera must be initialized to free camera ...
	view_xform =
	{
		1,0,0,
		0,1,0,
	   -pgo->position.x,-pgo->position.y,1
	};

	camwin_to_ndc_xform = {
		2.f / GLHelper::width, 0,0,
		0, 2.f / GLHelper::height, 0,
		0,0,1.f
	};

	world_to_ndc_xform = camwin_to_ndc_xform * view_xform;


}

void GLApp::Camera2D::update(GLFWwindow* window)
{
	// check keyboard button presses to enable camera interactivity

	int stateH = glfwGetKey(window, GLFW_KEY_H);
	int stateK = glfwGetKey(window, GLFW_KEY_K);
	
	if(stateH == GLFW_PRESS)
	{
		pgo->orientation.x += pgo->orientation.y;
	} else if (stateK == GLFW_PRESS)
	{
		pgo->orientation.x -= pgo->orientation.y;
	}
	
	double PI = 3.141592;
	double angle_radian_x = pgo->orientation.x * (PI / 180);
	double angle_radian_y = pgo->orientation.x * (PI / 180);
	
	up = glm::vec2{ -sin(angle_radian_x), cos(angle_radian_y) };
	right = glm::vec2{ cos(angle_radian_x), sin(angle_radian_y) };

	int stateU = glfwGetKey(window, GLFW_KEY_U); // rotate cam
	int stateZ = glfwGetKey(window, GLFW_KEY_Z); //zoom
	int stateV = glfwGetKey(window, GLFW_KEY_V);
	
	if( stateU == GLFW_PRESS)
	{
		pgo->position = pgo->position + linear_speed * up;
	}

	static bool max = false;
	if( stateZ == GLFW_PRESS) //ar
	{
		if( max == false)
		{
			height += height_chg_val;
		}
		if (height >= max_height  )
		{
			max = true;
		}
		
		if (max ==true)
		{
			height -= height_chg_val;
		}
		 if ( height <= min_height && max == true)
		{
			max = false;
		}
	}

	static bool Press = false;

	if (Press == false && stateV == GLFW_PRESS)
	{
		std::cout << "hello" << std::endl;
		 view_xform= {
			1,0,0,
			0,1,0,
			-pgo->position.x, -pgo->position.y, 1
		};
		Press = true;
	}
	else if (Press==true && stateV == GLFW_PRESS)
	{
		view_xform = {
			right.x,up.y,0,
			right.y,up.x,0,
			-right*pgo->position, -up*pgo->position, 1
		};
		Press = false;
	}

	camwin_to_ndc_xform = glm::mat3{ 2. / (GLsizei)(ar * height), 0,0, 0, 2. / height, 0,0,0,1 };
	world_to_ndc_xform = camwin_to_ndc_xform * view_xform;
}








