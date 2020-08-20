#include "RZPCH.h"
#include "OpenGLShader.h"

#include "Razer/Core/Log.h"
#include <fstream>
#include <sstream>

namespace rz {
	static ShaderDataType OpenGLEnumToShaderDataType(GLenum type) {
		switch(type) {
			case GL_NONE:                  return ShaderDataType::None;
			case GL_BOOL:                  return ShaderDataType::BOOL;
			case GL_UNSIGNED_INT:          return ShaderDataType::UINT;
			case GL_UNSIGNED_INT_VEC2:     return ShaderDataType::UINT2;
			case GL_UNSIGNED_INT_VEC3:     return ShaderDataType::UINT3;
			case GL_UNSIGNED_INT_VEC4:     return ShaderDataType::UINT4;
			case GL_INT:                   return ShaderDataType::INT;
			case GL_INT_VEC2:              return ShaderDataType::INT2;
			case GL_INT_VEC3:              return ShaderDataType::INT3;
			case GL_INT_VEC4:              return ShaderDataType::INT4;
			case GL_FLOAT:                 return ShaderDataType::FLOAT;
			case GL_FLOAT_VEC2:            return ShaderDataType::FLOAT2;
			case GL_FLOAT_VEC3:            return ShaderDataType::FLOAT3;
			case GL_FLOAT_VEC4:            return ShaderDataType::FLOAT4;
			case GL_FLOAT_MAT2:            return ShaderDataType::MAT2;
			case GL_FLOAT_MAT3:            return ShaderDataType::MAT3;
			case GL_FLOAT_MAT4:            return ShaderDataType::MAT4;
			case GL_SAMPLER_2D:            return ShaderDataType::SAMPLER2D;
		}
		RZ_CORE_ASSERT(false, "This GLenum Type not supported");
		return ShaderDataType::None;
	}

	static GLuint StringToOpenGLShaderType(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		else if (type == "fragment") return GL_FRAGMENT_SHADER;

		RZ_CORE_ASSERT(false, "Unknown Shader Type");
		return GL_NONE;
	}

