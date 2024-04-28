/**
 * Created by:  VPR
 * Created:     September 14th, 2021
 *
 * Updated by:  VPR
 * Updated:     April 28th, 2024
**/

#include   "injector.hpp"
#include   "parser.hpp"

int main(int argc, char** argv) {
    auto p = parser::argument_parser(argc, argv);
    if (p.parse())
    {
        return 1; // init GUI?
    }

    const auto& args = p.parsed_args();
    inject_payload(args);

    return 0;
}
