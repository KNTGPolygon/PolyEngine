#pragma once

#include <pe/Defines.hpp>
#include <variant>

namespace pe::core::utils
{
	template <typename T, typename E>
	class Result
	{
	public:
		Result() = delete;
		Result(Result&) = delete;
		Result(Result&&) = default;
		Result& operator=(Result&) = delete;
		Result& operator=(Result&&) = default;
		Result(T okVal) : m_value(std::move(okVal)) {}
		Result(E errVal) : m_value(std::move(errVal)) {}
		
		bool isOk() const { return m_value.index() == 0; }
		bool isErr() const { return !isOk(); }
		
		T& getValue() { return std::get<T>(m_value); }
		E& getError() { return std::get<E>(m_value); }
		const T& getValue() const { return std::get<T>(m_value); }
		const E& getError() const { return std::get<E>(m_value); }
	
		Result<T, E> join(Result<Result<T, E>, E> res) 
		{ 
			if (res.isOk())
				return res.getValue() 
			else 
				return res.getError(); 
		}

	private:
		std::variant<T, E> m_value;
	};
	
	template <typename E>
	class Result<void, E> final : public BaseObjectLiteralType<>
	{
	public:
		Result() = default;
		Result(Result&) = delete;
		Result(Result&&) = default;
		Result& operator=(Result&) = delete;
		Result& operator=(Result&&) = default;
		Result(E errVal) : m_value(std::move(errVal)) {}

		bool isOk() const { return m_value.has_value() == false; }
		bool isErr() const { return !isOk(); }

		E& getError() { return m_value.value(); }
		const E& getError() const { return m_value.value(); }
	
	private:
		std::optional<E> m_value;
	};
}
