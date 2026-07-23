#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <vector>

namespace sm
{
	class GAS_World;

	class GameplayAbilitySystem : public godot::Node
	{
		GDCLASS(GameplayAbilitySystem, godot::Node)

	protected:
		static void _bind_methods() {};

	public:
		virtual void _notification(int notification) final;

		virtual void OnEnterTree() {};
		virtual void OnExitTree() {};
		virtual void OnReady() {};
		virtual void OnProcess() {};
		virtual void OnParented() {};
		virtual void OnUnparented() {};
		virtual void OnChildOrderChanged() {};
	};

	namespace NodeUtils
	{
		template <typename T>
		static T* GetParentNodeOfType(godot::Node* target)
		{
			if (!target)
			{
				return nullptr;
			}

			godot::Node* parent = target->get_parent();

			while (parent)
			{
				if (T* typed = godot::Object::cast_to<T>(parent))
				{
					return typed;
				}

				parent = parent->get_parent();
			}

			return nullptr;
		}

		template <typename T>
		static T* GetChildNodeOfType(godot::Node* target)
		{
			if (!target)
			{
				return nullptr;
			}

			std::vector<godot::Node*> nodesStack;
			for (int i = 0; i < target->get_child_count(); ++i)
			{
				nodesStack.push_back(target->get_child(i));
			}

			while (!nodesStack.empty())
			{
				godot::Node* currentNode = nodesStack.back();
				nodesStack.pop_back();

				T* typed = godot::Object::cast_to<T>(currentNode);
				if (typed)
				{
					return typed;
				}

				for (int i = 0; i < currentNode->get_child_count(); ++i)
				{
					nodesStack.push_back(currentNode->get_child(i));
				}
			}

			return nullptr;
		}

		template <typename T>
		static std::vector<T*> GetAllChildNodesOfType(godot::Node* target)
		{
			std::vector<T*> nodesOfType;

			if (!target)
			{
				return nodesOfType;
			}

			std::vector<godot::Node*> nodesStack;
			for (int i = 0; i < target->get_child_count(); ++i)
			{
				nodesStack.push_back(target->get_child(i));
			}

			while (!nodesStack.empty())
			{
				godot::Node* currentNode = nodesStack.back();
				nodesStack.pop_back();

				T* typed = godot::Object::cast_to<T>(currentNode);
				if (typed)
				{
					nodesOfType.push_back(typed);
				}

				for (int i = 0; i < currentNode->get_child_count(); ++i)
				{
					nodesStack.push_back(currentNode->get_child(i));
				}
			}

			return nodesOfType;
		}


		template <typename T>
		bool HasSiblingOfType(godot::Node* target)
		{
			godot::TypedArray<godot::Node> siblings = target->get_parent()->get_children();

			for (size_t i = 0; i < siblings.size(); i++)
			{
				if (auto* node = godot::Object::cast_to<T>(siblings[i]);
					node && node != target)
				{
					return true;
				}
			}

			return false;
		}

		godot::Node* GetSceneRoot(godot::Node* owner);
	}
}