#version 330 core
out vec4 FragColor;
in vec3 ourColor;
void main(){
	FragColor=vec4(ourColor,1.0);
    //FragColor=ourColor;
	//FragColor=vec4(1.0f,0.5f,0.2f,1.0f);
	//最后一个alpha值为1.0代表完全不透明
};
