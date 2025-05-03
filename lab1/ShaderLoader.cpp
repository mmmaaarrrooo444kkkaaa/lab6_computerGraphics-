#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

std::string ShaderLoader::ReadFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

void ShaderLoader::CheckCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}

GLuint ShaderLoader::CompileShader(const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    CheckCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" :
        shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY");
    return shader;
}

GLuint ShaderLoader::LoadShader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        return 0;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex = CompileShader(vShaderCode, GL_VERTEX_SHADER);
    GLuint fragment = CompileShader(fShaderCode, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    CheckCompileErrors(program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

GLuint ShaderLoader::LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);
    std::string geometryCode = ReadFile(geometryPath);

    if (vertexCode.empty() || fragmentCode.empty() || geometryCode.empty()) {
        return 0;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    GLuint vertex = CompileShader(vShaderCode, GL_VERTEX_SHADER);
    GLuint fragment = CompileShader(fShaderCode, GL_FRAGMENT_SHADER);
    GLuint geometry = CompileShader(gShaderCode, GL_GEOMETRY_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glAttachShader(program, geometry);
    glLinkProgram(program);
    CheckCompileErrors(program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);

    return program;
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, float value) {
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, float v1, float v2) {
    glUseProgram(shaderProgram);
    glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, float v1, float v2, float v3) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2, v3);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, float v1, float v2, float v3, float v4) {
    glUseProgram(shaderProgram);
    glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2, v3, v4);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, int value) {
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, int v1, int v2) {
    glUseProgram(shaderProgram);
    glUniform2i(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, int v1, int v2, int v3) {
    glUseProgram(shaderProgram);
    glUniform3i(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2, v3);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, bool value) {
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void ShaderLoader::SetUniformMatrix4(GLuint shaderProgram, const std::string& name, const glm::mat4& matrix) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderLoader::SetUniformMatrix3(GLuint shaderProgram, const std::string& name, const glm::mat3& matrix) {
    glUseProgram(shaderProgram);
    glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, const glm::vec2& value) {
    glUseProgram(shaderProgram);
    glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, const glm::vec3& value) {
    glUseProgram(shaderProgram);
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}

void ShaderLoader::SetUniform(GLuint shaderProgram, const std::string& name, const glm::vec4& value) {
    glUseProgram(shaderProgram);
    glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
}