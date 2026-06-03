#pragma once
#include "internal/Types.h"
#include "internal/smUID.h"

#include <vector>
#include <functional>

namespace sm
{
	template<typename... Args>
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		uint32 SubscribeEvent(std::function<void(Args...)> callback)
		{
			uint32 id = m_IDs.GenerateUID();
			m_Listeners.emplace_back(Listener{ id, callback });

			return id;
		}

		void Unsubscribe(uint32 id)
		{
			std::erase_if(m_Listeners,
				[id](const Listener& listener)
				{
					return listener.id == id;
				});
		}

		void Notify(Args... args)
		{
			std::vector<Listener> copy = m_Listeners;

			for (Listener& listener : copy)
			{
				listener.callback(args...);
			}
		}

	private:
		struct Listener
		{
			uint32 id;
			std::function<void(Args...)> callback;
		};

		DumbUID m_IDs;

		std::vector<Listener> m_Listeners;
	};
}