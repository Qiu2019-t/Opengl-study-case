#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>//读取流
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
	//用glGetUniformLocation查询uniform ourColor的位置值
	int _location = glGetUniformLocation(shaderProgram,"ourColor");
	glUniform4f(_location, 0.0f, _green, 0.0f, 1.0f);
*/
	//针对哪个VAO进行绘图
	glBindVertexArray(VAO);
	//类型三角形，顶点起始索引，绘制多少顶点
	glDrawArrays(GL_TRIANGLES,0,3);
	//6个顶点，索引位置0开始读取
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
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//传输数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
*/
	//vbo indx
	glGenBuffers(1, &VBO);//同时获取多个
	//绑定vbo的indx
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//给vbo分配显存空间 传输数据
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);//顶点不变																	
	//告诉shader数据解析方式                 紧密排列都是顶点坐标可以设置为0
	//VAO记录了VBO的锚点信息                                    偏移量
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	//激活锚点
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initShader(const char*_vertexPath,const char * _fragPath) {
	//读取两个Shader的代码
	string _vertexCode("");
	string _fragCode("");
	//文件读取器
	ifstream _vShaderFile;
	ifstream _fShaderFile;
	_vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);//文件损坏读取失败
	_fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{	//打开文件
		_vShaderFile.open(_vertexPath);
		_fShaderFile.open(_fragPath);
		//两个文件的读取流
		stringstream _vShaderStream, _fShaderStream;
		//读取文件的缓冲内容到数据流
		_vShaderStream << _vShaderFile.rdbuf();
		_fShaderStream << _fShaderFile.rdbuf();
		//关闭文件处理器
		_vShaderFile.close();
		_fShaderFile.close();
		//转换数据流到string
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
	//shader的编译与链接
	unsigned int _vertexID = 0, _fragID = 0;
	char _infoLog[512];//存取错误信息
	int _successFlag = 0;
	//编译
	//顶点着色器
	_vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexID,1,&_vShaderStr,NULL);//传入代码
	glCompileShader(_vertexID);
	//打印编译错误(如果有的话)
	glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);//编译是否成功
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
	//链接
	shaderProgram = glCreateProgram();//创建shader程序
	glAttachShader(shaderProgram, _vertexID);//加入编译好的两个shader
	glAttachShader(shaderProgram, _fragID);
	glLinkProgram(shaderProgram);//两个shader放一起
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&_successFlag);
	if (!_successFlag) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	//删除着色器，已经链接到程序中，留下编译好的shaderProgram
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
}
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//核心模式3版本
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//宽，高，窗口名字
	GLFWwindow* window = glfwCreateWindow(800,600,"OpenGL Core",NULL,NULL);
	if (window==NULL) {
		cout << "Failed create GLFW window" << endl;
		glfwTerminate();//退出
		return -1;
	}
	glfwMakeContextCurrent(window);//虚拟机和当前窗口绑定

	//函数指针??
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLFW"<<endl;
		return -1;
	}
	glViewport(0,0,800,600);
	//窗口大小发生变动
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	initModel();
	initShader("vertexShader.glsl", "fragmentShader.glsl");
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f,0.3f,0.3f,0.1f);
		glClear(GL_COLOR_BUFFER_BIT);//清画布

		rend();

		glfwSwapBuffers(window);//双缓存
		glfwPollEvents();//分发事件
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}