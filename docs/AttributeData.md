# AttributeData

**Inherits:** `GameplayAbilitySystemResource`

## Brief Description
Resource defining a numerical gameplay attribute along with its default values and clamping limits.

## Description
`AttributeData` is a custom resource used in the Gameplay Ability System to represent individual numerical stats (ex. Health, Mana, Attack Power, or Movement Speed).

It defines the attribute's unique identifier name, initial base value, and the minimum/maximum clamping bounds applied to the attribute when modified by gameplay effects.

## Properties

| Type | Name | Default |
| --- | --- | --- |
| `float` | **base_value** | `0.0` |
| `float` | **max_value** | `3.40282e+38` |
| `float` | **min_value** | `0.0` |
| `StringName` | **name** | `&""` |

**base_value**:

The initial base value of this attribute before any gameplay effects or modifiers are applied.

**max_value**:

The maximum allowed upper bound for this attribute's current value.

**Note:** This is a strict architectural clamp boundary for the raw attribute value, not a dynamic gameplay stat like "Max HP". Conceptually, stats like "Max Health" or "Max Mana" should be created as separate `AttributeData` resources (ex. an `Health` attribute and a `MaxHealth` attribute), whereas `max_value` acts as the absolute upper ceiling allowed for this specific attribute.

**min_value**:

The minimum allowed lower bound for this attribute's current value (ex. preventing health or stats from dropping below `0.0`).

**name**:

The unique `StringName` identifier used to reference this attribute in containers and modifier calculations (e.g., `&"Health"`).

