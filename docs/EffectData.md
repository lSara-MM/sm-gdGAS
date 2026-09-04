# EffectData

**Inherits:** `GameplayAbilitySystemResource`

A data resource that defines a gameplay effect, including stat modifiers, durations, and tag alterations.

## Description

`EffectData` is a configuration resource used to define buffs, debuffs, or instant changes (like healing or damage) applied to a target. It allows you to modify attributes using `ModifierData` and manipulate a character's gameplay tags for a specified duration type.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `float` | [duration](#duration) | `0.0` |
| `int` | [effect_type](#effect_type) | `0` |
| `ModifierData[]` | [modifiers](#modifiers) | `[]` |
| `StringName` | [name](#name) | `&""` |
| `PackedInt32Array` | [tags_to_add](#tags_to_add) | `PackedInt32Array()` |
| `PackedInt32Array` | [tags_to_remove](#tags_to_remove) | `PackedInt32Array()` |

## Constants

- **Instant** = `0` (Enum: `Type`)
  - The effect applies instantly and its changes are baked permanently into the base attributes. It does not look at `duration` and cannot be naturally reverted.
- **Temporary** = `1` (Enum: `Type`)
  - The effect behaves like a traditional buff or debuff. Its changes persist only for the amount of seconds specified in `duration`, after which it is automatically removed and its changes are reverted.
- **Persistent** = `2` (Enum: `Type`)
  - The effect stays active indefinitely until explicitly removed by a gameplay mechanic, script, or ability (Ex: a toggled stance or a curse that requires a specific cure).

## Property Descriptions

### duration
The active lifetime of the effect in seconds. This is only used if `effect_type` is set to `Temporary`.

### effect_type
Determines how the effect's lifetime is treated (Ex: whether it is instant/permanent, lasts for a countdown, or persists until manually removed).

### modifiers
A list of attribute modifiers (like "+10 Strength" or "-15% Movement Speed") applied to the target by this effect.

### name
The unique identifier for this effect, useful for debugging, UI displays, or searching for active effects on an entity.

### tags_to_add
A collection of gameplay tags granted to the target while this effect is active (Ex: adding a `Stunned` or `Poisoned` tag).

These are removed automatically when the effect is removed.

### tags_to_remove
A collection of gameplay tags stripped from the target when this effect is applied (e.g., removing a `Stealth` tag upon taking damage).

These are added back automatically when the effect is removed.
