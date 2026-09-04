# AbilityContainer

**Inherits:** `GameplayAbilitySystem`

## Brief Description
A container node that manages granting, executing, tracking, and revoking gameplay abilities for an entity.

## Description
`AbilityContainer` serves as the runtime ability manager linked to a specific entity. It holds a list of granted `AbilityData` resources, evaluates activation constraints (such as checking tags, costs, and cooldowns), tracks ongoing ability cooldown timers, and provides virtual hook methods for custom logic when granting or executing abilities.

## Properties

| Type | Name | Default |
| --- | --- | --- |
| `AbilityData[]` | **abilities** | `[]` |
| `NodePath` | **entity_node_path** | `NodePath("")` |

**abilities**:

The collection of `AbilityData` resources currently granted to and usable by this container.

**entity_node_path**:

The `NodePath` pointing to the owning entity node (such as a `GAS_Entity`) that this container manages abilities for.

## Methods

### `bool` _can_activate(AbilityData ability)

Virtual method called to evaluate whether the specified `ability` can currently be activated. Can be overridden in scripts or inherited C++ classes to implement custom activation conditions.

### `bool` _can_be_granted(AbilityData ability)

Virtual method called to check whether the specified `ability` is allowed to be added to this container. Override this method to implement custom grant requirements.

### `bool` _on_ability_already_granted(AbilityData ability)

Virtual method triggered when attempting to grant an `ability` that is already present in this container. Override this to define duplicate ability behaviors (such as leveling up or ignoring duplicate grants).

### `void` clear()

Revokes all currently granted abilities from this container and clears active state tracking.

### `float` get_cooldown(int id)

Returns the cooldown of the specified `ability`.

### `float` get_cost(int id)



### `float` get_current_cooldown(int id)

Returns the remaining cooldown time in seconds for the specified `ability`. Returns `0.0` if the ability is off cooldown or not active.

### `bool` grant_ability(AbilityData ability)

Attempts to grant the specified `ability` to this container. Returns `true` if the ability was successfully granted, or `false` if validation failed or the operation was rejected. Calls the ability_granted signal.

### `bool` has_ability(AbilityData ability)

Returns `true` if the specified `ability` is currently granted to and present within this container.

### `bool` is_active(int id)

Returns `true` if the ability matching the specified `id` is currently executing.

### `bool` is_on_cooldown(int id)

Returns `true` if the ability is currently on cooldown.

### `bool` revoke_ability(AbilityData ability)

Ends the specified `ability` if it's activa and removes it from this container. Returns `true` if the ability was found and successfully removed.

### `bool` set_entity_node_path(NodePath path)

Sets the `NodePath` pointing to the target entity node. Returns `true` if the path was valid and successfully updated.

### `bool` try_activate(int id)

Attempts to activate the `id` ability after validating resource costs, cooldowns, and tag constraints. Returns `true` if activation succeeded.

### `bool` try_activate_abilities_with_tags(PackedInt32Array ids)

Attempts to activate all granted abilities that match the provided gameplay ability tag `ids`. Returns `true` if at least one ability was successfully activated.

## Signals

- **abilities_cleared**(Object entity)
  - Emitted when all granted abilities are cleared from this container via `clear()`.
- **ability_activated**(Object entity, AbilityData ability)
  - Emitted when `ability` is successfully activated for the specified target `entity`.
- **ability_ended**(Object entity, AbilityData ability)
  - Emitted when `ability` finishes execution or is cancelled for the specified target `entity`.
- **ability_granted**(Object entity, AbilityData ability)
  - Emitted when `ability` is successfully granted to this container for the specified target `entity`.
- **ability_revoked**(Object entity, AbilityData ability)
  - Emitted when `ability` is removed from this container for the specified target `entity`. Using `clear()` does not emit this signal.

