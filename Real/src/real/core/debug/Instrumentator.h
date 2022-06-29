#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Real {
	struct ProfileResult {
		std::string name;
		long long	startPoint, endPoint;
		uint32_t	tid;
	};

	struct InstrumentationSession {
		std::string name;
	};

	class Instrumentor {
	public:
		Instrumentor()
			: currentSession(nullptr), profileCount(0) {}

		void beginSession(const std::string& name, const std::string& filepath = "results.json") {
			outStream.open(filepath);
			writeHeader();
			currentSession = new InstrumentationSession{name};
		}

		void endSession() {
			writeFooter();
			outStream.close();
			delete currentSession;
			currentSession = nullptr; // early nullptr dereference error
			profileCount   = 0;
		}

		void writeProfile(const ProfileResult& result) {
			if (profileCount++ > 0)
				outStream << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			outStream << "{"
					  << "\"cat\":\"function\","
					  << "\"dur\":" << (result.endPoint - result.startPoint) << ','
					  << "\"name\":\"" << name << "\","
					  << "\"ph\":\"X\","
					  << "\"pid\":0,"
					  << "\"tid\":" << result.tid << ","
					  << "\"ts\":" << result.startPoint
					  << "}";

			flush();
		}

		void writeHeader() {
			outStream << "{\"otherData\": {},\"traceEvents\":[";
			flush();
		}

		void writeFooter() {
			outStream << "]}";
			flush();
		}

		static Instrumentor& get() {
			static Instrumentor instance;
			return instance;
		}

	private:
		inline void flush() {
#ifndef RE_INSTRUMENTOR_NO_FLUSH
			outStream.flush();
#endif
		}

		InstrumentationSession* currentSession;
		std::ofstream			outStream;
		int						profileCount;
	};

	class Timer final {
	public:
		Timer() {
			reset();
		}
		void reset() {
			startTime = std::chrono::high_resolution_clock::now();
		}

		float elapsedMillis() {
			//It's written like that only to get realy high resolution clock time
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count() * 0.001f * 0.001f;
		}

	protected:
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};

	class InstrumentorTimer final {
	public:
		InstrumentorTimer(const char* name)
			: name(name), startTime(std::chrono::high_resolution_clock::now()), stopped(false) {
		}

		~InstrumentorTimer() {
			if (!stopped)
				stop();
		}

		void stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
			long long end	= std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			unsigned tid = static_cast<unsigned>(
				std::hash<std::thread::id>{}(std::this_thread::get_id()));
			Instrumentor::get().writeProfile({name, start, end, tid});

			stopped = true;
		}

	private:
		const char*													name;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
		bool														stopped;
	};

	namespace InstrumentorUtils {
		template <size_t N>
		struct ChangeResult {
			char data[N];
		};

		template <size_t N, size_t K>
		constexpr auto cleanupOutputString(const char (&expr)[N], const char (&remove)[K]) {
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N) {
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}

}

#ifdef RE_PROFILE
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define RE_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define RE_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define RE_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define RE_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define RE_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define RE_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define RE_FUNC_SIG __func__
	#else
		#define RE_FUNC_SIG "VKR_FUNC_SIG unknown"
	#endif

	#define RE_INTERNAL_PROFILE_SCOPE_LINE(name, line)               \
		constexpr auto fixedName##line = ::Real::InstrumentorUtils:: \
			cleanupOutputString(name, "__cdecl ");                   \
		::Real::InstrumentorTimer timer##line(fixedName##line.data)


	#define RE_PROFILE_BEGIN_SESSION(name, filepath) ::Real::Instrumentor::get().beginSession(name, filepath)
	#define RE_PROFILE_END_SESSION()				 ::Real::Instrumentor::get().endSession();
	#define RE_PROFILE_SCOPE(name)					 RE_INTERNAL_PROFILE_SCOPE_LINE(name, __LINE__);
	#define RE_PROFILE_FUNCTION()					 RE_PROFILE_SCOPE(RE_FUNC_SIG)
#else
	#define RE_PROFILE_BEGIN_SESSION(name, filepath)
	#define RE_PROFILE_END_SESSION()
	#define RE_PROFILE_SCOPE(name)
	#define RE_PROFILE_FUNCTION()
#endif