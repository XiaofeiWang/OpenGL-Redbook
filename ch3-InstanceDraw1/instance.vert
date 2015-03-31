 #version 430 core

 layout (location = 0) in vec4 position; //ʵ������
 layout (location = 1) in vec3 normal; //ʵ������
 layout (location = 2) in vec4 color; //ʵ����ɫ
 layout (location = 3) in mat4 model_matrix; //ʵ��ģ�;���

 uniform mat4 view_matrix;
 uniform mat4 proj_matrix;

out VS_OUT {
   vec3  normal; //����
   vec4  color;  //��ɫ
} vs_out;

void main(void)
{
    mat4 model_view_matrix = view_matrix * model_matrix;
    gl_Position = proj_matrix * (model_view_matrix * position); //mvp matrix
	vs_out.normal = mat3(model_view_matrix) * normal; //ƽ�Ʋ��ı䷨�ߣ�����ֻʹ�����Ͻǵ�3x3����
	vs_out.color = color;
}