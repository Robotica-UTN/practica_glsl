#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <thread>
//Including the triangle data (shaders and vertices)
#include "resources/triangle.cpp"
#include "resources/rectangle.cpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void renderLoop(GLFWwindow *window, unsigned int VAOS[2]) {
  const int VAOS_LENGHT = 2;
  //Logic to render each VAO (form) each duration_in_seconds whitout blocking processInput()
  auto duration_in_seconds = std::chrono::duration<double>(1);
  auto start = std::chrono::system_clock::now() - duration_in_seconds * 2;
  int step = 0;
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glfwPollEvents();
    if ((std::chrono::system_clock::now() - start) > duration_in_seconds) {
      start = std::chrono::system_clock::now();
      step++;
      //Restart step if reached end of array
      if (step == VAOS_LENGHT) {
        step = 0;
      }
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //Each duration_in_seconds one of this forms is drawn
    if (step == 0) {
      glBindVertexArray(VAOS[0]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    } else {
      glBindVertexArray(VAOS[1]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    //Render what was drawn in the back buffer
    glfwSwapBuffers(window);
  }
}

int main() {
  //Initialization stuff
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  //This creates a resizable window
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //Create the vertex shader and verify its compilation
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  //Create the fragment shader and verify its compilation
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  //Create the shader program from the previous shaders and verify its compilation
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  //We can delete the shaders after creating the program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  //Set the buffers
  unsigned int VBO_TRIANGLE;
  glGenBuffers(1, &VBO_TRIANGLE);
  unsigned int VBO_RECTANGLE;
  glGenBuffers(1, &VBO_RECTANGLE);
  unsigned int VAO_TRIANGLE;
  glGenVertexArrays(1, &VAO_TRIANGLE);
  //This is for the rectangle
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  //Create new VAO for the rectangle
  unsigned int VAO_RECTANGLE;
  glGenVertexArrays(1, &VAO_RECTANGLE);
  //Bind the VAO to store following operations for the triangle
  glBindVertexArray(VAO_TRIANGLE);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TRIANGLE);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  //Bind the VAO of the rectangle
  glBindVertexArray(VAO_RECTANGLE);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_RECTANGLE);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  //Stop saving operations to a VAO
  glBindVertexArray(NULL);
  //This operation isnt saved in the VAO and we have only one program so this doesnt need to go in the render loop
  glUseProgram(shaderProgram);
  unsigned int VAOS[] = {VAO_TRIANGLE, VAO_RECTANGLE};
  //Render the program
  renderLoop(window, VAOS);

  glfwTerminate();
  return 0;
}
