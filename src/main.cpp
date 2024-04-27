/**
 * Created by:  VPR
 * Created:     September 14th, 2021
 *
 * Updated by:  VPR
 * Updated:     April 27th, 2024
**/

#include "injector.hpp"
#include "logger.hpp"
#include "parser.hpp"

int main(int argc, char** argv) {
    auto p = parser::argument_parser(argc, argv);
    if (p.parse())
    {
        return 1;
    }
    const parser::types::parsed_args_t& args = p.parsed_args();

    types::resource res{ .process_id = args.process_id,
                  .target_process = args.process_name.c_str(),
                  .relative_payload_path = args.relative_payload_path.c_str() };
    types::injector inj{ .status = 0,
                  .operation = args.operation,
                  .delay_ms = static_cast<std::uint32_t>(args.delay.count()),
                  .silent = args.silent,
                  .stealth = args.stealth,
                  .verbosity = args.verbosity,
                  .logger = loggers[args.verbosity],
                  .output_file{} };

    inject_payload(res, inj);

    return 0;
}
