#include <stdio.h>
#include <GL/glew.h>      // ϳ��������� GLEW ��� ��������� ������������ OpenGL
#include <GLFW/glfw3.h>   // ϳ��������� GLFW ��� ��������� ���� �� ������� �����

const GLint WIDTH = 800;   // ������ ����
const GLint HEIGHT = 600;  // ������ ����

int main()
{
    // ����������� GLFW
    if (glfwInit() == GL_FALSE) { // ��������, �� ������� ������������ GLFW
        printf("GLFW init fail");
        glfwTerminate();
        return 1;
    }

    /// ������������ GLFW
    // ���������� ���� OpenGL (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // ������ ����� (major)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // �������� ����� (minor)

    // ������������ ����� ��������� (core) ������� OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������� ������������ �������� ����� OpenGL (��� macOS)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // ��������� ���� GLFW (��������� �� ����� ���������� � ����� window)
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (!window) {  // ��������, �� ������� �������� ����
        printf("Window creation fail");
        glfwTerminate();
        return 1;
    }

    // ��������� ���������� ������ ������ ����� (framebuffer) � �������
    int bufWidth, bufHeight;
    glfwGetFramebufferSize(window, &bufWidth, &bufHeight);

    // ������������ ��������� OpenGL ��� ���������� ����
    glfwMakeContextCurrent(window);

    // ����� �� ������������ ����������������� ������� GLEW
    glewExperimental = GL_TRUE;

    /// ����������� GLEW
    if (glewInit() != GLEW_OK) {  // ��������, �� ������� ������������ GLEW
        printf("GLEW not init");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // ������������ ������ ��������� (viewport) �� ���� ����� ������ �����
    glViewport(0, 0, bufWidth, bufHeight);

    //
    while (!glfwWindowShouldClose(window)) {  // �������� ���� ��������
        // ��������� ���� (���������, ���� ����)
        glfwPollEvents();

        // �������� ������ �������� (GLclampf - �������� � ������� [0,1])
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // ��������, �������, ����, ��������� (RGBA)
        glClear(GL_COLOR_BUFFER_BIT);

        /*
            GL_COLOR_BUFFER_BIT - ����� ����� ������� (������� ��������� ����)
            GL_DEPTH_BUFFER_BIT - ����� ����� ������� (��� ���������� ����������� 3D-��'����)
            GL_STENCIL_BUFFER_BIT - ����� ����� ��������� (��������������� ��� ���������� ��������)
        */

        // ����� ���������� ������ (front) �� ����� (back), ��� ���������� ����� ����
        glfwSwapBuffers(window);
    }

    // �������� ���� �� ���������� ������ GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
