/*********************************************************************
This file is part of the uMOF library.
Copyright (C) 2014 Artem Shal
artiom.shal@gmail.com

The uMOF library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.
**********************************************************************/

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
inline T* any_cast(Any const* operand)
{
	return any_cast<T>(const_cast<Any*>(operand));
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

template <typename T>
T const& any_cast(Any const& operand)
{
	typedef typename std::remove_reference<T>::type nonref;


	return any_cast<nonref const&>(const_cast<Any&>(operand));
}

#endif