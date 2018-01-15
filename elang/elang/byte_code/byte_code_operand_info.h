#pragma once

#ifndef ELANG_BYTE_CODE_OPERAND_INFO_H
#define ELANG_BYTE_CODE_OPERAND_INFO_H

#include <variant>

#include "../common/error.h"

#include "../memory/memory_table.h"
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

		struct memory_destination{
			memory::table *mem_table;
			unsigned __int64 address;
		};

		typedef std::variant<memory::memory_register *, memory_destination> destination_type;

		struct destination_query{
			template <class value_type>
			static void write(destination_type &dest, value_type value){
				if (!std::holds_alternative<memory::memory_register *>(dest)){//Memory
					auto &info = std::get<memory_destination>(dest);
					info.mem_table->write(info.address, value);
				}
				else//Register
					std::get<memory::memory_register *>(dest)->write(value);
			}
			
			static void write(destination_type &dest, const char *buffer, std::size_t size){
				if (!std::holds_alternative<memory::memory_register *>(dest)){//Memory
					auto &info = std::get<memory_destination>(dest);
					info.mem_table->write(info.address, buffer, size);
				}
				else//Register
					std::get<memory::memory_register *>(dest)->write(buffer, size);
			}

			template <class target_type>
			static target_type read(destination_type &dest){
				if (std::holds_alternative<memory::memory_register *>(dest))
					return std::get<memory::memory_register *>(dest)->read<target_type>();

				auto &info = std::get<memory_destination>(dest);
				return info.mem_table->read<target_type>(info.address);
			}

			static void read(destination_type &dest, char *buffer, std::size_t size){
				if (!std::holds_alternative<memory::memory_register *>(dest)){//Memory
					auto &info = std::get<memory_destination>(dest);
					info.mem_table->read(info.address, buffer, size);
				}
				else//Register
					std::get<memory::memory_register *>(dest)->read(buffer, size);
			}
		};

		template <typename target_type>
		static target_type extract_source(memory::table &mem_table, memory::register_table &reg_tbl){
			auto iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
			auto fmt = mem_table.read_bytes<format>(iptr);

			reg_tbl.instruction_pointer()->write(++iptr);//Update
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
				auto offset = 0i64;
				for (auto count = fmt->value; count > 0u; --count)
					offset += extract_source<__int64>(mem_table, reg_tbl);

				return mem_table.read<target_type>(offset);
			}
			case type::offset:
			{
				auto offset = 0i64;
				auto count = mem_table.read<unsigned char>(iptr);

				reg_tbl.instruction_pointer()->write(++iptr);//Update
				for (; count > 0u; --count)
					offset += extract_source<__int64>(mem_table, reg_tbl);

				return static_cast<target_type>(offset);
			}
			case type::immediate:
			{
				reg_tbl.instruction_pointer()->write(iptr + fmt->value);//Update
				if (sizeof(target_type) == fmt->value)
					return mem_table.read<target_type>(iptr);

				auto value = target_type();//Zero out bytes
				mem_table.read(iptr, (char *)&value, fmt->value);//Copy applicable bytes

				return value;
			}
			default:
				break;
			}

			throw common::error::byte_code_unknown_operand_type;
		}

		static void extract_destination(memory::table &mem_table, memory::register_table &reg_tbl, destination_type &dest){
			auto iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
			auto fmt = mem_table.read_bytes<format>(iptr);

			reg_tbl.instruction_pointer()->write(++iptr);//Update
			switch (fmt->type){
			case type::register_:
				dest = reg_tbl.find(fmt->value);
				return;
			case type::memory:
				break;
			case type::offset:
			case type::immediate:
				throw common::error::byte_code_bad_destination_operand_type;
				return;
			default:
				throw common::error::byte_code_unknown_operand_type;
				return;
			}

			auto offset = 0i64;
			for (auto count = fmt->value; count > 0u; --count)
				offset += extract_source<__int64>(mem_table, reg_tbl);

			dest = memory_destination{ &mem_table, static_cast<unsigned __int64>(offset) };
		}
	};
}

#endif /* !ELANG_BYTE_CODE_OPERAND_INFO_H */
