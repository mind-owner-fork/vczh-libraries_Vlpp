#include "AssertCollection.h"

namespace TestList_TestObjects
{
	void TestArray(Array<vint>& arr)
	{
		arr.Resize(0);
		CHECK_EMPTY_LIST(arr);

		arr.Resize(10);
		for (vint i = 0; i < 10; i++)
		{
			arr.Set(i, i);
		}
		CHECK_LIST_ITEMS(arr, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 });

		arr.Resize(15);
		for (vint i = 10; i < 15; i++)
		{
			arr.Set(i, i * 2);
		}
		CHECK_LIST_ITEMS(arr, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 20 _ 22 _ 24 _ 26 _ 28 });

		arr.Resize(5);
		CHECK_LIST_ITEMS(arr, { 0 _ 1 _ 2 _ 3 _ 4 });
	}

	template<typename TCollection>
	void TestCollectionWithIncreasingItems(TCollection& collection)
	{
		collection.Clear();
		CHECK_EMPTY_LIST(collection);

		collection.Add(0);
		CHECK_LIST_ITEMS(collection, { 0 });

		for (vint i = 1; i < 10; i++)
		{
			collection.Add(i);
		}
		CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 });

		collection.RemoveAt(2);
		CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 });

		collection.RemoveRange(3, 4);
		CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 3 _ 8 _ 9 });

		TEST_ASSERT(collection.Remove(3) == true);
		CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 8 _ 9 });

		TEST_ASSERT(collection.Remove(9) == true);
		CHECK_LIST_ITEMS(collection, { 0 _ 1 _ 8 });

		TEST_ASSERT(collection.Remove(0) == true);
		CHECK_LIST_ITEMS(collection, { 1 _ 8 });

		for (vint i = 0; i < 10; i++)
		{
			if (i != 1 && i != 8)
			{
				TEST_ASSERT(collection.Remove(i) == false);
				CHECK_LIST_ITEMS(collection, { 1 _ 8 });
			}
		}

		collection.Clear();
		CHECK_EMPTY_LIST(collection);
	}

	template<typename TCollection>
	void TestSortedCollection(TCollection& collection)
	{
		vint items[] = { 7, 1, 12, 2, 8, 3, 11, 4, 9, 5, 13, 6, 10 };
		collection.Clear();
		for (vint i = 0; i < sizeof(items) / sizeof(*items); i++)
		{
			collection.Add(items[i]);
		}
		CHECK_LIST_ITEMS(collection, { 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });

		TEST_ASSERT(collection.Remove(7) == true);
		CHECK_LIST_ITEMS(collection, { 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });

		TEST_ASSERT(collection.Remove(1) == true);
		CHECK_LIST_ITEMS(collection, { 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 });

		TEST_ASSERT(collection.Remove(12) == true);
		CHECK_LIST_ITEMS(collection, { 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 13 });

		TEST_ASSERT(collection.Remove(2) == true);
		CHECK_LIST_ITEMS(collection, { 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 13 });

		TEST_ASSERT(collection.Remove(8) == true);
		CHECK_LIST_ITEMS(collection, { 3 _ 4 _ 5 _ 6 _ 9 _ 10 _ 11 _ 13 });

		TEST_ASSERT(collection.Remove(3) == true);
		CHECK_LIST_ITEMS(collection, { 4 _ 5 _ 6 _ 9 _ 10 _ 11 _ 13 });

		for (vint i = 0; i < sizeof(items) / sizeof(*items); i++)
		{
			collection.Add(items[i]);
		}
		CHECK_LIST_ITEMS(collection, { 1 _ 2 _ 3 _ 4 _ 4 _ 5 _ 5 _ 6 _ 6 _ 7 _ 8 _ 9 _ 9 _ 10 _ 10 _ 11 _ 11 _ 12 _ 13 _ 13 });

		collection.Clear();
		CHECK_EMPTY_LIST(collection);
	}

	template<typename TList>
	void TestNormalList(TList& list)
	{
		list.Clear();
		for (vint i = 0; i < 10; i++)
		{
			list.Insert(i / 2, i);
		}
		CHECK_LIST_ITEMS(list, { 1 _ 3 _ 5 _ 7 _ 9 _ 8 _ 6 _ 4 _ 2 _ 0 });

		for (vint i = 0; i < 10; i++)
		{
			list.Set(i, 9 - i);
		}
		CHECK_LIST_ITEMS(list, { 9 _ 8 _ 7 _ 6 _ 5 _ 4 _ 3 _ 2 _ 1 _ 0 });

		for (vint i = 0; i < 10; i++)
		{
			list.Set(i, i * 2);
		}
		CHECK_LIST_ITEMS(list, { 0 _ 2 _ 4 _ 6 _ 8 _ 10 _ 12 _ 14 _ 16 _ 18 });
	}

	void TestSortedDictionary(Dictionary<vint, vint>& dictionary)
	{
		dictionary.Clear();
		CHECK_EMPTY_DICTIONARY(dictionary);

		dictionary.Add(1, 1);
		dictionary.Add(2, 4);
		dictionary.Add(3, 9);
		dictionary.Add(4, 16);
		CHECK_DICTIONARY_ITEMS(dictionary, { 1 _ 2 _ 3 _ 4 }, { 1 _ 4 _ 9 _ 16 });

		dictionary.Set(1, -1);
		dictionary.Set(2, -4);
		dictionary.Set(3, -9);
		dictionary.Set(4, -16);
		dictionary.Set(5, -25);
		CHECK_DICTIONARY_ITEMS(dictionary, { 1 _ 2 _ 3 _ 4 _ 5 }, { -1 _ - 4 _ - 9 _ - 16 _ - 25 });

		dictionary.Remove(4);
		dictionary.Remove(5);
		dictionary.Remove(6);
		CHECK_DICTIONARY_ITEMS(dictionary, { 1 _ 2 _ 3 }, { -1 _ - 4 _ - 9 });

		dictionary.Clear();
		CHECK_EMPTY_DICTIONARY(dictionary);
	}

	void TestSortedGroup(Group<vint, vint>& group)
	{
		group.Clear();
		CHECK_EMPTY_GROUP(group);

		for (vint i = 0; i < 20; i++)
		{
			group.Add(i / 5, i);
		}
		CHECK_GROUP_ITEMS(group, { 0 _ 1 _ 2 _ 3 }, { 0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 }, { 5 _ 5 _ 5 _ 5 });

		group.Remove(1);
		CHECK_GROUP_ITEMS(group, { 0 _ 2 _ 3 }, { 0 _ 1 _ 2 _ 3 _ 4 _ 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19 }, { 5 _ 5 _ 5 });

		for (vint i = 13; i < 18; i++)
		{
			group.Remove(i / 5, i);
		}
		CHECK_GROUP_ITEMS(group, { 0 _ 2 _ 3 }, { 0 _ 1 _ 2 _ 3 _ 4 _ 10 _ 11 _ 12 _ 18 _ 19 }, { 5 _ 3 _ 2 });

		group.Clear();
		CHECK_EMPTY_GROUP(group);
	}

	bool dividable(vint a, vint b)
	{
		return b % a == 0;
	}
}

using namespace TestList_TestObjects;

TEST_FILE
{
	TEST_CASE(L"Test Array<T>")
	{
		Array<vint> arr;
		TestArray(arr);
	});

	TEST_CASE(L"Test SortedList<T>")
	{
		SortedList<vint> list;
		TestCollectionWithIncreasingItems(list);
		TestSortedCollection(list);
	});

	TEST_CASE(L"Test List<T>")
	{
		List<vint> list;
		TestCollectionWithIncreasingItems(list);
		TestNormalList(list);
	});

	TEST_CASE(L"Test Dictionary<K, V>")
	{
		Dictionary<vint, vint> dictionary;
		TestSortedDictionary(dictionary);
	});

	TEST_CASE(L"Test Group<K, V>")
	{
		Group<vint, vint> group;
		TestSortedGroup(group);
	});
}