#include "ll1_parser.hpp"
#include <iostream>

using namespace LL1;

void testOperatorPrecedence() {
    std::cout << "\n=== TESTING OPERATOR PRECEDENCE ===\n";
    
    Grammar grammar = ParserFactory::createIntermediateHulkGrammar();
    std::cout << "Grammar is LL(1): " << (grammar.isLL1() ? "YES" : "NO") << std::endl;
    
    if (!grammar.isLL1()) {
        std::cout << "❌ Grammar is not LL(1)" << std::endl;
        return;
    }
    
    auto parser = ParserFactory::createIntermediateHulkParser();
    
    // Test cases para operadores - usando strings directamente
    std::vector<std::string> testCases = {
        // Operadores aritméticos básicos
        "3 + 4 * 2;",
        "10 - 5 + 2;",
        "8 / 2 * 3;",
        "7 % 3 + 1;",
        
        // Operadores de comparación
        "x == 5;",
        "x != 5;",
        "y == z;",
        
        // Operadores relacionales
        "x < 5;",
        "x > 5;",
        "x <= 5;",
        "x >= 5;",
        "a < b;",
        "c > d;",
        
        // Operadores lógicos
        "true && false;",
        "true || false;",
        "flag1 && flag2;",
        "condition1 || condition2;",
        
        // Expresiones complejas con precedencia
        "x + 1 == y * 2;",
        "a * b < c + d;",
        "x < 5 && y > 3;",
        "x == 1 || y != 2;",
        "a + b == c && d < e;",
        
        // Expresiones con paréntesis
        "(x + 1) == (y * 2);",
        "(a < b) && (c > d);",
        "(x == 1) || (y != 2);",
        
        // Let con operadores
        "let x := 5 + 3 in x == 8;",
        "let a := 10, b := 5 in a > b;",
        "let result := x * 2 in result < 20;",
        "let flag := true in flag && false;",
        
        // Let anidados con operadores
        "let x := 5 in let y := x + 1 in x < y;",
        "let a := 10 in let b := a / 2 in a >= b;",
        
        // Precedencia compleja
        "1 + 2 * 3 == 7;",
        "x && y || z;",
        "a < b && c > d || e == f;",
        "let x := 1 + 2 in x * 3 == 9 && true;",
        
        // Casos con diferentes tipos de literales
        "42 > 0;",
        "\"hello\" == \"world\";",
        "true != false;",
        "3.14 < 4.0;"
    };
    
    int passed = 0;
    int total = testCases.size();
    
    for (const auto& testCase : testCases) {
        std::cout << "\n--- Testing: " << testCase << " ---\n";
        
        try {
            auto result = parser->parse(testCase);
            if (result) {
                std::cout << "✓ PASSED\n";
                passed++;
            } else {
                std::cout << "✗ FAILED - Parse failed\n";
            }
        } catch (const std::exception& e) {
            std::cout << "✗ FAILED - Exception: " << e.what() << "\n";
        }
    }
    
    std::cout << "\n=== RESULTS ===\n";
    std::cout << "Passed: " << passed << "/" << total << " tests\n";
    if (passed == total) {
        std::cout << "🎉 All operator precedence tests passed!\n";
    } else {
        std::cout << "❌ Some tests failed\n";
    }
}

void testOperatorAssociativity() {
    std::cout << "\n=== TESTING OPERATOR ASSOCIATIVITY ===\n";
    
    Grammar grammar = ParserFactory::createIntermediateHulkGrammar();
    auto parser = ParserFactory::createIntermediateHulkParser();
    
    // Casos para verificar asociatividad
    std::vector<std::string> testCases = {
        // Asociatividad izquierda para aritméticos
        "1 + 2 + 3;",
        "10 - 5 - 2;",
        "8 / 2 / 2;",
        "2 * 3 * 4;",
        
        // Asociatividad izquierda para comparaciones
        "1 == 2 == 0;", // (1 == 2) == 0 -> false == 0
        "x != y != z;",
        "a < b < c;",
        "x > y > z;",
        
        // Asociatividad izquierda para AND
        "true && false && true;",
        "flag1 && flag2 && flag3;",
        
        // Asociatividad izquierda para OR
        "true || false || true;",
        "condition1 || condition2 || condition3;",
        
        // Casos mixtos complejos
        "a + b + c == d;",
        "x < y && z > w;",
        "p || q && r;", // q && r primero por precedencia
        "let x := 1 + 2 + 3 in x == 6;"
    };
    
    int passed = 0;
    int total = testCases.size();
    
    for (const auto& testCase : testCases) {
        std::cout << "\n--- Testing associativity: " << testCase << " ---\n";
        
        try {
            auto result = parser->parse(testCase);
            if (result) {
                std::cout << "✓ PASSED\n";
                passed++;
            } else {
                std::cout << "✗ FAILED - Parse failed\n";
            }
        } catch (const std::exception& e) {
            std::cout << "✗ FAILED - Exception: " << e.what() << "\n";
        }
    }
    
    std::cout << "\n=== RESULTS ===\n";
    std::cout << "Passed: " << passed << "/" << total << " tests\n";
    if (passed == total) {
        std::cout << "🎉 All associativity tests passed!\n";
    } else {
        std::cout << "❌ Some tests failed\n";
    }
}

void printGrammarInfo() {
    std::cout << "\n=== GRAMMAR INFORMATION ===\n";
    
    Grammar grammar = ParserFactory::createIntermediateHulkGrammar();
    
    std::cout << "Grammar is LL(1): " << (grammar.isLL1() ? "YES" : "NO") << std::endl;
    std::cout << "Number of productions: " << grammar.getProductions().size() << std::endl;
    std::cout << "Number of non-terminals: " << grammar.getNonTerminals().size() << std::endl;
    std::cout << "Number of terminals: " << grammar.getTerminals().size() << std::endl;
    
    std::cout << "\nOperator precedence hierarchy (highest to lowest):\n";
    std::cout << "1. Primary expressions (numbers, identifiers, parentheses)\n";
    std::cout << "2. Multiplicative (*, /, %)\n";
    std::cout << "3. Additive (+, -)\n";
    std::cout << "4. Relational (<, >, <=, >=)\n";
    std::cout << "5. Equality (==, !=)\n";
    std::cout << "6. Logical AND (&&)\n";
    std::cout << "7. Logical OR (||)\n";
    std::cout << "8. Let expressions (let...in)\n";
    
    std::cout << "\nAll operators are left-associative.\n";
}

int main() {
    std::cout << "=== INTERMEDIATE HULK GRAMMAR - OPERATOR TESTS ===\n";
    
    printGrammarInfo();
    testOperatorPrecedence();
    testOperatorAssociativity();
    
    std::cout << "\n=== OPERATOR TESTS COMPLETED ===\n";
    
    return 0;
}
