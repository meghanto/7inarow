#include "test_framework.h"

// Test declarations
void test_edge_generation();
void test_potential_calculation();

int main() {
    std::cout << "Running tests...\n\n";
    
    test_edge_generation();
    test_potential_calculation();
    
    return test::TestRunner::instance().run();
}
