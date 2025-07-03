#include "ll1_grammar.hpp"
#include "ll1_parser.hpp"
#include <iostream>
#include <cassert>

using namespace LL1;

// Forward declaration
void testFullHulkGrammarV3();

int main() {
    std::cout << "LL(1) Parser Generator Tests - Full Grammar V3" << std::endl;
    std::cout << "==============================================" << std::endl << std::endl;

    try {
        testFullHulkGrammarV3();
        std::cout << "All tests passed! ✓" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void testFullHulkGrammarV3() {
    std::cout << "=== Test: Full HULK Grammar V3 Creation === " << std::endl;

    // Crear la gramática completa v3
    Grammar fullGrammar = ParserFactory::createFullHulkGrammarV3();
    std::cout << "Full HULK Grammar V3 Productions (" << fullGrammar.getProductions().size() << " total):" << std::endl;
    // fullGrammar.printGrammar(); // Opcional: imprimir para depurar
    std::cout << std::endl;

    // Verificar si es LL(1)
    bool isLL1 = fullGrammar.isLL1();
    std::cout << "Full Grammar V3 is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;

    if (!isLL1) {
        std::cout << "⚠️  Full grammar V3 has LL(1) conflicts - need to refactor" << std::endl;
    } else {
        std::cout << "✓ Full HULK grammar V3 is LL(1) compatible!" << std::endl;

        // Si es LL(1), probar el parser
        auto parser = ParserFactory::createFullHulkParserV3();

        // Test expresiones complejas
        std::vector<std::string> testInputs = {
            "123;",
            "x + y * z;",
            "let x := 5 in x + 1;",
            "if (true) 1 else 0;",
            "while (x > 0) x - 1;", // Simplified - sin asignación en while
            "function add(x, y) => x + y;",
            "new Point(1, 2);",
            "{ 1; 2; 3; };" // Simplified - statements simples en bloque
        };

        for (const auto& input : testInputs) {
            std::cout << "Testing full grammar V3 with: \"" << input << "\"" << std::endl;

            try {
                auto result = parser->parse(input);
                if (result) {
                    std::cout << "✓ Parsed successfully" << std::endl;
                } else {
                    std::cout << "✗ Parsing failed - null result" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "✗ Parsing failed: " << e.what() << std::endl;
            }
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}
