#ifndef _GRIBPP_TYPE_TRAITS_HPP_
#define _GRIBPP_TYPE_TRAITS_HPP_

#include <type_traits>

namespace gribpp {
	namespace type_traits {

		template <class T, template <class...> class Template>
		struct is_specialization : std::false_type {};

		template <template <class...> class Template, class... Args>
		struct is_specialization<Template<Args...>, Template> : std::true_type {};
	};
};

#endif // !_GRIBPP_TYPE_TRAITS_HPP_

