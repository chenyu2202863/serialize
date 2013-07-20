#ifndef __SERIALIZE_OUT_HPP
#define __SERIALIZE_OUT_HPP


#include <cassert>
#include <type_traits>


namespace serialize
{
	namespace detail
	{
		template < typename CharT, typename ImplT >
		class serialize_out_t
		{
		public:
			typedef CharT				value_type;
			typedef CharT *				pointer;
			typedef value_type &		reference;
			typedef const CharT *		const_pointer;
			typedef const value_type &	const_reference;

		private:
			size_t out_pos_;

		public:
			serialize_out_t()
				: out_pos_(0)
			{}

		public:
			size_t out_length() const
			{
				return out_pos_;
			}

		public:
			template < typename T >
			void pop(T &val)
			{
				static_assert(std::is_pod<T>::value, "T must POD type");

				assert(sizeof(T) + out_pos_ <= impl()->buffer_length());
				if( sizeof(T) + out_pos_ > impl()->buffer_length() )
					throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

				pointer buf = reinterpret_cast<pointer>(&val);
				impl()->read(buf, sizeof(T), out_pos_);
				out_pos_ += sizeof(T);
			}

			template < typename T, size_t N >
			void pop_array(T (&arr)[N])
			{
				static_assert(std::is_pod<T>::value, "T must POD type");

				const size_t len = sizeof(T) * N;
				assert(len + out_pos_ <= impl()->buffer_length());
				if( len + out_pos_ > impl()->buffer_length() )
					throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

				pointer buf = reinterpret_cast<pointer>(&arr);
				impl()->read(buf, len, out_pos_);
				out_pos_ += len;
			}

			template < typename T >
			void pop_pointer(T *ptr, size_t cnt = 1)
			{
				static_assert(std::is_pod<T>::value, "T must POD type");

				const size_t len = sizeof(T) * cnt;
				assert(len + out_pos_ <= impl()->buffer_length());
				if( len + out_pos_ > impl()->buffer_length() )
					throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

				pointer buf = reinterpret_cast<pointer>(ptr);
				impl()->read(buf, len, out_pos_);
				out_pos_ += len;
			}

		private:
			ImplT *impl()
			{
				return static_cast<ImplT *>(this);
			}
		};
	}
}




#endif