# GAS_Entity

**Inherits:** `GameplayAbilitySystem`

Node representing an active character participating in the Gameplay Ability System.

## Description

`GAS_Entity` serves as the main integration component for an actor in the Gameplay Ability System (GAS).

It connects attribute management (`AttributeContainer`) and state/tag tracking (`TagContainer`) into a single interface. It provides capabilities to apply gameplay effects (`EffectData`), manage active modifiers, and query system stats or status tags on the attached node.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `NodePath` | [attribute_container_node_path](#attribute_container_node_path) | `NodePath("")` |
| `NodePath` | [tag_container_node_path](#tag_container_node_path) | `NodePath("")` |

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `int` | [add_effect](#add_effect)(`effect`: `EffectData`) |
| `AttributeContainer` | [get_attribute_container](#get_attribute_container)() |
| `TagContainer` | [get_tag_container](#get_tag_container)() |
| `void` | [set_attribute_container](#set_attribute_container)(`node`: `AttributeContainer`) |
| `void` | [set_tag_container](#set_tag_container)(`node`: `TagContainer`) |

## Property Descriptions

### attribute_container_node_path
*(No additional description provided)*

### tag_container_node_path
*(No additional description provided)*

## Method Descriptions

### add_effect
```gdscript
func add_effect(effect: EffectData) -> int
```
Applies a `EffectData` to this entity. Returns an integer identifier representing the active effect instance.

### get_attribute_container
```gdscript
func get_attribute_container() -> AttributeContainer
```
Returns the assigned `AttributeContainer` node associated with this entity.

### get_tag_container
```gdscript
func get_tag_container() -> TagContainer
```
Return current `TagContainer` associated to this entity.

### set_attribute_container
```gdscript
func set_attribute_container(node: AttributeContainer) -> void
```
Assigns the `AttributeContainer` node used by this entity to manage attribute sets and stats.

### set_tag_container
```gdscript
func set_tag_container(node: TagContainer) -> void
```
Assigns the `TagContainer` node used by this entity to track active gameplay tags.
