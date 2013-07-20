#ifndef __SERIALIZE_IN_HPP
#define __SERIALIZE_IN_HPP

#include <cassert>
#include <type_traits>
#include <cstdint>
#include <sstream>


namespace serialize { namespace detail {


	template <
		typename CharT,
		template < typename > class BufferT
	>
	class binary_in_t
	{
		typedef BufferT<CharT>		buffer_t;

	public:
		typedef CharT				value_type;
		typedef CharT *				pointer;
		typedef value_type &		reference;
		typedef const CharT *		const_pointer;
		typedef const value_type &	const_reference;

	public:
		binary_in_t(buffer_t &buffer)
			: in_pos_(0)
			, buffer_(buffer)
		{}

	private:
		std::uint32_t in_pos_;
		buffer_t &buffer_;

	public:
		std::uint32_t in_length() const
		{
			return in_pos_;
		}

	public:
		template < typename T >
		void push(const T &val)
		{
			static_assert( std::is_pod<T>::value, "T must be a POD type" );

			// 检测T类型
			assert(sizeof( T ) + in_pos_ <= buffer_.buffer_length());
			if(sizeof( T ) + in_pos_ > buffer_.buffer_length())
				throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

			const_pointer buf = reinterpret_cast< const_pointer >( &val );
			buffer_.write(buf, sizeof( T ), in_pos_);
			in_pos_ += sizeof( T );
		}

		template < typename T, std::uint32_t N >
		void push_array(const T(&arr)[N])
		{
			static_assert( std::is_pod<T>::value, "T must be a POD type" );

			const std::uint32_t len = sizeof( T ) * N;
			assert(len + in_pos_ <= buffer_.buffer_length());
			if(len + in_pos_ > buffer_.buffer_length())
				throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

			const_pointer buf = reinterpret_cast< const_pointer >( &arr );
			buffer_.write(buf, len, in_pos_);
			in_pos_ += len;
		}

		template < typename T >
		void push_pointer(const T * const ptr, std::uint32_t cnt = 1)
		{
			static_assert( std::is_pod<T>::value, "T must be a POD type" );

			const std::uint32_t len = sizeof( T ) * cnt;
			assert(len + in_pos_ <= buffer_.buffer_length());
			if(len + in_pos_ > buffer_.buffer_length())
				throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

			const_pointer buf = reinterpret_cast< const_pointer >( ptr );
			buffer_.write(buf, len, in_pos_);
			in_pos_ += len;
		}
	};


	template <
		typename CharT,
		template < typename > class BufferT
	>
	class text_in_t
	{
		typedef BufferT<CharT>		buffer_t;

	public:
		typedef CharT				value_type;
		typedef CharT *				pointer;
		typedef value_type &		reference;
		typedef const CharT *		const_pointer;
		typedef const value_type &	const_reference;

	public:
		text_in_t(buffer_t &buffer)
			: in_pos_(0)
			, buffer_(buffer)
		{}

	private:
		std::uint32_t in_pos_;
		buffer_t &buffer_;

	public:
		std::uint32_t in_length() const
		{
			return in_pos_;
		}

	public:
		template < typename T >
		void push(const T &val)
		{
			static_assert( std::is_pod<T>::value, "T must be a POD type" );

			// 检测T类型
			assert(sizeof( T ) + in_pos_ <= buffer_.buffer_length());
			if(sizeof( T ) + in_pos_ > buffer_.buffer_length())
				throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

			std::basic_ostringstream<CharT> os;
			os << val;

			const auto &t = os.str();
			buffer_.write(t.c_str(), t.size(), in_pos_);
			in_pos_ += t.size();
		}

		template < typename T, std::uint32_t N >
		void push_array(const T(&arr)[N])
		{
			static_assert( std::is_pod<T>::value, "T must be a POD type" );

			const std::uint32_t len = sizeof( T ) * N;
			assert(len + in_pos_ <= buffer_.buffer_length());
			if(len + in_pos_ > buffer_.buffer_length())
				throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

			std::basic_ostringstream<CharT> os;
			os.write(arr, N);

			const auto &t = os.str();
			buffer_.write(t.c_str(), t.size(), in_pos_);
			in_pos_ += t.size();
		}

		template < typename T >
		void push_pointer(const T * const ptr, std::uint32_t cnt = 1)
		{
			static_assert( std::is_pod<T>::value, "T must be a POD type" );

			const std::uint32_t len = sizeof( T ) * cnt;
			assert(len + in_pos_ <= buffer_.buffer_length());
			if(len + in_pos_ > buffer_.buffer_length())
				throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

			std::basic_ostringstream<CharT> os;
			os << ptr;

			const auto &t = os.str();
			buffer_.write(t.c_str(), t.size(), in_pos_);
			in_pos_ += t.size();
		}
	};
}
}






#endif