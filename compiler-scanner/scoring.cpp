#include <iostream>
#include <fstream>
#include <filesystem>

#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " <directory>" << std::endl;
        return 1;
    }

    if (argc >= 3) {
        if (std::string(argv[2]) == "-d") {
            std::string command = "cd " + std::string(argv[1]) + "; rm *.out";
            system(command.c_str());
            return 0;
        }
    }

    std::cout << "[SCORING] Creating Files..." << std::endl;
    // .cm 파일을 읽어서, cminus_cimpl과 cminus_lex로 testN_cimpl.out과 testN_lex.out파일을 생성합니다.
    // testN_result와 testN_cimpl.out, testN_lex.out을 비교해서, 점수를 매깁니다.
    for (auto& p: std::filesystem::directory_iterator(argv[1])) {
        if (p.path().extension() == std::string(".cm")) {
            std::string command = std::string(p.path().relative_path()) + " > " + std::string(p.path().relative_path());
            system(("./cminus_cimpl " + command + ".cimpl.out").c_str());
            std::cout << ("./cminus_cimpl " + command) << std::endl;
            system(("./cminus_lex " + command + ".lex.out").c_str());
            std::cout << ("./cminus_lex " + command) << std::endl;
        }
    }

    sleep(1);

    std::cout << "[SCORING] File has been created." << std::endl;
    for (auto& p: std::filesystem::directory_iterator(argv[1])) {
        // 채점을 진행합니다.
        // _result 파일을 읽어옵니다.
        if (std::string(p.path().filename()).find("_result") != std::string::npos) {
            // _result 파일의 번호를 검사합니다.
            std::string filename = std::string(p.path().filename());
            std::string number = filename.substr(4, filename.find("_result") - 4);
            std::string cimpl_file = "test" + number + ".cm.cimpl.out";
            std::string lex_file = "test" + number + ".cm.lex.out";
            std::string result_file = "test" + number + "_result";

            std::cout << "[SCORING] Scoring " << number << "..." << std::endl;

            // _result 파일을 읽어옵니다.
            std::ifstream result_fin(p.path());
            std::ifstream cimpl_fin(std::string(argv[1] + std::string("/") + cimpl_file));
            std::ifstream lex_fin(std::string(argv[1] + std::string("/") + lex_file));

            // _result 파일의 내용을 읽어옵니다.
            std::string result_str;
            std::string cimpl_str;
            std::string lex_str;

            // 앞 두 줄은 무시합니다.
            for (int i = 0; i < 2; ++i) {
                std::getline(result_fin, result_str);
                std::getline(cimpl_fin, cimpl_str);
                std::getline(lex_fin, lex_str);
            }

            bool is_correct = true;
            while (std::getline(result_fin, result_str)) {
                std::getline(cimpl_fin, cimpl_str);
                std::getline(lex_fin, lex_str);

                if (result_str != cimpl_str) {
                    std::cout << "[SCORING] " << number << " is wrong!" << std::endl;
                    std::cout << "[SCORING] " << result_str << " != " << cimpl_str << std::endl;
                    is_correct = false;
                    break;
                }

                if (result_str != lex_str) {
                    std::cout << "[SCORING] " << number << " is wrong!" << std::endl;
                    std::cout << "[SCORING] " << result_str << " != " << lex_str << std::endl;
                    is_correct = false;
                    break;
                }
            }

            if (is_correct) {
                std::cout << "[SCORING] Test Case " << (number != "" ? number : "1") << " is correct output." << std::endl;
            }
        }
    }

    // 모든 파일을 삭제합니다.
    std::string command = "cd " + std::string(argv[1]) + "; rm *.out";
    system(command.c_str());
    
    std::cout << "[SCORING] Scoring finished!" << std::endl;

    return 0;
}