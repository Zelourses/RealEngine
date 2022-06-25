#pragma once

#include <string>
#include <optional>

namespace Real {
	struct FileDialogs final {
		/**
		* @brief creates window for to choose file that will be opened.
		* @param filter what extensions you want to see
		* @return path to file on success, otherwise empty string
		*
		* Creates platform-dependent 'open file' dialog, setting the filtering by extension
		*	(but user still can choose to show all files).
		* If user actually chose file, it will be returned
		* 
		* NOTE: here string_view and string will be broken if using winAPI, because it's syntax looks like <name>\0*.something\0\0
		*	string_view will cut string on first null terminator.
		*/
		static std::optional<std::string> openFile(const char* filter);

		/**
		* @brief creates window for to choose file that will be used for writing data.
		* @param filter what extensions you want to see
		* @return path to file on success, otherwise empty string
		*
		* Creates platform-dependent 'save file' dialog, setting the filtering by extension
		*	(but user still can choose to show all files).
		* If user actually chose file, it will be returned
		* 
		* NOTE: here string_view and string will be broken if using winAPI, because it's syntax looks like <name>\0*.something\0\0
		*	string_view will cut string on first null terminator.
		*/
		static std::optional<std::string> saveFile(const char* filter);
	};
}
