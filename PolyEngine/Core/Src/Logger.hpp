#pragma once

#include "Defines.hpp"
#include "EnumUtils.hpp"
#include "OutputStream.hpp"
#include <streambuf>

namespace Poly 
{
	/**
	*  Enum describing possible levels of logging
	*/
	enum class eLogLevel { LVL_DEBUG, LVL_INFO, LVL_WARNING, LVL_ERROR, _COUNT };
	REGISTER_ENUM_NAMES_IN_POLY(eLogLevel, "DEBUG", "INFO", "WARNING", "ERROR");

	constexpr eLogLevel LOG_LEVEL_FILTER = eLogLevel::LVL_DEBUG;

	/**
	*  Corner case logging function when there are no arguments to log.
	*
	*  @param[in] stream Output stream.
	*  @param[in] level Level of logging.
	*  @param[in] fmt Format string.
	*/
	inline void sprint(std::ostream& stream, eLogLevel level,
		const std::string& fmt) {
		(void)level;  // suppress unused warning
		stream << fmt.c_str() << std::endl;
	}
	

	/**
	*  Variadic template type safe logging implementation
	*
	*  @param[in] stream Output stream.
	*  @param[in] level Level of logging.
	*  @param[in] fmt Format string.
	*  @param[in] head Next (head) argument from variadic template argument list.
	*  @param[in] tail Rest of the variadic template arguments.
	*/
	template <typename T, typename... Args>
	void sprint(std::ostream& stream, eLogLevel level, const std::string& fmt,
		const T& head, Args&&... tail) {
		constexpr char marker[] = "{}";
		std::string::size_type format_marker_pos = fmt.find(marker);
		stream << fmt.substr(0, format_marker_pos);
		if (format_marker_pos != fmt.npos) {
			stream << head;
			sprint(stream, level, fmt.substr(format_marker_pos + sizeof(marker) - 1),
				tail...);
		}
		else
			stream << std::endl;
	}

	class CORE_DLLEXPORT Console : public BaseObject<> 
	{
	public:
		Console() : Ostream(new std::ostream(std::cout.rdbuf())) {}

		template <typename S, typename... Args>
		void RegisterStream(Args&&... args)
		{
			constexpr bool isStream = std::is_base_of<OutputStream, S>::value; // Strange workaround to STATIC_ASSERTE macro on MSVC
			STATIC_ASSERTE(isStream, "Provided value is not stream!");
			if (CurrentStream)
				CurrentStream->OnUnregister();
			CurrentStream = std::make_unique<S>(std::forward<Args>(args)...);
			Ostream = std::make_unique<std::ostream>(CurrentStream.get());
		}
		
		void RegisterDefaultStream()
		{
			if (CurrentStream)
				CurrentStream->OnUnregister();
			CurrentStream = nullptr;
			Ostream = std::make_unique<std::ostream>(std::cout.rdbuf());
		}
		
		/**
		*  Set of methods for easy logging. Only those should be used in engine code.
		*  Future compatibility is guaranteed
		*
		*  Usage:
		*  - First argument contains main message to log.
		*  - Other arguments are optional, each provided optional argument must
		* override stream << operator.
		*  - For each optional argument {} marker should be placed in main message
		* string similarly to C-style printf.
		*  - Markers that do not have coresponding arguments will be treated as normal
		* string.
		*  - Arguments that do not have coresponding markers will be ignored.
		*/
		template <typename... Args>
		void Log(eLogLevel lvl, const std::string& fmt, Args&&... args) { LogImpl(lvl, GetEnumName(lvl), fmt, std::forward<Args>(args)...); }

		template <typename... Args>
		void LogDebug(const std::string& fmt, Args&&... args) { Log(eLogLevel::LVL_DEBUG, fmt, std::forward<Args>(args)...); }
		template <typename... Args>
		void LogInfo(const std::string& fmt, Args&&... args) { Log(eLogLevel::LVL_INFO, fmt, std::forward<Args>(args)...); }
		template <typename... Args>
		void LogWarning(const std::string& fmt, Args&&... args) { Log(eLogLevel::LVL_WARNING, fmt, std::forward<Args>(args)...); }
		template <typename... Args>
		void LogError(const std::string& fmt, Args&&... args) { Log(eLogLevel::LVL_ERROR, fmt, std::forward<Args>(args)...); }

	private:

		/**
		*  Main logging function. Future compatibility is not guaranteed. DO NOT USE
		* IT!
		*
		*  @param[in] level Level of logging.
		*  @param[in] levelStr String with logging level name.
		*  @param[in] fmt Format string.
		*  @param[in] args Variadic template argument list.
		*/
		template <typename... Args>
		void LogImpl(eLogLevel level, const std::string& levelStr, const std::string& fmt,
			Args&&... args) {
			if (level >= LOG_LEVEL_FILTER)
				sprint(*Ostream, level, "[" + levelStr + "] " + fmt, args...);
		}

		std::unique_ptr<OutputStream> CurrentStream;
		std::unique_ptr<std::ostream> Ostream;
	};

	CORE_DLLEXPORT extern Console gConsole;
} //namespace Poly
