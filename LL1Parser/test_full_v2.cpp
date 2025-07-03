#include "ll1_parser.hpp"
#include <iostream>

using namespace LL1;

void testFullHulkGrammarV2() {
    std::cout << "=== Test: Full HULK Grammar V2 Creation ===" << std::endl;
    
    // Crear la gramática completa V2
    Grammar fullGrammarV2 = ParserFactory::createFullHulkGrammarV2();
    std::cout << "Full HULK Grammar V2 Productions (" << fullGrammarV2.getProductions().size() << " total):" << std::endl;
    fullGrammarV2.printGrammar();
    std::cout << std::endl;
    
    // Verificar si es LL(1)
    bool isLL1 = fullGrammarV2.isLL1();
    std::cout << "Full Grammar V2 is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;
    
    if (isLL1) {
        std::cout << "✅ Full HULK grammar V2 is LL(1) compatible!" << std::endl;
        
        // Mostrar algunos conjuntos FIRST y FOLLOW para debug
        std::cout << "\n=== DEBUG: FIRST and FOLLOW sets for full grammar V2 ===" << std::endl;
        fullGrammarV2.printFirstSets();
        fullGrammarV2.printFollowSets();
        std::cout << "=== END DEBUG ===\n" << std::endl;
        
        // Si es LL(1), probar el parser
        auto parser = ParserFactory::createFullHulkParserV2();
        
        // Test expresiones progresivamente más complejas
        std::vector<std::string> testInputs = {
            // Expresiones básicas
            "123;",
            "x;",
            "true;",
            "false;",
            
            // Operadores
            "x + y * z;",
            "a == b && c > d;",
            "x < 5 || y != 3;",
            
            // Let expressions
            "let x := 5 in x + 1;",
            "let a := 10, b := 5 in a > b;",
            
            // If expressions
            "if (true) 1 else 0;",
            "if (x > 0) x else 0;",
            "if (a == b) 1 elif (a > b) 2 else 3;",
            
            // While y for
            "while (x > 0) x := x - 1;",
            "for (i in range) print(i);",
            
            // Bloques
            "{ x := 5; y := x + 1; };",
            
            // Llamadas a función
            "add(1, 2);",
            "max(a, b, c);",
            "foo();",
            
            // Instanciación
            "new Point(1, 2);",
            "new Vector();",
            
            // Funciones simples
            "function add(x, y) => x + y;",
            "function square(x) => x * x;",
            "function factorial(n) { if (n <= 1) 1 else n * factorial(n - 1); };",
            
            // Casos más complejos
            "let result := if (x > 0) add(x, 1) else 0 in result * 2;",
            "function fibonacci(n) => if (n <= 1) n else fibonacci(n-1) + fibonacci(n-2);",
            "let numbers := [1, 2, 3] in for (num in numbers) print(num);"
        };
        
        int passed = 0;
        int total = testInputs.size();
        
        for (const auto& input : testInputs) {
            std::cout << "\n--- Testing full grammar V2 with: \"" << input << "\" ---" << std::endl;
            
            try {
                auto result = parser->parse(input);
                if (result) {
                    std::cout << "✅ PASSED - Parsed successfully" << std::endl;
                    passed++;
                } else {
                    std::cout << "❌ FAILED - Parse failed (null result)" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "❌ FAILED - Exception: " << e.what() << std::endl;
            }
        }
        
        std::cout << "\n=== RESULTS ===" << std::endl;
        std::cout << "Passed: " << passed << "/" << total << " tests" << std::endl;
        if (passed == total) {
            std::cout << "🎉 All full grammar V2 tests passed!" << std::endl;
        } else {
            std::cout << "⚠️  Some tests failed - need to debug" << std::endl;
        }
        
    } else {
        std::cout << "❌ Full grammar V2 has LL(1) conflicts - need to refactor" << std::endl;
        
        // Mostrar información de debug para identificar conflictos
        std::cout << "\n=== DEBUG: Grammar info ===" << std::endl;
        std::cout << "Number of productions: " << fullGrammarV2.getProductions().size() << std::endl;
        std::cout << "Number of non-terminals: " << fullGrammarV2.getNonTerminals().size() << std::endl;
        std::cout << "Number of terminals: " << fullGrammarV2.getTerminals().size() << std::endl;
    }
    
    std::cout << std::endl;
}

void comparaisonTest() {
    std::cout << "=== Comparison: Intermediate vs Full V2 Grammar ===" << std::endl;
    
    Grammar intermediate = ParserFactory::createIntermediateHulkGrammar();
    Grammar fullV2 = ParserFactory::createFullHulkGrammarV2();
    
    std::cout << "Intermediate Grammar:" << std::endl;
    std::cout << "  - Productions: " << intermediate.getProductions().size() << std::endl;
    std::cout << "  - LL(1): " << (intermediate.isLL1() ? "YES" : "NO") << std::endl;
    
    std::cout << "Full Grammar V2:" << std::endl;
    std::cout << "  - Productions: " << fullV2.getProductions().size() << std::endl;
    std::cout << "  - LL(1): " << (fullV2.isLL1() ? "YES" : "NO") << std::endl;
    
    std::cout << "\nFeatures added in Full V2:" << std::endl;
    std::cout << "  - Function declarations" << std::endl;
    std::cout << "  - If/elif/else expressions" << std::endl;
    std::cout << "  - While loops" << std::endl;
    std::cout << "  - For loops" << std::endl;
    std::cout << "  - Block expressions" << std::endl;
    std::cout << "  - Function calls" << std::endl;
    std::cout << "  - Object instantiation" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "=== FULL HULK GRAMMAR V2 TESTS ===" << std::endl;
    
    comparaisonTest();
    testFullHulkGrammarV2();
    
    std::cout << "=== FULL HULK GRAMMAR V2 TESTS COMPLETED ===" << std::endl;
    
    return 0;
}
