# AbilityContainer

**Inherits:** `GameplayAbilitySystem`

A container node that manages granting, executing, tracking, and revoking gameplay abilities for an entity.

## Description

`AbilityContainer` serves as the runtime ability manager linked to a specific entity. It holds a list of granted `AbilityData` resources, evaluates activation constraints (such as checking tags, costs, and cooldowns), tracks ongoing ability cooldown timers, and provides virtual hook methods for custom logic when granting or executing abilities.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `AbilityData[]` | [abilities](#abilities) | `[]` |
| `NodePath` | [entity_node_path](#entity_node_path) | `NodePath("")` |

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `bool` | [_can_activate](#_can_activate)(`ability`: `AbilityData`) |
| `bool` | [_can_be_granted](#_can_be_granted)(`ability`: `AbilityData`) |
| `bool` | [_on_ability_already_granted](#_on_ability_already_granted)(`ability`: `AbilityData`) |
| `void` | [clear](#clear)() |
| `float` | [get_cooldown](#get_cooldown)(`id`: `int`) |
| `float` | [get_cost](#get_cost)(`id`: `int`) |
| `float` | [get_current_cooldown](#get_current_cooldown)(`id`: `int`) |
| `bool` | [grant_ability](#grant_ability)(`ability`: `AbilityData`) |
| `bool` | [has_ability](#has_ability)(`ability`: `AbilityData`) |
| `bool` | [is_active](#is_active)(`id`: `int`) |
| `bool` | [is_on_cooldown](#is_on_cooldown)(`id`: `int`) |
| `bool` | [revoke_ability](#revoke_ability)(`ability`: `AbilityData`) |
| `bool` | [set_entity_node_path](#set_entity_node_path)(`path`: `NodePath`) |
| `bool` | [try_activate](#try_activate)(`id`: `int`) |
| `bool` | [try_activate_abilities_with_tags](#try_activate_abilities_with_tags)(`ids`: `PackedInt32Array`) |

## Signals

### abilities_cleared(`entity`: `Object`)
Emitted when all granted abilities are cleared from this container via `clear()`.

### ability_activated(`entity`: `Object`, `ability`: `AbilityData`)
Emitted when `ability` is successfully activated for the specified target `entity`.

### ability_ended(`entity`: `Object`, `ability`: `AbilityData`)
Emitted when `ability` finishes execution or is cancelled for the specified target `entity`.

### ability_granted(`entity`: `Object`, `ability`: `AbilityData`)
Emitted when `ability` is successfully granted to this container for the specified target `entity`.

### ability_revoked(`entity`: `Object`, `ability`: `AbilityData`)
Emitted when `ability` is removed from this container for the specified target `entity`. Using `clear()` does not emit this signal.

## Property Descriptions

### abilities
The collection of `AbilityData` resources currently granted to and usable by this container.

### entity_node_path
The `NodePath` pointing to the owning entity node (such as a `GAS_Entity`) that this container manages abilities for.

## Method Descriptions

### _can_activate
```gdscript
func _can_activate(ability: AbilityData) -> bool
```
Virtual method called to evaluate whether the specified `ability` can currently be activated. Can be overridden in scripts or inherited C++ classes to implement custom activation conditions.

### _can_be_granted
```gdscript
func _can_be_granted(ability: AbilityData) -> bool
```
Virtual method called to check whether the specified `ability` is allowed to be added to this container. Override this method to implement custom grant requirements.

### _on_ability_already_granted
```gdscript
func _on_ability_already_granted(ability: AbilityData) -> bool
```
Virtual method triggered when attempting to grant an `ability` that is already present in this container. Override this to define duplicate ability behaviors (such as leveling up or ignoring duplicate grants).

### clear
```gdscript
func clear() -> void
```
Revokes all currently granted abilities from this container and clears active state tracking.

### get_cooldown
```gdscript
func get_cooldown(id: int) -> float
```
Returns the cooldown of the specified `ability`.

### get_cost
```gdscript
func get_cost(id: int) -> float
```
*(No additional description provided)*

### get_current_cooldown
```gdscript
func get_current_cooldown(id: int) -> float
```
Returns the remaining cooldown time in seconds for the specified `ability`. Returns `0.0` if the ability is off cooldown or not active.

### grant_ability
```gdscript
func grant_ability(ability: AbilityData) -> bool
```
Attempts to grant the specified `ability` to this container. Returns `true` if the ability was successfully granted, or `false` if validation failed or the operation was rejected. Calls the ability_granted signal.

### has_ability
```gdscript
func has_ability(ability: AbilityData) -> bool
```
Returns `true` if the specified `ability` is currently granted to and present within this container.

### is_active
```gdscript
func is_active(id: int) -> bool
```
Returns `true` if the ability matching the specified `id` is currently executing.

### is_on_cooldown
```gdscript
func is_on_cooldown(id: int) -> bool
```
Returns `true` if the ability is currently on cooldown.

### revoke_ability
```gdscript
func revoke_ability(ability: AbilityData) -> bool
```
Ends the specified `ability` if it's activa and removes it from this container. Returns `true` if the ability was found and successfully removed.

### set_entity_node_path
```gdscript
func set_entity_node_path(path: NodePath) -> bool
```
Sets the `NodePath` pointing to the target entity node. Returns `true` if the path was valid and successfully updated.

### try_activate
```gdscript
func try_activate(id: int) -> bool
```
Attempts to activate the `id` ability after validating resource costs, cooldowns, and tag constraints. Returns `true` if activation succeeded.

### try_activate_abilities_with_tags
```gdscript
func try_activate_abilities_with_tags(ids: PackedInt32Array) -> bool
```
Attempts to activate all granted abilities that match the provided gameplay ability tag `ids`. Returns `true` if at least one ability was successfully activated.
