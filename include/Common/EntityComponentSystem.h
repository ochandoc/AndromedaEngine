#pragma once

#include "base.h"

#include "Common/Entity.h"

namespace And
{
	namespace internal
	{
		struct component_list_base 
		{
			virtual void add_empty() = 0;
			virtual void clear_comp(size_t indes) = 0;
		};

		template<typename T>
		struct component_list_imp : public component_list_base
		{
			virtual void add_empty() override
			{
				m_Coponents.push_back(std::nullopt);
			}

			virtual void clear_comp(size_t index)
			{
				m_Coponents[index] = std::nullopt;
			}

			std::vector<std::optional<T>> m_Coponents;
		};
	}

	class EntityComponentSystem
	{
		NON_COPYABLE_CLASS(EntityComponentSystem)
		NON_MOVABLE_CLASS(EntityComponentSystem)
	public:
		EntityComponentSystem() = default;

		~EntityComponentSystem() = default;

		template<typename comp_t>
		void add_component_class()
		{
			std::unique_ptr<internal::component_list_imp<comp_t>> comps = std::make_unique<internal::component_list_imp<comp_t>>();
			m_Components.insert({typeid(comp_t).hash_code(), std::move(comps)});
		}

		template<typename... comps_t>
		Entity new_entity(comps_t... comps)
		{
			m_CurrentId++;
			Entity new_e = m_CurrentId;
			size_t index;
			if (m_EnitiesDeleted.size())
			{
				index = m_EnitiesDeleted.front();
				m_EnitiesDeleted.pop();
			}
			else
			{
				index = m_CurrentIndex;
				m_CurrentIndex++;

				for (auto& comp : m_Components)
				{
					comp.second->add_empty();
				}
			}


			(insert_comp(comps, index), ...);

			m_Entities.insert({new_e, index });
			return new_e;
		}

		void remove_entity(Entity e)
		{
			if (m_Entities.contains(e))
			{
				size_t index = m_Entities[e];
				m_Entities.erase(e);
				for (auto& comp : m_Components)
				{
					comp.second->clear_comp(index);
				}
				m_EnitiesDeleted.push(index);
			}
		}

		template<typename comp_t>
		comp_t* get_entity_component(Entity e)
		{
			if (!e) return nullptr;
			
			size_t type_id = typeid(comp_t).hash_code();
			if (m_Components.contains(type_id))
			{
				internal::component_list_imp<comp_t>* list = CAST_PTR(internal::component_list_imp<comp_t>, m_Components[type_id].get());
				size_t index = m_Entities[e];
				std::optional<comp_t>& opt = list->m_Coponents[index];
				if (opt.has_value())
				{
					return &opt.value();
				}
			}
			return nullptr;
		}

	private:

		template<typename comp_t>
		void insert_comp(comp_t& comp, size_t index)
		{
			size_t type_id = typeid(comp_t).hash_code();
			if (m_Components.contains(type_id))
			{
				internal::component_list_imp<comp_t>* list = CAST_PTR(internal::component_list_imp<comp_t>, m_Components[type_id].get());

				list->m_Coponents[index] = comp;
			}
		}

		std::unordered_map<size_t, std::unique_ptr<internal::component_list_base>> m_Components;
		std::unordered_map<Entity, size_t> m_Entities;
		std::queue<size_t> m_EnitiesDeleted;
		uint64 m_CurrentId = 0;
		size_t m_CurrentIndex = 0;
	};

}