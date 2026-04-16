#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace KaiEngine {
    class Logger {
    public:
        static void Init() {
            // 设置日志输出格式：[时间] [日志级别] 消息内容
            spdlog::set_pattern("%^[%T] %n: %v%$");
            
            // 创建一个带颜色的控制台日志器
            s_CoreLogger = spdlog::stdout_color_mt("KAI_CORE");
            s_CoreLogger->set_level(spdlog::level::trace);

            s_CoreLogger->info("Logger Initialized!");
        }

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
}

// 定义宏
#define KAI_CORE_TRACE(...)    ::KaiEngine::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define KAI_CORE_INFO(...)     ::KaiEngine::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define KAI_CORE_WARN(...)     ::KaiEngine::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define KAI_CORE_ERROR(...)    ::KaiEngine::Logger::GetCoreLogger()->error(__VA_ARGS__)