/***********************************************************************
Author: Zihan Chen (vczh)
Licensed under https://github.com/vczh-libraries/License
***********************************************************************/

#ifndef VCZH_GLOBALSTORAGE
#define VCZH_GLOBALSTORAGE

#include "Pointer.h"
#include "Strings/String.h"

namespace vl
{
	/// <summary>
	/// Base type of all global storages.
	/// A global storage stores multiple values using a name.
	/// The "BEGIN_GLOBAL_STOREGE_CLASS" macro is recommended to create a global storage.
	/// </summary>
	/// <remarks>
	/// All values are shared like global variables, but they are created at the first time when they need to be accessed.
	/// <see cref="FinalizeGlobalStorage"/> is recommended after you don't need any global storages any more, it frees memory.
	/// </remarks>
	/// <example><![CDATA[
	/// BEGIN_GLOBAL_STORAGE_CLASS(MyStorage)
	///     Ptr<vint> data;
	/// INITIALIZE_GLOBAL_STORAGE_CLASS
	///     data = new vint(100);
	/// FINALIZE_GLOBAL_STORAGE_CLASS
	///     data = nullptr;
	/// END_GLOBAL_STORAGE_CLASS(MyStorage)
	///
	/// int main()
	/// {
	///     // GetMyStorage is generated by defining MyStorage
	///     Console::WriteLine(itow(*GetMyStorage().data.Obj()));
	///     FinalizeGlobalStorage();
	/// }
	/// ]]></example>
	class GlobalStorage : public Object
	{
	private:
		bool					cleared = false;
	public:
		GlobalStorage(const wchar_t* key);
		~GlobalStorage();
		GlobalStorage(const GlobalStorage&) = delete;
		GlobalStorage(GlobalStorage&&) = delete;

		bool					Cleared();
		virtual void			ClearResource() = 0;
	};

	extern GlobalStorage* GetGlobalStorage(const wchar_t* key);
	extern GlobalStorage* GetGlobalStorage(const WString& key);

	extern void InitializeGlobalStorage();
	/// <summary>Free all memories used by global storages.</summary>
	extern void FinalizeGlobalStorage();
}

#define BEGIN_GLOBAL_STORAGE_CLASS(NAME)		\
	class NAME : public vl::GlobalStorage		\
	{											\
	public:										\
		NAME()									\
			:vl::GlobalStorage(L ## #NAME)		\
		{										\
			InitializeClearResource();			\
		}										\
		~NAME()									\
		{										\
			if(!Cleared())ClearResource();		\
		}										\

#define INITIALIZE_GLOBAL_STORAGE_CLASS			\
		void InitializeClearResource()			\
		{										\

#define FINALIZE_GLOBAL_STORAGE_CLASS			\
		}										\
		void ClearResource()					\
		{										\

#define END_GLOBAL_STORAGE_CLASS(NAME)			\
		}										\
	};											\
	NAME& Get##NAME()							\
	{											\
		static NAME __global_storage_##NAME;	\
		return __global_storage_##NAME;			\
	}											\

#define EXTERN_GLOBAL_STORAGE_CLASS(NAME)\
	class NAME;\
	extern NAME& Get##NAME();\

#endif
