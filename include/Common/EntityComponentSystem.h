#pragma once

#include "base.h"

#include "Common/ID.h"
#include "Common/Entity.h"

namespace And
{
	namespace internal
	{
		template<typename T>
		struct component
		{
			component(ID i) : id(i), value() {}
			component(ID i, const T& v) : id(i) { value = v; }
			ID id;
			T value;
			
			bool operator <(const component& other) { return id < other.id; }
			bool operator >(const component& other) { return id > other.id; }
			bool operator ==(const component& other)	{	return id == other.id;	}
		};

		template<typename T>
		class component_list_iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = component<T>;
			using pointer = component<T>*;
			using reference = component<T>&;

			component_list_iterator(component<T>* ptr) : m_Ptr(ptr) {}
			component_list_iterator(const component_list_iterator& other) { m_Ptr = other.m_Ptr; }
			component_list_iterator(component_list_iterator&& other) { m_Ptr = other.m_Ptr; }

			~component_list_iterator() {}

			component_list_iterator& operator =(const component_list_iterator& other) { if (this != &other) { m_Ptr = other.m_Ptr; } return *this; }
			component_list_iterator& operator =(component_list_iterator&& other) { if (this != &other) { std::swap(m_Ptr, other.m_Ptr); } return *this; }

			component<T>& operator *() { return *m_Ptr; }
			const component<T>& operator *() const { return *m_Ptr; }

			component<T>* operator ->() { return m_Ptr; }
			const component<T>* operator ->() const { return m_Ptr; }

			component_list_iterator& operator++() { m_Ptr++; return *this; }
			component_list_iterator operator++(int) { component_list_iterator tmp = *this; ++(*this); return tmp; }

			bool operator ==(const component_list_iterator& other) { return m_Ptr == other.m_Ptr; }
			bool operator !=(const component_list_iterator& other) { return m_Ptr != other.m_Ptr; }

		private:
			component<T>* m_Ptr;
		};

		class component_list_abs
		{
		public:
			component_list_abs() = default;

			virtual ~component_list_abs() = default;

			virtual void add_empty(ID id) = 0;
			virtual void remove(ID id) = 0;
			virtual size_t size() = 0;
		};

		template<typename T>
		class component_list_base : public component_list_abs
		{
		public:
			component_list_base() = default;

			virtual ~component_list_base() = default;

			virtual T* get_component(ID id) = 0;
			virtual void add(ID id, const T& value) = 0;
		};

		template<typename T>
		class component_list_imp : public component_list_base<T>
		{
		public:
			using iterator = component_list_iterator<T>;

			component_list_imp() = default;

			virtual ~component_list_imp() = default;

			virtual void add_empty(ID id) override
			{
				m_Sorted = false;
				m_Components.push_back(component<T>(id));
			}

			virtual void add(ID id, const T& value)
			{
				m_Sorted = false;
				m_Components.emplace_back(id, value);
			}

			virtual void remove(ID id) override
			{
				sort();
				component<T> tmp(id);
				auto first = m_Components.begin();
				auto last = m_Components.end();
				first = std::lower_bound(first, last, tmp);
				if ((first != last) && !(tmp < *first))
				{
					m_Components.erase(first);
				}
				//m_Sorted = false;
			}

			virtual size_t size() override
			{
				return m_Components.size();
			}

			void sort()
			{
				if (!m_Sorted)
				{
					std::sort(m_Components.begin(), m_Components.end(), [](const component<T>& c1, const component<T>& c2) { return c1.id.get() < c2.id.get(); });
					m_Sorted = true;
				}
			}

			virtual T* get_component(ID id) override
			{
				sort();
				component<T> tmp(id);
				auto first = m_Components.begin();
				auto last = m_Components.end();
				first = std::lower_bound(first, last, tmp);
				if ((first != last) && !(tmp < *first))
				{
					return &first->value;
				}
				return nullptr;
			}

			iterator begin()
			{
				sort();
				return iterator(m_Components.data());
			}

