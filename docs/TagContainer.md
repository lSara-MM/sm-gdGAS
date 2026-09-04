# TagContainer

**Inherits:** `GameplayAbilitySystem`

A container node responsible for holding, tracking, and signaling changes to an entity's gameplay tags.

## Description

`TagContainer` manages active gameplay tags for an entity at runtime. It stores tags as a collection of unique integer IDs (`PackedInt32Array`) and emits signals whenever tags are added or removed, allowing abilities, effects, and entities to respond dynamically to state changes.

Gameplay Tags are dot-separated identifiers (such as `Effect.Status.Stunned` or `Ability.Skill.Fireball`) used throughout the system to categorise entity states, query capabilities, trigger effects, and apply conditional requirements.

> **Tip:** To use them in code, refer to `int` variables found in `Tags` from the generated file `gen_tags.gd` to avoid spelling mistakes and improve code readability (Ex., `Tags.Effect.Status.Stunned`).

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `PackedInt32Array` | [tags](#tags) | `PackedInt32Array()` |

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `void` | [add_tag](#add_tag)(`tag_id`: `int`) |
| `void` | [add_tags](#add_tags)(`tags`: `PackedInt32Array`) |
| `bool` | [has_all_tags](#has_all_tags)(`tags`: `Array`) **const** |
| `bool` | [has_any_tags](#has_any_tags)(`tags`: `Array`) **const** |
| `bool` | [has_tag](#has_tag)(`tag_id`: `int`) **const** |
| `void` | [remove_tag](#remove_tag)(`tag_id`: `int`) |
| `void` | [remove_tags](#remove_tags)(`tags`: `PackedInt32Array`) |

## Signals

### tag_added(`owner`: `Object`, `id`: `int`)
Emitted when a new gameplay tag with the specified `id` is granted or added to the `owner`.

### tag_removed(`owner`: `Object`, `id`: `int`)
Emitted when an active gameplay tag with the specified `id` is removed or cleared from the `owner`.

## Property Descriptions

### tags
- **Type:** `PackedInt32Array`
- **Default:** `PackedInt32Array()`
- **Setter:** `set_tags`
- **Getter:** `get_tags`

The array of unique tag IDs currently assigned to and active within this container.

## Method Descriptions

### add_tag
```gdscript
func add_tag(tag_id: int) -> void
```
Adds the specified `tag_id` to this container. Increases stack count if the tag is already present.

### add_tags
```gdscript
func add_tags(tags: PackedInt32Array) -> void
```
Adds all valid tag IDs in the provided `tags` array to this container.

### has_all_tags
```gdscript
func has_all_tags(tags: Array) -> bool
```
Returns `true` if this container possesses all tags in the given `tags` array. Array elements can be tag ID integers, tag path strings/names (`String` / `StringName`), or `TagData` resource objects, although it is recommended to use TagID integers (provided by codegen).

### has_any_tags
```gdscript
func has_any_tags(tags: Array) -> bool
```
Returns `true` if this container possesses at least one of the tags in the given `tags` array. Array elements can be tag ID integers, tag path strings/names (`String` / `StringName`), or `TagData` resource objects, although it is recommended to use TagID integers (provided by codegen).

### has_tag
```gdscript
func has_tag(tag_id: int) -> bool
```
Returns `true` if the specified `tag_id` is active in this container.

### remove_tag
```gdscript
func remove_tag(tag_id: int) -> void
```
Decrements the reference stack count for `tag_id`. If the stack count reaches zero, the tag is removed from the container and `tag_removed` is emitted.

### remove_tags
```gdscript
func remove_tags(tags: PackedInt32Array) -> void
```
Removes all valid tag IDs in the provided `tags` array from this container.
