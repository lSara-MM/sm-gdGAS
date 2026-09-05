# ModifierData

**Inherits:** `GameplayAbilitySystemResource`

A data resource that defines a specific mathematical operation to an attribute.

## Description

`ModifierData` configurations specify how a target attribute should be modified when a gameplay effect is applied. It defines which attribute is targeted `target_id`, how much it changes by `value`, and the specific mathematical operation used to perform the change `operation_type`.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `int` | [operation_type](#operation_type) | `0` |
| `StringName` | [source_id](#source_id) | `&""` |
| `StringName` | [target_id](#target_id) | `&""` |
| `float` | [value](#value) | `0.0` |

## Constants

- **Add** = `0` (Enum: `OperationType`)
  - Adds a flat numerical value directly to the attribute's current value.
  - *Formula: CurrentValue += ModifierValue*
- **Multiply** = `1` (Enum: `OperationType`)
  - Multiplies the attribute's current value by a raw scaling factor.
  - *Formula: CurrentValue* **= ModifierValue*
- **PercentAdd** = `2` (Enum: `OperationType`)
  - Calculates a percentage based strictly on the attribute's base value, then adds that calculated amount to the current value.
  - *Formula: CurrentValue += (BaseValue * (ModifierValue / 100))*
- **PercentStack** = `3` (Enum: `OperationType`)
  - Multiplies the attribute's current value directly by a percentage factor.
  - *Formula: CurrentValue* *= (ModifierValue / 100)*
- **Override** = `4` (Enum: `OperationType`)
  - Ignores all other active modifiers and forces the attribute's current value to match this exact value.

## Property Descriptions

### operation_type
The mathematical operation used to apply `value` to the target attribute (Ex. adding flat amounts, scaling, or overriding).

### source_id
The identifier of the source attribute or tag backing this modifier calculation, if applicable.

### target_id
The unique `StringName` of the attribute that this modifier will alter (Ex. `&"Health"` or `&"Speed"`).

### value
The numerical value used in the calculation. Depending on `operation_type`, this can represent a flat value, multiplier, or percentage.
