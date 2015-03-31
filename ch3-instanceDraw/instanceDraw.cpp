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
const int  INSTANCE_COUNT = 200 ; //����ʵ����
    // Member variables
static float aspect;
GLuint vao[2];
GLuint vbo[2];
GLuint weight_vbo;
GLuint color_vbo;
GLuint program;
GLint  modelLoc;
GLint  projLoc;
VBObject object;
Shader instanceShader("instance Shader");

void load_shaders()
{
	instanceShader.init();
	instanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	instanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	instanceShader.link();
	instanceShader.use();
	program = instanceShader.program;  //��ȡ�����uniformλ��
	modelLoc = glGetUniformLocation(program, "model_matrix");
	projLoc= glGetUniformLocation(program, "proj_matrix");

}

void init(void)
{
	load_shaders();
	//glEnable(GL_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 0.0); //����Ϊ��ɫ
    glEnable(GL_CULL_FACE);  //�����޳�
    glDisable(GL_DEPTH_TEST); //�ر���Ȳ���

	 //����ģ��
    object.LoadFromVBM("../media/objects/armadillo_low.vbm", 0, 1, 2); //���㣬����������
    // Bind its vertex array object so that we can append the instanced attributes
    object.BindVertexArray();
	//�󶨶�������
    // Generate the colors of the objects

    glm::vec4 colors[INSTANCE_COUNT]; //��ͬʵ�����費ͬ��ɫ
	for (int n = 0; n < INSTANCE_COUNT; n++) {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;
        colors[n][0] = 0.5f * (sinf(a + 1.0f) + 1.0f);
        colors[n][1] = 0.5f * (sinf(b + 2.0f) + 1.0f);
        colors[n][2] = 0.5f * (sinf(c + 3.0f) + 1.0f);
        colors[n][3] = 1.0f;
    }

    glGenBuffers(1, &weight_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, weight_vbo); //ʹ��colors��ʼ������Ϊ���ߴ�С��ͬ
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    // Here is the instanced vertex attribute - set the divisor
    glVertexAttribDivisor(3, 1); //��һ�������Ƕ��������������ڶ�������ָ��ÿ�θ���һ����ͬ��ֵ
    // It's otherwise the same as any other vertex attribute - set the pointer and enable it
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    // ��ɫ�������
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribDivisor(4, 1);//��һ�������Ƕ��������������ڶ�������ָ��ÿ�θ���һ����ͬ����ɫֵ
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(4);

    // Done (unbind the object's VAO)
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

    // Set weights for each instance
    glm::vec4 weights[INSTANCE_COUNT];

    for (int n = 0; n < INSTANCE_COUNT; n++) {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;
        weights[n][0] = 0.5f * (sinf(t * 6.28318531f * 8.0f + a) + 1.0f);
        weights[n][1] = 0.5f * (sinf(t * 6.28318531f * 26.0f + b) + 1.0f);
        weights[n][2] = 0.5f * (sinf(t * 6.28318531f * 21.0f + c) + 1.0f);
        weights[n][3] = 0.5f * (sinf(t * 6.28318531f * 13.0f + a + b) + 1.0f);
    }

    // ���ڸı�weight������
    glBindBuffer(GL_ARRAY_BUFFER, weight_vbo);  //ÿ�λ���ʹ�õ�weight��ͬ
    glBufferData(GL_ARRAY_BUFFER, sizeof(weights), weights, GL_DYNAMIC_DRAW);

    //�����ɫ����Ȼ������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);  //��Ȳ��ԣ�zֵС������Ȼ����ֵ����
    glDepthFunc(GL_LEQUAL);

    // Set four model matrices
    static glm::mat4 model_matrix[4];
	//ģ�;���
    for (int n = 0; n < 4; n++) {
        model_matrix[n] = (glm::scale(glm::mat4(1.0), glm::vec3(5.0f, 5.0f, 5.0f) ) *
                           glm::rotate(glm::mat4(1.0), t * 360.0f * 40.0f + float(n + 1) * 29.0f, glm::vec3(0.0f, 1.0f, 0.0f))*
                           glm::rotate(glm::mat4(1.0), t * 360.0f * 20.0f + float(n + 1) * 35.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
                           glm::rotate(glm::mat4(1.0), t * 360.0f * 30.0f + float(n + 1) * 67.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                           glm::translate(glm::mat4(1.0), glm::vec3( (float)n * 10.0f - 15.0f, 0.0f, 0.0f) )*
                           glm::scale(glm::mat4(1.0), glm::vec3(0.01f, 0.01f, 0.01f)) );
    }
	//��ģ�;�����shader�� 4������4��ģ�;���
	glUniformMatrix4fv(modelLoc, 4, GL_FALSE, glm::value_ptr(model_matrix[0]));

    //��ͶӰ������shader
    glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) 
		                       * glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -50.0f)) );
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
   
    object.Render(0, INSTANCE_COUNT);	//ʹ��ʵ������ģ��
	glutSwapBuffers();
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	////glutInitContextVersion(4, 3);
	////glutInitContextProfile(GLUT_CORE_PROFILE);
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


