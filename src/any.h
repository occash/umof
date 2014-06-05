#ifndef ANY_H
#define ANY_H

#include "type_traits.h"
#include <new>

class Any
{
public:
	Any();
	Any(Any const& x);
	~Any();

	template<typename T>
	Any(T const& x);
	template<typename T, std::size_t N>
	Any(T(&x)[N]);

	void reset();
	std::type_info const& type() const;

private:
	template <typename T>
	friend T* any_cast(Any*);

	TypeTable* _table;
	void* _object;
};

template<typename T>
Any::Any(T const& x) :
	_table(Table<T>::get()),
	_object(nullptr)
{
	if (Table<T>::is_small::value)
		new (&_object) T(x);
	else
		_object = new T(x);
}

template<typename T, std::size_t N>
Any::Any(T(&x)[N]) :
_table(Table<T*>::get()),
_object(nullptr)
{
	if (Table<T*>::is_small::value)
		new (&_object) T*(&x[0]);
	else
		_object = new T*(&x[0]);
}

template <typename T>
inline T* any_cast(Any* operand)
{
	typedef Bool<std::is_pointer<T>::value> is_pointer;
	typedef typename CheckType<T, is_pointer>::type T_no_cv;

	if (operand && operand->type() == typeid(T_no_cv)) {
		return Table<T>::is_small::value ?
			const_cast<T*>(reinterpret_cast<T_no_cv*>(&operand->_object)) :
			const_cast<T*>(reinterpret_cast<T_no_cv*>(operand->_object));
	}

	return nullptr;
}

template <typename T>
T any_cast(Any& operand)
{
	typedef typename std::remove_reference<T>::type nonref;

	nonref* result = any_cast<nonref>(&operand);
	if (!result)
		throw std::runtime_error("Bad cast");
	return *result;
}

#endif