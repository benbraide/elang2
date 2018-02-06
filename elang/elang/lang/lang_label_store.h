#pragma once

#ifndef ELANG_LANG_LABEL_STORE_H
#define ELANG_LANG_LABEL_STORE_H

#include <string>
#include <unordered_map>

namespace elang::lang{
	class label_store{
	public:
		enum class target_type{
			constant,
			non_constant,
			function,
		};

		struct target_info{
			unsigned __int64 index;
			std::string id;
		};

		typedef std::unordered_map<target_type, target_info> map_type;

		label_store(){
			map_[target_type::constant] = target_info{ 0ui64, "C" };
			map_[target_type::non_constant] = target_info{ 0ui64, "N" };
			map_[target_type::function] = target_info{ 0ui64, "F" };
		}

		std::string generate(target_type target){
			auto entry = map_.find(target);
			if (entry == map_.end())//Use default
				entry = map_.find(target_type::constant);

			return (".L" + entry->second.id + std::to_string(entry->second.index++));
		}

	private:
		map_type map_;
	};
}

#endif /* !ELANG_LANG_LABEL_STORE_H */
