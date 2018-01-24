#include "asm_instruction_table.h"

void elang::easm::instruction_table::add(section_id section){
	active_section_ = &section_map_[section];
}

void elang::easm::instruction_table::add(const std::string &label){
	if (active_section_ == nullptr)
		throw common::error::asm_no_active_section;

	auto &label_ref = label_map_[label];
	if (std::find(active_section_->labels.begin(), active_section_->labels.end(), &label_ref) == active_section_->labels.end()){
		label_ref.offset = active_section_->size;
		label_ref.offset_ptr = nullptr;
		label_ref.equ = nullptr;
		active_section_->labels.push_back(&label_ref);
	}
	else
		throw common::error::asm_label_redefinition;
}

void elang::easm::instruction_table::add(const std::string &label, std::shared_ptr<equ_instruction> equ){
	if (active_section_ == nullptr)
		throw common::error::asm_no_active_section;

	auto &label_ref = label_map_[label];
	if (std::find(active_section_->labels.begin(), active_section_->labels.end(), &label_ref) == active_section_->labels.end()){
		equ->validate();
		label_ref.offset = active_section_->size;
		label_ref.offset_ptr = &(label_ref.equ = equ)->value();
		active_section_->labels.push_back(&label_ref);
	}
	else
		throw common::error::asm_label_redefinition;
}

void elang::easm::instruction_table::add(iptr_type instruction){
	if (active_section_ == nullptr)
		throw common::error::asm_no_active_section;

	instruction->validate();
	auto size = instruction->encoded_size();

	active_section_->size += size;
	active_section_->instructions.push_back(instruction_info{ size, instruction });
}

void elang::easm::instruction_table::set_start_label(const std::string &label){
	start_label_ = label;
}

void elang::easm::instruction_table::set_stack_size(std::size_t size){
	stack_size_ = size;
}

void elang::easm::instruction_table::encode(char *buffer, std::size_t size, std::size_t offset){
	static std::vector<section_id> order({
		section_id::rodata,
		section_id::data,
		section_id::text
	});

	auto section_offset = offset;
	common::binary_output_writer writer(buffer, size);

	for (auto id : order){//Resolve labels
		auto section = section_map_.find(id);
		if (section == section_map_.end())
			continue;//Section not present

		if (id == section_id::rodata){//Set write protect
			write_protect_.from = section_offset;
			write_protect_.to = ((section_offset + section->second.size) - 1u);
		}

		for (auto label : section->second.labels){//Update labels
			label->offset += section_offset;
			if (label->equ == nullptr)
				label->offset_ptr = &label->offset;
			else//Evaluate expression
				label->equ->update_position(label->offset);
		}

		section_offset += section->second.size;
	}

	section_offset = offset;
	for (auto id : order){
		auto section = section_map_.find(id);
		if (section == section_map_.end())
			continue;//Section not present

		for (auto instruction : section->second.instructions){
			instruction.value->update_position(section_offset);
			instruction.value->encode(writer, reg_tbl_);
			section_offset += instruction.size;
		}
	}
}

std::size_t elang::easm::instruction_table::size() const{
	std::size_t value = 0;
	for (auto &section : section_map_)
		value += section.second.size;
	return value;
}

std::size_t elang::easm::instruction_table::stack_size() const{
	return stack_size_;
}

unsigned __int64 elang::easm::instruction_table::start_address() const{
	if (start_label_.empty())
		return 0u;

	auto entry = label_map_.find(start_label_);
	return ((entry == label_map_.end() || (entry->second.offset_ptr == nullptr)) ? 0u : entry->second.offset);
}

unsigned __int64 elang::easm::instruction_table::write_protect_start() const{
	return write_protect_.from;
}

unsigned __int64 elang::easm::instruction_table::write_protect_end() const{
	return write_protect_.to;
}

elang::memory::memory_register *elang::easm::instruction_table::find_register(const std::string &name) const{
	return reg_tbl_.find(name);
}

unsigned __int64 *&elang::easm::instruction_table::find_label(const std::string &name){
	return label_map_[name].offset_ptr;
}
