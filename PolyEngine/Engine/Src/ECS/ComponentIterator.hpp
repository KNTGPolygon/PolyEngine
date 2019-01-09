#pragma once

#include "Defines.hpp"

namespace Poly
{
	template<typename T>
	class Dynarray;
	class ComponentBase;
	class Scene;
	template<typename PrimaryComponent, typename... SecondaryComponents>
	struct IteratorProxy;

	class IEntityIteratorHelper : public BaseObject<>
	{
		public:
			virtual bool operator==(const IEntityIteratorHelper&) const = 0;
			virtual bool operator!=(const IEntityIteratorHelper&) const = 0;
			virtual Entity* get() const = 0;
			virtual void increment() = 0;
			virtual bool isValid() const = 0;
	};

	template<typename PrimaryComponent, typename... SecondaryComponents>
	class ComponentIterator : public BaseObject<>,
							public std::iterator<std::forward_iterator_tag, std::tuple<typename std::add_pointer<PrimaryComponent>::type, typename std::add_pointer<SecondaryComponents>::type...>>	
	{
		public:
			explicit ComponentIterator(std::unique_ptr<IEntityIteratorHelper> iter) : Iter(std::move(iter))
			{
				UpdateCache();
			}
			bool operator==(const  ComponentIterator& rhs) const { return Iter.get() == rhs.Get(); }
			bool operator!=(const  ComponentIterator& rhs) const { return !(Iter.get() == rhs.Get()); }

			std::tuple<typename std::add_pointer<PrimaryComponent>::type, typename std::add_pointer<SecondaryComponents>::type... > operator*() const //canot be const if  change cache
			{
				return GetCache();
			}
			std::tuple<typename std::add_pointer<PrimaryComponent>::type, typename std::add_pointer<SecondaryComponents>::type... > operator->() const
			{
				return GetCache();
			}

			ComponentIterator& operator++() { Increment(); return *this; }
			ComponentIterator operator++(int) { ComponentIterator ret(Iter); Increment(); return ret; } //test for double incrementing etc

			IEntityIteratorHelper* Get() const
			{
				return Iter.get();
			}
		private:
			void Increment()
			{
				Iter.get()->increment();
				ASSERTE(Iter.get()->isValid(), "Next Iterator is not valid!");		
				bCacheValid = false;
			}

			void UpdateCache() const 
			{
				Entity* ent = Iter.get()->get();
				PrimaryComponent* primary = ent->GetComponent<PrimaryComponent>();
				if constexpr (!Trait::IsVariadicEmpty<SecondaryComponents...>::value)
				{
					Cache = std::make_tuple(primary, primary->template GetSibling<SecondaryComponents>()...);
					bCacheValid = true;
					return;
				}
				else
				{
					Cache = std::make_tuple(primary);
				}

				bCacheValid = true;
			}

			std::tuple<typename std::add_pointer<PrimaryComponent>::type, typename std::add_pointer<SecondaryComponents>::type... > GetCache() const
			{
				if (!bCacheValid)
					UpdateCache();
				Entity* ent = Iter.get()->get();
				if (!HasComponents<PrimaryComponent, SecondaryComponents...>(ent))
					ASSERTE(false, "Updated cache does not contain all needed components!"); //@fixme: celeborth, when put in asserte clause it fails (passing to macro fails template deduction?)
				return Cache;
			}

			//------------------------------------------------------------------------------
			template<int zero = 0>
			bool HasComponents(const Entity* entity) const { return true; }

			//------------------------------------------------------------------------------
			template<typename Component, typename... Rest>
			bool HasComponents(const Entity* entity) const
			{
				return entity->template HasComponent<Component>() && HasComponents<Rest...>(entity);
			}

			friend struct IteratorProxy<PrimaryComponent, SecondaryComponents...>;

			mutable bool bCacheValid;
			std::unique_ptr<IEntityIteratorHelper> Iter;
			mutable std::tuple<typename std::add_pointer<PrimaryComponent>::type, typename std::add_pointer<SecondaryComponents>::type... > Cache;
	};

	/// Iterator proxy
	template<typename PrimaryComponent, typename... SecondaryComponents>
	struct IteratorProxy : BaseObject<>
	{
		IteratorProxy(Scene* s) : S(s) {}
		ComponentIterator<PrimaryComponent, SecondaryComponents...> Begin()
		{
			return ComponentIterator<PrimaryComponent, SecondaryComponents...>(S->MakeSceneComponentIteratorHelper<PrimaryComponent, SecondaryComponents...>());
		}
		ComponentIterator<PrimaryComponent, SecondaryComponents...> End() //better pass scene and move this method inside component (13.12. need to be sure about h
		{
			return ComponentIterator<PrimaryComponent, SecondaryComponents...>(S->MakeSceneComponentIteratorHelper<PrimaryComponent, SecondaryComponents...>());
		}
		auto begin() { return Begin(); }
		auto end() { return End(); }
		Scene* const S;
	};
}