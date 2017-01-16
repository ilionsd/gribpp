#ifndef _META_META_CASE_HPP_
#define _META_META_CASE_HPP_

namespace meta {

	template<typename T, unsigned N>
	struct meta_case : public T {
		using _MyTy = meta_case<T, N>;
		using value_type = T;
		static constexpr unsigned case_n = N;

		using value_type::value_type;

		template<typename T2>
		_MyTy& operator= (T2 other) {
			value_type::operator=(other);
			return *this;
		};

		inline constexpr unsigned case_number() const {
			return case_n;
		};


	};
};	//-- namespace meta --

#endif //-- _META_META_CASE_HPP_ --

