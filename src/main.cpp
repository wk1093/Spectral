#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include <fstream>


#include "moduleLib.h"

struct ExecutableReturnInfo {
    int exit_code;
    std::string log;
};

ExecutableReturnInfo run_executable(const std::string& path, int argc, char* argv[]) {
    ExecutableReturnInfo result = { 0, "" };
#ifdef WIN32
    // Windows specific code to run the executable and capture output
    // FILE* pipe = _popen((path + " 2>&1").c_str(), "r");
    std::string command = path + " ";
    for (int i = 1; i < argc; ++i) {
        command += argv[i];
        command += " ";
    }
    command += "2>&1";
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        perror("popen");
        return result;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result.log += buffer;
    }

    result.exit_code = _pclose(pipe);
    if (result.exit_code == -1) {
        perror("pclose");
        return result;
    }
#else
    // Linux specific code to run the executable and capture output
    std::string command = path + " ";
    for (int i = 1; i < argc; ++i) {
        command += argv[i];
        command += " ";
    }
    command += "2>&1";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        perror("popen");
        return result;
    }
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result.log += buffer;
    }
    result.exit_code = pclose(pipe);
    if (result.exit_code == -1) {
        perror("pclose");
        return result;
    }
#endif
    return result;
}

void message_box(const std::string& title, const std::string& message) {
#ifdef WIN32
    MessageBoxA(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
#else
    // for now, we assume linux users know how to read a log file.
    // in the future, we might want to use a library like GTK or Qt to show a message box.
    // for now, we will just print the message to stderr.
    fprintf(stderr, "%s: %s\n", title.c_str(), message.c_str());
#endif
}

int main(int argc, char* argv[]) {
    // all this program does, is run the executable in the same directory as this one.
    // we will also capture the stdout and stderr and write them to a file.
    // If the program crashes with a non-zero exit code, we will show a message box with the error code and the last few lines of the log file.

    const char* executable_name = SPECTRALENGINE_EXE_NAME;

    // the exe should be in the same directory as this one

    std::string path = (getexedir() / executable_name).string();

    // check if the file exists
    if (!std::filesystem::exists(path)) {
        printf("Error: %s does not exist\n", path.c_str());
        return 1;
    }
   
    // run it while capturing the output and error code
    ExecutableReturnInfo result = run_executable(path, argc, argv);
    std::string log_path = (getexedir() / "log.txt").string();
    std::ofstream log_file(log_path);
    log_file << "Exit code: " << result.exit_code << "\n";
    log_file << "output:\n" << result.log << "\n";
    log_file.close();

    if (result.exit_code != 0) {
        // write the output and error code to a file

        // show a message box with the error code and the last few lines of the log file
        message_box("Error", "The executable crashed with exit code " + std::to_string(result.exit_code) + ".\nCheck the log file for more details.");
    }

    printf("Engine exited with code %d\n", result.exit_code);

    return result.exit_code;
}