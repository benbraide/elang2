#pragma once

#ifndef ELANG_UTILITIES_H
#define ELANG_UTILITIES_H

#include <string>

namespace elang::common{
	class utils{
	public:
		template <typename iterator_type>
		static void escape_string(iterator_type begin, iterator_type end, std::string &escaped, bool is_wide = false){
			if (begin == end)//Empty buffer
				return;

			escaped.reserve(escaped.size() + (std::distance(begin, end) * (is_wide ? sizeof(wchar_t) : sizeof(char))));
			for (; begin != end;)//Accumulate
				escape_char(begin, end, escaped, is_wide);
		}

		static void escape_string(const std::string &value, std::string &escaped, bool is_wide = false){
			escape_string(value.begin(), value.end(), escaped, is_wide);
		}

		template <typename iterator_type>
		static void escape_char(iterator_type &begin, iterator_type end, std::string &escaped, bool is_wide){
			char c = *(begin++);
			if (c != '\\' || begin == end)//Not escaped
				return append_char(c, escaped, is_wide);

			switch (c = *(begin++)){
			case 'a':
				return append_char('\a', escaped, is_wide);
			case 'b':
				return append_char('\b', escaped, is_wide);
			case 'f':
				return append_char('\f', escaped, is_wide);
			case 'n':
				return append_char('\n', escaped, is_wide);
			case 'r':
				return append_char('\r', escaped, is_wide);
			case 't':
				return append_char('\t', escaped, is_wide);
			case 'v':
				return append_char('\v', escaped, is_wide);
			case '"':
				return append_char('\"', escaped, is_wide);
			case '\'':
				return append_char('\'', escaped, is_wide);
			case '\\':
				return append_char('\\', escaped, is_wide);
			case '0':
				return escape_oct_char(begin, end, escaped, is_wide);
			case 'x':
				return escape_hex_char(begin, end, escaped, is_wide);
			default:
				break;
			}

			append_char(c, escaped, is_wide);
		}

		template <typename iterator_type>
		static void escape_oct_char(iterator_type &begin, iterator_type end, std::string &escaped, bool is_wide){
			std::string value;
			for (; begin != end && *begin >= '0' && *begin < '8'; ++begin)
				value.push_back(*begin);//Extract digits

			auto int_value = (value.empty() ? 0i64 : std::stoll(value, nullptr, 8));
			if (is_wide){
				auto wchar_value = static_cast<wchar_t>(int_value);
				escaped.append(reinterpret_cast<char *>(&wchar_value), 2);
			}
			else//Narrow
				escaped.push_back(static_cast<char>(int_value));
		}

		template <typename iterator_type>
		static void escape_hex_char(iterator_type &begin, iterator_type end, std::string &escaped, bool is_wide){
			std::string value;
			for (; begin != end && std::isxdigit(*begin); ++begin)
				value.push_back(*begin);//Extract digits

			if (!value.empty()){
				auto int_value = std::stoll(value, nullptr, 16);
				if (is_wide){
					auto wchar_value = static_cast<wchar_t>(int_value);
					escaped.append(reinterpret_cast<char *>(&wchar_value), 2);
				}
				else//Narrow
					escaped.push_back(static_cast<char>(int_value));
			}
			else//No value
				append_char('x', escaped, is_wide);
		}

		static void append_char(char value, std::string &escaped, bool is_wide){
			auto wchar_value = static_cast<wchar_t>(value);
			escaped.append(reinterpret_cast<char *>(&wchar_value), 2);
		}
	};
}

#endif /* !ELANG_UTILITIES_H */
