#pragma once

#ifndef ELANG_LANG_STATIC_TYPE_RESOLVER_H
#define ELANG_LANG_STATIC_TYPE_RESOLVER_H

#include "lang_primitive_type_info.h"

namespace elang::lang{
	template <class target_type>
	struct static_type_resolver;

	template <>
	struct static_type_resolver<__int8>{
		static type_info::ptr_type type(){
			return std::make_shared<primitive_type_info>(primitive_type_info::id_type::int8_);
		}
	};

	template <>
	struct static_type_resolver<__int16>{
		static type_info::ptr_type type(){
			return std::make_shared<primitive_type_info>(primitive_type_info::id_type::int16_);
		}
	};

	template <>
	struct static_type_resolver<__int32>{
		static type_info::ptr_type type(){
			return std::make_shared<primitive_type_info>(primitive_type_info::id_type::int32_);
		}
	};

	template <>
	struct static_type_resolver<__int64>{
		static type_info::ptr_type type(){
			return std::make_shared<primitive_type_info>(primitive_type_info::id_type::int64_);
		}
	};
	  
	template <>
	struct static_type_resolver<long double>{
		static type_info::ptr_type type(){
			return std::make_shared<primitive_type_info>(primitive_type_info::id_type::float_);
		}
	};

	template <>
	struct static_type_resolver<unsigned __int8>{
		static type_info::ptr_type type(){
			return std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int8_);
		}
	};

	template <>
	struct static_type_resolver<unsigned __int16>{
		static type_info::ptr_type type(){
			return std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int16_);
		}
	};

	template <>
	struct static_type_resolver<unsigned __int32>{
		static type_info::ptr_type type(){
			return std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int32_);
		}
	};

	template <>
	struct static_type_resolver<unsigned __int64>{
		static type_info::ptr_type type(){
			return std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int64_);
		}
	};
}

#endif /* !ELANG_LANG_STATIC_TYPE_RESOLVER_H */
