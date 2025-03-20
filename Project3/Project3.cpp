#include <stdio.h>
#include <string.h>
#include <cmath>
#include <ctime>

#include <GL/glew.h>      
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderId;

// clock_t = long
// CLOCKS_PER_SEC = clock_t { 1000 }
const double fps = 60.0f;
const clock_t CLOCKS_PER_FRAME = CLOCKS_PER_SEC / fps;  // �� ����� ������������� � clock_t
clock_t result, lastTime;

GLuint uniformXMove, uniformYMove;
bool directionX = true; 
bool directionY = true;
float triOffsetX = 0.0f;
float triOffsetY = 0.0f;
float triMaxOffsetX = 0.90f;
float triMaxOffsetY = 0.90f;
float triIncrementX = 0.001f;
float triIncrementY = 0.005f;

static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
uniform float xMove;                                        \n\
uniform float yMove;                                        \n\
                                                            \n\
void main() {                                               \n\
  gl_Position = vec4(pos.x + xMove, pos.y + yMove, pos.z, 1.0);     \n\
}                                                           \n\
";

static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
void main() {                                               \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}                                                           \n\
";

GLfloat triangle1[] = {
    -0.1f, -0.1f, 0.0f,
     0.1f, -0.1f, 0.0f,
     0.0f,  0.1f, 0.0f,
};

void CreateTriangle(GLfloat vertices[], GLsizei vertexCount) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint &shaderId, const char* code, GLenum type) {
    GLuint subShaderId = glCreateShader(type);

    // GLchar = char
    const GLchar* shaderCode[1];
    shaderCode[0] = code;

    // GLint = int
    GLint codeLength[1];
    codeLength[0] = strlen(code);

    glShaderSource(subShaderId, 1, shaderCode, codeLength);
    glCompileShader(subShaderId);

    // �������� ������� ��������� ������� (vertex,fragment, a �� ��������)
    // shaderId � �� ������� ������, � �� �������� ��������
    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glGetShaderiv(subShaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(subShaderId, sizeof(errorLog), NULL, errorLog);
        printf("������� ��������� ������� '%d': '%s'\n", type, errorLog);
        glDeleteShader(subShaderId);
        return;
    }

    glAttachShader(shaderId, subShaderId);
    // ��������� ������ ���� ��������� �� �������� (����� �� ��������)
    glDeleteShader(subShaderId);
}

void CompileShaders(GLuint &shaderId, const char* vert, const char* frag) {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("�� ������� �������� �������� ��������!\n");
        return;
    }

    // ������� �������� � ����������� ������� � ����� ��������
    AddShader(shaderId, vert, GL_VERTEX_SHADER);
    AddShader(shaderId, frag, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // ����������, �� �������� �� �������
    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("������� ��������� �������: '%s'\n", errorLog);
        glDeleteProgram(shaderId);
        return;
    }

    // ����������, �� ����������� ��������
    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("������� �������� �������: '%s'\n", errorLog);
        glDeleteProgram(shaderId);
        return;
    }

    uniformXMove = glGetUniformLocation(shaderId, "xMove");
    uniformYMove = glGetUniformLocation(shaderId, "yMove");
}

int main() {
    if (!glfwInit()) {
        printf("�� ������� ������������ GLFW\n");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (!mainWindow) {
        printf("�� ������� �������� ���� GLFW\n");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("�� ������� ������������ GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    CompileShaders(shaderId, vShader, fShader);
    CreateTriangle(triangle1, 9);  // 9 float = 3 ������� * 3 ����������

    // �������� ������ ���, ����� ���������� ���� ��� ������� �����
    lastTime = std::clock();

    while (!glfwWindowShouldClose(mainWindow)) {
        // �������� ��������� ���� � ������ ��������� �����, ���� ������� fps
        result = std::clock();
        if (result - lastTime < CLOCKS_PER_FRAME) {
            continue;
        }
        lastTime = result;

        glfwPollEvents();

        if (directionX) triOffsetX += triIncrementX;
        else triOffsetX -= triIncrementX;

        if (directionY) triOffsetY += triIncrementY;
        else triOffsetY -= triIncrementY;

        if (abs(triOffsetX) >= triMaxOffsetX) 
            directionX = !directionX;
        if (abs(triOffsetY) >= triMaxOffsetY)
            directionY = !directionY;

        // ������� ������ ������� ������ ��������
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // uniform ���� ����������� �� ��������-������� ���� ������ �� ����
        // �������� ���� �� ���-�������
        glUseProgram(shaderId);
        glBindVertexArray(VAO);
        
        // ������, ���� �� ��������� glDrawArrays
        // ���� �� ��� ���������� ���������
        // � �� glDrawArrays ���� ����������
        glUniform1f(uniformXMove, triOffsetX);
        glUniform1f(uniformYMove, triOffsetY);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ���'�������� ������� 
        // 0 - ��������� �������� ��� Vertex Array Object (VAO)
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    // �������� ������� OpenGL, ��� �������� ������ ���'��
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderId);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}
