#ifndef __string_h_
#define __string_h_

#include <cstddef>
#include <cstring>

namespace qtl
{
	class string;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace qinternal
    {
        class string_forward_iterator
        {
        public:
            string_forward_iterator(const char* data, std::size_t cursor);
            string_forward_iterator& operator++();
            bool operator==(const string_forward_iterator& it) const;
            bool operator!=(const string_forward_iterator& it) const;

            char& operator*();
            const char& operator*() const;
        private:
			friend class qtl::string;
            char* __data;
            std::size_t __cursor;
        };
    }
#endif // DOXYGEN_SHOULD_SKIP_THIS
    class string
    {
    public:
        typedef qinternal::string_forward_iterator forward_iterator;
        typedef const qinternal::string_forward_iterator const_forward_iterator;

        string();
        string(const string& str);
        string(const string& str, std::size_t pos, std::size_t len = npos);
        string(const char * s);
        string(const char * s, std::size_t n);
        string(std::size_t n, char c);
        string(string&& str) noexcept;

        ~string();

        string& operator=(const string& str);
        string& operator=(string&& str) noexcept;
		string& operator=(const char * str);

        std::size_t size() const noexcept;
        std::size_t length() const noexcept;
        std::size_t max_size() const noexcept;
        void clear();

        forward_iterator begin();
        const forward_iterator begin() const;

        forward_iterator end();
        const forward_iterator end() const;

        char& operator[](std::size_t idx);
        const char& operator[](std::size_t idx) const;

        char& at(std::size_t idx);
        const char& at(std::size_t idx) const;

        char& front();
        const char& front() const;

        char& back();
        const char& back() const;

        string& operator+=(const string& str);
        string& operator+=(const char * s);
        string& operator+=(char c);

        string& append(const string& str);
        string& append(const char* s);
        string& append(char c);

        void push_back(char c);

        string& assign(const string& str);
        string& assign(const string& str, std::size_t pos, std::size_t len = npos);
        string& assign(const char * s);
        string& assign(const char * s, std::size_t n);
        string& assign(string&& str) noexcept;

        string& insert(std::size_t pos, const string& str);
        string& insert(std::size_t pos, const string& str, std::size_t subpos, std::size_t sublen = npos);
        string& insert(std::size_t pos, const char* s);
        string& insert(std::size_t pos, const char* s, std::size_t n);
        string& insert(std::size_t pos, std::size_t n, char c);
        forward_iterator insert(const_forward_iterator it, std::size_t n, char c);
        forward_iterator insert(const_forward_iterator it, char c);

        string& erase(std::size_t pos = 0, std::size_t len = npos);
        forward_iterator erase(forward_iterator it);
        forward_iterator erase(forward_iterator begin, forward_iterator end);

        string& replace(std::size_t pos, std::size_t len, const string& str);
        string& replace(const_forward_iterator begin, const_forward_iterator end, const string& str);
        string& replace(std::size_t pos, std::size_t len, const char * s);
        string& replace(const_forward_iterator begin, const_forward_iterator end, const char * s);
        string& replace(std::size_t pos, std::size_t len, const char * s, std::size_t n);
        string& replace(const_forward_iterator begin, const_forward_iterator end, const char * s, std::size_t n);
        string& replace(std::size_t pos, std::size_t len, std::size_t n, char c);
        string& replace(const_forward_iterator begin, const_forward_iterator end, std::size_t n, char c);

        void swap(string& other);
        
        void pop_back();

        const char * c_str() const;
        
        const char * data() const noexcept;
        
        std::size_t copy(char * s, std::size_t len, std::size_t pos = 0) const;
        
        std::size_t find(const string& str, std::size_t pos = 0) const noexcept;
        std::size_t find(const char * s, std::size_t pos = 0) const;
        std::size_t find(char c, std::size_t pos = 0) const noexcept;
        
        std::size_t rfind(const string& str, std::size_t pos = npos) const noexcept;
        std::size_t rfind(const char * s, std::size_t pos = npos) const;
        std::size_t rfind(char c, std::size_t pos = npos) const noexcept;

        std::size_t find_first_of(const string& str, std::size_t pos = 0) const noexcept;
        std::size_t find_first_of(const char * s, std::size_t pos = 0) const;
        std::size_t find_first_of(char c, std::size_t pos = 0) const noexcept;
        
        std::size_t find_last_of(const string& str, std::size_t pos = 0) const noexcept;
        std::size_t find_last_of(const char * s, std::size_t pos = 0) const;
        std::size_t find_last_of(char c, std::size_t pos = 0) const noexcept;

        std::size_t find_first_not_of(const string& str, std::size_t pos = 0) const noexcept;
        std::size_t find_first_not_of(const char * s, std::size_t pos = 0) const;
        std::size_t find_first_not_of(char c, std::size_t pos = 0) const noexcept;
        
        std::size_t find_last_not_of(const string& str, std::size_t pos = 0) const noexcept;
        std::size_t find_last_not_of(const char * s, std::size_t pos = 0) const;
        std::size_t find_last_not_of(char c, std::size_t pos = 0) const noexcept;
        
        string substr(std::size_t pos = 0, std::size_t len = npos) const;
        
        int compare(const string& str) const noexcept;
        int compare(std::size_t pos, std::size_t len, const string& str) const;
        int compare(std::size_t pos, std::size_t len, const string& str, std::size_t subpos, std::size_t sublen) const;
        int compare(const char * s) const;
        int compare(std::size_t pos, std::size_t len, const char * s) const;
        int compare(std::size_t pos, std::size_t len, const char * s, std::size_t n) const;

        static constexpr const std::size_t npos = -1;
    private:
        char * __data;
        std::size_t __len;

		void __compute_longest_possible_pre_suffix(const char* pattern, const std::size_t pattern_len, std::size_t * lps) const noexcept;
    };
}

#endif