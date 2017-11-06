#pragma once

#if defined(_ENGINE)
#    define EXPORT_TEMPLATE
#else
#    define EXPORT_TEMPLATE extern
#endif

#define REGISTER_COMPONENT(FAMILY, COMPONENT) \
	EXPORT_TEMPLATE template size_t ENGINE_DLLEXPORT FAMILY::GetComponentTypeID<COMPONENT>() noexcept;

namespace Poly
{
	/// <summary>Identifier generator.
	/// Class template that allows for creating groups of types.
	/// In such group every type is uniquely identified by static int value
	/// Use different specializations to create separate sets of identifiers.
	/// </summary>

	/// <para>Example:</para>
	/// <code>
	/// using engine_components_family = Family<struct EngineComponents>;
	/// using world_engine_components_family = Family<struct EngineWorldComponents>;
	/// </code>

	/// <para>
	/// A component identified by this generator and used across dll linking
	/// (f.ex. created in Engine and being used in Game)
	/// MUST(!) define a REGISTER_COMPONENT macro under it's declaration
	/// (i.e. under component class in .hpp file).
	/// Otherwise the two compilation units (f.ex. Engine and Game)
	/// won't know about each other templates (i.e. about unique ids for components).
	/// Definitions are kept in FamilyImpl.hpp file, which is included once
	/// in ComponentBase.hpp. This allows compiler to create a definition
	/// of Family::GetComponentTypeID<U>() type (where U is a type of the component)
	/// where the component is used for the first time (f.ex. created).
	/// https://itk.org/Wiki/Proposals:Explicit_Instantiation#DLL_Symbol_Resolution_on_Windows
	/// </para>
	template<typename T>
	class Family
	{
		static size_t Identifier() noexcept;

	public:
		///<summary>
		/// Returns an unique identifier for the given type.
		///</summary>
		template<typename U>
		static size_t GetComponentTypeID() noexcept;
	};

	using ComponentsFamily = Family<struct ComponentsFamilyType>;
	using WorldComponentsFamily = Family<struct WorldComponentsFamilyType>;
}