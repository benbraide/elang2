#pragma once

#ifndef ELANG_BYTE_CODE_OPERAND_INFO_H
#define ELANG_BYTE_CODE_OPERAND_INFO_H

#include <variant>

#include "../common/error.h"

#include "../memory/memory_table.h"
#include "../memory/memory_register_table.h"

#include "byte_code_debug.h"

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

		enum class offset_op_type : unsigned char{
			add,
			sub,
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
		static target_type extract_source(memory::table &mem_table, memory::register_table &reg_tbl, bool is_memory = false){
			auto iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
			auto fmt = mem_table.read_bytes<format>(iptr);

			iptr += sizeof(format);
			reg_tbl.instruction_pointer()->write(iptr);//Update

			switch (fmt->type){
			case type::register_:
			{
				auto reg = reg_tbl.find(fmt->value);
				if (reg == nullptr || (reg = reg->match(sizeof(target_type))) == nullptr)//Error
					throw common::error::register_not_found;

				if (debug::debugger != nullptr){
					if (is_memory)
						debug::debugger->log(reg->name());
					else//Operand
						debug::debugger->log_operand(reg->name());
				}

				return reg->read<target_type>();
			}
			case type::memory:
			{
				if (debug::debugger != nullptr)
					debug::debugger->log_operand("[");

				auto value = mem_table.read<target_type>(extract_source<__int64>(mem_table, reg_tbl, true));
				if (debug::debugger != nullptr)
					debug::debugger->log("]");

				return value;
			}
			case type::offset:
			{
				auto offset = 0i64;
				for (auto count = fmt->value; count > 0u; --count){
					iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
					switch (*mem_table.read_bytes<offset_op_type>(iptr)){
					case offset_op_type::sub:
						if (debug::debugger != nullptr && count != fmt->value)
							debug::debugger->log(" - ");

						reg_tbl.instruction_pointer()->write(iptr + sizeof(offset_op_type));//Update
						offset -= extract_source<__int64>(mem_table, reg_tbl, is_memory);
						is_memory = true;

						break;
					default:
						if (debug::debugger != nullptr && count != fmt->value)
							debug::debugger->log(" + ");

						reg_tbl.instruction_pointer()->write(iptr + sizeof(offset_op_type));//Update
						offset += extract_source<__int64>(mem_table, reg_tbl, is_memory);
						is_memory = true;

						break;
					}
				}

				return static_cast<target_type>(offset);
			}
			case type::immediate:
			{
				auto value = target_type();//Zero out bytes
				reg_tbl.instruction_pointer()->write(iptr + fmt->value);//Update

				if (sizeof(target_type) == fmt->value)
					value = mem_table.read<target_type>(iptr);
				else//Copy applicable bytes
					mem_table.read(iptr, (char *)&value, fmt->value);

				if (debug::debugger != nullptr){
					if (is_memory)
						debug::debugger->log(std::to_string(value));
					else//Operand
						debug::debugger->log_operand(std::to_string(value));
				}

				return value;
			}
			default:
				break;
			}

			throw common::error::byte_code_unknown_operand_type;
		}

		static void extract_destination(std::size_t size, memory::table &mem_table, memory::register_table &reg_tbl, destination_type &dest){
			auto iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
			auto fmt = mem_table.read_bytes<format>(iptr);

			reg_tbl.instruction_pointer()->write(++iptr);//Update
			switch (fmt->type){
			case type::register_:
			{
				auto reg = reg_tbl.find(fmt->value);
				if (reg == nullptr || (reg = reg->match(size)) == nullptr)//Error
					throw common::error::register_not_found;

				dest = reg;
				if (debug::debugger != nullptr)
					debug::debugger->log_operand(reg->name());

				return;
			}
			case type::memory:
				if (debug::debugger != nullptr)
					debug::debugger->log_operand("[");

				dest = memory_destination{ &mem_table, extract_source<unsigned __int64>(mem_table, reg_tbl, true) };
				if (debug::debugger != nullptr)
					debug::debugger->log("]");

				return;
			case type::offset:
			case type::immediate:
				throw common::error::byte_code_bad_destination_operand_type;
				return;
			default:
				break;
			}

			throw common::error::byte_code_unknown_operand_type;
		}
	};
}

#endif /* !ELANG_BYTE_CODE_OPERAND_INFO_H */
