#pragma once

#ifndef ELANG_ASM_INSTRUCTION_TRANSLATOR_H
#define ELANG_ASM_INSTRUCTION_TRANSLATOR_H

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/iostreams/device/mapped_file.hpp>

#include "../common/file_resource.h"
#include "../common/file_destination.h"

#include "../byte_code/byte_code_translator.h"

#include "../grammar/asm/asm_grammar.h"

#include "asm_instruction_table.h"

namespace elang::easm{
	class instruction_translator{
	public:
		static void encode_file(const std::string &file, const std::string &target_file,
			const std::string &dir = "test/asm", const std::string &target_dir = "test/bcd"){
			auto path = common::file_resource::resolve(dir, file, "easm");
			if (!boost::filesystem::exists(path))
				throw common::error::file_not_found;

			boost::iostreams::mapped_file source(path, boost::iostreams::mapped_file::readonly);
			if (source.is_open())
				encode(source.const_begin(), source.const_end(), target_file, target_dir);
			else//Error
				throw common::error::file_not_found;
		}

		template <typename iterator_type>
		static void encode(iterator_type start, iterator_type end, const std::string &file, const std::string &dir = "test/bcd"){
			ELANG_AST_QNAME(asm_instruction_set) instruction_set;
			parse(start, end, instruction_set);

			instruction_table table;
			grammar::asm_ast_visitor::visit(instruction_set, table);

			auto size = table.size();
			auto stack_size = table.stack_size();

			auto path = common::file_destination::resolve(dir, file, "ebcd");
			boost::iostreams::mapped_file source(path, boost::iostreams::mapped_file::readwrite, (size + 41u));
			if (!source.is_open())
				throw common::error::file_not_opened;

			memcpy(source.data(), &size, sizeof(size));
			memcpy((source.data() + 8u), &stack_size, sizeof(stack_size));
			memset((source.data() + 16u), 0, 16u);

			table.encode(source.data(), source.size(), 41u);

			auto start_address = table.start_address();
			memcpy((source.data() + 32u), &start_address, sizeof(start_address));

			*(source.data() + 40u) = '\0';
		}

		static void translate_file(const std::string &file, const std::string &dir = "test/asm"){
			auto path = common::file_resource::resolve(dir, file, "easm");
			if (!boost::filesystem::exists(path))
				throw common::error::file_not_found;

			boost::iostreams::mapped_file source(path, boost::iostreams::mapped_file::readonly);
			if (source.is_open())
				translate(source.const_begin(), source.const_end());
			else//Error
				throw common::error::file_not_found;
		}

		template <typename iterator_type>
		static void translate(iterator_type start, iterator_type end){
			ELANG_AST_QNAME(asm_instruction_set) instruction_set;
			parse(start, end, instruction_set);

			instruction_table table;
			grammar::asm_ast_visitor::visit(instruction_set, table);
			
			auto size = table.size();
			auto stack_size = table.stack_size();

			std::string source;
			source.resize(table.size() + 41u);
			table.encode(source.data(), source.size(), 41u);

			memcpy(source.data(), &size, sizeof(size));
			memcpy((source.data() + 8u), &stack_size, sizeof(stack_size));
			memset((source.data() + 16u), 0, 16u);

			auto start_address = table.start_address();
			memcpy((source.data() + 32u), &start_address, sizeof(start_address));

			*(source.data() + 40u) = '\0';
			byte_code::translator::translate(source.data());
		}

		template <typename iterator_type>
		static void parse(iterator_type start, iterator_type end, ELANG_AST_QNAME(asm_instruction_set) &instruction_set){
			auto result = boost::spirit::x3::phrase_parse(
				start,
				end,
				ELANG_GRAMMAR_RULE_QNAME(asm_instruction_set),
				ELANG_GRAMMAR_RULE_QNAME(asm_skip),
				instruction_set
			);

			if (!result)
				throw common::error::asm_parse_error;
		}
	};
}

#endif /* !ELANG_ASM_INSTRUCTION_TRANSLATOR_H */
