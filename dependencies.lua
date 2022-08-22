-- file for all dependecies in project, like vulkan, submodules, etc.

VULKAN_SDK = os.getenv("VULKAN_SDK");

-- all directories that will be included in projects
includeDir = {}
includeDir["GLFW"] = "%{wks.location}/Real/vendor/glfw/include"
includeDir["Glad"] = "%{wks.location}/Real/vendor/glad/include"
includeDir["ImGUI"] = "%{wks.location}/Real/vendor/imgui"
includeDir["glm"] = "%{wks.location}/Real/vendor/glm"
includeDir["stb_image"] = "%{wks.location}/Real/vendor/stb_image"
includeDir["entt"] = "%{wks.location}/Real/vendor/entt/single_include"
includeDir["spdlog"] = "%{wks.location}/Real/vendor/spdlog/include"
includeDir["yamlcpp"] = "%{wks.location}/Real/vendor/yaml-cpp/include"
includeDir["ImGuizmo"] = "%{wks.location}/Real/vendor/ImGuizmo"
includeDir["vulkanSDK"] = "%{VULKAN_SDK}/Include"

---- all libraries that will be linked with projects ----
library = {}
library["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"
library["VulkanUtils"] = "%{VULKAN_SDK}/Lib/VkLayer_utils.lib"

-- debug versions of libs --
library["ShaderC_debug"] = "%{VULKAN_SDK}/Lib/shaderc_sharedd.lib"
library["SPIRVcross_debug"] = "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib"
library["SPIRVcrossGLSL_debug"] = "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib"
library["SPIRVTools_debug"] = "%{VULKAN_SDK}/Lib/SPIRV-Toolsd.lib"
-- release versions of libs --
library["ShaderC_release"] = "%{VULKAN_SDK}/Lib/shaderc_shared.lib"
library["SPIRVcross_release"] = "%{VULKAN_SDK}/Lib/spirv-cross-core.lib"
library["SPIRVcrossGLSL_release"] = "%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib"

