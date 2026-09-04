# AttributeSetData

**Inherits:** `GameplayAbilitySystemResource`

Resource container holding a collection of gameplay attributes.

## Description

`AttributeSetData` is a custom resource used in the Gameplay Ability System to group multiple `AttributeData` resources together.

It allows developers to define reusable sets of attributes (such as basic character stats, combat attributes, or custom status attributes) that can be assigned or loaded into an attribute container as a single package.

## Properties

| Type | Name | Default |
| :--- | :--- | :--- |
| `AttributeData[]` | [attributes](#attributes) | `[]` |

## Methods

| Return Type | Method Signature |
| :--- | :--- |
| `void` | [add_attribute](#add_attribute)(`attr`: `AttributeData`) |

## Property Descriptions

### attributes
An array of `AttributeData` resources contained within this attribute set.

## Method Descriptions

### add_attribute
```gdscript
func add_attribute(attr: AttributeData) -> void
```
Appends an array of `AttributeData` resources to this attribute set.
