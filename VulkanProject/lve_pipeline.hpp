#pragma once

#include "lve_device.hpp"
#include <string>
#include <vector>

namespace lve {

	struct PipelineConfigInfo {};
	class LvePipeLine {
	public:
		LvePipeLine(
			LveDevice &device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);
		~LvePipeLine() {}

		LvePipeLine(const LvePipeLine&) = delete;
		void operator=(const LvePipeLine&) = delete;
			
		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(
			const std::string& vertFilePath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
		LveDevice& lveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}	