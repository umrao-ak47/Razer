#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Buffer.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace rz{
	struct RZAPI UniformElement {
		std::string Name;
		ShaderDataType Type;
		// unsigned int Size;
		// unsigned int Count;

		UniformElement()
			: Name("None"), Type(ShaderDataType::None) {}

		UniformElement(ShaderDataType type, const std::string& name)
			: Name(name), Type(type) {}
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
		friend class ShaderManager;
	public:
		virtual ~Shader() {}

		virtual const std::string& GetName() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniform(const std::string& name, int value) = 0;
		virtual void UploadUniform(const std::string& name, float value) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadUniform(const std::string& name,const glm::vec4& value) = 0;
		virtual void UploadUniform(const std::string& name, const glm::mat4& value) = 0;

		virtual const Ref<BufferLayout>& ExtractLayout() const = 0;
		virtual const Ref<UniformLayout>& ExtractUniformLayout() const = 0;
	private:
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
	};


	class RZAPI ShaderManager {
	private:
		ShaderManager() {}
	public:
		~ShaderManager() {}

		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);

		Ref<Shader> Get(const std::string& name) const;
		bool Exists(const std::string& name) const { return m_Shaders.find(name) != m_Shaders.end(); }
		
		static Ref<ShaderManager> GetManager() {
			if (s_Instance == nullptr) {
				s_Instance = Ref<ShaderManager>(new ShaderManager);
			}
			return s_Instance;
		}
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		static Ref<ShaderManager> s_Instance;
	};
}