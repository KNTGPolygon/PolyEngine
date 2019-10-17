#pragma once

#include <pe/Defines.hpp>

#include <pe/api/deps/IDependencyPtr.hpp>
#include <pe/api/deps/DependencyManager.hpp>

namespace pe::api::deps {

/// @brief Non-owning smart pointer that holds pointer to the dependency.
/// Dependencies get injected into the instance of this class based on
/// the type provided via template argument.
/// @tparam T Type of the high-level interface of the held dependency instance.
template <typename T>
class DependencyPtr final : public IDependencyPtr
{
public:
	/// Default and only constructor of DependencyPtr
	/// It registers the pointer instance in the DependencyManager.
	DependencyPtr() { DependencyManager::get().registerDependencyPtr(this); }

	/// Destructor of DependencyPtr
	/// It unregisters the pointer instance from the DependencyManager.
	~DependencyPtr() { DependencyManager::get().unregisterDependencyPtr(this); }

	/// @see IDependencyPtr::getMutable()
	IDependency* getMutable() const override
	{ 
		STATIC_ASSERTE(!std::is_const<T>::value, "Cannot cast const to mutable.");
		return m_ptr;
	}

	/// @see IDependencyPtr::get()
	const IDependency* get() const override { return m_ptr; }

	T& operator*() const noexcept { return m_ptr; }
	T* operator->() const noexcept { return m_ptr; }

	explicit operator bool() const noexcept { return m_ptr != nullptr; }

	bool operator==(std::nullptr_t) const { return m_ptr == nullptr; }
	bool operator==(const T* rhs) const { return m_ptr == rhs; }
	bool operator==(const DependencyPtr<T>& rhs) const { return m_ptr == rhs.m_ptr; }
	
	friend std::ostream& operator<< (std::ostream& stream, const DependencyPtr<T>& ptr)
	{
		return stream << "DependencyPtr[" << ptr.get() << "]";
	}
protected:
	void inject(IDependency* dep) override
	{
		ASSERTE(dep, "Cannot inject nullptr!");
		ASSERTE(dep->getType() == getType(), "Invalid type!");
		// This static cast is safe, as the integrality of the types in 
		// dependency injection system is taken care of by the DependencyManager.
		m_ptr = static_cast<T*>(dep);
	}

	void reset() override { m_ptr = nullptr; }
	std::type_index getType() const override { return typeid(T); }

private:
	// Not copyable object, must be managed by DependencyManager
	DependencyPtr(const DependencyPtr&) = delete;
	DependencyPtr(DependencyPtr&&) = delete;
	DependencyPtr& operator=(const DependencyPtr&) = delete;
	DependencyPtr& operator=(DependencyPtr&&) = delete;

	T* m_ptr = nullptr;
};

}