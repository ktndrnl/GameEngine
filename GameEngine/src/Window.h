#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();
    int initialize();
    [[nodiscard]] GLint getBufferWidth() const { return bufferWidth; }
    [[nodiscard]] GLint getBufferHeight() const { return bufferHeight; }
    [[nodiscard]] GLfloat getAspectRatio() const;
    [[nodiscard]] bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }
    bool* getKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();
    void swapBuffers() const { glfwSwapBuffers(mainWindow); }
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    GLfloat aspectRatio;

    bool keys[1024];
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    void createCallbacks() const;
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
    static void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};

