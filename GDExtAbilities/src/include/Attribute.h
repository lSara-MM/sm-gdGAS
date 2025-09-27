#pragma once
#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class Attribute : public godot::Resource
	{
		GDCLASS(Attribute, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		Attribute();
		~Attribute();

		bool GetActive() const { return isActive; }
		void SetActive(const bool active = true) { isActive = active; }

	private:
		bool isActive = false;
	};
}