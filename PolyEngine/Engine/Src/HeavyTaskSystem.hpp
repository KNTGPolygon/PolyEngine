#pragma once

#include "HeavyTaskImplementation.hpp"
#include "World.hpp"

namespace Poly
{
	namespace HeavyTaskSystem
	{
		void HeavyTaskPhase(World* w);

		inline UniqueID SpawnEntityImmediate(World* w)
		{
			SpawnEntityHeavyTask *task = new SpawnEntityHeavyTask();
			task->Execute(w);
			gConsole.LogDebug("New immediate task executed: {}",task->GetDescription());
			UniqueID ID = task->GetID();
			delete task;
			return ID;
		}

		inline void DestroyEntity(World* w, const UniqueID& entityId)
		{
			w->GetHeavyTaskQueue().Push(new DestroyEntityHeavyTask(entityId));
			gConsole.LogDebug("New task scheduled: {}", w->GetHeavyTaskQueue().Back()->GetDescription());
		}

		template<typename T, typename ...Args>
		void AddComponent(World* w, const UniqueID & entityId, Args && ...args)
		{
			//HeavyTaskBase *task = new AddComponentHeavyTask<T,Args...>(entityId, std::forward<Args>(args)...);
			w->GetHeavyTaskQueue().Push(new AddComponentHeavyTask<T, typename std::conditional<!std::is_array<typename std::remove_reference<Args>::type>::value, Args, typename std::decay<Args>::type>::type...>(entityId, std::forward<Args>(args)...));
			gConsole.LogDebug("New task scheduled: {}", w->GetHeavyTaskQueue().Back()->GetDescription());
		}

		template<typename T, typename ...Args>
		void AddComponentImmediate(World* w, const UniqueID & entityId, Args && ...args)
		{
			//HeavyTaskBase *task = new AddComponentHeavyTask<T,Args...>(entityId, std::forward<Args>(args)...);
			HeavyTaskBase *task = new AddComponentHeavyTask<T, typename std::conditional<!std::is_array<typename std::remove_reference<Args>::type>::value, Args, typename std::decay<Args>::type>::type...>(entityId, std::forward<Args>(args)...);
			task->Execute(w);
			gConsole.LogDebug("New immediate task executed: {}", task->GetDescription());
			delete task;
		}

		template<typename T>
		void RemoveComponent(World* w, const UniqueID & entityId)
		{
			w->GetHeavyTaskQueue().Push(new RemoveComponentHeavyTask<T>(entityId));
			gConsole.LogDebug("New task scheduled: {}", w->GetHeavyTaskQueue().Back()->GetDescription());
		}
	}
}