			iterator end()
			{
				sort();
				return iterator(m_Components.data() + m_Components.size());
			}

			template<typename...>
			friend class tuple_iterator;
			friend class EntityComponentSystem;
		private:
			std::vector<component<T>> m_Components;
			bool m_Sorted;
		};

		template<typename... comps_t>
		class tuple_iterator
		{
		public:
			tuple_iterator(component_list_imp<comps_t>&... comps_lists) : m_IteratorBegin(comps_lists.begin()...), m_IteratorEnd(comps_lists.end()...)	
			{
			}
			tuple_iterator(const tuple_iterator& other) : m_IteratorBegin(other.m_IteratorBegin), m_IteratorEnd(other.m_IteratorEnd) {}
			tuple_iterator(tuple_iterator&& other) : m_IteratorBegin(other.m_IteratorBegin), m_IteratorEnd(other.m_IteratorEnd) {}

			~tuple_iterator() = default;

			tuple_iterator& operator =(const tuple_iterator& other) { if (this != &other) { m_IteratorBegin = other.m_IteratorBegin; m_IteratorEnd = other.m_IteratorEnd; } return *this; }
			tuple_iterator& operator =(tuple_iterator&& other) { if (this != &other) { std::swap(m_IteratorBegin, other.m_IteratorBegin); std::swap(m_IteratorEnd, other.m_IteratorEnd); } return *this; }

			tuple_iterator& operator++()
			{
				auto& it = std::get<0>(m_IteratorBegin);
				++it;
				next_result_fold result{it->id, true, false};
				do
				{
					result = next_all(std::make_integer_sequence<int, sizeof...(comps_t)>{}, result.max);

				} while (!result.equal && !result.finished);
				return *this;
			}

		private:

			struct next_result_fold
			{
				uint64 max;
				bool equal;
				bool finished;

				next_result_fold operator +(uint64 value)
				{
					if (finished) return *this;
					if (value == 0) return { 0, false, true };
					if (max)
					{
						if (equal && value != max)
						{
							equal = false;
						}
						if (value > max)
						{
							max = value;
						}
					}
					else
					{
						max = value;
					}
					return *this;
				}
			};

			template<int... ints>
			next_result_fold next_all(std::integer_sequence<int, ints...> int_seq, uint64 id)
			{
				next_result_fold initial{0, true, false};
				(initial + ... + (next(std::get<ints>(m_IteratorBegin), std::get<ints>(m_IteratorEnd), id)));
				return initial;
			}

			template<typename T>
			uint64 next(component_list_iterator<T>& first, component_list_iterator<T>& end, uint64 id)
			{
				while (first != end && first->id.get() < id)
				{
					first++;
				}
				if (first == end)
				{
					return 0;
				}
				return first->id;
			}

