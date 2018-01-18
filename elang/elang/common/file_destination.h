#pragma once

#ifndef ELANG_FILE_DESTINATION_H
#define ELANG_FILE_DESTINATION_H

#include <string>

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/filesystem.hpp>

namespace elang::common{
	struct file_destination{
		static boost::filesystem::path resolve(const std::string &dir, const std::string &file, const std::string &ext){
			boost::filesystem::path path(file);
			if (!dir.empty() && path.is_relative())//Use base path
				path = (boost::filesystem::absolute(dir) / path);

			if (!ext.empty() && !path.has_extension())
				path = (path.string() + "." + ext);

			return path;
		}
	};
}

#endif /* !ELANG_FILE_DESTINATION_H */
