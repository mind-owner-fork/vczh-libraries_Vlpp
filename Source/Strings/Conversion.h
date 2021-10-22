/***********************************************************************
Author: Zihan Chen (vczh)
Licensed under https://github.com/vczh-libraries/License
***********************************************************************/

#ifndef VCZH_STRINGS_CONVERSION
#define VCZH_STRINGS_CONVERSION

#include "String.h"

namespace vl
{
	namespace encoding
	{

/***********************************************************************
UtfConversion<T>
***********************************************************************/

		template<typename T>
		struct UtfConversion;

		template<>
		struct UtfConversion<wchar_t>
		{
#if defined VCZH_WCHAR_UTF16
			static const vint		BufferLength = 2;
#elif defined VCZH_WCHAR_UTF32
			static const vint		BufferLength = 1;
#endif

			static vint				From32(char32_t source, wchar_t(&dest)[BufferLength]);
			static vint				To32(const wchar_t* source, vint sourceLength, char32_t& dest);
		};

		template<>
		struct UtfConversion<char8_t>
		{
			static const vint		BufferLength = 6;

			static vint				From32(char32_t source, char8_t(&dest)[BufferLength]);
			static vint				To32(const char8_t* source, vint sourceLength, char32_t& dest);
		};

		template<>
		struct UtfConversion<char16_t>
		{
			static const vint		BufferLength = 2;

			static vint				From32(char32_t source, char16_t(&dest)[BufferLength]);
			static vint				To32(const char16_t* source, vint sourceLength, char32_t& dest);
		};

/***********************************************************************
Utfto32ReaderBase<T> and UtfFrom32ReaerBase<T>
***********************************************************************/

		template<typename T, typename TBase>
		class UtfFrom32ReaderBase : public Object
		{
			static const vint		BufferLength = UtfConversion<T>::BufferLength;
			vint					read = 0;
			vint					available = 0;
			T						buffer[BufferLength];
		public:
			T Read()
			{
				if (available == -1) return 0;
				if (read == available)
				{
					char32_t c = static_cast<TBase*>(this)->Consume();
					if (c)
					{
						available = UtfConversion<T>::From32(c, buffer);
						if (available == -1) return 0;
					}
					else
					{
						available = -1;
						return 0;
					}
					read = 0;
				}
				return buffer[read++];
			}
		};

		template<typename T, typename TBase>
		class UtfTo32ReaderBase : public Object
		{
			static const vint		BufferLength = UtfConversion<T>::BufferLength;
			vint					available = 0;
			T						buffer[BufferLength];
		public:
			char32_t Read()
			{
				if (available == -1) return 0;
				while (available < BufferLength)
				{
					T c = static_cast<TBase*>(this)->Consume();
					if (c)
					{
						buffer[available++] = c;
					}
					else
					{
						if (available == 0)
						{
							available = -1;
							return 0;
						}
						break;
					}
				}

				char32_t dest = 0;
				vint result = UtfConversion<T>::To32(buffer, available, dest);
				if (result == -1)
				{
					available = -1;
					return 0;
				}
				available -= result;
				for (vint i = 0; i < available; i++)
				{
					buffer[i] = buffer[i + (BufferLength - available)];
				}
				return dest;
			}
		};

/***********************************************************************
UtfStringTo32Reader<T> and UtfStringFrom32Reader<T>
***********************************************************************/

		template<typename T, typename TBase>
		class UtfStringConsumer : public TBase
		{
		protected:
			const T*				starting = nullptr;
			const T*				consuming = nullptr;

			T Consume()
			{
				T c = *consuming;
				if (c) consuming++;
				return c;
			}
		public:
			UtfStringConsumer(const T* _starting)
				: starting(_starting)
				, consuming(_starting)
			{
			}

			const T* Starting() const
			{
				return starting;
			}

			const T* Current() const
			{
				return consuming;
			}
		};

		template<typename T>
		class UtfStringFrom32Reader : public UtfStringConsumer<char32_t, UtfFrom32ReaderBase<T, UtfStringFrom32Reader<T>>>
		{
			template<typename T, typename TBase>
			friend class UtfFrom32ReaderBase;
		public:
			UtfStringFrom32Reader(const char32_t* _starting)
				: UtfStringConsumer<char32_t, UtfFrom32ReaderBase<T, UtfStringFrom32Reader<T>>>(_starting)
			{
			}
		};

		template<typename T>
		class UtfStringTo32Reader : public UtfStringConsumer<T, UtfTo32ReaderBase<T, UtfStringTo32Reader<T>>>
		{
			template<typename T, typename TBase>
			friend class UtfTo32ReaderBase;
		public:
			UtfStringTo32Reader(const T* _starting)
				: UtfStringConsumer<T, UtfTo32ReaderBase<T, UtfStringTo32Reader<T>>>(_starting)
			{
			}
		};
	}

/***********************************************************************
String Conversions (buffer walkthrough)
***********************************************************************/

	extern vint					_wtoa(const wchar_t* w, char* a, vint chars);
	extern vint					_atow(const char* a, wchar_t* w, vint chars);

	template<typename T>
	vint						_utftou32(const T* s, char32_t* d, vint chars);
	template<typename T>
	vint						_u32toutf(const char32_t* d, T* s, vint chars);

	extern template vint		_utftou32<wchar_t>(const wchar_t* s, char32_t* d, vint chars);
	extern template vint		_utftou32<char8_t>(const char8_t* s, char32_t* d, vint chars);
	extern template vint		_utftou32<char16_t>(const char16_t* s, char32_t* d, vint chars);
	extern template vint		_u32toutf<wchar_t>(const char32_t* s, wchar_t* d, vint chars);
	extern template vint		_u32toutf<char8_t>(const char32_t* s, char8_t* d, vint chars);
	extern template vint		_u32toutf<char16_t>(const char32_t* s, char16_t* d, vint chars);

/***********************************************************************
String Conversions (direct)
***********************************************************************/

	template<typename TFrom, typename TTo, vint(*Convert)(const TFrom*, TTo*, vint)>
	ObjectString<TTo>			ConvertStringDirect(const ObjectString<TFrom>& source);

	extern template AString		ConvertStringDirect<wchar_t, char, _wtoa>(const WString& source);
	extern template WString		ConvertStringDirect<char, wchar_t, _atow>(const AString& source);

	extern template U32String	ConvertStringDirect<wchar_t, char32_t, _utftou32<wchar_t>>(const WString& source);
	extern template WString		ConvertStringDirect<char32_t, wchar_t, _u32toutf<wchar_t>>(const U32String& source);

	extern template U32String	ConvertStringDirect<char8_t, char32_t, _utftou32<char8_t>>(const U8String& source);
	extern template U8String	ConvertStringDirect<char32_t, char8_t, _u32toutf<char8_t>>(const U32String& source);

	extern template U32String	ConvertStringDirect<char16_t, char32_t, _utftou32<char16_t>>(const U16String& source);
	extern template U16String	ConvertStringDirect<char32_t, char16_t, _u32toutf<char16_t>>(const U32String& source);

	/// <summary>Convert a Unicode string to an Ansi string.</summary>
	/// <returns>The converted string.</returns>
	/// <param name="string">The string to convert.</param>
	extern AString				wtoa(const WString& string);
	/// <summary>Convert an Ansi string to an Unicode string.</summary>
	/// <returns>The converted string.</returns>
	/// <param name="string">The string to convert.</param>
	extern WString				atow(const AString& string);

/***********************************************************************
String Conversions (indirect)
***********************************************************************/
}

#endif
