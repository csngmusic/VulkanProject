#include "lve_pipeline.hpp"
#include <fstream>
#include <iostream>

namespace lve {

	LvePipeLine::LvePipeLine(
		LveDevice& device,
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) : lveDevice{device} {
		
		createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	std::vector<char> LvePipeLine::readFile(const std::string& filepath) {
	
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file: " + filepath);
		}

		size_t fileSize = static_cast<size_t> (file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void LvePipeLine::createGraphicsPipeline(
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) {
	
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
	}

	void LvePipeLine::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create shader module");
		}
	}

	PipelineConfigInfo LvePipeLine::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
		PipelineConfigInfo configInfo{};

		return configInfo;
	}

}