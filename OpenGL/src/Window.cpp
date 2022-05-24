#include <Window.h>

Window::Window() {
    windowTitle = "Test Window";
	width = 800;
	height = 600;
    bufferWidth = 0;
    bufferHeight = 0;
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

Window::Window(int windowWidth, int windowHeight) {
    windowTitle = "Test Window";
	width = windowWidth;
	height = windowHeight;
    bufferWidth = 0;
    bufferHeight = 0;
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}
Window::Window(int windowWidth, int windowHeight, std::string title) {
    windowTitle = title;
    width = windowWidth;
    height = windowHeight;
    bufferWidth = 0;
    bufferHeight = 0;
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

int Window::initialize() {
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "GLFW failed to initialize." << std::endl;
        return -1;
    }

    //Setup GLFW window properties
    //Set OpenGL version to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Enable debugging in the current context
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    mainWindow = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        return -1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    /* Make the window's context current */
    glfwMakeContextCurrent(mainWindow);

    //Handle key and mouse input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW NOT OKAY. UNGK ANGRY! <{ 0 <}" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

float Window::getXChange() {
    float curChange = xChange;
    xChange = 0.0f;
    return curChange;
}

float Window::getYChange() {
    float curChange = yChange;
    yChange = 0.0f;
    return curChange;
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            windowPtr->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            windowPtr->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (windowPtr->mouseFirstMoved) {
        windowPtr->lastX = xPos;
        windowPtr->lastY = yPos;
        windowPtr->mouseFirstMoved = false;
    }
    windowPtr->xChange = xPos - windowPtr->lastX;
    windowPtr->yChange = windowPtr->lastY - yPos;

    windowPtr->lastX = xPos;
    windowPtr->lastY = yPos;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
