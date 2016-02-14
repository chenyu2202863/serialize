#ifndef __SERIALIZE_CONTAINER_DISPATCH_HPP
#define __SERIALIZE_CONTAINER_DISPATCH_HPP


#include <tuple>
#include <string>
#include <map>
#include <set>
#include <list>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <unordered_map>
#include <unordered_set>

namespace serialize { namespace detail {

	template < typename ContainerT >
	struct is_container_t
	{
		static const bool is_defined_type = false;
		static const bool value = false;
	};

	template < typename KeyT, typename ValueT >
	struct is_container_t< std::pair<KeyT, ValueT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename ...Args >
	struct is_container_t< std::tuple<Args...> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename T, std::size_t N >
	struct is_container_t< std::array<T, N> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename T, typename AllocatorT >
	struct is_container_t< std::vector<T, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename T, typename AllocatorT >
	struct is_container_t< std::list<T, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename T, typename AllocatorT >
	struct is_container_t< std::deque<T, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename ValueT, typename LessT, typename AllocatorT >
	struct is_container_t< std::map<KeyT, ValueT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename LessT, typename AllocatorT >
	struct is_container_t< std::set<KeyT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename ValueT, typename LessT, typename AllocatorT >
	struct is_container_t< std::unordered_map<KeyT, ValueT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename LessT, typename AllocatorT >
	struct is_container_t< std::unordered_set<KeyT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename ValueT, typename LessT, typename AllocatorT >
	struct is_container_t< std::multimap<KeyT, ValueT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename LessT, typename AllocatorT >
	struct is_container_t< std::multiset<KeyT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename ValueT, typename LessT, typename AllocatorT >
	struct is_container_t< std::unordered_multimap<KeyT, ValueT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	template < typename KeyT, typename LessT, typename AllocatorT >
	struct is_container_t< std::unordered_multiset<KeyT, LessT, AllocatorT> >
	{
		static const bool is_defined_type = true;
		static const bool value = true;
	};

	// -----------------------------------------------------------------
	// struct container_traits_t

	template< typename ValTraitsT, typename EnbaleT = void >
	struct container_traits_t
	{
		typedef ValTraitsT value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type& val)
		{
			io << val;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			io >> val;
		}
	};


