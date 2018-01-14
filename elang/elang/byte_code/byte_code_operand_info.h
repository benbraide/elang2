#pragma once

#ifndef ELANG_BYTE_CODE_OPERAND_INFO_H
#define ELANG_BYTE_CODE_OPERAND_INFO_H

#include "../common/error.h"

#include "../memory/memory_register_table.h"

namespace elang::byte_code{
	struct operand_info{
		enum class type : unsigned char{
			register_,
			memory,
			offset,
			immediate,
		};

		struct format{
			operand_info::type type	: 2;
			unsigned char value		: 6;
		};

		template <typename target_type>
		static target_type copy(char *&buffer, std::size_t size){
			auto target = target_type();
			if (sizeof(&target) > size){//Copy and zero higher bits
				memcpy(&target, buffer, size);
				memset(((&target) + size), 0, (sizeof(target) - size));
			}
			else//Copy applicable bits
				memcpy(&target, buffer, size);

			buffer += size;
			return target;
		}

		template <typename target_type>
		static target_type extract_source(char *&data, char *base_ptr, memory::register_table &reg_tbl, bool is_memory = false){
			auto fmt = reinterpret_cast<format *>(data++);
			switch (fmt->type){
			case type::register_:
			{
				auto reg = reg_tbl.find(fmt->value);
				if (reg == nullptr)//Error
					throw common::error::register_not_found;
				return reg->read<target_type>();
			}
			case type::memory:
			{
				if (is_memory)//Memory inside memory
					throw common::error::byte_code_bad_source_operand_type;

				auto offset = 0i64;
				for (auto count = fmt->value; count > 0u; --count)
					offset += extract_source<__int64>(data, base_ptr, reg_tbl, true);

				auto value = target_type();
				memcpy(&value, (base_ptr + offset), sizeof(target_type));

				return value;
			}
			case type::offset:
			{
				auto offset = 0i64;
				for (auto count = static_cast<unsigned char>(*(data++)); count > 0u; --count)
					offset += extract_source<__int64>(data, base_ptr, reg_tbl, true);

				return static_cast<target_type>(offset);
			}
			case type::immediate:
				return copy<target_type>(data, fmt->value);
			default:
				break;
			}

			throw common::error::byte_code_unknown_operand_type;
		}

		static char *extract_destination(char *&data, char *base_ptr, memory::register_table &reg_tbl){
			auto fmt = reinterpret_cast<format *>(data++);
			switch (fmt->type){
			case type::register_:
			{
				auto reg = reg_tbl.find(fmt->value);
				if (reg == nullptr)//Error
					throw common::error::register_not_found;

				return reg->data();
			}
			case type::memory:
			{
				auto offset = 0i64;
				for (auto count = fmt->value; count > 0u; --count)
					offset += extract_source<__int64>(data, base_ptr, reg_tbl, true);
				return (base_ptr + offset);
			}
			case type::offset:
			case type::immediate:
				throw common::error::byte_code_bad_destination_operand_type;
				break;
			default:
				break;
			}

			throw common::error::byte_code_unknown_operand_type;
		}
	};
}

#endif /* !ELANG_BYTE_CODE_OPERAND_INFO_H */
