# AbilityData

**Inherits:** `GameplayAbilitySystemResource`

A configuration resource that defines the static properties, costs, cooldowns, and tag requirements of a gameplay ability.

## Description

`AbilityData` is a data resource used to configure and define individual skills or spells before they are instantiated. It acts as a template, holding configuration parameters like resource `cost`, `cooldown` timers, associated `EffectData` elements, and gameplay tags that dictate when and how the ability can be cast.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `Script` | [ability](#ability) | `` |
| `PackedInt32Array` | [activation_blocked_tags](#activation_blocked_tags) | `PackedInt32Array()` |
| `PackedInt32Array` | [activation_tags](#activation_tags) | `PackedInt32Array()` |
| `PackedInt32Array` | [block_abilities_with_tags](#block_abilities_with_tags) | `PackedInt32Array()` |
| `float` | [cooldown](#cooldown) | `0.0` |
| `float` | [cost](#cost) | `0.0` |
| `StringName` | [cost_resource_attribute](#cost_resource_attribute) | `&""` |
| `EffectData[]` | [effect](#effect) | `[]` |
| `StringName` | [tag_name](#tag_name) | `&""` |
| `PackedInt32Array` | [tags](#tags) | `PackedInt32Array()` |

## Property Descriptions

### ability
`Optional`. The `GameplayAbility` script containing the runtime execution logic (such as targeting, animations, and behavior) for this resource. If assigned, it uses a script template with virtual methods available for overriding.

### activation_blocked_tags
The `GameplayAbility` cannot be activated if the owner has any of these `GameplayTag`s.

### activation_tags
The required `GameplayTag`s that the executing entity must possess to successfully activate this `GameplayAbility`.

### block_abilities_with_tags
Other `GameplayAbility`s that have these `GameplayTag`s in their Ability Tags are blocked from activating while this `GameplayAbility` is active.

### cooldown
By default 0, the duration in seconds that the entity must wait before this ability can be cast again.

### cost
By default 0, the resource value (ex. Mana, Stamina, or Health) required and consumed upon successfully committing the ability.
It needs an existing attribute on the owner entity to deplete from.

### cost_resource_attribute
StringName of the name of the attribute that contains the resource of the cost. (Ex. Mana, Stamina, etc.).

### effect
The array of `EffectData` resources (buffs, debuffs, or instant modifications) applied to targets when this ability executes.

### tag_name
*(No additional description provided)*

### tags
`GameplayTag`s to describe the `GameplayAbility`.
