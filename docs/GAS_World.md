# GAS_World

**Inherits:** `GameplayAbilitySystem`

A global manager used to control system-wide settings and query entities within the Gameplay Ability System.

## Description

`GAS_World` acts as a central control node for global GAS operations. It provides master switches to freeze ability execution or status effects across the entire game (useful for pausing gameplay, managing UI overlays, or staging cutscenes) and offers query methods for tag-based node filtering.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `bool` | [enable_abilities](#enable_abilities) | `true` |
| `bool` | [enable_effects](#enable_effects) | `true` |
| `int` | [entity_count](#entity_count) | `-1` |

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `Node[]` | [all_with_tags](#all_with_tags)(`tags`: `PackedInt32Array`) |
| `Node[]` | [any_with_tags](#any_with_tags)(`tags`: `PackedInt32Array`) |
| `int` | [find_tag](#find_tag)(`tag`: `StringName`) |
| `Node[]` | [none_with_tags](#none_with_tags)(`tags`: `PackedInt32Array`) |

## Property Descriptions

### enable_abilities
If `true`, ability activations are processed normally. If toggled to `false`, all ability execution requests are globally blocked.

### enable_effects
If `true`, gameplay effects tick and apply normally. If toggled to `false`, effect processing is globally paused or ignored.

### entity_count
(ReadOnly) The total number of GAS-compatible entities currently tracked by this world manager instance.

## Method Descriptions

### all_with_tags
```gdscript
func all_with_tags(tags: PackedInt32Array) -> Node[]
```
Returns an array of registered entities that possess **all** of the gameplay tags specified in the `tags` array (AND logic match).

### any_with_tags
```gdscript
func any_with_tags(tags: PackedInt32Array) -> Node[]
```
Returns an array of registered entities that possess **at least one** of the gameplay tags specified in the `tags` array (OR logic match).

### find_tag
```gdscript
func find_tag(tag: StringName) -> int
```
Returns the TagID of `tag` path if it exists. Return 0 if tag was not found.

### none_with_tags
```gdscript
func none_with_tags(tags: PackedInt32Array) -> Node[]
```
Returns an array of registered entities that do **not** possess any of the gameplay tags specified in the `tags` array (NOT logic match).
