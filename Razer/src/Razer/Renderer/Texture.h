#pragma once

#include "Razer/Core.h"
#include <string>
namespace rz {
	class RZAPI Texture {
	public:
		struct Image {
			int Width;
			int Height;
			int Channels;
			unsigned char* data = nullptr;
		};

		virtual ~Texture() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Texture* Create(const std::string& file, bool transparent= false);
	};
}