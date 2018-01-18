#pragma once

#ifndef ELANG_ASM_INSTRUCTION_TRANSLATOR_H
#define ELANG_ASM_INSTRUCTION_TRANSLATOR_H

#include <boost/iostreams/device/mapped_file.hpp>

#include "../common/file_resource.h"
#include "../common/file_destination.h"

#include "../grammar/asm/asm_grammar.h"

#include "asm_instruction_table.h"

namespace elang::easm{
	class instruction_translator{
	public:
		static void encode_file(const std::string &file, const std::string &target_file,
			const std::string &dir = "test/asm", const std::string &target_dir = "test/bcd");

		template <typename iterator_type>
		static void encode(iterator_type start, iterator_type end, const std::string &file, const std::string &dir = "test/bcd"){
			ELANG_AST_QNAME(asm_instruction_set) instruction_set;
			parse(start, end, instruction_set);

			instruction_table table;

			auto path = common::file_destination::resolve(dir, file, "ebcd");
			boost::iostreams::mapped_file source(path, boost::iostreams::mapped_file::readwrite, (table.size() + 24u));
			if (!source.is_open())
				throw common::error::file_not_opened;
		}

		static void translate_file(const std::string &file, const std::string &dir = "test/asm");

		template <typename iterator_type>
		static void translate(iterator_type start, iterator_type end){
			ELANG_AST_QNAME(asm_instruction_set) instruction_set;
			parse(start, end, instruction_set);

			instruction_table table;

			std::string buffer;
			buffer.resize(table.size() + 24u);
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
