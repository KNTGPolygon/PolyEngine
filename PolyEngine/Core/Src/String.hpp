#pragma once

#include "Dynarray.hpp"
#include "Defines.hpp"

namespace Poly {

	size_t StrLen(const char* str);

	class CORE_DLLEXPORT String : BaseObject<> {
	public:
		String() : String("") {};
		String(const char* data);
		String(const String& rhs);
		String(String&& rhs);

		static String From(int var);
		static String From(float var, int precision = 2);
		static String From(double var, int precision = 2);
		static String From(char var); //??
		static String From(const char* var);
		static String From(const std::string& var);

		bool Contains(const String& var) const;
		bool Contains(char var) const;
		String ToLower() const;
		String ToUpper() const;
		bool IsEmpty() const;
		String Replace(char a, char b) const;
		String Replace(const String& a, const String& b) const;
		String Split(char delimiter) const;
		String Split(const String& delimiter) const;
		static String Join(const String* vars, size_t size, const String& separator);
		static String Join(const String* vars, size_t size, char separator);
		bool StartsWith(char var) const;
		bool EndsWith(char var) const;
		String Substring(int end) const;
		String Substring(int start, int end) const;
		String GetTrimmed() const;


		String& operator=(const String& rhs);
		String& operator=(String&& rhs);
		bool operator==(const char* str) const;
		bool operator==(const String& str) const;
		bool operator<(const String& rhs) const;
		String operator+(const String& rhs) const;
		String operator+(char rhs) const;
		char operator[](int idx) const;
		size_t GetLength() const { return Data.GetSize() - 1; }
		const char* GetCStr() const { return Data.GetData(); }

		/*CORE_DLLEXPORT*/ friend std::ostream& operator<< (std::ostream& stream, const String& rhs) { return stream << rhs.GetCStr(); }

	private:
		Dynarray<char> Data;
	};
}
