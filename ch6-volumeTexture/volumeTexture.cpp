#include <GL/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
#include <shader.h>
#include <loadTexture.cpp>

float aspect;
GLuint base_prog;
GLuint vao;
GLuint quad_vbo, tex;
GLuint gSampler, tc_rotate_loc;
Shader textureShader("Texture shader");
const int Width = 800, Height = 640; //���ڿ��

void load_shaders()
{
	textureShader.init();
	textureShader.attach(GL_VERTEX_SHADER, "volume.vert");
	textureShader.attach(GL_FRAGMENT_SHADER, "volume.frag");
	textureShader.link();
	textureShader.use();
	base_prog = textureShader.program;
    gSampler = glGetUniformLocation(base_prog, "gSampler");  //sampler��shader��ȡ�������ݵķ���
    glUniform1i(gSampler, 0); //ָ������������Ϊ0��

}

void init(void)
{
	load_shaders();

	vglImageData image;
	tex = vglLoadTexture("../media/textures/cloud.dds", 0, &image);
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	vglUnloadImage(&image);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	static const GLfloat quad_data[] = {
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
	static const unsigned int start_time = GetTickCount();
    float t = float((GetTickCount() - start_time)) / float(0x3FFF);

	//�����ɫ����Ȼ���
    glDisable(GL_CULL_FACE);
    glUseProgram(base_prog);
	glm::mat4 tc_matrix = glm::rotate(glm::mat4(1.0), t * 170.0f, glm::vec3(1.0, 0.0, 0.0)) *
                          glm::rotate(glm::mat4(1.0), t * 137.0f, glm::vec3(0.0, 1.0, 0.0))  *
                          glm::rotate(glm::mat4(1.0), t * 93.0f, glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(tc_rotate_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));
	glBindTexture(GL_TEXTURE0, tex); //����Ҫʹ�õ�����
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