	// pair
	template< typename FirstT, typename SecondT >
	struct container_traits_t<std::pair<FirstT, SecondT>>
	{
		typedef std::pair<FirstT, SecondT> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type& val)
		{
			io << val.first;
			io << val.second;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			io >> val.first;
			io >> val.second;
		}
	};


	// pair<const char *, std::uint32_t>
	template< >
	struct container_traits_t<std::pair<const char *, std::uint32_t>>
	{
		typedef std::pair<const char *, std::uint32_t> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type& val)
		{
			io << val.second;
			io.push_pointer(val.first, val.second);
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			static_assert(false, "not impl");
		}
	};


	// tuple
	template< typename ...Args >
	struct container_traits_t<std::tuple<Args...>>
	{
		struct impl_t
		{
			template < std::uint32_t I, std::uint32_t N, typename StreamT, typename TupleT >
			static typename std::enable_if<I == N>::type push(StreamT &os, const TupleT& t)
			{
				os << std::get<I>(t);
			}

			template< std::uint32_t I, std::uint32_t N, typename StreamT, typename TupleT >
			static typename std::enable_if < I < N>::type push(StreamT &os, const TupleT& t)
			{
				os << std::get<I>(t);
				push<I + 1, std::tuple_size<TupleT>::value - 1>(os, t);
			}

			template < std::uint32_t I, std::uint32_t N, typename StreamT, typename TupleT >
			static typename std::enable_if<I == N>::type pop(StreamT &os, TupleT& t)
			{
				os >> std::get<I>(t);
			}

			template< std::uint32_t I, std::uint32_t N, typename StreamT, typename TupleT >
			static typename std::enable_if < I < N>::type pop(StreamT &os, TupleT& t)
			{
				os >> std::get<I>(t);
				pop<I + 1, std::tuple_size<TupleT>::value - 1>( os, t );
			}
		};


		typedef std::tuple<Args...>	value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type& val)
		{
			enum
			{
				TUPLE_SIZE = std::tuple_size<value_type>::value
			};
			impl_t::push<0, TUPLE_SIZE == 1 ? 0 : TUPLE_SIZE>( io, val );
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			enum
			{
				TUPLE_SIZE = std::tuple_size<value_type>::value
			};
			impl_t::pop<0, TUPLE_SIZE == 1 ? 0 : TUPLE_SIZE>( io, val );
		}
	};


	// array
	template< typename T, std::size_t N >
	struct container_traits_t<std::array<T, N>, typename std::enable_if<!std::is_pod<T>::value>::type>
	{
		typedef std::array<T, N> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());
			for( const auto &v : val )
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type& val)
		{
			std::uint32_t val_size = 0;
			pop_length_impl(io, val_size);

			assert(N == val_size);
			if( N != val_size )
				throw std::runtime_error("array length error");

			for( auto i = 0; i != val_size; ++i )
			{
				value_type::value_type v;
				io >> v;

				val[i] = std::move(v);
			}
		}
	};
	
	// array<T, N>, T is pod type
	template< typename T, std::size_t N >
	struct container_traits_t<std::array<T, N>, typename std::enable_if<std::is_pod<T>::value>::type>
	{
		typedef std::array<T, N> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());
			io.push_pointer(val.data(), (std::uint32_t)val.size());
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type& val)
		{
			std::uint32_t val_size = 0;
			pop_length_impl(io, val_size);

			assert(N == val_size);
			if( N != val_size )
				throw std::runtime_error("array length error");

			io.pop_pointer(val.data(), (std::uint32_t)val.size());
		}
	};



	// vector
	template< typename T, typename AllocatorT >
	struct container_traits_t<std::vector<T, AllocatorT>, typename std::enable_if<!std::is_pod<T>::value>::type>
	{
		typedef std::vector<T, AllocatorT> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			for(const auto &v:val )
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type& val)
		{
			std::uint32_t val_size = 0;
			pop_length_impl(io, val_size);

			val.resize(val_size);
			for( auto i = 0; i != val_size; ++i )
			{
				value_type::value_type v;
				io >> v;

				val[i] = std::move(v);
			}
		}
	};

	// vector<T>, T is pod type
	template< typename T, typename AllocatorT >
	struct container_traits_t<std::vector<T, AllocatorT>, typename std::enable_if<std::is_pod<T>::value>::type>
	{
		typedef std::vector<T, AllocatorT> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			io.push_pointer(val.data(), (std::uint32_t)val.size());
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type& val)
		{
			std::uint32_t val_size = 0;
			pop_length_impl(io, val_size);

			val.resize(val_size);
			io.pop_pointer(val.data(), (std::uint32_t)val.size());
		}
	};


	// map
	template< typename KeyT, typename DataT, typename LessT, typename AllocT >
	struct container_traits_t<std::map<KeyT, DataT, LessT, AllocT>>
	{
		typedef std::map<KeyT, DataT, LessT, AllocT> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			for(const auto &v:val)
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type& val)
		{
			std::uint32_t valSize = 0;
			pop_length_impl(io, valSize);

			for( ; valSize > 0; --valSize )
			{
				value_type::key_type key;
				io >> key;

				value_type::mapped_type value;
				io >> value;

				val.emplace(std::move(key), std::move(value));
			}
		}
	};


	// multimap 
	template< typename KeyT, typename DataT, typename LessT, typename AllocT >
	struct container_traits_t<std::multimap<KeyT, DataT, LessT, AllocT>>
	{
		using value_type = std::multimap<KeyT, DataT, LessT, AllocT>;

		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			for( const auto &v : val )
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			std::uint32_t valSize = 0;
			pop_length_impl(io, valSize);

			for( ; valSize > 0; --valSize )
			{
				value_type::key_type key;
				io >> key;

				value_type::mapped_type value;
				io >> value;

				val.emplace(std::move(key), std::move(value));
			}
		}
	};



	// set
	template< typename KeyT, typename LessT, typename AllocT >
	struct container_traits_t<std::set<KeyT, LessT, AllocT>>
	{
		typedef std::set<KeyT, LessT, AllocT> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type& val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			for(const auto &v: val)
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			std::uint32_t valSize = 0;
			pop_length_impl(io, valSize);

			for(; valSize > 0; --valSize)
			{
				value_type::value_type key;
				io >> key;

				val.emplace(std::move(key));
			}
		}
	};


	// multiset

	template<typename KeyT, typename LessT, typename AllocT >
	struct container_traits_t<std::multiset<KeyT, LessT, AllocT>>
	{
		typedef std::multiset<KeyT, LessT, AllocT> value_type;

		template < typename StreamT >
		static void push(StreamT &io, const value_type& val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			for(const auto &v: val)
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			std::uint32_t valSize = 0;
			pop_length_impl(io, valSize);

			for(; valSize > 0; --valSize)
			{
				value_type::value_type key;
				io >> key;

				val.emplace(std::move(key));
			}
		}
	};


	// list 
	template< typename KeyT, typename AllocT >
	struct container_traits_t<std::list<KeyT, AllocT>>
	{
	public:
		typedef std::list<KeyT, AllocT> value_type;

	public:
		template < typename StreamT >
		static void push(StreamT &io, const value_type &val)
		{
			push_length_impl(io, (std::uint32_t)val.size());

			for(const auto &v:val )
				io << v;
		}

		template < typename StreamT >
		static void pop(StreamT &io, value_type &val)
		{
			std::uint32_t valSize = 0;
			pop_length_impl(io, valSize);

			for( ; valSize > 0; --valSize )
			{
				value_type::value_type key;
				io >> key;

				val.emplace_back(std::move(key));
			}
		}
	};

	


	// deque
	template< typename KeyT, typename AllocT >
	struct container_traits_t<std::deque<KeyT, AllocT>>
	{
		typedef std::deque<KeyT, AllocT> value_type;
	};

	template< typename ValueT, typename ContainerT >
	struct container_traits_t<std::queue<ValueT, ContainerT>>
	{
		typedef std::queue<ValueT, ContainerT> value_type;
	};

	template< typename ValueT, typename ContainerT >
	struct container_traits_t<std::stack<ValueT, ContainerT>>
	{
		typedef std::stack<ValueT, ContainerT> value_type;
	};

	template< typename ValueT, typename ContainerT, typename LessT >
	struct container_traits_t<std::priority_queue<ValueT, ContainerT, LessT>>
	{
		typedef std::priority_queue<ValueT, ContainerT, LessT> value_type;
	};
}
}







#endif