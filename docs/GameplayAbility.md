# GameplayAbility

**Inherits:** `RefCounted`

## Brief Description
Base class for runtime gameplay ability instances managing execution logic, costs, cooldowns, and targeting.

## Description
`GameplayAbility` represents an active or ready instance of a gameplay ability bound to an owner `GAS_Entity`. It manages checking resource costs, applying cooldowns, validating tag conditions, resolving target entities, and applying gameplay effects.

Custom ability behaviors can be defined by extending this class and overriding virtual hook methods like `_activate_ability()`, `_calculate_targets()`, and `_check_availability()`.

## Methods

### `bool` _activate_ability()

Virtual method called during `try_activate()` when custom activation logic is defined. Return `true` if the ability successfully activated. Overriding this bypasses the default automatic effect application and auto-ending flow.

### `GAS_Entity[]` _calculate_targets()

Virtual method called during default ability execution to resolve target entities. Returns an array of `GAS_Entity` instances to which the ability's gameplay effects will be applied.

### `bool` _check_availability()

Virtual method called during `can_activate()` to evaluate custom availability conditions. Must return `true` to allow ability activation.

### `void` _end_ability(bool was_cancelled)

Virtual method called when the ability ends or is cancelled. Override this to perform cleanup or trigger termination logic. `was_cancelled` is `true` if execution was prematurely interrupted.

### `void` apply_effects_to_target(GAS_Entity entity)

Applies all configured `EffectData` resources from this ability to the specified target `entity`. If `entity` is `null`, effects are applied to the owner entity instead.

### `bool` can_activate()

Returns `true` if the ability is currently in an idle state, meets resource costs, passes tag requirements, is not on cooldown, and satisfies `_check_availability()`.

### `bool` commit_ability()

Commits the ability by validating availability, applying resource costs, and starting cooldown timers. Returns `true` if the commit succeeded.

### `AbilityData` get_ability_data()

Returns the `AbilityData` resource configuration associated with this runtime ability instance.

### `GAS_Entity` get_entity_owner()

Returns the owner `GAS_Entity` that owns and executes this ability.

### `bool` is_on_cooldown()

Returns `true` if this ability's applied cooldown effect is currently active and unexpired.

### `void` set_ability_data(AbilityData data)

Sets the `AbilityData` resource configuration for this ability instance.

### `bool` try_activate()

Attempts to activate the ability. Checks `can_activate()` before executing custom logic in `_activate_ability()` or proceeding with default commit, targeting, and effect application routines. Returns `true` if activation succeeded.

### `bool` try_end(bool wasCancelled)

Attempts to end execution of this ability. If currently active, invokes `_end_ability()` and resets the state to idle.

