#pragma once


namespace RealEngine {
	class Timestep {
	public:
		Timestep(float time = 0.0f) : time(time){}

		operator float() const noexcept { return time; }

		float getSeconds() const noexcept { return time; }
		float getMilliseconds() const noexcept { return time * 1000.0f; }
	private:
		float time;
	};
}
