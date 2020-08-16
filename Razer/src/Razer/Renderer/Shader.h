#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Buffer.h"

#include <string>
#include <glm/glm.hpp>


namespace rz{
	struct RZAPI UniformElement {
		std::string Name;
		ShaderDataType Type;
		unsigned int Size;
		//unsigned int Count;

		UniformElement()
			: Name("None"), Type(ShaderDataType::None), Size(0) {}

		UniformElement(ShaderDataType type, const std::string& name)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)) {}
	};

	class RZAPI UniformLayout {
		using UniformList = std::vector<UniformElement>;
	public:
		UniformLayout() {}

		UniformLayout(const std::initializer_list<UniformElement>& elements)
			: m_Elements(elements) {}

		UniformLayout(const UniformList& elements)
			: m_Elements(elements) {}

		const UniformList& GetElements() const { return m_Elements; }

		UniformList::iterator begin() { return m_Elements.begin(); }
		UniformList::iterator end() { return m_Elements.end(); }
		UniformList::const_iterator begin() const { return m_Elements.begin(); }
		UniformList::const_iterator end() const { return m_Elements.end(); }
	private:
		UniformList m_Elements;
	};


	class RZAPI Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniform(const std::string& name, int value) = 0;
		virtual void UploadUniform(const std::string& name, float value) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadUniform(const std::string& name,const glm::vec4& value) = 0;

		virtual const std::shared_ptr<BufferLayout>& ExtractLayout() const = 0;
		virtual const std::shared_ptr<UniformLayout>& ExtractUniformLayout() const = 0;

		static Shader* Create(const std::string& vertSrc, const std::string& fragSrc);
	};
}