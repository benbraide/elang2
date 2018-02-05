#pragma once

#ifndef ELANG_LANG_TYPE_STORE_H
#define ELANG_LANG_TYPE_STORE_H

#include "lang_primitive_type_info.h"

namespace elang::lang{
	struct type_store{
		static const type_info::ptr_type auto_type;
		static const type_info::ptr_type void_type;
		static const type_info::ptr_type bool_type;
		static const type_info::ptr_type int8_type;
		static const type_info::ptr_type int16_type;
		static const type_info::ptr_type int32_type;
		static const type_info::ptr_type int64_type;
		static const type_info::ptr_type uint8_type;
		static const type_info::ptr_type uint16_type;
		static const type_info::ptr_type uint32_type;
		static const type_info::ptr_type uint64_type;
		static const type_info::ptr_type float_type;
		static const type_info::ptr_type nullptr_type;
	};
}

#endif /* !ELANG_LANG_TYPE_STORE_H */
