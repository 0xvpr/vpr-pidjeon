#include "parser.hpp"
#include "util.hpp"

#include <windows.h>
#include <conio.h>

#include <iostream>
#include <string>
#include <cstdio>

void get_cursor_position(short& x, short& y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;
}

void goto_xy(short x, short y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void print_options(const std::vector<DWORD>& processes, std::size_t selected_option, short x_start, short y_start) {
    std::size_t size = processes.size();
    for (std::size_t i = 0; i < size; ++i) {
        goto_xy(x_start, y_start + static_cast<short>(i & 0xFFFF));
        if (i == selected_option) {
            std::cout << "[+] " << processes[i] << "\n";
        } else {
            std::cout << "[ ] " << processes[i] << "\n";
        }
    }
}

static inline
std::uint32_t select_process(const std::vector<DWORD>& processes) {
    std::size_t selected_option = 0;
    std::size_t size = processes.size();

    if (size == 1) {
        return processes[0];
    }

    std::cout << "Multiple processed found. Select process:\n\n";

    // Get initial cursor position
    short x_start, y_start;
    get_cursor_position(x_start, y_start);

    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo{};
    cursorInfo.bVisible = false;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    print_options(processes, selected_option, x_start, y_start);

    while (true) {
        // Print options
        print_options(processes, selected_option, x_start, y_start);

        // Handle input
        int key = _getch();
        if (key == 27) // Escape
            break;
        else if (key == 224) { // Arrow key
            key = _getch();
            switch (key) {
                case 72: // Up arrow
                    selected_option = (selected_option - 1 + size) % size;
                    break;
                case 80: // Down arrow
                    selected_option = (selected_option + 1) % size;
                    break;
            }
        }
    }

    // Show cursor before exiting
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    return processes[selected_option];
}

static inline
inject::operation get_operation(std::string_view op_str) {
    if (op_str == "lla" || op_str == "LoadLibraryA") {
        return inject::operation::load_library_a;
    } else if (op_str == "llw" || op_str == "LoadLibraryW") {
        return inject::operation::load_library_w;
    } else if (op_str == "crt" || op_str == "CreateRemoteThread") {
        return inject::operation::crt;
    } else if (op_str == "mm" || op_str == "ManualMap") {
        return inject::operation::manual_map;
    }

    return inject::operation::no_operation;
}

static inline
std::string path_to_string(const std::filesystem::path& path) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(path.wstring());
}

namespace parser {

argument_parser::argument_parser(int argc_, char** argv_)
: argc( argc_ ),
  argv( argv_ ),
  parsed_args_{ .program_name = argv[0],
                .operation = inject::operation::load_library_a }
{}

types::error_codes argument_parser::parse() {
    if (argc < 3) {
        usage("Not enough arguments provided");
    }

    bool process_id_set = false;
    bool payload_path_set = false;

    types::error_codes status = types::error_codes::no_error;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                status = parse_option(arg, argv[++i]);
                if (status != types::error_codes::no_error) return status;
            } else {
                status = set_flag(arg);
                if (status != types::error_codes::no_error) return status;
            }
        } else {
            if (!process_id_set) {
                DWORD process_id = 0;
                std::istringstream iss(argv[i]);
                iss >> process_id;
                if (!iss.fail() && process_id != 0) {
                    parsed_args_.process_id = process_id;
                    parsed_args_.process_name = get_process_name_by_process_id(parsed_args_.process_id);

                    process_id_set = true;
                    continue;
                }

                auto processes = get_process_id_by_process_name(argv[i]);
                if (processes.size()) {
                    parsed_args_.process_name = argv[i];
                    parsed_args_.process_id = select_process(processes);

                    process_id_set = true;
                }
            } if (!payload_path_set && std::filesystem::exists(argv[i])) {
                parsed_args_.payload_path = argv[i];
                parsed_args_.relative_payload_path = path_to_string(parsed_args_.payload_path);

                payload_path_set = true;
            }
        }
    }

    if (!parsed_args_.process_id || !std::filesystem::exists(parsed_args_.payload_path)) {
        usage("Positional arguments missing");
        return types::error_codes::invalid_arguments;
    }


    std::cout << "Selected process: " << parsed_args_.process_id << "." << std::endl;
    return types::error_codes::no_error;
}

const types::parsed_args_t& argument_parser::parsed_args() const {
    return parsed_args_;
}

void argument_parser::usage(const std::string error_message) const {
    fprintf(stderr,
        "Error message: %s.\n"
        "\n"
        "Usage: %s <target_process> <path/to/payload> [ <optional arguments> ]\n"
        "\n"
        "positional arguments:\n"
        "  target_process.exe, path/to/payload\n"
        "\n"
        "optional arguments:\n"
        "  -i\t\tspecify injection method\n"
        "  -S,--stealth\tspecify stealth level\n"
        "  -s,--silent\tsuppress output\n"
        "  -o,--output\tspecify output log file\n"
        "  -d\t\tadd delay to the injection (milliseconds)\n"
        "  -v,-vv\t\tset verbosity level\n"
        "\n"
        "example:\n"
        "  %s calc.exe ./payload.dll -i LoadLibraryA -d 2000\n"
        , error_message.c_str(), parsed_args_.program_name.c_str(), parsed_args_.program_name.c_str()
    );

    exit(1);
}

types::error_codes argument_parser::parse_option(const std::string& option, const std::string& value) {
    if (option == "-i") {
        auto operation = get_operation(value);
        if (operation == inject::no_operation) {
            usage("Invalid option: " + value);
        }

        parsed_args_.operation = operation;
    } else if (option == "-d") {
        std::istringstream iss(value);
        std::uint64_t delay_raw;
        iss >> delay_raw;
        if (iss.fail()) {
            usage("Invalid format for frequency");
        }

        parsed_args_.delay = std::chrono::milliseconds{delay_raw};
    } else {
        usage("Unknown option " + option);
    }
    return types::error_codes::no_error;
}

types::error_codes argument_parser::set_flag(const std::string& flag) {
    if (flag == "-S" || flag == "--stealth") {
        parsed_args_.stealth = 1;
    } else if (flag == "-s" || flag == "--silent") {
        parsed_args_.silent = true;
    } else if (flag == "-v") {
        parsed_args_.verbosity = 1;
    } else if (flag == "-vv") {
        parsed_args_.verbosity = 2;
    } else {
        usage("Unsupported flag " + flag);
        return types::error_codes::unsupported_arguments;
    }
    return types::error_codes::no_error;
}

} // namespace parser
