#pragma once

#include "Razer/Renderer/Shader.h"
#include <glad/glad.h>

namespace rz {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void UploadUniform(const std::string& name, int value) override;
		void UploadUniform(const std::string& name, float value) override;
		void UploadUniform(const std::string& name, const glm::vec3& value) override;
		void UploadUniform(const std::string& name, const glm::vec4& value) override;
		void UploadUniform(const std::string& name, const glm::mat4& value) override;
		
		const Ref<BufferLayout>& ExtractLayout() const override { return m_Layout; }
		const Ref<UniformLayout>& ExtractUniformLayout() const override { return m_UniformLayout; }
	private:
		std::string ReadFile(const std::string& filepath);
		std::vector<std::pair<GLuint, std::string>> ProcessSourceData(const std::string& source);

		void Compile(const std::vector<std::pair<GLuint, std::string>>& sources);

		void ComputeLayout();
		void ComputeUniformLayout();

		int ValidateUnform(const std::string& name, ShaderDataType type);
	private:
		unsigned int m_RendererID;
		Ref<UniformLayout> m_UniformLayout;
		Ref<BufferLayout> m_Layout;
	};
}