			std::tuple<component_list_iterator<comps_t>...> m_IteratorBegin;
			std::tuple<component_list_iterator<comps_t>...> m_IteratorEnd;
		};
		/*
		template<typename T, typename... comps_t>
		class tuple_iterator
		{
		public:
			tuple_iterator(component<T>* less_comp) : m_Ptr(less_comp) {}
			tuple_iterator(component_list_imp<T>& less_comp, component_list_imp<comps_t>&... comps_lists) : m_Ptr(less_comp.m_Components.data()), m_CompsList(comps_lists...), m_ValidComps(nullptr)
			{ 
				bool ok = true;
				do
				{
					ok = get_components(std::make_integer_sequence<int, sizeof...(comps_t)>{}, m_Ptr->id);
					if (!ok)
					{
						m_Ptr++;
					}
				} while (!ok);
			}
			tuple_iterator(const tuple_iterator& other) : m_Ptr(other.m_Ptr), m_CompsList(other.m_CompsList), m_ValidComps(other.m_ValidComps) {}
			tuple_iterator(tuple_iterator&& other) : m_Ptr(other.m_Ptr), m_CompsList(other.m_CompsList), m_ValidComps(other.m_ValidComps) {}

			~tuple_iterator() = default;

			tuple_iterator& operator =(const tuple_iterator& other) { if (this != &other) { m_Ptr = other.m_Ptr; m_CompsList = other.m_CompsList; m_ValidComps = other.m_ValidComps; } return *this; }
			tuple_iterator& operator =(tuple_iterator&& other) { if (this != &other) { std::swap(m_Ptr, other.m_Ptr); std::swap(m_CompsList, other.m_CompsList); std::swap(m_ValidComps, other.m_ValidComps); } return *this; }

			// TODO do something
			tuple_iterator& operator ++()
			{
				bool ok = true;
				do
				{
					m_Ptr++;
					ok = get_components(std::make_integer_sequence<int, sizeof...(comps_t)>{}, m_Ptr->id);
				} while (!ok);
				return *this;
			}

			tuple_iterator operator ++(int)
			{
				tuple_iterator tmp = *this; ++(*this); return tmp;
			}

			const std::tuple<T*, comps_t*...> operator *() const
			{
				return get_tuple(std::make_integer_sequence<int, sizeof...(comps_t)>{});
			}

			bool operator ==(const tuple_iterator& other) { return m_Ptr == other.m_Ptr; }
			bool operator !=(const tuple_iterator& other) { return m_Ptr != other.m_Ptr; }

		private:

			template<int... ints>
			const std::tuple<T*, comps_t*...> get_tuple(std::integer_sequence<int, ints...> int_seq) const
			{
				return { &m_Ptr->value, ((std::get<ints>(m_ValidComps)), ...) };
			}

			template<int... ints>
			bool get_components(std::integer_sequence<int, ints...> int_seq, ID id)
			{
				((std::get<ints>(m_ValidComps) = std::get<ints>(m_CompsList).get_component(id)), ...);
				return ((std::get<ints>(m_ValidComps) != nullptr) && ...);
			}

			component<T>* m_Ptr;
			std::tuple<component_list_imp<comps_t>&...> m_CompsList;
			std::tuple<comps_t*...> m_ValidComps;
		};
		*/
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
			m_Components.insert({typeid(comp_t).hash_code(), std::make_unique<internal::component_list_imp<comp_t>>()});
		}

		template<typename... comps_t>
		Entity new_entity(comps_t... comps)
		{
			Entity new_e;

			(add_entity_component<comps_t>(new_e, comps), ...);

			return new_e;
		}

		void remove_entity(Entity e)
		{
			for (auto& [key, value] : m_Components)
			{
				value->remove(e.get_id());
			}
		}

		template<typename comp_t>
		comp_t* get_entity_component(Entity e)
		{
			size_t type_id = typeid(comp_t).hash_code();
			internal::component_list_imp<comp_t>* list = CAST_PTR(internal::component_list_imp<comp_t>, m_Components[type_id].get());
			return list->get_component(e.get_id());
		}

		template<typename comp_t>
		void add_entity_component(Entity e, comp_t& comp)
		{
			size_t type_id = typeid(comp_t).hash_code();
			internal::component_list_imp<comp_t>* list = CAST_PTR(internal::component_list_imp<comp_t>, m_Components[type_id].get());
			assert(list->get_component(e.get_id()) == nullptr && "Component already inserted in the entity");
			list->add(e.get_id(), comp);
		}

		template<typename comp_t>
		void remove_entity_component(Entity e)
		{
			size_t type_id = typeid(comp_t).hash_code();
			internal::component_list_imp<comp_t>* list = CAST_PTR(internal::component_list_imp<comp_t>, m_Components[type_id].get());
			assert(list->get_component(e.get_id()) != nullptr && "Component already inserted in the entity");
			list->remove(e.get_id());
		}

		template<typename func_t, typename... comps_t>
		void execute_system(func_t system)
		{
			internal::tuple_iterator<comps_t...> it();


		}

	private:
		std::unordered_map<size_t, std::unique_ptr<internal::component_list_abs>> m_Components;
		uint64 m_CurrentId = 0;
	};

}