	static inline std::string& Trim(std::string& str) {
		const std::string ws = " \t\n\r\f\v";
		// right trim
		str.erase(str.find_last_not_of(ws) + 1);
		// left trim
		str.erase(0, str.find_first_not_of(ws));
		RZ_CORE_INFO("{0}", str);
		return str;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string shaderSource = ReadFile(filepath);
		Compile(ProcessSourceData(shaderSource));
		// Extract name from file name
		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
		size_t lastDot = filepath.rfind(".");
		size_t count = (lastDot == std::string::npos) ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
		RZ_CORE_TRACE(m_Name);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
		: m_Name{name} {
		std::string shaderSource = ReadFile(filepath);
		Compile(ProcessSourceData(shaderSource));
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
		: m_Name{name} {
		std::vector<std::pair<GLuint, std::string>> shaderSources;
		shaderSources.push_back(std::make_pair(GL_VERTEX_SHADER, vertSrc));
		shaderSources.push_back(std::make_pair(GL_FRAGMENT_SHADER, fragSrc));
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			RZ_CORE_ERROR("Could not open the file: '{0}'", filepath);
		}

		return result;
	}

	std::vector<std::pair<GLuint, std::string>> OpenGLShader::ProcessSourceData(const std::string& source) {
		std::vector<std::pair<GLuint, std::string>> shaderSources;

		const std::string token = "#type";
		size_t tokenLength = token.size();
		size_t pos = source.find(token, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + tokenLength;
			GLuint type = StringToOpenGLShaderType(Trim(source.substr(begin, eol - begin)));

			size_t nextLinePos = source.find_first_not_of(" \r\n", eol);
			pos = source.find(token, nextLinePos);
			shaderSources.push_back(std::make_pair(type, source.substr(nextLinePos, pos - nextLinePos)));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::vector<std::pair<GLuint, std::string>>& sources) {
		std::vector<GLuint> shaders;

		for (const auto& [type, source] : sources) {
			// Create an empty shader handle
			GLuint shader = glCreateShader(type);
			const GLchar* src = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &src, 0);
			// Compile the shader
			glCompileShader(shader);

			// push into array
			shaders.push_back(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				for (const auto& shader : shaders) {
					glDeleteShader(shader);
				}
				
				// Use the infoLog
				RZ_CORE_ERROR("{0}", infoLog.data());
				RZ_CORE_ASSERT(false, "Shader:: Compilation Failed");
				return;
			}
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		for (const auto& shader : shaders) {
			glAttachShader(m_RendererID, shader);
		}

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			for (const auto& shader : shaders) {
				glDeleteShader(shader);
			}
			
			// Use the infoLog as you see fit.
			RZ_CORE_ERROR("{0}", infoLog.data());
			RZ_CORE_ASSERT(false, "Shader:: Program: Link Failed")
				return;
		}

		// Always detach shaders after a successful link.
		for (const auto& shader : shaders) {
			glDeleteShader(shader);
		}

		// Compute Layout of Shader
		ComputeLayout();
		ComputeUniformLayout();
	}

	int OpenGLShader::ValidateUnform(const std::string& name, ShaderDataType type) {
		int index = 0;
		const auto& elements = m_UniformLayout->GetElements();
		for (const auto& element : elements) {
			if (element.Name == name) {
				break;
			}
			index++;
		}
		// not found 
		if (index == elements.size()) {
			RZ_CORE_WARN("No Unifrom varibale is Named: {0}", name);
			return -1;
		}
		// data type not matches
		if (elements[index].Type != type) {
			RZ_CORE_ERROR("Data Type of Unifrom varibale {0} does not match", name);
			return -1;
		}
		return index;
	}

	void OpenGLShader::UploadUniform(const std::string& name, int value) {
		int index = ValidateUnform(name, ShaderDataType::INT);
		if (index == -1) return;
		glUniform1i(index, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, float value) {
		int index = ValidateUnform(name, ShaderDataType::FLOAT);
		if (index == -1) return;
		glUniform1f(index, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec3& value) {
		int index = ValidateUnform(name, ShaderDataType::FLOAT3);
		if (index == -1) return;
		glUniform3f(index, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& value) {
		int index = ValidateUnform(name, ShaderDataType::FLOAT4);
		if (index == -1) return;
		glUniform4f(index, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& value) {
		int index = ValidateUnform(name, ShaderDataType::MAT4);
		if (index == -1) return;
		glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::ComputeLayout() {
		std::vector<BufferElement> elements;

		GLint count = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &count);
		RZ_CORE_WARN("Shader:: Number of Attributes: {0}", count);

		GLint maxSize = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxSize);

		GLenum type;
		GLint size;
		GLsizei length;

		std::vector<GLchar> name(maxSize);

		for (GLint i = 0; i < count; i++) {
			glGetActiveAttrib(m_RendererID, (GLuint)i, maxSize, &length, &size, &type, &name[0]);
			RZ_CORE_TRACE("Attrribute: {0} Type: {1} Name: {2} Size: {3} Length: {4} MaxSize: {5}", i, type, name.data(), size, length, maxSize);

			BufferElement element(OpenGLEnumToShaderDataType(type), std::string(name.data()));
			elements.push_back(element);
		}

		m_Layout = Ref<BufferLayout>(new BufferLayout(elements));
	}

	void OpenGLShader::ComputeUniformLayout() {
		std::vector<UniformElement> elements;

		GLint count = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &count);
		RZ_CORE_WARN("Shader:: Number of Uniform: {0}", count);

		GLint maxSize = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxSize);

		GLenum type;
		GLint size;
		GLsizei length;

		std::vector<GLchar> name(maxSize);

		for (GLint i = 0; i < count; i++) {
			glGetActiveUniform(m_RendererID, (GLuint)i, maxSize, &length, &size, &type, &name[0]);
			RZ_CORE_TRACE("Attrribute: {0} Type: {1} Name: {2} Size: {3} Length: {4} MaxSize: {5}", i, type, name.data(), size, length, maxSize);

			UniformElement element(OpenGLEnumToShaderDataType(type), std::string(name.data()));
			elements.push_back(element);
		}

		m_UniformLayout = Ref<UniformLayout>(new UniformLayout(elements));
	}
}

