#define GLFW_INCLUDE_NONE // prevent the GLFW header from including the OpenGL header and allow headers in any order

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

bool compilationSuccess(unsigned int shaderOrProgram, GLenum statusEnum);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

constexpr unsigned int SCREEN_WIDTH  = 1280;
constexpr unsigned int SCREEN_HEIGHT = 720;

// Defines how to process each vertex
// Right now, does basically no processing and just passes on the vertex data
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// Defines what color the output pixels are
// Right now, just make the output pixel an orange-y color
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";



int main() {
    std::cout << "Hello, OpenGL!\n";

    // GLFW Init and Config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Modern OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load Glad - OpenGL Function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Vertex Input - 3 points of a triangle
    float vertices[] = {
            -0.5F, -0.5F, 0.0F, //
            0.5F,  -0.5F, 0.0F, //
            0.0F,  0.5F,  0.0F  //
    };

    // Store Vertex configurations
    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);

    // OpenGL doesn't yet know how to interpret the vertex data, so we must define that
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

    // Store the vertices on the memory for the GPU for the graphics pipeline
    unsigned int vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create Vertex Shader then attach the shader source code to the shader
    // object and compile the shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    if (!compilationSuccess(vertexShader, GL_COMPILE_STATUS)) { return -1; }

    // Fragment Shader - similar to the vertex shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    if (!compilationSuccess(fragmentShader, GL_COMPILE_STATUS)) { return -1; }

    // The Shader Program object is the final linked version of multiple shaders combined. To use the recently compiled
    // shaders we have to link them to a shader program object and then activate this shader program when rendering
    // objects. The activated shader program's shaders will be used when we issue render calls.
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!compilationSuccess(shaderProgram, GL_LINK_STATUS)) { return -1; }
    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObject);

    // Now that the shader program is created, we no longer need the raw shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);


    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Input handle
        processInput(window);

        // Render commands
        glClearColor(1.0F, 0.25F, 1.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and check for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/**
 * Query GLFW whether relevant keys are pressed/released this frame and react
 * accordingly.
 */
void framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

/**
 * Executes whenever the window size changed.
 */
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

/**
 * Checks if the shader compiled successfully.
 */
bool compilationSuccess(unsigned int shaderOrProgram, GLenum statusEnum) {
    int  success;
    char infoLog[512];

    switch (statusEnum) {
        case GL_LINK_STATUS:
            glGetProgramiv(shaderOrProgram, GL_LINK_STATUS, &success);
            if (success) { return true; }
            glGetProgramInfoLog(shaderOrProgram, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
            return false;
        case GL_COMPILE_STATUS:
            glGetShaderiv(shaderOrProgram, statusEnum, &success);
            if (success) { return true; }
            glGetShaderInfoLog(shaderOrProgram, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
            return false;
        default:
            std::cout << "Unknown status type.\n";
            return false;
    }
}