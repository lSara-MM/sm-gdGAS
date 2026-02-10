#include "GameplayEffect.h"

#include "GameplayAttribute.h"
#include "gdEffectData.h"

namespace sm
{
	class EffectSystem
	{
	private:
		using EffectPtr = std::unique_ptr<GameplayEffect>;

	public:
		EffectSystem() = default;

		void Update(float dt);

		GameplayEffect* FindEffect(EffectID effectID);

		void AddEffect(godot::Ref<EffectData> gdEffect);

		void RemoveEffect(GameplayAttribute& attr, godot::Ref<EffectData> gdEffect);

		void RemoveEffect(GameplayAttribute& attr, EffectID gdEffectID);

	private:
		std::vector<EffectPtr> m_ActiveEffects;
	};
}