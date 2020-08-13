#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>


namespace rz {
	class RZAPI Log
	{
	public:
		static void init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}

		static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}


// Core Log Macros
#define RZ_CORE_TRACE(...)    ::rz::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RZ_CORE_INFO(...)     ::rz::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RZ_CORE_WARN(...)     ::rz::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RZ_CORE_ERROR(...)    ::rz::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RZ_CORE_FATAL(...)    ::rz::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Log Macros
#define RZ_TRACE(...)         ::rz::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RZ_INFO(...)          ::rz::Log::GetClientLogger()->info(__VA_ARGS__)
#define RZ_WARN(...)          ::rz::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RZ_ERROR(...)         ::rz::Log::GetClientLogger()->error(__VA_ARGS__)
#define RZ_FATAL(...)         ::rz::Log::GetClientLogger()->critical(__VA_ARGS__)


