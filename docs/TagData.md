# TagData

**Inherits:** `GameplayAbilitySystemResource`

Resource representing a hierarchical Gameplay Tag node in the Gameplay Ability System (GAS).

## Description

`TagData` is a custom resource used to define, organise, and store metadata for individual Gameplay Tags within the Gameplay Ability System (GAS).

Gameplay Tags are dot-separated identifiers (such as `Effect.Status.Stunned` or `Ability.Skill.Fireball`) used throughout the system to categorise entity states, query capabilities, trigger effects, and apply conditional requirements. `TagData` objects form a tree structure where parent tags contain nested child tags.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `TagData[]` | [children](#children) | `[]` |
| `int` | [id](#id) | `0` |
| `String` | [name](#name) | `""` |
| `String` | [path](#path) | `""` |

## Property Descriptions

### children
- **Type:** `TagData[]`
- **Default:** `[]`
- **Setter:** `set_children`
- **Getter:** `get_children`

An array of child `TagData` resources nested directly beneath this tag node in the hierarchy.

### id
- **Type:** `int`
- **Default:** `0`
- **Setter:** `set_internal_id`
- **Getter:** `get_internal_id`

The unique internal integer identifier assigned to this tag within the tag registry system.

### name
- **Type:** `String`
- **Default:** `""`
- **Setter:** `set_name`
- **Getter:** `get_name`

The local segment name of this specific tag node (e.g., `"Stunned"` in `State.Status.Stunned`).

### path
- **Type:** `String`
- **Default:** `""`
- **Setter:** `set_path`
- **Getter:** `get_path`

The full dot-separated hierarchical path string representing the complete tag (e.g., `"State.Status.Stunned"`).
