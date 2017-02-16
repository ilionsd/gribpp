/*
 * field_map.hpp
 *
 *  Created on: Jan 25, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_OCTET_MAPPING_OCTET_MAP_HPP_
#define _GRIBPP_OCTET_MAPPING_OCTET_MAP_HPP_

#include <cstddef>
#include <unordered_map>
#include <experimental/optional>

#include "../../utility/enum_hash.hpp"

namespace gribpp {
	namespace octet_mapping {

		namespace _stdex = std::experimental;

		using std::size_t;

		template<typename E>
		class octet_map {
		public:
			using enum_type = E;
			using map_type = typename std::unordered_map<enum_type, std::pair<std::size_t, std::size_t>, utility::enum_hash<enum_type>>;
			using value_type 	= typename map_type::value_type;
			using key_type 		= typename map_type::key_type;
			using mapped_type 	= typename map_type::mapped_type;



			inline mapped_type& operator[] (key_type&& keyVal) {
				return mMapping[keyVal];
			};
			inline mapped_type& operator[] (const key_type& keyVal) {
				return mMapping[keyVal];
			};
			inline mapped_type& at(const key_type& keyVal) {
				return mMapping.at(keyVal);
			};
			inline const mapped_type& at(const key_type& keyVal) const {
				return mMapping.at(keyVal);
			};

			inline const map_type& mapping() const {
				return mMapping;
			};

			inline bool empty() const {
				return mMapping.empty();
			};

			void shift_mapping(const std::size_t pos) {
				for (auto it = mapping().begin(); it != mapping().end(); ++it) {
					it->second.first += pos;
					it->second.second += pos;
				}
			};

			_stdex::optional<std::size_t> first_octet() const {
				if (empty())
					return {};
				else {
					std::size_t firstOctet = std::numeric_limits<std::size_t>::max();
					for (auto it = mapping().cbegin(); it != mapping().cend(); ++it) {
						if (firstOctet > it->second.first)
							firstOctet = it->second.first;
					}
					return firstOctet;
				}
			};

			_stdex::optional<std::size_t> last_octet() const {
				if (empty())
					return {};
				else {
					std::size_t lastOctet = std::numeric_limits<std::size_t>::min();
					for (auto it = mapping().cbegin(); it != mapping().cend(); ++it) {
						if (lastOctet < it->second.second)
							lastOctet = it->second.second;
					}
					return lastOctet;
				}
			};

		protected:
			inline octet_map() :
				mMapping()
			{};
			inline octet_map(const map_type& octetMap) :
				mMapping(octetMap)
			{};
			inline octet_map(const octet_map& other) :
				mMapping(other.mMapping)
			{};
			inline octet_map(octet_map &&other) :
				mMapping(std::move(other.mMapping))
			{};
			inline octet_map(std::initializer_list<value_type> initList) :
				mMapping(initList)
			{};

			inline octet_map& operator= (const octet_map& other) {
				mMapping = other.mMapping;
				return (*this);
			};
			inline octet_map& operator= (octet_map&& other) {
				mMapping = std::move(other);
				return (*this);
			};
			inline octet_map& operator= (std::initializer_list<value_type> initList) {
				mMapping = initList;
				return (*this);
			};

		private:
			inline map_type& mapping() {
				return mMapping;
			};

			map_type mMapping;

		};

	};	//-- octet_mapping --
};	//-- namespace gribpp --



#endif /* _GRIBPP_OCTET_MAPPING_OCTET_MAP_HPP_ */
