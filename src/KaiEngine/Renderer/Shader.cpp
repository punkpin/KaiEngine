#include "Shader.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include "Logger.h"

namespace KaiEngine {

    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        m_RendererID = 0;

        std::string vCode, fCode;
        std::ifstream vFile(vertexPath), fFile(fragmentPath);

        if (vFile) {
            std::stringstream vStream;
            vStream << vFile.rdbuf();
            vCode = vStream.str();
        } else {
            KAI_CORE_ERROR("Shader: 无法打开顶点着色器文件: {0}", vertexPath);
        }

        if (fFile) {
            std::stringstream fStream;
            fStream << fFile.rdbuf();
            fCode = fStream.str();
        } else {
            KAI_CORE_ERROR("Shader: 无法打开片段着色器文件: {0}", fragmentPath);
        }

        if (vCode.empty() || fCode.empty()) {
            KAI_CORE_ERROR("Shader: 源码为空，取消创建 Program");
            return;
        }

        const char* vShaderCode = vCode.c_str();
        const char* fShaderCode = fCode.c_str();

        uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        int isCompiled = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

            std::string infoLog;
            infoLog.resize((size_t)maxLength);
            glGetShaderInfoLog(vertex, maxLength, &maxLength, infoLog.data());

            KAI_CORE_ERROR("Shader: 顶点着色器编译失败: {0}", vertexPath);
            KAI_CORE_ERROR("{0}", infoLog);
            glDeleteShader(vertex);
            return;
        }

        uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

            std::string infoLog;
            infoLog.resize((size_t)maxLength);
            glGetShaderInfoLog(fragment, maxLength, &maxLength, infoLog.data());

            KAI_CORE_ERROR("Shader: 片段着色器编译失败: {0}", fragmentPath);
            KAI_CORE_ERROR("{0}", infoLog);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertex);
        glAttachShader(m_RendererID, fragment);
        glLinkProgram(m_RendererID);

        int isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {
            int maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            std::string infoLog;
            infoLog.resize((size_t)maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());

            KAI_CORE_ERROR("Shader: Program 链接失败");
            KAI_CORE_ERROR("{0}", infoLog);

            glDeleteProgram(m_RendererID);
            m_RendererID = 0;
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        KAI_CORE_INFO("Shader: 成功编译并链接程序 [ID: {0}]", m_RendererID);
    }

    Shader::~Shader() {
        if (m_RendererID) {
            glDeleteProgram(m_RendererID);
        }
    }
    void Shader::Bind() const {
        if (m_RendererID) {
            glUseProgram(m_RendererID);
        }
    }
    void Shader::Unbind() const { glUseProgram(0); }

    void Shader::SetInt(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }

    // 新增：传递数组给 Shader
    void Shader::SetIntArray(const std::string& name, int* values, uint32_t count) {
        glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, values);
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}