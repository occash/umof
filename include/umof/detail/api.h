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

#pragma once

namespace umof
{
    namespace detail
    {
        template<typename T, typename F, typename Yes>
        struct Members
        {
            template<typename Has = Yes>
            static auto table() -> typename std::enable_if<Has::value, F>::type
            {
                return T::table;
            }
            template<typename Has = Yes>
            static auto table() -> typename std::enable_if<!Has::value, F>::type
            {
                return nullptr;
            }
            template<typename Has = Yes>
            static auto size() -> typename std::enable_if<Has::value, unsigned int>::type
            {
                return sizeof(T::table) / sizeof(T::table[0]);
            }
            template<typename Has = Yes>
            static auto size() -> typename std::enable_if<!Has::value, unsigned int>::type
            {
                return 0;
            }
        };

        template<typename T>
        struct Has
        {
            template<class C> static std::true_type test(decltype(C::table));
            template<class C> static std::false_type test(...);
            using Table = decltype(test<T>(0));
        };
    }
}