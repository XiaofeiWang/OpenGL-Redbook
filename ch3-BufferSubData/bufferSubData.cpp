#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>

using namespace std;

#include "shader.h""

enum VAO_IDs { Triangles, NumVAOs };  //VAO
enum Buffer_IDs { ArrayBuffer, NumBuffers }; //VBO
enum Attrib_IDs { vPosition = 0, vColor = 1 };  //��������

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 3;
Shader triangleShader("triangle shader");  //shader����
const int Width = 800, Height = 640; //���ڿ��

void load_shaders()
{
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	triangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	triangleShader.link();
	triangleShader.use();
}


static const GLfloat positions[] = {  //����
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
};

static const GLfloat colors[] = {  //��ɫ
	1.0f, 0.0f, 0.0f, 1.0f, // red
	0.0f, 1.0f, 0.0f, 1.0f,//green
	0.0f, 0.0f, 1.0f, 1.0f//blue                                                       //white
};

void init(void)
{
	load_shaders();
	glClearColor(0.0, 0.0, 0.0, 0.0); //����Ϊ��ɫ
	glGenVertexArrays(NumVAOs, VAOs);  //����VAO����״̬
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers); //����VBO�����������ݴ���
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors),
		         NULL, GL_STATIC_DRAW);//�ȷ����ڴ棬����������

    //ʹ��sub�������������ɫ���ݴ��뵽ͬһ��VBO��
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
	//��ɫ���ݴ���ʱ��������ǰvbo��λ�ã����������ݺ�

    //VBO���ݴ���shader��ע�ⶥ�����ɫ��ͬһvbo�У�Ҫ�������ǵ�λ��
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(positions));
    glEnableVertexAttribArray(vColor); //��ɫ�����ڶ������ݺ���

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //�����ɫ����
	glBindVertexArray(VAOs[0]);  //ʹ�����VAO��������״̬����
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);  //����������

	glFlush();  //ǿ��ִ�����ˢ��
}


int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("BufferSubData");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
