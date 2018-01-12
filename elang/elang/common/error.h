#pragma once

#ifndef ELANG_ERROR_H
#define ELANG_ERROR_H

namespace elang::common{
	enum class error{
		nil,
		register_not_found,
		stack_overflow,
		stack_underflow,
	};
}

#endif /* !ELANG_ERROR_H */
