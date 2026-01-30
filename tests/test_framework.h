#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace test {

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

class TestRunner {
public:
    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }
    
    void add_result(const TestResult& result) {
        results_.push_back(result);
        if (result.passed) {
            std::cout << "[PASS] " << result.name << "\n";
        } else {
            std::cout << "[FAIL] " << result.name << ": " << result.message << "\n";
        }
    }
    
    int run() {
        int failed = 0;
        for (const auto& r : results_) {
            if (!r.passed) ++failed;
        }
        
        std::cout << "\n" << results_.size() << " tests, " 
                  << (results_.size() - static_cast<size_t>(failed)) << " passed, " 
                  << failed << " failed\n";
        
        return failed;
    }
    
private:
    std::vector<TestResult> results_;
};

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            test::TestRunner::instance().add_result({__func__, false, msg}); \
            return; \
        } \
    } while (0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            test::TestRunner::instance().add_result({__func__, false, msg}); \
            return; \
        } \
    } while (0)

#define TEST_PASS() \
    test::TestRunner::instance().add_result({__func__, true, ""})

} // namespace test
