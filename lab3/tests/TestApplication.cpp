#include <gtest/gtest.h>
#include "../Application.h"

TEST(TestApplication, HelpCommand) {
    Application app;
    char* argv[] = {(char*)"program", (char*)"-h"};
    EXPECT_EQ(app.run(2, argv), 0);
}

TEST(TestApplication, HelpWithExtraArgsFails) {
    Application app;
    char* argv[] = {(char*)"program", (char*)"-h", (char*)"extra"};
    EXPECT_EQ(app.run(3, argv), 1);
}

TEST(TestApplication, RunModeValid) {
    std::ofstream config("config.txt");
    config << "mute 0 1";
    config.close();
    
    std::ofstream wav("input.wav", std::ios::binary);
    char header[44] = {
        'R','I','F','F', 0x24,0,0,0, 'W','A','V','E',
        'f','m','t',' ', 0x10,0,0,0, 0x01,0, 0x01,0,
        0x44,0xAC,0,0, 0x88,0x58,0x01,0, 0x02,0, 0x10,0,
        'd','a','t','a', 0x00,0,0,0
    };
    wav.write(header, 44);
    wav.close();
    
    Application app;
    char* argv[] = {
        (char*)"program", (char*)"-c", 
        (char*)"config.txt", (char*)"output.wav", (char*)"input.wav"
    };
    
    EXPECT_EQ(app.run(5, argv), 0);
    
    remove("config.txt");
    remove("input.wav");
    remove("output.wav");
}

TEST(TestApplication, RunModeMissingArgsFails) {
    Application app;
    
    char* argv1[] = {(char*)"program", (char*)"-c"};
    EXPECT_EQ(app.run(2, argv1), 1);
    
    char* argv2[] = {(char*)"program", (char*)"-c", (char*)"config.txt"};
    EXPECT_EQ(app.run(3, argv2), 1);
    
    char* argv3[] = {(char*)"program", (char*)"-c", (char*)"config.txt", (char*)"output.wav"};
    EXPECT_EQ(app.run(4, argv3), 1);
}

TEST(TestApplication, UnknownModeFails) {
    Application app;
    char* argv[] = {(char*)"program", (char*)"-x"};
    EXPECT_EQ(app.run(2, argv), 1);
}

TEST(TestApplication, NoArgsFails) {
    Application app;
    char* argv[] = {(char*)"program"};
    EXPECT_EQ(app.run(1, argv), 1);
}

TEST(TestApplication, NonWavFileFails) {
    std::ofstream config("config.txt");
    config << "mute 0 1";
    config.close();
    
    std::ofstream txt("input.txt");
    txt << "not a wav";
    txt.close();
    
    Application app;
    char* argv[] = {
        (char*)"program", (char*)"-c", 
        (char*)"config.txt", (char*)"output.wav", (char*)"input.txt"
    };
    
    EXPECT_EQ(app.run(5, argv), 1);
    
    remove("config.txt");
    remove("input.txt");
}
