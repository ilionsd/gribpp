/*
 * sequence_fn.hpp
 *
 *  Created on: Feb 28, 2017
 *      Author: ilion
 */

#ifndef _UTILITY_FN_SEQUENCE_FN_HPP_
#define _UTILITY_FN_SEQUENCE_FN_HPP_


#include <array>
#include <experimental/array>
#include <utility>
#include <type_traits>


namespace utility {
	namespace fn {

		namespace stdex = std::experimental;

		template<typename F>
		struct sequence_fn {
			using function_type = F;

			constexpr sequence_fn() : fn { F() }
			{};
			constexpr sequence_fn(F f) : fn { f }
			{};

			const function_type& fn;

		};

		template<typename T, typename... Ts>
		constexpr auto first_value(T& val, Ts&... vals) {
			return val;
		};


		template<typename F>
		struct reduce : public sequence_fn<F> {

			using base_type = sequence_fn<F>;
			using base_type::base_type;


			template<typename T, typename... Ts>
			constexpr std::enable_if_t<sizeof...(Ts), T>
				operator() (const T& val, const Ts&... vals) const
			{
				return this->fn(val, this->operator()(vals...));
			};

			template<typename T, typename... Ts>
			constexpr std::enable_if_t<!sizeof...(Ts), T>
				operator() (const T& val, const Ts&... vals) const
			{
				return val;
			};

		};


		template<typename F>
		struct map : public sequence_fn<F> {

			using base_type = sequence_fn<F>;
			using base_type::base_type;

			template<typename... Ts>
			constexpr auto
				operator() (const Ts&... vals) const
			{
				return stdex::make_array( (this->fn(vals))... );
			};

		};


		template<typename F>
		struct apply : public sequence_fn<F> {

			using base_type = sequence_fn<F>;
			using base_type::base_type;

			template<typename T, std::size_t N>
			constexpr auto
				operator() (const std::array<T, N>& a) const
			{
				return apply_impl(a, std::make_index_sequence<N>());
			};

			template<typename T, std::size_t N, std::size_t... I>
			constexpr auto
				apply_impl(const std::array<T, N>& a, std::index_sequence<I...>) const
			{
				return this->fn(std::get<I>(a)...);
			};

		};


		template<typename F> constexpr auto	make_reduce(F f)	-> reduce<F>	{	return reduce<F> { f };	};
		template<typename F> constexpr auto	make_map(F f)		-> map<F>		{	return map<F> { f };	};
		template<typename F> constexpr auto make_apply(F f)		-> apply<F> 	{	return apply<F> { f };	};


		template<template<typename > class F, typename T>
		class unary {
		public:
			using function_type = F<T>;

			constexpr unary() : fn {}, savedVal {}
			{};
			constexpr unary(T val) : fn {}, savedVal { val }
			{};
			constexpr unary(function_type f, T val) : fn { f }, savedVal { val }
			{};

			constexpr auto operator() (const T& val) const {
				return this->fn(savedVal, val);
			};

		private:
			const function_type& fn;
			const T& savedVal;
		};


	};	//-- namespace fn --
};	//-- namespace utility --



#endif /* _UTILITY_FN_SEQUENCE_FN_HPP_ */
