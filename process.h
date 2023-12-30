#ifndef RANDOMLY_PROCESS_H
#define RANDOMLY_PROCESS_H

#include <cstdint>
#include <optional>
#include <string>

namespace randomly {

std::optional<std::intptr_t> run_process(std::initializer_list<std::string> command_line);

// wrapper for run_process that prints the command in debug builds
std::optional<std::intptr_t> call(const std::initializer_list<std::string> &command_line);

} // namespace randomly

#endif // RANDOMLY_PROCESS_H
