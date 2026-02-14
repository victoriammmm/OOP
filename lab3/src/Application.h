#pragma once

class Application {
public:
    int run(int argc, char* argv[]);

private:
    void printHelp() const;
    static bool isWavFile(const char* str);
};