#pragma once
#include <vector>
#include <functional>

namespace sm
{
	template <typename T>
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		void SubscribeEvent(std::function<void(T)> callback) { m_Listeners.push_back(callback); };

		void Notify(T arg) 
		{
			for (auto& cb : m_Listeners)
			{
				cb(arg);
			}
		}

	private:
		std::vector<std::function<void(T)>> m_Listeners;
	};
}