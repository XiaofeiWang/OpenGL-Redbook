#include <GL/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
#include <shader.h>
#include <loadTexture.h>

float aspect;
GLuint base_prog;
GLuint vao;
GLuint quad_vbo;
GLuint texture0, texture1;
GLuint tex0Loc, tex1Loc;
GLuint timeLoc;
Shader textureShader("Texture shader");
const int Width = 800, Height = 640; //���ڿ��

void load_shaders()
{
	textureShader.init();
	textureShader.attach(GL_VERTEX_SHADER, "multiTexture.vert");
	textureShader.attach(GL_FRAGMENT_SHADER, "multiTexture.frag");
	textureShader.link();
	textureShader.use();
	base_prog = textureShader.program;
	tex0Loc = glGetUniformLocation(base_prog, "tex0");  //sampler��shader��ȡ�������ݵķ���
    glUniform1i(tex0Loc, 0); //��mengָ��Ϊ0
	tex1Loc = glGetUniformLocation(base_prog, "tex1");  //sampler��shader��ȡ�������ݵķ���
    glUniform1i(tex1Loc, 1); //��mengָ��Ϊ1
    timeLoc = glGetUniformLocation(base_prog, "time");
}
void init(void)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
	load_shaders();
    texture0 = loadTexture("../media/textures/test.dds");  //����ͼ��
	texture1 = loadTexture("../media/textures/test3.dds");

	static const GLfloat quad_data[] =
    {
         0.8f, -0.8f,  //���������� ,��ʱ�����
        -0.8f, -0.8f,
        -0.8f, 0.8f,
         0.8f, 0.8f,

         0.0f, 0.0f,  //�����������ݣ�Ҫ��ôӳ�䵽�����ϣ����½�Ϊ��0,0��
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
    };
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
	//������������������ݴ���shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
    aspect = float(width) / float(height);
}


void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�����ɫ����Ȼ���
	float t = float(GetTickCount() & 0x3FFF) / float(0x3FFF); //ÿ�ε���ı����

    glUseProgram(base_prog);
    glUniform1f(timeLoc, t);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);  //ʹ�ö�������
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //�����ı���
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Load Texture");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


