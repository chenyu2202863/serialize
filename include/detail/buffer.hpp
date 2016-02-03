#ifndef __SERAILIZE_MEMORY_BUFFER_HPP
#define __SERAILIZE_MEMORY_BUFFER_HPP


#include <fstream>
#include <vector>

#pragma warning(disable: 4996)

#ifdef max
#undef max
#endif


namespace serialize { namespace detail {

	template < typename CharT >
	class memory_t
	{
	public:
		typedef CharT				value_type;
		typedef CharT *				pointer;
		typedef value_type &		reference;
		typedef const CharT *		const_pointer;
		typedef const value_type &	const_reference;

	private:
		pointer buf_;
		const std::uint32_t buf_len_;

	public:
		template < std::uint32_t N >
		memory_t(value_type (&buf)[N])
			: buf_(buf)
			, buf_len_(N)
		{}

		memory_t(pointer buf, std::uint32_t len)
			: buf_(buf)
			, buf_len_(len)
		{}

	public:
		pointer buffer()
		{
			return buf_;
		}

		const_pointer buffer() const
		{
			return buf_;
		}

		std::uint32_t buffer_length() const
		{
			return buf_len_;
		}

		bool grow(std::uint32_t)
		{
			return false;
		}

		void read(pointer buf, std::uint32_t len, std::uint32_t pos) const
		{
			::memmove(buf, buf_ + pos, len);
		}

		void write(const_pointer buf, std::uint32_t len, std::uint32_t pos)
		{
			::memmove(buf_ + pos, buf, len);
		}
	};


	template < typename CharT, typename BufferT = std::vector<char> >
	class dynamic_memory_t
	{
	public:
		typedef CharT				value_type;
		typedef CharT *				pointer;
		typedef value_type &		reference;
		typedef const CharT *		const_pointer;
		typedef const value_type &	const_reference;

	private:
		BufferT &buffer_;

	public:
		dynamic_memory_t(BufferT &buffer)
			: buffer_(buffer)
		{}

	public:
		pointer buffer()
		{
			return buffer_.data();
		}

		const_pointer buffer() const
		{
			return buffer_.data();
		}

		std::uint32_t buffer_length() const
		{
			return (std::uint32_t)buffer_.size();
		}

		bool grow(std::uint32_t min_size)
		{
			buffer_.resize(buffer_.size() * 2 + min_size);
			return true;
		}

		void read(pointer buf, std::uint32_t len, std::uint32_t pos) const
		{
			::memmove(buf, buffer_.data() + pos, len);
		}

		void write(const_pointer buf, std::uint32_t len, std::uint32_t pos)
		{
			::memmove(buffer_.data() + pos, buf, len);
		}
	};


	template < typename CharT >
	class file_t
	{
	public:
		typedef CharT				value_type;
		typedef CharT *				pointer;
		typedef value_type &		reference;
		typedef const CharT *		const_pointer;
		typedef const value_type &	const_reference;

	private:
		std::basic_fstream<CharT> file_;

	public:
		template < typename PathT >
		explicit file_t(const PathT &path,
						std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary,
						int prot = std::ios_base::_Openprot)
		{
			file_.open(path, mode, prot);
			if(!file_.good())
			{
				{
					std::ofstream out(path);
				}
				file_.open(path, mode, prot);
				assert(file_.good());
			}
		}

		~file_t()
		{

		}

	public:
		std::uint32_t buffer_length() const
		{
			return std::numeric_limits<std::uint32_t>::max();
		}

		bool grow(std::uint32_t)
		{
			return false;
		}

		void read(pointer buf, std::uint32_t len, std::uint32_t pos)
		{
			assert(file_.good());
			if(!file_.good())
				throw std::runtime_error("file not good");

			file_.seekp(pos);
			file_.read(buf, len);
		}

		void write(const_pointer buf, std::uint32_t len, std::uint32_t pos)
		{
			assert(file_.good());
			if(!file_.good())
				throw std::runtime_error("file not good");

			file_.seekg(pos);
			file_.write(buf, len);
		}
	};
}
}


#endif