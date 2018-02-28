#pragma once

#ifndef ELANG_RAII_H
#define ELANG_RAII_H

namespace elang::common{
	template <class value_type>
	class raii_value{
	public:
		raii_value(value_type &ref_obj, const value_type &value)
			: ref_obj_(ref_obj), old_value_(ref_obj){
			ref_obj = value;
		}

		~raii_value(){
			ref_obj_ = old_value_;
		}

	private:
		value_type &ref_obj_;
		value_type old_value_;
	};
}

#endif /* !ELANG_RAII_H */
