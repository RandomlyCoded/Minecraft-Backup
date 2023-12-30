#include "process.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

#ifdef __WIN32
#define RANDOMLY_HAS_SPAWNV
#else
// A pretty bold assumption, but hey!
#define RANDOMLY_HAS_FORK
#endif

#ifdef RANDOMLY_HAS_FORK
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace randomly {

std::optional<std::intptr_t> run_process(std::initializer_list<std::string> command_line)
{
    const auto make_c_str = [](const std::string &s) {
        return s.c_str();
    };

    auto args = std::vector<const char *>{};
    args.reserve(command_line.size() + 1);

    std::transform(command_line.begin(), command_line.end(),
                   std::back_inserter(args), make_c_str);

    args.emplace_back(nullptr);

#ifdef RANDOMLY_HAS_SPAWNV

    if (const auto result = ::_spawnvp(P_WAIT, args[0], args.data());
        result != -1)
        return result; // succeeded

    std::cerr << "spawnvp(" << args[0] << ") has failed: "
              << std::strerror(errno) << std::endl;
    return {};

#elif defined(RANDOMLY_HAS_FORK)

    if (const auto pid = ::fork();
        pid == pid_t{-1}) {
        // This is in the calling process when fork() has failed.

        std::cerr << "fork(" << args[0] << ") has failed: "
                  << std::strerror(errno) << std::endl;
        return {};
    } else if (pid != 0) {
        // This is in the calling process when fork() succeeded, and we became parent of a process.

        auto status = int{};
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
            return WEXITSTATUS(status);

        std::cerr << "process has failed" << std::endl;
        return {};
    } else {
        // This is in the child process created by fork().

        if (execvp(args[0], const_cast<char **>(args.data())) == -1) {
            std::cerr << "execvp(" << args[0] << ") has failed: "
                      << std::strerror(errno) << std::endl;

            std::abort();
        }

        return {}; // actually this line is never reached
    }

#else

#error Unsupported

#endif
}

std::optional<std::intptr_t>  call(const std::initializer_list<std::string> &command_line)
{
#ifdef RANDOMLY_DEBUGGING
    std::cout << "\033[01;33m";
    for (auto arg: command_line)
        std::cout << arg << " ";

    std::cout  << "\033[0m\b" << std::endl;
#endif // RANDOMLY_DEBUGGING

    return randomly::run_process (command_line);
}


} // namespace randomly
