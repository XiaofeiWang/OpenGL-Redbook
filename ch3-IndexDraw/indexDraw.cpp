#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;
#include <shader.h>

Shader triangleShader("triangle shader");  //shader����
const int Width = 800, Height = 640; //���ڿ��
GLuint program;
GLuint modelLoc;
GLuint mvpLoc;
GLuint ebo[1], vbo[1], vao[1];

void load_shaders()
{
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	triangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	triangleShader.link();
	triangleShader.use();
	program = triangleShader.program;  //��ȡ�����uniformλ��
	modelLoc = glGetUniformLocation(program, "model_matrix");
	mvpLoc= glGetUniformLocation(program, "mvp_matrix");

}

static const GLfloat vertex_positions[] = //position we use to draw triangle
 {
        -1.0f, -1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  0.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 1.0f,
};

    // Color for each vertex
static const GLfloat vertex_colors[] = //the color we use 
{
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f
};

    // Indices for the triangle strips
static const GLushort vertex_indices[] = // index to use
{
        0, 1, 2  //��������������VBO�е�һ��������
};

void init(void)
{
	load_shaders();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Set up the vertex attributes to vao
    glGenVertexArrays(1, vao); //�����������״̬
    glBindVertexArray(vao[0]);

	glGenBuffers(1, ebo);  //�����������黺�����, ���������ݴ���
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

	//set up the vertex to vbo
    glGenBuffers(1, vbo);  //���㻺��洢����
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);
	//vbo is vertex's + color's data
	//enable the data to shader

	//���ƶ����ݴ���shader
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void display(void)
{
    glEnable(GL_CULL_FACE);  //��ʼ�����޳�
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model= glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, 0.0, -5.0));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
	
	glm::mat4 mvp  = projection * view;
	//����mvp����
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(modelLoc,1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

	glDrawArrays(GL_TRIANGLES, 0, 3); 
	// ��һ�������Σ�����ǰ������
	//ÿ�λ�����һ�������Σ�����ƽ�Ʋ���
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 0.0, -5.0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);
	//�ڶ���������ʹ����������

    // DrawElementsBaseVertex
  	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -5.0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 1);
	 //�����������Σ�the indices[i] + basevertex(the last element)

    // ʵ������DrawArraysInstanced
    model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0, 0.0, -5.0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

	glutSwapBuffers();  //����˫����
}

void idle()
{
	glutPostRedisplay();  //��ʱ����
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("IndewDraw");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


