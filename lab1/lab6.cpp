#define GLEW_DLL
#define GLFW_DLL

#include "ShaderLoader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstdio>
#include <cmath>
#include "Mesh.h"
#include "Model.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void settingMat4(GLuint shaderID, const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void processInput(GLFWwindow* window) {
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Model Viewer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR: Failed to initialize GLEW.\n");
        return 1;
    }

    GLuint shaderProgram = ShaderLoader::LoadShader("vertex_shader.glsl", "fragment_shader.glsl");

    Model ourModel("assets/model/model.obj");

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5f));

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        glUseProgram(shaderProgram);

        settingMat4(shaderProgram, "projection", projection);
        settingMat4(shaderProgram, "view", view);
        settingMat4(shaderProgram, "model", model);
        ShaderLoader::SetUniformMatrix3(shaderProgram, "normalMatrix", normalMatrix);


        ShaderLoader::SetUniform(shaderProgram, "light.position", lightPos.x, lightPos.y, lightPos.z);
        ShaderLoader::SetUniform(shaderProgram, "light.ambient", 0.2f, 0.2f, 0.2f);
        ShaderLoader::SetUniform(shaderProgram, "light.diffuse", 0.5f, 0.5f, 0.5f);
        ShaderLoader::SetUniform(shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);

        glm::vec3 metalColor = glm::vec3(0.4f, 0.5f, 1.0f);

        ShaderLoader::SetUniform(shaderProgram, "material.ambient",
            0.1f * metalColor.r,
            0.1f * metalColor.g,
            0.1f * metalColor.b);

        ShaderLoader::SetUniform(shaderProgram, "material.diffuse",
            0.5f * metalColor.r,
            0.5f * metalColor.g,
            0.5f * metalColor.b);

        ShaderLoader::SetUniform(shaderProgram, "material.specular",
            metalColor.r,
            metalColor.g,
            metalColor.b);

        ShaderLoader::SetUniform(shaderProgram, "material.shininess", 64.0f);

        ShaderLoader::SetUniform(shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

        ourModel.Draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}