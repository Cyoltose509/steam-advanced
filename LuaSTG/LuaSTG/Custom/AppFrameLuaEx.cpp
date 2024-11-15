#include "AppFrame.h"
#include "Config.h"
#include "Core/FileManager.hpp"

#include "Platform/HResultChecker.hpp"

namespace LuaSTGPlus
{
    bool AppFrame::OnLoadLaunchScriptAndFiles()
    {
        bool is_launch_loaded = false;
        #ifdef USING_LAUNCH_FILE
        spdlog::info("[luastg] 加载初始化脚本");
        std::vector<uint8_t> src;
        if (GFileManager().loadEx(LUASTG_LAUNCH_SCRIPT, src))
        {
            if (SafeCallScript((char const*)src.data(), src.size(), LUASTG_LAUNCH_SCRIPT))
            {
                is_launch_loaded = true;
                spdlog::info("[luastg] 加载脚本'{}'", LUASTG_LAUNCH_SCRIPT);
            }
            else
            {
                spdlog::error("[luastg] 加载初始化脚本'{}'失败", LUASTG_LAUNCH_SCRIPT);
            }
        }
        if (!is_launch_loaded)
        {
            spdlog::error("[luastg] 找不到文件'{}'", LUASTG_LAUNCH_SCRIPT);
        }
        #endif

        return true;
    };
    
    bool AppFrame::OnLoadMainScriptAndFiles()
    {
        spdlog::info("[luastg] 加载入口点脚本");
        std::string_view entry_scripts[3] = {
            "core.lua",
            "main.lua",
            "src/main.lua",
        };
        std::vector<uint8_t> src;
        bool is_load = false;
        for (auto& v : entry_scripts)
        {
            if (GFileManager().loadEx(v, src))
            {
                if (SafeCallScript((char const*)src.data(), src.size(), v.data()))
                {
                    spdlog::info("[luastg] 加载脚本'{}'", v);
                    is_load = true;
                    break;
                }
            }
        }
        if (!is_load)
        {
            spdlog::error("[luastg] 找不到文件'{}'、'{}'或'{}'", entry_scripts[0], entry_scripts[1], entry_scripts[2]);
        }
        return true;
    }
}
