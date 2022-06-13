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
#include <fstream>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
GLApp::GLModel GLApp::mdl;
GLApp::Vertex vertex;
std::vector<GLApp::Vertex> GLApp::vertices;
GLuint texobj_hdl0;
using namespace std;

GLuint setup_texobj(std::string pathname) {

	unsigned int VBO, EBO;
	GLuint width{ 256 }, height{ 256 }, bytes_per_texel{ 4 };

	ifstream readImage(pathname, std::ifstream::binary);
	char* ptr_texels = nullptr;
	if (readImage)
	{
		//seekg를 이용한 파일크기 추출
		readImage.seekg(0, readImage.end);
		int length = (int)readImage.tellg();
		readImage.seekg(0, readImage.beg);

		ptr_texels = new char[length];

		// read data as a block:
		readImage.read(ptr_texels, length);
		readImage.close();
	};
	GLuint texobj_hdl;
	// define and initialize a handle to texture object that will
	// encapsulate two-dimensional textures
	glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);
	// allocate GPU storage for texture image data loaded from file
	glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, width, height);
	// copy image data from client memory to GPU texture buffer memory
	glTextureSubImage2D(texobj_hdl, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);
	// client memory not required since image is buffered in GPU memory
	delete[] ptr_texels;
	// nothing more to do - return handle to texture object
	return texobj_hdl;
}

void GLApp::init() {
	glClearColor(1.f, 1.f, 1.f, 1.f); 
	GLint w{ GLHelper::width }, h{ GLHelper::height };
	glViewport(0,0,w,h);
	std::cout << "w: " << w << "h: " << h << std::endl;
	mdl.setup_vao();
	mdl.setup_shdrpgm();
	GLHelper::print_specs();
	mdl.shdr_pgm.Use();

	GLint color = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "uColor");
	GLint model = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "model");
	GLint size = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "size");
	GLint toggle = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "toggle");
	glUniform1i(color, 0);
	glUniform1i(model, 0);
	glUniform1i(size, 32);
	glUniform1i(toggle, 0);
	texobj_hdl0 = setup_texobj("../images/duck-rgba-256.tex");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLApp::update(double delta_time)
{
	mdl.shdr_pgm.Use();
	int stateT = glfwGetKey(GLHelper::ptr_window,GLFW_KEY_T);
	int stateM = glfwGetKey(GLHelper::ptr_window,GLFW_KEY_M );
	int stateA = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_A);
	GLint size = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "size");
	GLint color = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "uColor");
	GLint model = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "model");
	GLint count = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "count");
	GLint toggle = glGetUniformLocation(mdl.shdr_pgm.GetHandle(), "toggle");

	// texobj_hdl0 = setup_texobj("../images/duck-rgba-256.tex");
	
	static bool PressM = false;
	static bool ReleaseM = true;
	static bool PressT = false;
	static bool PressA = false;
	static bool ReleaseA = true;
	
	float Smin = 16.f;
	float Smax = 256.f;
	int Size = 0;

	static double timer = 0;
	timer += delta_time;
	float T = timer / 30.f;
	float PI = 3.141592;
	float e = (sin((PI * T )- (PI / 2.f)) + 1.f) / 2.f;

	if( PressM == false && ReleaseM == true &&stateM == GLFW_PRESS)
	{
		PressM = true;
		glUniform1i(color, 1);

		ReleaseM = false;
	}
	else if (PressM == false && ReleaseM == false && stateM == GLFW_PRESS)
	{
		PressM = true;
		glUniform1i(color, 0);
		ReleaseM = true;
	}
	if(stateM == GLFW_RELEASE && PressM==true)
	{
		PressM = false;
	}
	///////////////////////////
	if (PressA == false && ReleaseA == true && stateA == GLFW_PRESS)
	{
		PressA = true;
		glEnable(GL_BLEND);
		ReleaseA = false;
	}
	else if (PressA == false && ReleaseA == false && stateA == GLFW_PRESS)
	{
		PressA = true;
		glDisable(GL_BLEND);
		ReleaseA = true;
	}
	if (stateA == GLFW_RELEASE && PressA == true)
	{
		PressA = false;
	}
	////////////////////////
	static int i = 0;
	if (i == 2)
	{
		Size = Smin + e * (Smax - Smin);
		glUniform1i(size, Size);
	}
	
	if(PressT == false && stateT == GLFW_PRESS)
	{
		i++;
		if(i == 1)
		{
			glUniform1i(size, 32);
			glUniform1i(model,i);
		}else if (i == 2)
		{
			glUniform1i(model, i);
		} else if (i == 3)
		{
			glUniform1i(model, i);
			
		}else if( i == 4 )
		{
			glTextureParameteri(texobj_hdl0, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texobj_hdl0, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glUniform1i(model, i);
			glUniform1i(count, 4);
		} else if( i == 5)
		{
			cout << "task5" << endl;
			glTextureParameteri(texobj_hdl0, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(texobj_hdl0, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glUniform1i(model, i);
			glUniform1i(count, 4);
		} else if( i == 6)
		{
			glTextureParameteri(texobj_hdl0, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texobj_hdl0, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glUniform1i(model, i);
			glUniform1i(count, 2);
			i = 0;
		}
		PressT = true;
	}
	if (stateT == GLFW_RELEASE && PressT == true)
	{
		PressT = false;
	}

}


void GLApp::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	mdl.draw();
}

void GLApp::cleanup() {
	// empty for now
}

void GLApp::GLModel::setup_vao()
{
	unsigned int VBO, EBO;
	// Define vertex position and color attributes
	std::array<glm::vec2, 4> pos_vtx{
	glm::vec2(-1.f, -1.f), glm::vec2(-1.f, 1.f),
	glm::vec2(1.f, 1.f), glm::vec2(1.f, -1.f)
	};
	
	std::array<glm::vec3, 4> clr_vtx{
	glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
	glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f)
	};
	std::array<glm::vec2, 4> tex_vtx{
	glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f)
	};

	vertices.push_back({ pos_vtx[0],clr_vtx[0],tex_vtx[0] });
	vertices.push_back({ pos_vtx[1],clr_vtx[1],tex_vtx[1] });
	vertices.push_back({ pos_vtx[2],clr_vtx[2],tex_vtx[2] });
	vertices.push_back({ pos_vtx[3],clr_vtx[3],tex_vtx[3] });
	
	primitive_type = GL_TRIANGLE_STRIP;
	std::array<GLushort, 4> idx_vtx{ 0, 1,3,2};
	idx_elem_cnt = idx_vtx.size();
	
	glGenVertexArrays(1, &vaoid);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_vtx.size() * sizeof(unsigned int),&idx_vtx[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void GLApp::GLModel::setup_shdrpgm()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/tutorial-5.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER,"../shaders/tutorial-5.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


void GLApp::GLModel::draw()
{
	//GLuint texobj_hdl0 = setup_texobj("../images/duck-rgba-256.tex");
	glBindVertexArray(GLApp::mdl.vaoid);
	glBindTextureUnit(6, texobj_hdl0);
	GLuint tex_loc = glGetUniformLocation(GLApp::mdl.shdr_pgm.GetHandle(), "uTex2d");
	glUniform1i(tex_loc, 6);
	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
	shdr_pgm.UnUse();
}








