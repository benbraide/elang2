#pragma once

#ifndef ELANG_ASM_SECTION_ID_H
#define ELANG_ASM_SECTION_ID_H

namespace elang::easm{
	enum class section_id{
		rodata,
		data,
		text,
	};
}

#endif /* !ELANG_ASM_SECTION_ID_H */
