#include "lang_symbol_table.h"

elang::lang::symbol_table::symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: name_(name), parent_(parent), attributes_(attributes){}

elang::lang::symbol_table::~symbol_table() = default;

elang::lang::symbol_table *elang::lang::symbol_table::parent() const{
	return parent_;
}

const std::string &elang::lang::symbol_table::name() const{
	return name_;
}

std::string elang::lang::symbol_table::mangle() const{
	if (parent_ == nullptr)
		return ("N" + std::to_string(name_.size()) + name_);
	return (parent_->mangle() + "N" + std::to_string(name_.size()) + name_);
}

elang::lang::symbol_table::entry_attribute_type elang::lang::symbol_table::attributes() const{
	return attributes_;
}

void elang::lang::symbol_table::add(std::shared_ptr<symbol_table> table){
	auto entry = map_.find(table->name_);
	if (entry != map_.end()){//Update existing
		if (entry->second.table == nullptr)
			entry->second.table = table;
		else//Error
			throw common::error::lang_symbol_exists;
	}
	else//New entry
		map_[table->name_] = entry_info{ table };

	auto type = dynamic_cast<type_info *>(table.get());
	if (type != nullptr)
		type_list_.push_back(type);
}

void elang::lang::symbol_table::add(const variable_entry_info &variable){
	auto entry = map_.find(variable.name);
	if (entry != map_.end()){//Update existing
		if (!entry->second.non_table.holds_value){
			entry->second.non_table.holds_value = true;
			entry->second.non_table.value = variable;
			order_list_.push_back(&std::get<variable_entry_info>(entry->second.non_table.value));
		}
		else//Error
			throw common::error::lang_symbol_exists;
	}
	else{//New entry
		map_[variable.name] = entry_info{ nullptr, non_table_entry_info{ true, variable } };
		order_list_.push_back(&std::get<variable_entry_info>(map_[variable.name].non_table.value));
	}
}

void elang::lang::symbol_table::add(const function_entry_info &function){
	auto entry = map_.find(function.name);
	if (entry != map_.end()){//Update existing
		if (std::holds_alternative<function_list_entry_info>(entry->second.non_table.value)){
			auto &func_list = std::get<function_list_entry_info>(entry->second.non_table.value);
			add_function_(func_list, function);
		}
		else if (!entry->second.non_table.holds_value){
			entry->second.non_table.holds_value = true;
			entry->second.non_table.value = function_list_entry_info{};
			add_function_(std::get<function_list_entry_info>(entry->second.non_table.value), function);
		}
		else//Error
			throw common::error::lang_symbol_exists;
	}
	else{//New entry
		auto &non_table = map_[function.name].non_table;
		non_table.holds_value = true;
		non_table.value = function_list_entry_info{};
		add_function_(std::get<function_list_entry_info>(non_table.value), function);
	}
}

void elang::lang::symbol_table::add_namespace(const std::string &name, entry_attribute_type attributes){
	add(std::make_shared<symbol_table>(name, this, attributes));
}

void elang::lang::symbol_table::add_variable(const std::string &name, type_info::ptr_type type, entry_attribute_type attributes){
	add(variable_entry_info{ this, attributes, type, name });
}

void elang::lang::symbol_table::add_function(const std::string &name, type_info::ptr_type return_type,
	std::vector<variable_entry_info> &&parameters, entry_attribute_type attributes){
	std::vector<type_info::ptr_type> parameter_types;
	if (!parameters.empty()){
		parameter_types.reserve(parameters.size());
		for (auto &parameter : parameters)
			parameter_types.push_back(parameter.type);
	}

	function_entry_info info;
	info.attributes = attributes;
	info.name = name;
	info.type = std::make_shared<function_type_info>(return_type, std::move(parameter_types));
	info.parameters = std::move(parameters);

	add(info);
}

elang::lang::symbol_table::entry_info *elang::lang::symbol_table::find(const std::string &name) const{
	auto entry = map_.find(name);
	return ((entry == map_.end()) ? nullptr : const_cast<entry_info *>(&entry->second));
}

elang::lang::symbol_table *elang::lang::symbol_table::find_table(const std::string &name) const{
	auto entry = map_.find(name);
	return ((entry == map_.end()) ? nullptr : entry->second.table.get());
}

elang::lang::symbol_table::non_table_type *elang::lang::symbol_table::find_non_table(const std::string &name) const{
	auto entry = map_.find(name);
	return ((entry == map_.end() || !entry->second.non_table.holds_value) ? nullptr : const_cast<non_table_type *>(&entry->second.non_table.value));
}

elang::lang::type_info *elang::lang::symbol_table::find_type(const std::string &name) const{
	auto entry = map_.find(name);
	return ((entry == map_.end()) ? nullptr : dynamic_cast<type_info *>(entry->second.table.get()));
}

elang::lang::symbol_table::variable_entry_info *elang::lang::symbol_table::find_variable(const std::string &name) const{
	auto non_table = find_non_table(name);
	if (non_table == nullptr)
		return nullptr;//Not found

	if (std::holds_alternative<variable_entry_info>(*non_table))
		return &std::get<variable_entry_info>(*non_table);

	auto &func_list = std::get<function_list_entry_info>(*non_table);
	return ((func_list.list.size() == 1u) ? &func_list.list.begin()->second : nullptr);
}

elang::lang::symbol_table::function_list_entry_info *elang::lang::symbol_table::find_function(const std::string &name) const{
	auto non_table = find_non_table(name);
	if (non_table == nullptr || !std::holds_alternative<function_list_entry_info>(*non_table))
		return nullptr;//Not found or not a function

	return &std::get<function_list_entry_info>(*non_table);
}

unsigned __int64 elang::lang::symbol_table::compute_offset(const symbol_table &table) const{
	return 0;
}

unsigned __int64 elang::lang::symbol_table::compute_offset(const variable_entry_info &var) const{
	return 0;
}

elang::lang::symbol_table::function_entry_info *elang::lang::symbol_table::match_function(const std::vector<function_entry_info> &list,
	const std::vector<type_info::ptr_type> &parameter_types){
	function_entry_info *matched = nullptr;
	auto matched_score = ELANG_TYPE_INFO_MIN_SCORE, score = 0, count = 0;
	for (auto &entry : list){
		if ((score = dynamic_cast<function_type_info *>(entry.type.get())->call_score(parameter_types)) > ELANG_TYPE_INFO_MIN_SCORE){
			if (score > matched_score){//Better match
				matched_score = score;
				matched = const_cast<function_entry_info *>(&entry);
				count = 1;
			}
			else if (score == matched_score)
				++count;//Ambiguous call
		}
	}

	if (matched == nullptr)
		throw common::error::lang_unmatched_call;

	if (count > 1)
		throw common::error::lang_ambiguous_call;

	return matched;
}

void elang::lang::symbol_table::add_function_(function_list_entry_info &list, const function_entry_info &info){
	auto key = dynamic_cast<function_type_info *>(info.type.get())->mangle_parameters();
	if (list.list.find(key) == list.list.end())
		list.list[key] = info;
	else//Error
		throw common::error::lang_symbol_exists;
}
