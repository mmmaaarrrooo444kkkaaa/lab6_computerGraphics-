#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class ShaderLoader {
public:
    static GLuint LoadShader(const char* vertexPath, const char* fragmentPath);

    static GLuint LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

    static void SetUniform(GLuint shaderProgram, const std::string& name, float value);
    static void SetUniform(GLuint shaderProgram, const std::string& name, float v1, float v2);
    static void SetUniform(GLuint shaderProgram, const std::string& name, float v1, float v2, float v3);
    static void SetUniform(GLuint shaderProgram, const std::string& name, float v1, float v2, float v3, float v4);

    static void SetUniform(GLuint shaderProgram, const std::string& name, int value);
    static void SetUniform(GLuint shaderProgram, const std::string& name, int v1, int v2);
    static void SetUniform(GLuint shaderProgram, const std::string& name, int v1, int v2, int v3);
    static void SetUniform(GLuint shaderProgram, const std::string& name, bool value);

    static void SetUniformMatrix4(GLuint shaderProgram, const std::string& name, const glm::mat4& matrix);
    static void SetUniformMatrix3(GLuint shaderProgram, const std::string& name, const glm::mat3& matrix);

    static void SetUniform(GLuint shaderProgram, const std::string& name, const glm::vec2& value);
    static void SetUniform(GLuint shaderProgram, const std::string& name, const glm::vec3& value);
    static void SetUniform(GLuint shaderProgram, const std::string& name, const glm::vec4& value);

private:
    static std::string ReadFile(const char* filePath);
    static void CheckCompileErrors(GLuint shader, const std::string& type);
    static GLuint CompileShader(const char* shaderCode, GLenum shaderType);
};

#endif