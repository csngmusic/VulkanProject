#include "lve_pipeline.hpp"
#include <fstream>
#include <iostream>

namespace lve {

	LvePipeLine::LvePipeLine(const std::string& vertFilepath, const std::string& fragFilepath) {
		
		createGraphicsPipeline(vertFilepath, fragFilepath);
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

	void LvePipeLine::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath) {
	
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
	}
}