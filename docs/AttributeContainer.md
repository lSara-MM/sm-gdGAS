# AttributeContainer

**Inherits:** `GameplayAbilitySystem`

## Brief Description
A node container that stores, manages, and evaluates runtime attributes and active modifiers for an entity.

## Description
`AttributeContainer` serves as the runtime manager for a character's stats defined by an `AttributeSetData`. It maintains base and calculated values for attributes, manages the application and removal of `ModifierData` instances, handles dirty-state recalculations, and signals whenever stats or modifiers change.

## Properties

| Type | Name | Default |
| --- | --- | --- |
| `AttributeSetData` | **attribute_set** | `` |

**attribute_set**:

The `AttributeSetData` resource template containing the default attributes and definitions managed by this container.

## Methods

### `float` _pre_attribute_change(StringName attribute_id, float calculated_value)

Override this method to intercept attribute changes. This is useful to apply gameplay caps before the actual value changes. (Ex. `Attribute` "Health" cannot be higher than `Attribute` "MaxHealth". In this case, use this method to apply the limit instead of the calculated value if that is higher than allowed).

### `int` add_modifier(StringName attribute_id, ModifierData modifier)

Applies a `ModifierData` instance to the attribute specified by `attribute_id`. Returns an integer handle or index associated with the newly applied modifier.

### `float` get_attribute_base_value(StringName attribute_id)

Returns the un-modified base value for the attribute matching `attribute_id`.

### `float` get_attribute_current_value(StringName attribute_id)

Returns the calculated current value for the attribute matching `attribute_id`, accounting for all active modifiers.

### `bool` is_attribute_dirty(StringName attribute_id)

Returns `true` if the attribute specified by `attribute_id` has pending modifications or requires a recalculation before its value can be read accurately.

### `bool` is_attribute_max(StringName attribute_id)

Returns `true` if the current value of the attribute specified by `attribute_id` has reached or exceeded its defined maximum threshold.

### `bool` is_attribute_min(StringName attribute_id)

Returns `true` if the current value of the attribute specified by `attribute_id` has dropped to or below its defined minimum threshold.

### `void` remove_modifier(StringName attribute_id, ModifierData modifier)

Removes an active `ModifierData` instance from the target attribute specified by `attribute_id` and triggers a recalculation.

## Signals

- **attribute_changed**(StringName attribute_name, float new_value, float old_value)
  - Emitted when an attribute in this container has been modified.
- **effect_added**(StringName attribute_name, Object effect_data)
  - Emitted when a gameplay effect affecting `attribute_name` is applied to the `owner`.
- **effect_removed**(StringName attribute_name, Object effect_data)
  - Emitted when an active gameplay effect affecting `attribute_name` is removed from the `owner`.
- **modifier_added**(StringName attribute_name, Object modifier_data)
  - Emitted when a new `ModifierData` instance is added to `attribute_name` on the `owner`.
- **modifier_removed**(StringName attribute_name, Object modifier_data)
  - Emitted when an existing `ModifierData` instance is removed from `attribute_name` on the `owner`.

