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
			vint					read = 0;
			vint					available = 0;
			T						buffer[UtfConversion<T>::BufferLength];
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
		class Utfto32ReaderBase : public Object
		{
		public:
			T Read()
			{
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
				char32_t c = *consuming;
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
		class UtfStringTo32Reader : public UtfStringConsumer<T, Utfto32ReaderBase<T, UtfStringTo32Reader<T>>>
		{
			template<typename T, typename TBase>
			friend class UtfTo32ReaderBase;
		public:
			UtfStringTo32Reader(const T* _starting)
				: UtfStringConsumer<T, Utfto32ReaderBase<T, UtfStringTo32Reader<T>>>(_starting)
			{
			}
		};
	}
}

#endif
