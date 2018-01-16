#pragma once

#ifndef ELANG_FILE_RESOURCE_H
#define ELANG_FILE_RESOURCE_H

#include <string>

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/filesystem.hpp>

namespace elang::common{
	struct file_resource{
		static boost::filesystem::path resolve(const std::string &dir, const std::string &file, const std::string &ext){
			boost::filesystem::path path(file);
			if (!dir.empty() && path.is_relative())//Use base path
				path = (boost::filesystem::absolute(dir) / path);

			if (boost::filesystem::exists(path))
				return path;//Resolved

			if (!ext.empty() && !path.has_extension() && boost::filesystem::exists(path = (path.string() + "." + ext)))
				return path;//Resolve

			return boost::filesystem::path();//Not resolved
		}
	};
}

#endif /* !ELANG_FILE_RESOURCE_H */
