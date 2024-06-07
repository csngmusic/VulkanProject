#pragma once

#include <string>
#include <vector>

namespace lve {
	class LvePipeLine {
	public:
		LvePipeLine(const std::string& vertFilepath, const std::string& fragFilepath);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilepath);
	};
}	