#include "lang_type_store.h"

const elang::lang::type_info::ptr_type elang::lang::type_store::auto_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::auto_);

const elang::lang::type_info::ptr_type elang::lang::type_store::void_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::void_);

const elang::lang::type_info::ptr_type elang::lang::type_store::bool_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::bool_);

const elang::lang::type_info::ptr_type elang::lang::type_store::int8_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::int8_);

const elang::lang::type_info::ptr_type elang::lang::type_store::int16_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::int16_);

const elang::lang::type_info::ptr_type elang::lang::type_store::int32_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::int32_);

const elang::lang::type_info::ptr_type elang::lang::type_store::int64_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::int64_);

const elang::lang::type_info::ptr_type elang::lang::type_store::uint8_type = std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int8_);

const elang::lang::type_info::ptr_type elang::lang::type_store::uint16_type = std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int16_);

const elang::lang::type_info::ptr_type elang::lang::type_store::uint32_type = std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int32_);

const elang::lang::type_info::ptr_type elang::lang::type_store::uint64_type = std::make_shared<unsigned_integral_type_info>(primitive_type_info::id_type::int64_);

const elang::lang::type_info::ptr_type elang::lang::type_store::float_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::float_);

const elang::lang::type_info::ptr_type elang::lang::type_store::nullptr_type = std::make_shared<primitive_type_info>(primitive_type_info::id_type::nullptr_);
