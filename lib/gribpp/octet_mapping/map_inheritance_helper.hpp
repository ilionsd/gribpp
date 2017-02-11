/*
 * inheritance_helper.hpp
 *
 *  Created on: Jan 28, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_OCTET_MAPPING_MAP_INHERITANCE_HELPER_HPP_
#define _GRIBPP_OCTET_MAPPING_MAP_INHERITANCE_HELPER_HPP_

#include <initializer_list>

namespace gribpp {
	namespace octet_mapping {\

		template<typename __Base, typename __Derived>
		struct map_inheritance_helper :
			public __Base
		{
			using base_type		= __Base;
			using derived_type	= __Derived;

			using map_type		= typename base_type::map_type;
			using value_type	= typename base_type::value_type;
			using key_type		= typename base_type::key_type;
			using mapped_type	= typename base_type::mapped_type;

			inline map_inheritance_helper() :
				base_type()
			{};
			inline map_inheritance_helper(const derived_type& other) :
				base_type(other)
			{};
			inline map_inheritance_helper(derived_type&& other) :
				base_type(other)
			{};
			inline map_inheritance_helper(std::initializer_list<value_type> initList) :
				base_type(initList)
			{};

			inline derived_type& operator= (const derived_type& other) {
				base_type::operator=(other);
				return static_cast<derived_type&>(*this);
			};
			inline derived_type& operator= (std::initializer_list<value_type> initList) {
				base_type::operator=(initList);
				return static_cast<derived_type&>(*this);
			};
		};

	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --



#endif /* _GRIBPP_OCTET_MAPPING_MAP_INHERITANCE_HELPER_HPP_ */
