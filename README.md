# Gameplay Ability System (GAS) for Godot - Documentation
Welcome to the documentation for the Gameplay Ability System resources, nodes, and components.
Here you will learn the necessary information to use this add-on.

### What is it?
A Gameplay Ability System (GAS) is a framework for managing character attributes, skills, and interactions. Built to support RPGs, action titles, and MOBAs, it scales cleanly from basic light attacks to complex spells triggering status effects driven by actor and target data. Built to support RPGs, action titles, and MOBAs, it scales cleanly from basic light attacks to complex spells triggering status effects driven by actor and target data.

## 📚 Class Index
### Core Nodes \& Entities

- [`GAS_Entity`](docs/GAS\_Entity.md) - Main integration component linking attributes and tags to an actor.

- [`GAS_World`](docs/GAS\_World.md) - Global manager for GAS operations, pauses, and entity querying.

### Containers

- [`AbilityContainer`](docs/AbilityContainer.md) - Node managing granting, executing, tracking, and revoking gameplay abilities.

- [`AttributeContainer`](docs/AttributeContainer.md) - Node managing runtime attributes, calculation states, and active modifiers.

- [TagContainer](docs/TagContainer.md) - Node managing runtime tags.

### Ability \& Effect Resources

- [`AbilityData`](docs/AbilityData.md) - Configuration resource defining costs, cooldowns, tags, and effects for abilities.

- [`GameplayAbility`](docs/GameplayAbility.md) - Base object representing the runtime logic of a gameplay ability.

- [`EffectData`](docs/EffectData.md) - Resource defining gameplay buffs, debuffs, stat modifications, and tag alterations.

### Attributes \& Modifiers

- [`AttributeData`](docs/AttributeData.md) - Resource defining individual numerical stats, defaults, and min/max limits.

- [`AttributeSetData`](docs/AttributeSetData.md) - Resource grouping multiple attributes into reusable packages.

- [`ModifierData`](docs/ModifierData.md) - Resource defining mathematical operations to alter attributes.

### Tags

- [TagData](docs/TagData.md) - Resource that forms a hierarchy and defines tags, later converted to integers.

