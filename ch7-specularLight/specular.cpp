#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;

#include <shader.h>
#include <vbm.cpp>

const int Width = 800, Height = 640; //���ڿ��

    // Member variables
static float aspect;
GLuint vao[1];
GLuint vbo[1];
GLuint program;
GLint  mvpLoc, diffuseLoc, dirctionLoc, ambientLoc;
VBObject object;
Shader diffuseShader("instance Shader");

void load_shaders()
{
	diffuseShader.init();
	diffuseShader.attach(GL_VERTEX_SHADER, "diffuse.vert");
	diffuseShader.attach(GL_FRAGMENT_SHADER, "diffuse.frag");
	diffuseShader.link();
	diffuseShader.use();
	program = diffuseShader.program;  //��ȡ�����uniformλ��
	mvpLoc = glGetUniformLocation(program, "mvp_matrix");
	diffuseLoc = glGetUniformLocation(program, "diffuse");

}

void init(void)
{
	//glEnable(GL_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 0.0); //����Ϊ��ɫ
    glEnable(GL_CULL_FACE);  //�����޳�
    glDisable(GL_DEPTH_TEST); //�ر���Ȳ���

	 //����ģ��
    object.LoadFromVBM("../media/objects/util_torus.vbm", 0, 1, 2); //���㣬����������
    //object.BindVertexArray();

    glBindVertexArray(0);

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	 aspect = float(w) / float(h);

}

void display(void)
{
    float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
	//ÿ���������ı����

    static float q = 0.0f;
    static const glm::vec3 X(1.0f, 0.0f, 0.0f);
    static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
    static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);  //��Ȳ��ԣ�zֵС������Ȼ����ֵ����
    glDepthFunc(GL_LEQUAL);

    // Set four model matrices
	glm::vec4 diffuse = glm::vec4(1.0, 1.0, 1.0, 0.0);
	glm::mat4 mv_matrix = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0),glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 proj_matrix = glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) ;                   
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniform4fv(diffuseLoc, 1, glm::value_ptr(diffuse));
    object.Render();	//ʹ��ʵ������ģ��
	glutSwapBuffers();
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Instance Draw");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


