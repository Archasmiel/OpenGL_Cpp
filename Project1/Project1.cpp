#include <stdio.h>
#include <GL/glew.h>      
#include <GLFW/glfw3.h>

// ������ ����
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    // ����������� GLFW
    if (!glfwInit()) {
        printf("GLFW init failed");
        glfwTerminate();
        return 1;
    }

    /// ��������� ����
    // ������� ������� MAJOR ���� 3.X �� "����" ������� MINOR �.3, ����� 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // ���� ����� ������ ����������� (deprecated) �� ��������� ��� ��������� ����� �����
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // ��������� ���� (��������� �� ����� - ������ *)
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed");
    }

    // �������� ������ ������ ����� � ����
    // ��� ����������� ������������ ������ ��������� (���� ���)
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
        // &bufferWidth - �������� �������� ����� ������� � �������������� (reference)
        // ���� �� ������ '&', ���� ������ ���� � ����� ��� ��

    // ������� ���� ��������
    glfwMakeContextCurrent(mainWindow);

    // ��������� ���������������(���) ���������� ����
    glewExperimental = GL_TRUE;

    // �������� �� �������� ���� GLEW ����������, �� ���� Bool
    if (glewInit() != GLEW_OK) {
        printf("GLEW init failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }

    // ��� ����������� ������������ ������ ���������
    glViewport(0, 0, bufferWidth, bufferHeight);

    // �������� ���� ����
    while (!glfwWindowShouldClose(mainWindow)) {
        // �������� �� �������� ����������� (�����, ���������)
        glfwPollEvents();

        // ������� (������������ ������ ������) ����� ��������
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);   // R G B Alpha  [0f, 1f]  float
        glClear(GL_COLOR_BUFFER_BIT);  // ���������� ��������
            /*
               GL_COLOR_BUFFER_BIT - ����� ����� �������, ��������� ��������� ����
               GL_DEPTH_BUFFER_BIT - ����� ����� �������, 
                ���������� ������������ ������� ����������
               GL_STENCIL_BUFFER_BIT - ����� �����-�������� (�����), ���� 
                ��������������� ��� ���������� (�������) ������ �������� ����������
            */

        glfwSwapBuffers(mainWindow);  // ������� ���� � ��� �� ����� � ����� ��������
    }

    return 0;
}