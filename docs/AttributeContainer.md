# AttributeContainer

**Inherits:** `GameplayAbilitySystem`

A node container that stores, manages, and evaluates runtime attributes and active modifiers for an entity.

## Description

`AttributeContainer` serves as the runtime manager for a character's stats defined by an `AttributeSetData`. It maintains base and calculated values for attributes, manages the application and removal of `ModifierData` instances, handles dirty-state recalculations, and signals whenever stats or modifiers change.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `AttributeSetData` | [attribute_set](#attribute_set) | `` |

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `float` | [_pre_attribute_change](#_pre_attribute_change)(`attribute_id`: `StringName`, `calculated_value`: `float`) |
| `int` | [add_modifier](#add_modifier)(`attribute_id`: `StringName`, `modifier`: `ModifierData`) |
| `float` | [get_attribute_base_value](#get_attribute_base_value)(`attribute_id`: `StringName`) |
| `float` | [get_attribute_current_value](#get_attribute_current_value)(`attribute_id`: `StringName`) |
| `bool` | [is_attribute_dirty](#is_attribute_dirty)(`attribute_id`: `StringName`) |
| `bool` | [is_attribute_max](#is_attribute_max)(`attribute_id`: `StringName`) |
| `bool` | [is_attribute_min](#is_attribute_min)(`attribute_id`: `StringName`) |
| `void` | [remove_modifier](#remove_modifier)(`attribute_id`: `StringName`, `modifier`: `ModifierData`) |

## Signals

### attribute_changed(`attribute_name`: `StringName`, `new_value`: `float`, `old_value`: `float`)
Emitted when an attribute in this container has been modified.

### effect_added(`attribute_name`: `StringName`, `effect_data`: `Object`)
Emitted when a gameplay effect affecting `attribute_name` is applied to the `owner`.

### effect_removed(`attribute_name`: `StringName`, `effect_data`: `Object`)
Emitted when an active gameplay effect affecting `attribute_name` is removed from the `owner`.

### modifier_added(`attribute_name`: `StringName`, `modifier_data`: `Object`)
Emitted when a new `ModifierData` instance is added to `attribute_name` on the `owner`.

### modifier_removed(`attribute_name`: `StringName`, `modifier_data`: `Object`)
Emitted when an existing `ModifierData` instance is removed from `attribute_name` on the `owner`.

## Property Descriptions

### attribute_set
The `AttributeSetData` resource template containing the default attributes and definitions managed by this container.

## Method Descriptions

### _pre_attribute_change
```gdscript
func _pre_attribute_change(attribute_id: StringName, calculated_value: float) -> float
```
Override this method to intercept attribute changes. This is useful to apply gameplay caps before the actual value changes. (Ex. `Attribute` "Health" cannot be higher than `Attribute` "MaxHealth". In this case, use this method to apply the limit instead of the calculated value if that is higher than allowed).

### add_modifier
```gdscript
func add_modifier(attribute_id: StringName, modifier: ModifierData) -> int
```
Applies a `ModifierData` instance to the attribute specified by `attribute_id`. Returns an integer handle or index associated with the newly applied modifier.

### get_attribute_base_value
```gdscript
func get_attribute_base_value(attribute_id: StringName) -> float
```
Returns the un-modified base value for the attribute matching `attribute_id`.

### get_attribute_current_value
```gdscript
func get_attribute_current_value(attribute_id: StringName) -> float
```
Returns the calculated current value for the attribute matching `attribute_id`, accounting for all active modifiers.

### is_attribute_dirty
```gdscript
func is_attribute_dirty(attribute_id: StringName) -> bool
```
Returns `true` if the attribute specified by `attribute_id` has pending modifications or requires a recalculation before its value can be read accurately.

### is_attribute_max
```gdscript
func is_attribute_max(attribute_id: StringName) -> bool
```
Returns `true` if the current value of the attribute specified by `attribute_id` has reached or exceeded its defined maximum threshold.

### is_attribute_min
```gdscript
func is_attribute_min(attribute_id: StringName) -> bool
```
Returns `true` if the current value of the attribute specified by `attribute_id` has dropped to or below its defined minimum threshold.

### remove_modifier
```gdscript
func remove_modifier(attribute_id: StringName, modifier: ModifierData) -> void
```
Removes an active `ModifierData` instance from the target attribute specified by `attribute_id` and triggers a recalculation.
