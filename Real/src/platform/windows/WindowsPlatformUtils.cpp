#include <repch.h>
#include "real/utils/PlatformUtils.h"


#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "real/core/Application.h"

namespace Real {

	//TODO: Rewrite into wide strings
	std::optional<std::string> FileDialogs::openFile(const char* filter) {
		OPENFILENAMEA ofn;
		CHAR		  szFile[260] = {0};
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize	 = sizeof(ofn);
		ofn.hwndOwner	 = glfwGetWin32Window((GLFWwindow*)Application::getApplication().getWindow().getNativeWindow());
		ofn.lpstrFile	 = szFile;
		ofn.nMaxFile	 = sizeof(szFile);
		ofn.lpstrFilter	 = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags		 = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
		return std::nullopt;
	}
	std::optional<std::string> FileDialogs::saveFile(const char* filter) {
		OPENFILENAMEA ofn;
		CHAR		  szFile[260] = {0};
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize	 = sizeof(ofn);
		ofn.hwndOwner	 = glfwGetWin32Window((GLFWwindow*)Application::getApplication().getWindow().getNativeWindow());
		ofn.lpstrFile	 = szFile;
		ofn.nMaxFile	 = sizeof(szFile);
		ofn.lpstrFilter	 = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags		 = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt	 = std::strchr(filter,'\0') + 1;
		if (GetSaveFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
		return std::nullopt;
	}
}