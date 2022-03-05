#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>//��ȡ��
using namespace std;
unsigned int VBO = 0;
unsigned int VAO = 0;
unsigned int shaderProgram = 0;
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
}
void rend() {
	glUseProgram(shaderProgram);
/*	float _time = glfwGetTime();
	float _green = (sin(_time)*0.5f) + 0.5f;
	//��glGetUniformLocation��ѯuniform ourColor��λ��ֵ
	int _location = glGetUniformLocation(shaderProgram,"ourColor");
	glUniform4f(_location, 0.0f, _green, 0.0f, 1.0f);
*/
	//����ĸ�VAO���л�ͼ
	glBindVertexArray(VAO);
	//���������Σ�������ʼ���������ƶ��ٶ���
	glDrawArrays(GL_TRIANGLES,0,3);
	//6�����㣬����λ��0��ʼ��ȡ
	//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	glUseProgram(0);
}
void initModel() {
	float vertices[] = {
	0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
   -0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,
	0.0f,0.5f,0.0f,  0.0f,0.0f,1.0f
	};

	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
/*
	unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	//��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//��������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
*/
	//vbo indx
	glGenBuffers(1, &VBO);//ͬʱ��ȡ���
	//��vbo��indx
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//��vbo�����Դ�ռ� ��������
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);//���㲻��																	
	//����shader���ݽ�����ʽ                 �������ж��Ƕ��������������Ϊ0
	//VAO��¼��VBO��ê����Ϣ                                    ƫ����
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	//����ê��
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initShader(const char*_vertexPath,const char * _fragPath) {
	//��ȡ����Shader�Ĵ���
	string _vertexCode("");
	string _fragCode("");
	//�ļ���ȡ��
	ifstream _vShaderFile;
	ifstream _fShaderFile;
	_vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);//�ļ��𻵶�ȡʧ��
	_fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{	//���ļ�
		_vShaderFile.open(_vertexPath);
		_fShaderFile.open(_fragPath);
		//�����ļ��Ķ�ȡ��
		stringstream _vShaderStream, _fShaderStream;
		//��ȡ�ļ��Ļ������ݵ�������
		_vShaderStream << _vShaderFile.rdbuf();
		_fShaderStream << _fShaderFile.rdbuf();
		//�ر��ļ�������
		_vShaderFile.close();
		_fShaderFile.close();
		//ת����������string
		_vertexCode = _vShaderStream.str();
		_fragCode = _fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		string errstr = "read shader fail";
		cout << errstr << endl;
	}
	const char* _vShaderStr = _vertexCode.c_str();
	const char* _fShaderStr = _fragCode.c_str();
	//shader�ı���������
	unsigned int _vertexID = 0, _fragID = 0;
	char _infoLog[512];//��ȡ������Ϣ
	int _successFlag = 0;
	//����
	//������ɫ��
	_vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexID,1,&_vShaderStr,NULL);//�������
	glCompileShader(_vertexID);
	//��ӡ�������(����еĻ�)
	glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);//�����Ƿ�ɹ�
	if (!_successFlag) {
		glGetShaderInfoLog(_vertexID, 512, NULL,_infoLog);
		string errStr(_infoLog);
		cout<<_infoLog<<endl;
	}

	_fragID= glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragID, 1, &_fShaderStr, NULL);
	glCompileShader(_fragID);
	glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	//����
	shaderProgram = glCreateProgram();//����shader����
	glAttachShader(shaderProgram, _vertexID);//�������õ�����shader
	glAttachShader(shaderProgram, _fragID);
	glLinkProgram(shaderProgram);//����shader��һ��
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&_successFlag);
	if (!_successFlag) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	//ɾ����ɫ�����Ѿ����ӵ������У����±���õ�shaderProgram
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
}
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����ģʽ3�汾
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//���ߣ���������
	GLFWwindow* window = glfwCreateWindow(800,600,"OpenGL Core",NULL,NULL);
	if (window==NULL) {
		cout << "Failed create GLFW window" << endl;
		glfwTerminate();//�˳�
		return -1;
	}
	glfwMakeContextCurrent(window);//������͵�ǰ���ڰ�

	//����ָ��??
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLFW"<<endl;
		return -1;
	}
	glViewport(0,0,800,600);
	//���ڴ�С�����䶯
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	initModel();
	initShader("vertexShader.glsl", "fragmentShader.glsl");
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f,0.3f,0.3f,0.1f);
		glClear(GL_COLOR_BUFFER_BIT);//�廭��

		rend();

		glfwSwapBuffers(window);//˫����
		glfwPollEvents();//�ַ��¼�
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}