# GameplayAbility

**Inherits:** `RefCounted`

Base class for runtime gameplay ability instances managing execution logic, costs, cooldowns, and targeting.

## Description

`GameplayAbility` represents an active or ready instance of a gameplay ability bound to an owner `GAS_Entity`. It manages checking resource costs, applying cooldowns, validating tag conditions, resolving target entities, and applying gameplay effects.

Custom ability behaviors can be defined by extending this class and overriding virtual hook methods like `_activate_ability()`, `_calculate_targets()`, and `_check_availability()`.

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `bool` | [_activate_ability](#_activate_ability)() |
| `GAS_Entity[]` | [_calculate_targets](#_calculate_targets)() |
| `bool` | [_check_availability](#_check_availability)() |
| `void` | [_end_ability](#_end_ability)(`was_cancelled`: `bool`) |
| `void` | [apply_effects_to_target](#apply_effects_to_target)(`entity`: `GAS_Entity`) |
| `bool` | [can_activate](#can_activate)() |
| `bool` | [commit_ability](#commit_ability)() |
| `AbilityData` | [get_ability_data](#get_ability_data)() |
| `GAS_Entity` | [get_entity_owner](#get_entity_owner)() |
| `bool` | [is_on_cooldown](#is_on_cooldown)() |
| `void` | [set_ability_data](#set_ability_data)(`data`: `AbilityData`) |
| `bool` | [try_activate](#try_activate)() |
| `bool` | [try_end](#try_end)(`wasCancelled`: `bool`) |

## Method Descriptions

### _activate_ability
```gdscript
func _activate_ability() -> bool
```
Virtual method called during `try_activate()` when custom activation logic is defined. Return `true` if the ability successfully activated. Overriding this bypasses the default automatic effect application and auto-ending flow.

### _calculate_targets
```gdscript
func _calculate_targets() -> GAS_Entity[]
```
Virtual method called during default ability execution to resolve target entities. Returns an array of `GAS_Entity` instances to which the ability's gameplay effects will be applied.

### _check_availability
```gdscript
func _check_availability() -> bool
```
Virtual method called during `can_activate()` to evaluate custom availability conditions. Must return `true` to allow ability activation.

### _end_ability
```gdscript
func _end_ability(was_cancelled: bool) -> void
```
Virtual method called when the ability ends or is cancelled. Override this to perform cleanup or trigger termination logic. `was_cancelled` is `true` if execution was prematurely interrupted.

### apply_effects_to_target
```gdscript
func apply_effects_to_target(entity: GAS_Entity) -> void
```
Applies all configured `EffectData` resources from this ability to the specified target `entity`. If `entity` is `null`, effects are applied to the owner entity instead.

### can_activate
```gdscript
func can_activate() -> bool
```
Returns `true` if the ability is currently in an idle state, meets resource costs, passes tag requirements, is not on cooldown, and satisfies `_check_availability()`.

### commit_ability
```gdscript
func commit_ability() -> bool
```
Commits the ability by validating availability, applying resource costs, and starting cooldown timers. Returns `true` if the commit succeeded.

### get_ability_data
```gdscript
func get_ability_data() -> AbilityData
```
Returns the `AbilityData` resource configuration associated with this runtime ability instance.

### get_entity_owner
```gdscript
func get_entity_owner() -> GAS_Entity
```
Returns the owner `GAS_Entity` that owns and executes this ability.

### is_on_cooldown
```gdscript
func is_on_cooldown() -> bool
```
Returns `true` if this ability's applied cooldown effect is currently active and unexpired.

### set_ability_data
```gdscript
func set_ability_data(data: AbilityData) -> void
```
Sets the `AbilityData` resource configuration for this ability instance.

### try_activate
```gdscript
func try_activate() -> bool
```
Attempts to activate the ability. Checks `can_activate()` before executing custom logic in `_activate_ability()` or proceeding with default commit, targeting, and effect application routines. Returns `true` if activation succeeded.

### try_end
```gdscript
func try_end(wasCancelled: bool) -> bool
```
Attempts to end execution of this ability. If currently active, invokes `_end_ability()` and resets the state to idle.
