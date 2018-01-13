#pragma once

#ifndef ELANG_BYTE_CODE_COMPARISON_H
#define ELANG_BYTE_CODE_COMPARISON_H

namespace elang::byte_code{
	enum class comparison_id : unsigned char{
		nil,
		less,
		equal,
		greater,
		less_or_equal,
		greater_or_equal,
	};

	struct comparison_info{
		unsigned char inverse	: 1;
		comparison_id id		: 7;
	};

	struct comparison{
		static bool compare(const comparison_info &info, char flag){
			if (flag < '\0')//Flag is less
				return ((info.id == comparison_id::less || info.id == comparison_id::less_or_equal) == (info.inverse == '\0'));

			if (flag == '\0'){//Flag is zero
				return ((info.id == comparison_id::equal || info.id == comparison_id::less_or_equal ||
					info.id == comparison_id::greater_or_equal) == (info.inverse == '\0'));
			}

			return ((info.id == comparison_id::greater || info.id == comparison_id::greater_or_equal) == (info.inverse == '\0'));
		}
	};
}

#endif /* !ELANG_BYTE_CODE_COMPARISON_H */
