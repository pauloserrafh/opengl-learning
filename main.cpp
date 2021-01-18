#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include <iostream>

GLuint compileShader(GLuint shaderType, const std::string& source) {
    GLuint shader;
    GLint params;
    const char* src;
    char* tmp;

    std::cout << "Compiling" <<
    (shaderType == GL_VERTEX_SHADER ? " vertex " : " fragment ") <<
    "shader " << std::endl;

    shader = glCreateShader(shaderType);

    src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    // params will contain the status of the compilation
    if (params == GL_FALSE) {
        // params will contain the length of the log message
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &params);

        tmp = (char*) malloc(params*sizeof(char));
        glGetShaderInfoLog(shader, params, &params, tmp);
        std::cout << "Error compiling shader: " << tmp << std::endl;

        free(tmp);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram() {
    GLuint program;
    GLuint vs, fs;

    auto vertexSource =
    #include "res/vertex.shader"
    ;

    // std::cout << "Vertex: " << vertexSource << std::endl;

    auto fragmentSource =
    #include "res/fragment.shader"
    ;

    program = glCreateProgram();

    // Create and attach shaders to program
    vs = compileShader(GL_VERTEX_SHADER, vertexSource);
    glAttachShader(program, vs);

    fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    // Clean-up
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void createTriangle() {
    uint8_t points_size, indexes_size;
    GLuint buffer, ibo;

    /* Define the points to create a triangle */
    float points[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };
    /* Calculates the size of the array dinamically */
     points_size = *(&points+1) - points;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, points_size*sizeof(float), points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    GLuint indexes[] = {
        0, 1, 2,
        2, 3, 0
    };
    indexes_size = *(&indexes+1) - indexes;

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_size*sizeof(GLuint), indexes, GL_STATIC_DRAW);

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        std::cout << "Error on glfwInit()" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Error on glfwCreateWindow()" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error on glewInit" << std::endl;
    }
    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;


    createTriangle();
    GLuint program = createProgram();
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw the binded triangle on the middle of the screen */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}
