#include "logger.hpp"
#include "util.hpp"

#include <filesystem>
#include <cstdlib>
#include <cstdio>
#include <ctime>

std::int32_t log_basic(const types::parsed_args_t& args, const char* event, std::uint32_t shiftwidth) {
    (void)args;
    int bytes_written = 0;

    time_t rawtime = 0;
    struct tm* info = nullptr;

    time(&rawtime);
    info = localtime(&rawtime);

    char prefix[64] = { 0 };
    strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
    
    while (shiftwidth--) bytes_written += fputc(' ', stdout);
    bytes_written += fprintf(stdout, "%s %s.\n", prefix, event);

    return bytes_written;
}

std::int32_t log_advanced(const types::parsed_args_t& args, const char* event, std::uint32_t shiftwidth) {
    int bytesWritten = 0;
    int verbosity = args.verbosity;
    char output_file[MAX_PATH]{ 0 };
    [&args, &output_file]() -> auto {
        const std::string& path_wstr = args.log_path.string();
        std::size_t str_size = path_wstr.size();
        for (std::size_t i = 0; i < str_size; ++i) {
            output_file[i] = static_cast<char>(path_wstr[i] & 0xFF);
        }
    }();

    FILE* fp = nullptr;
    if (!std::filesystem::exists(output_file)) {
        if ((fp = fopen(output_file, "wb"))) {
            time_t rawtime = 0;
            struct tm* info = nullptr;

            time(&rawtime);
            info = localtime(&rawtime);

            char prefix[64] = { 0 };
            strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
            
            while (shiftwidth--) fputc(' ', fp);
            bytesWritten += fprintf(fp, "%s Log file created.\n", prefix);
            fclose(fp);
        }
        else {
            if (verbosity > 2) {
                fprintf(stderr, "Error while opening '%s'.", output_file);
            }

            return -1;
        }
    }

    if ((fp = fopen(output_file, "ab"))) {
        time_t rawtime = 0;
        struct tm* info = nullptr;

        time(&rawtime);
        info = localtime(&rawtime);

        char prefix[64] = { 0 };
        strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
        
        while (shiftwidth--) fputc(' ', fp);
        bytesWritten += fprintf(fp, "%s %s.\n", prefix, event);
        fclose(fp);
    } else {
        if (verbosity > 1) {
            fprintf(stderr, "Error while opening '%s'.", output_file);
        }

        return -1;
    }

    return bytesWritten;
}
