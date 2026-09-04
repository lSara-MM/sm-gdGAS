**# Gameplay Ability System (GAS) for Godot - Documentation**



Welcome to the documentation for the Gameplay Ability System resources, nodes, and components.

Here you will learn the necessary information to use this add-on.



\### What is it?

A Gameplay Ability System (GAS) is a framework for managing character attributes, skills, and interactions. Built to support RPGs, action titles, and MOBAs, it scales cleanly from basic light attacks to complex spells triggering status effects driven by actor and target data. Built to support RPGs, action titles, and MOBAs, it scales cleanly from basic light attacks to complex spells triggering status effects driven by actor and target data.



\## 📚 Class Index



\### Core Nodes \& Entities

\- \[`GAS\_Entity`](GAS\_Entity.md) - Main integration component linking attributes and tags to an actor.

\- \[`GAS\_World`](GAS\_World.md) - Global manager for GAS operations, pauses, and entity querying.



\### Containers

\- \[`AbilityContainer`](AbilityContainer.md) - Node managing granting, executing, tracking, and revoking gameplay abilities.

\- \[`AttributeContainer`](AttributeContainer.md) - Node managing runtime attributes, calculation states, and active modifiers.

\- \[TagContainer](TagContainer.md) - Node managing runtime tags.



\### Ability \& Effect Resources

\- \[`AbilityData`](AbilityData.md) - Configuration resource defining costs, cooldowns, tags, and effects for abilities.

\- \[`GameplayAbility`](GameplayAbility.md) - Base object representing the runtime logic of a gameplay ability.

\- \[`EffectData`](EffectData.md) - Resource defining gameplay buffs, debuffs, stat modifications, and tag alterations.



\### Attributes \& Modifiers

\- \[`AttributeData`](AttributeData.md) - Resource defining individual numerical stats, defaults, and min/max limits.

\- \[`AttributeSetData`](AttributeSetData.md) - Resource grouping multiple attributes into reusable packages.

\- \[`ModifierData`](ModifierData.md) - Resource defining mathematical operations to alter attributes.



\### Tags

\- \[TagData](TagData.md)

