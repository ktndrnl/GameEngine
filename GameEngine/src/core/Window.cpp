#include "Window.h"

#include <iostream>

Window::Window()
{
    width = 800;
    height = 600;
}

Window::Window(const GLint windowWidth, const GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    for (bool& key : keys)
    {
        key = false;
    }
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
    mouseLocked = true;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    for (bool& key : keys)
    {
        key = false;
    }
}

int Window::initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Game Engine", nullptr, nullptr);
    if (!mainWindow)
    {
        std::cout << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle key + mouse input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugMessageCallback, nullptr);

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

    glfwSetWindowSizeCallback(mainWindow, frameBufferSizeCallback);

    return 0;
}

GLfloat Window::getAspectRatio() const
{
    return static_cast<GLfloat>(bufferWidth) / static_cast<GLfloat>(bufferHeight);
}

GLfloat Window::getXChange()
{
    const GLfloat change = xChange;
    xChange = 0.0f;
    return change;
}

GLfloat Window::getYChange()
{
    const GLfloat change = yChange;
    yChange = 0.0f;
    return change;
}

void Window::toggleMouseLock()
{
    if (glfwGetInputMode(mainWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseLocked = false;
    }
    else
    {
        glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseLocked = true;
    }
}

bool Window::getShouldToggleFlashlight()
{
    if (shouldToggleFlashlight)
    {
        shouldToggleFlashlight = false;
        return true;
    }
    return false;
}

void Window::createCallbacks() const
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, const int key, int code, const int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
    {
        theWindow->toggleMouseLock();
    }

    if (key == GLFW_KEY_F && action == GLFW_RELEASE)
    {
        theWindow->shouldToggleFlashlight = true;
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, const double xPos, const double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

void GLAPIENTRY Window::debugMessageCallback(GLenum source, const GLenum type, GLuint id, const GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message << std::endl;
}

// resize callback
void Window::frameBufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}
