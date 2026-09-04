# GAS_Entity

**Inherits:** `GameplayAbilitySystem`

## Brief Description
Node representing an active character participating in the Gameplay Ability System.

## Description
`GAS_Entity` serves as the main integration component for an actor in the Gameplay Ability System (GAS).

It connects attribute management (`AttributeContainer`) and state/tag tracking (`TagContainer`) into a single interface. It provides capabilities to apply gameplay effects (`EffectData`), manage active modifiers, and query system stats or status tags on the attached node.

## Properties

| Type | Name | Default |
| --- | --- | --- |
| `NodePath` | **attribute_container_node_path** | `NodePath("")` |
| `NodePath` | **tag_container_node_path** | `NodePath("")` |

## Methods

### `int` add_effect(EffectData effect)

Applies a `EffectData` to this entity. Returns an integer identifier representing the active effect instance.

### `AttributeContainer` get_attribute_container()

Returns the assigned `AttributeContainer` node associated with this entity.

### `TagContainer` get_tag_container()

Return current `TagContainer` associated to this entity.

### `void` set_attribute_container(AttributeContainer node)

Assigns the `AttributeContainer` node used by this entity to manage attribute sets and stats.

### `void` set_tag_container(TagContainer node)

Assigns the `TagContainer` node used by this entity to track active gameplay tags.

