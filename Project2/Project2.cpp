#include <stdio.h>
#include <string.h>
#include <GL/glew.h>      
#include <GLFW/glfw3.h>

// ��������� ������: ���������� ������� ������ � ���-������
// �������: 'in' ������ ������� �������� � ������, � �� ����� �������� ���������
static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
void main() {                                               \n\
  gl_Position = vec4(1.0*pos.x, 1.0*pos.y, pos.z, 1.0);     \n\
}                                                           \n\
";

// ����������� ������: ���� �������� ���� ������� ������
static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
void main() {                                               \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}                                                           \n\
";

struct ShaderData {
    const char* vert;
    const char* frag;
    GLuint shader;
};

struct DrawData {
    GLuint VAO, VBO, shader;
};

const GLint WIDTH = 800, HEIGHT = 600;

GLfloat triangle1[] = {
    -0.2f, -0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
     0.0f,  1.0f, 0.0f,
};

void CreateTriangle(DrawData& d, GLfloat vertices[], GLsizei vertexCount) {
    glGenVertexArrays(1, &d.VAO);
    glBindVertexArray(d.VAO);

    glGenBuffers(1, &d.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    // �������� ������� ������ ������ ������ (������� float * ����� float)
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // ������ 0: ����� �������� � ������ (������� layout � vShader)
    // 3: ������� ��������� �� ������� (x, y, z)
    // GL_FLOAT: ��� ����� ������� ����������
    // GL_FALSE: �� ������������ ��������
    // ������ 0: ���� �� ��������� (0 ������ ������ ���������)
    // ����� 0: ������� �� ������� ������ (���� �������)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint programId, const char* code, GLenum type) {
    GLuint shaderId = glCreateShader(type);

    const GLchar* shaderCode[1];
    shaderCode[0] = code;

    GLint codeLength[1];
    codeLength[0] = strlen(code);

    glShaderSource(shaderId, 1, shaderCode, codeLength);
    glCompileShader(shaderId);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // �������� ������� ��������� ������� (�� ��������)
    // shaderId � �� ������� ������, � �� �������� ��������
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("������� ��������� ������� '%d': '%s'\n", type, errorLog);
        glDeleteShader(shaderId);
        return;
    }

    glAttachShader(programId, shaderId);
    // ��������� ������ ���� ��������� �� �������� (�� ����� �� �������)
    glDeleteShader(shaderId);
}

void CompileShaders(ShaderData& d) {
    GLuint shader = glCreateProgram();

    if (!shader) {
        printf("�� ������� �������� �������� ��������!\n");
        return;
    }

    AddShader(shader, d.vert, GL_VERTEX_SHADER);
    AddShader(shader, d.frag, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // ˳����� �������� � ����������� ������� � ����� ��������
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("������� ��������� �������: '%s'\n", errorLog);
        glDeleteProgram(shader);
        return;
    }

    // ����������, �� ���� �������� ��������� � ��������� ���� OpenGL
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("������� �������� �������: '%s'\n", errorLog);
        glDeleteProgram(shader);
        return;
    }

    d.shader = shader;
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

    ShaderData sd1;
    sd1.vert = vShader;
    sd1.frag = fShader;
    CompileShaders(sd1);

    DrawData dd1;
    dd1.shader = sd1.shader;
    CreateTriangle(dd1, triangle1, 9);  // 9 float = 3 ������� * 3 ����������

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(dd1.shader);
        glBindVertexArray(dd1.VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    // �������� ������� OpenGL, ��� �������� ������ ���'��
    glDeleteVertexArrays(1, &dd1.VAO);
    glDeleteBuffers(1, &dd1.VBO);
    glDeleteProgram(dd1.shader);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}
