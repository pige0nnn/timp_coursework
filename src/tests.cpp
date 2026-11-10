#include <UnitTest++/UnitTest++.h>
#include "../include/ClientDB.h"
#include "../include/Authenticator.h"
#include "../include/VectorProcessor.h"
#include "../include/UserInterface.h"
#include "../include/Logger.h"
#include <fstream>
#include <vector>
#include <cstdio>

// --- ТЕСТЫ ДЛЯ CLIENTDB ---
SUITE(ClientDBTest) {
    struct DBFixture {
        DBFixture() {
            std::ofstream f("test_auth.txt");
            f << "user:P@ssW0rd\nadmin:12345\n";
        }
        ~DBFixture() { std::remove("test_auth.txt"); }
    };

    TEST_FIXTURE(DBFixture, LoadAndCheck) {
        ClientDB db;
        db.load("test_auth.txt");
        CHECK(db.userExists("user"));
        CHECK(db.userExists("admin"));
        CHECK(!db.userExists("unknown"));
    }

    TEST_FIXTURE(DBFixture, GetPassword) {
        ClientDB db;
        db.load("test_auth.txt");
        CHECK_EQUAL("P@ssW0rd", db.getPassword("user"));
        CHECK_EQUAL("", db.getPassword("unknown"));
    }
}

// --- ТЕСТЫ ДЛЯ AUTHENTICATOR ---
SUITE(AuthenticatorTest) {
    const std::string SALT = "AF95CC0F68EA56A3";
    const std::string PASS = "P@ssW0rd";
    const std::string HASH = "a18bebc2104544c09833b986e09d7cb299ccaba7bad0419d6365027a3";

    TEST(ComputeHash) {
        std::string computed = Authenticator::computeSHA224Hash(SALT, PASS);
        CHECK_EQUAL(HASH, computed);
    }

    TEST(VerifyHashSuccess) {
        CHECK(Authenticator::verifyHash(SALT, PASS, HASH));
    }

    TEST(VerifyHashFailure) {
        CHECK(!Authenticator::verifyHash(SALT, "WrongPassword", HASH));
    }
}

// --- ТЕСТЫ ДЛЯ VECTORPROCESSOR ---
SUITE(VectorProcessorTest) {
    TEST(EmptyVector) {
        std::vector<double> v = {};
        CHECK_EQUAL(1.0, VectorProcessor::multiply(v));
    }

    TEST(SingleElement) {
        std::vector<double> v = {5.5};
        CHECK_EQUAL(5.5, VectorProcessor::multiply(v));
    }

    TEST(MultipleElements) {
        std::vector<double> v = {2.0, 3.0, 4.0};
        CHECK_EQUAL(24.0, VectorProcessor::multiply(v));
    }

    TEST(NegativeElements) {
        std::vector<double> v = {-2.0, 3.0};
        CHECK_EQUAL(-6.0, VectorProcessor::multiply(v));
    }
}

// --- ТЕСТЫ ДЛЯ USERINTERFACE ---
SUITE(UserInterfaceTest) {
    TEST(HelpWithoutParams) {
        UserInterface ui;
        const char* argv[] = {"server"};
        CHECK(!ui.Parser(1, argv));
        CHECK(!ui.getDescription().empty());
    }

    TEST(HelpFlag) {
        UserInterface ui;
        const char* argv[] = {"server", "-h"};
        CHECK(!ui.Parser(2, argv));
    }

    TEST(CustomParams) {
        UserInterface ui;
        const char* argv[] = {"server", "-p", "12345", "-a", "my_auth.txt"};
        CHECK(ui.Parser(5, argv));
        CHECK_EQUAL(12345, ui.getParams().port);
        CHECK_EQUAL("my_auth.txt", ui.getParams().authFile);
    }
}

// --- ТЕСТЫ ДЛЯ LOGGER ---
SUITE(LoggerTest) {
    TEST(LogCreationAndWriting) {
        std::string testLogFile = "test_server.log";
        std::remove(testLogFile.c_str());

        {
            Logger logger(testLogFile);
            logger.log("Тестовое сообщение", 0);
        }

        std::ifstream f(testLogFile);
        CHECK(f.is_open());
        
        std::string line;
        std::getline(f, line);
        CHECK(line.find("[INFO]") != std::string::npos);
        CHECK(line.find("Тестовое сообщение") != std::string::npos);
        
        f.close();
        std::remove(testLogFile.c_str());
    }
}

int main() {
    return UnitTest::RunAllTests();
}
