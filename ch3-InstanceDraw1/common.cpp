
#include "common.h"

using std::cout; using std::endl;
using std::cerr;

char *textFileRead(const char *fn) {  
	FILE *fp;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  
		fp = fopen(fn,"rt");  

		if (fp != NULL) {  
			fseek(fp, 0, SEEK_END);  
			count = ftell(fp);  
			rewind(fp);  

			if (count > 0) {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			}  
			fclose(fp);  
		} 
		else 
			cout << "Fail to open the shader file" << endl;
	}  
	return content;  
}  


GLuint initShader(char *VShaderFile, char *FShaderFile)  
{  
	//1���鿴GLSL��OpenGL�İ汾  
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion =   
		glGetString( GL_SHADING_LANGUAGE_VERSION );  
	GLint major, minor;  
	glGetIntegerv(GL_MAJOR_VERSION, &major);  
	glGetIntegerv(GL_MINOR_VERSION, &minor);  
	cout << "GL Vendor    :" << vendor << endl;  
	cout << "GL Renderer  : " << renderer << endl;  
	cout << "GL Version (string)  : " << version << endl;  
	cout << "GL Version (integer) : " << major << "." << minor << endl;  
	cout << "GLSL Version : " << glslVersion << endl;   

	//2��������ɫ��  
	//������ɫ�����󣺶�����ɫ��  
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);  
	//������  
	if (0 == vShader)  
	{  
		cerr << "ERROR : Create vertex shader failed" << endl;  
		exit(1);  
	}  

	//����ɫ��Դ�������ɫ�����������  
	const GLchar *vShaderCode = textFileRead(VShaderFile);  
	const GLchar *vCodeArray[1] = {vShaderCode};  
	glShaderSource(vShader,1,vCodeArray,NULL);  

	//������ɫ������  
	glCompileShader(vShader);  


	//�������Ƿ�ɹ�  
	GLint compileResult;  
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&compileResult);  
	if (GL_FALSE == compileResult)  
	{  
		GLint logLen;  
		//�õ�������־����  
		glGetShaderiv(vShader,GL_INFO_LOG_LENGTH,&logLen);  
		if (logLen > 0)  
		{  
			char *log = (char *)malloc(logLen);  
			GLsizei written;  
			//�õ���־��Ϣ�����  
			glGetShaderInfoLog(vShader,logLen,&written,log);  
			cerr << "vertex shader compile log : " << endl;  
			cerr << log << endl;  
			free(log);//�ͷſռ�  
		}  
	}  

	//������ɫ������Ƭ����ɫ��  
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);  
	//������  
	if (0 == fShader)  
	{  
		cerr << "ERROR : Create fragment shader failed" << endl;  
		exit(1);  
	}  

	//����ɫ��Դ�������ɫ�����������  
	const GLchar *fShaderCode = textFileRead(FShaderFile);  
	const GLchar *fCodeArray[1] = {fShaderCode};  
	glShaderSource(fShader,1,fCodeArray,NULL);  

	//������ɫ������  
	glCompileShader(fShader);  

	//�������Ƿ�ɹ�  
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&compileResult);  
	if (GL_FALSE == compileResult)  
	{  
		GLint logLen;  
		//�õ�������־����  
		glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&logLen);  
		if (logLen > 0)  
		{  
			char *log = (char *)malloc(logLen);  
			GLsizei written;  
			//�õ���־��Ϣ�����  
			glGetShaderInfoLog(fShader,logLen,&written,log);  
			cerr << "fragment shader compile log : " << endl;  
			cerr << log << endl;  
			free(log);//�ͷſռ�  
		}  
	}  

	//3��������ɫ������  
	//������ɫ������  
    GLuint programHandle = glCreateProgram();  
	if (!programHandle)  
	{  
		cerr << "ERROR : create program failed" << endl;  
		exit(1);  
	}  
	//����ɫ���������ӵ��������ĳ�����  
	glAttachShader(programHandle,vShader);  
	glAttachShader(programHandle,fShader);  
	//����Щ�������ӳ�һ����ִ�г���  
	glLinkProgram(programHandle);  
	//��ѯ���ӵĽ��  
	GLint linkStatus;  
	glGetProgramiv(programHandle,GL_LINK_STATUS,&linkStatus);  
	if (GL_FALSE == linkStatus)  
	{  
		cerr << "ERROR : link shader program failed" << endl;  
		GLint logLen;  
		glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH,  
			&logLen);  
		if (logLen > 0)  
		{  
			char *log = (char *)malloc(logLen);  
			GLsizei written;  
			glGetProgramInfoLog(programHandle,logLen,  
				&written,log);  
			cerr << "Program log : " << endl;  
			cerr << log << endl;  
		}  
	}  
	else//���ӳɹ�����OpenGL������ʹ����Ⱦ����  
	{  
		glUseProgram(programHandle);  
	  cout << "Success to use the shader!" << endl;
	}  
	return programHandle;
}

/*
GLuint loadTexture(const  char* theFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	GLuint textureID;			// Create a texture ID as a GLuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state
	ilInit();
	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image
	success = ilLoadImage(theFileName); 	// Load the image file
	// If we managed to load the image, then we can start to do things with it...
	if (success) {
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		// Quit out if we failed the conversion

		// Generate a new texture
		glGenTextures(1, &textureID);
		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);
 
		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 
		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
					 0,				// Pyramid level (for mip-mapping) - 0 is the top level
					 ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
					 ilGetInteger(IL_IMAGE_WIDTH),	// Image width
					 ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
					 0,				// Border width in pixels (can either be 1 or 0)
					 ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
					 GL_UNSIGNED_BYTE,		// Image data type
					 ilGetData());			// The actual image data itself
 	}

 
 	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
 
	cout << "Texture creation successful." << endl;
 
	return textureID; // Return the GLuint to the texture so you can use it!
}

*/