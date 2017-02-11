#ifndef _GRIBPP_READER_OCTET_READER_HPP_
#define _GRIBPP_READER_OCTET_READER_HPP_

#ifndef NDEBUG
#define _INCLUDE_READER_POS
#endif


#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <experimental/any>
#include <experimental/optional>
#include <type_traits>

#include "../type_traits.hpp"
#include "../../utility/byte_reverse.hpp"


namespace gribpp {
	namespace reader {

		namespace _stdex = std::experimental;

		class octet_reader {
		public:
			friend octet_reader open_grib(const char *);
			friend octet_reader open_grib(std::string &);

			octet_reader(const octet_reader& other) :
				mFile(other.mFile),
				mOctetsToRead(other.mOctetsToRead),
				mReadHelper(other.mReadHelper)
#ifdef _INCLUDE_READER_POS
				,__mPos__(other.__mPos__)
#endif
			{};

			octet_reader(octet_reader &&other) :
				mFile(std::move(other.mFile)),
				mOctetsToRead(std::move(other.mOctetsToRead)),
				mReadHelper(std::move(other.mReadHelper))
#ifdef _INCLUDE_READER_POS
				,__mPos__(other.__mPos__)
#endif
			{};

			~octet_reader() {
				std::fclose(mFile);
			};

			octet_reader& operator=(const octet_reader& other) {
				mFile = other.mFile;
				mOctetsToRead = other.mOctetsToRead;
#ifdef _INCLUDE_READER_POS
				__mPos__ = other.__mPos__;
#endif
				return *this;
			};

			template<typename T>
			typename std::enable_if<!std::is_integral<T>::value, octet_reader&>::type operator() (const T& readHelper) {
				mReadHelper = readHelper;
				return *this;
			};

			template<typename T>
			typename std::enable_if<std::is_integral<T>::value, octet_reader&>::type operator() (const T octetsToRead) {
				mOctetsToRead = octetsToRead;
				return *this;
			};

			template<typename T>
			typename std::enable_if<std::is_integral<T>::value, octet_reader&>::type operator >> (T& octets) {
				fread(&octets, 1, sizeof(T), file());
#ifdef _INCLUDE_READER_POS
				__mPos__ = get_pos();
#endif
				return *this;
			};

			octet_reader& operator >> (std::unique_ptr<char[]> &octets) {
				fread(octets.get(), 1, use_octets_to_read(), file());
#ifdef _INCLUDE_READER_POS
				__mPos__ = get_pos();
#endif
				return *this;
			};

			template<typename T>
			typename std::enable_if<std::is_unsigned<T>::value, T>::type read() {
				T val;
				fread(&val, 1, sizeof(T), file());
#ifdef _INCLUDE_READER_POS
				__mPos__ = get_pos();
#endif
				return utility::byte_reverse(val);
			};

			template<typename... Ts>
			std::tuple<Ts...> read_all() {
				return std::tuple<Ts...>{read<Ts>()...};
			};

			std::FILE* file() const {
				return mFile;
			};

			std::size_t get_pos() const {
				std::size_t pos = std::ftell( file());
				return pos;
			};
			void set_pos(const std::size_t pos) {
				std::fseek(file(), pos, SEEK_SET);
#ifdef _INCLUDE_READER_POS
				__mPos__ = get_pos();
#endif
			};

			_stdex::any& read_helper() {
				return mReadHelper;
			};

		protected:
			octet_reader(std::FILE* file) :
				mFile(file),
				mOctetsToRead(1),
				mReadHelper()
#ifdef _INCLUDE_READER_POS
				,__mPos__(0)
#endif
			{};


			inline std::size_t use_octets_to_read() {
				std::size_t temp = mOctetsToRead;
				mOctetsToRead = 1;
				return temp;
			}

			inline _stdex::any use_any_to_read() {
				_stdex::any temp = std::move(mReadHelper);
				return temp;
			};

		private:
			std::FILE* mFile;
			std::size_t mOctetsToRead;
			_stdex::any mReadHelper;

#ifdef _INCLUDE_READER_POS
			std::size_t __mPos__;
#endif

		};	//-- class octet_reader --


		inline octet_reader open_grib(const char* fileName) {
			std::FILE* file = std::fopen(fileName, "r");
			return octet_reader(file);
		};

		inline octet_reader open_grib(const std::string &fileName) {
			return open_grib(fileName.c_str());
		};

		template<typename F>
		auto try_read(F func, octet_reader& reader) -> _stdex::optional<typename decltype(func(reader))::value_type>
		{
			std::size_t initPos = reader.get_pos();
			_stdex::optional<typename decltype(func(reader))::value_type> optRes = func(reader);
			if (!optRes)
				reader.set_pos(initPos);
			return optRes;
		};





	};	//-- namespace reader --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_READER_OCTET_READER_HPP_ --
