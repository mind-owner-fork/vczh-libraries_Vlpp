#include <string.h>
#include "../../../Source/UnitTest/UnitTest.h"
#include "../../../Source/Strings/Conversion.h"

using namespace vl;
using namespace vl::encoding;

namespace test_utf8_reader
{
	template<typename T, vint SourceLength, vint DestLength>
	void TestTo32(const T(&source)[SourceLength], const char32_t(&dest)[DestLength])
	{
	}

	template<typename T, vint SourceLength, vint DestLength>
	void TestFrom32(const char32_t(&source)[SourceLength], const T(&dest)[DestLength])
	{
	}

	template<typename T>
	void TestTo32Boundary(const T(&source)[UtfConversion<T>::BufferLength], char32_t dest)
	{
	}

	template<typename T>
	void TestFrom32Boundary(char32_t source, const T(&dest)[UtfConversion<T>::BufferLength])
	{
	}
}
using namespace test_utf8_reader;

TEST_FILE
{
	const wchar_t textL[] = L"𩰪㦲𦰗𠀼 𣂕𣴑𣱳𦁚 Vczh is genius!@我是天才";
	const char8_t textU8[] = u8"𩰪㦲𦰗𠀼 𣂕𣴑𣱳𦁚 Vczh is genius!@我是天才";
	const char16_t textU16[] = u"𩰪㦲𦰗𠀼 𣂕𣴑𣱳𦁚 Vczh is genius!@我是天才";
	const char32_t textU32[] = U"𩰪㦲𦰗𠀼 𣂕𣴑𣱳𦁚 Vczh is genius!@我是天才";

	TEST_CATEGORY(L"wchar_t -> char32_t")
	{
		TestTo32(textL, textU32);
	});

	TEST_CATEGORY(L"char8_t -> char32_t")
	{
		TestTo32(textU8, textU32);
	});

	TEST_CATEGORY(L"char16_t -> char32_t")
	{
		TestTo32(textU16, textU32);
	});

	TEST_CATEGORY(L"char32_t -> wchar_t")
	{
		TestFrom32(textU32, textL);
	});

	TEST_CATEGORY(L"char32_t -> char8_t")
	{
		TestFrom32(textU32, textU8);
	});

	TEST_CATEGORY(L"char32_t -> char16_t")
	{
		TestFrom32(textU32, textU16);
	});

	TEST_CATEGORY(L"char8_t -> char32_t boundary")
	{
	});

	TEST_CATEGORY(L"char32_t -> char8_t boundary")
	{
	});

	TEST_CATEGORY(L"char16_t -> char32_t boundary")
	{
	});

	TEST_CATEGORY(L"char32_t -> char16_t boundary")
	{
	});
}