#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <typeinfo>
#include <type_traits>

//Bool template type
template<bool B> struct Bool;
typedef Bool<true> True;
typedef Bool<false> False;

//Bool implementation
template<bool B>
struct Bool
{
	static const bool value = B;
	typedef Bool type;
};

//Remove all quilifiers
template<typename T, typename Ptr>
struct CheckType;

template<typename T>
struct CheckType<T, True>
{
	typedef typename std::add_pointer<
		    typename std::remove_cv<
			typename std::remove_pointer<T>
			::type>::type>::type type;
};

template<typename T>
struct CheckType<T, False>
{
	typedef typename std::remove_cv<T>::type type;
};

//Table with basic functions
struct TypeTable
{
	std::type_info const&(*get_type)();
	void(*static_delete)(void**);
	void(*destruct)(void**);
	void(*clone)(void* const*, void**);
	void(*move)(void* const*, void**);
};

//Basic function implementation
template<typename Small>
struct TypeFuncs;

//Basic functions for small type
template<>
struct TypeFuncs<True>
{
	template<typename T>
	struct type
	{
		static std::type_info const& get_type()
		{
			return typeid(T);
		}
		static void static_delete(void** x)
		{
			reinterpret_cast<T*>(x)->~T();
		}
		static void destruct(void** x)
		{
			reinterpret_cast<T*>(x)->~T();
		}
		static void clone(void* const* src, void** dest)
		{
			new (dest)T(*reinterpret_cast<T const*>(src));
		}
		static void move(void* const* src, void** dest)
		{
			*reinterpret_cast<T*>(dest) =
				*reinterpret_cast<T const*>(src);
		}
	};
};

//Basic functions for larger type
template<>
struct TypeFuncs<False>
{
	template<typename T>
	struct type
	{
		static std::type_info const& get_type()
		{
			return typeid(T);
		}
		static void static_delete(void** x)
		{
			// destruct and free memory
			delete (*reinterpret_cast<T**>(x));
		}
		static void destruct(void** x)
		{
			// destruct only, we'll reuse memory
			(*reinterpret_cast<T**>(x))->~T();
		}
		static void clone(void* const* src, void** dest)
		{
			*dest = new T(**reinterpret_cast<T* const*>(src));
		}
		static void move(void* const* src, void** dest)
		{
			**reinterpret_cast<T**>(dest) =
				**reinterpret_cast<T* const*>(src);
		}
	};
};

//Get the table for type
template<typename T>
struct Table
{
	typedef Bool<(sizeof(T) <= sizeof(void*))> is_small;
	typedef Bool<std::is_pointer<T>::value> is_pointer;
	typedef typename CheckType<T, is_pointer>::type T_no_cv;

	static TypeTable *get()
	{
		static TypeTable staticTable
		{
			TypeFuncs<is_small>::type<T_no_cv>::get_type,
			TypeFuncs<is_small>::type<T_no_cv>::static_delete,
			TypeFuncs<is_small>::type<T_no_cv>::destruct,
			TypeFuncs<is_small>::type<T_no_cv>::clone,
			TypeFuncs<is_small>::type<T_no_cv>::move
		};
		return &staticTable;
	}
};

#endif