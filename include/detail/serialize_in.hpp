#ifndef __SERIALIZE_IN_HPP
#define __SERIALIZE_IN_HPP

#include <type_traits>


namespace serialize
{
	namespace detail
	{
		template < typename CharT, typename ImplT >
		class serialize_in_t
		{
		public:
			typedef CharT				value_type;
			typedef CharT *				pointer;
			typedef value_type &		reference;
			typedef const CharT *		const_pointer;
			typedef const value_type &	const_reference;

		public:
			serialize_in_t()
				: in_pos_(0)
			{}

		private:
			size_t in_pos_;

		public:
			size_t in_length() const
			{
				return in_pos_;
			}

		public:
			template < typename T >
			void push(const T &val)
			{
				static_assert(std::is_pod<T>::value, "T must be a POD type");

				// ºÏ≤‚T¿‡–Õ
				assert(sizeof(T) + in_pos_ <= impl()->buffer_length());
				if( sizeof(T) + in_pos_ > impl()->buffer_length() )
					throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

				const_pointer buf = reinterpret_cast<const_pointer>(&val);
				impl()->write(buf, sizeof(T), in_pos_);
				in_pos_ += sizeof(T);
			}

			template < typename T, size_t N >
			void push_array(const T (&arr)[N])
			{
				static_assert(std::is_pod<T>::value, "T must be a POD type");

				const size_t len = sizeof(T) * N;
				assert(len + in_pos_ <= impl()->buffer_length());
				if( len + in_pos_ > impl()->buffer_length() )
					throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

				const_pointer buf = reinterpret_cast<const_pointer>(&arr);
				impl()->write(buf, len, in_pos_);
				in_pos_ += len;
			}

			template < typename T >
			void push_pointer(const T * const ptr, size_t cnt = 1)
			{
				static_assert(std::is_pod<T>::value, "T must be a POD type");

				const size_t len = sizeof(T) * cnt;
				assert(len + in_pos_ <= impl()->buffer_length());
				if( len + in_pos_ > impl()->buffer_length() )
					throw std::out_of_range("sizeof(T) + pos_ > bufLen_");

				const_pointer buf = reinterpret_cast<const_pointer>(ptr);
				impl()->write(buf, len, in_pos_);
				in_pos_ += len;
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