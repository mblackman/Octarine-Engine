#include "Lua/Modules/IoModuleLuaBinding.h"

#include <fstream>
#include <string>
#include <vector>

#include "General/Logger.h"

namespace
{
    std::vector<std::string> ReadFileLines(const std::string& filename)
    {
        std::vector<std::string> lines;
        if (std::ifstream file(filename); file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                lines.push_back(line);
            }
            file.close();
        }
        else
        {
            Logger::Error("Failed to open file: " + filename);
        }
        return lines;
    }
} // namespace

void LuaModuleBinding<IoModule>::install(sol::state& lua, Game& /*game*/)
{
    lua.set_function("read_file_lines", &ReadFileLines);
}
