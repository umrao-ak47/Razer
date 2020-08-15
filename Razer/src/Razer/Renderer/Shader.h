#pragma once

#include "Razer/Core.h"
#include <string>

namespace rz{
	class RZAPI Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertSrc, const std::string& fragSrc);
	};
}