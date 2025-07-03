#include "ll1_parser.hpp"
#include "intermediate_hulk_grammar.cpp"
#include <iostream>

using namespace LL1;

int main() {
    std::cout << "=== Test Operadores Relacionales ===" << std::endl;
    
    // Crear gramática intermedia
    auto parser = ParserFactory::createIntermediateHulkParser();
    
    // Casos de prueba específicos para operadores relacionales
    std::vector<std::string> testInputs = {
        "5 < 10;",
        "8 > 3;", 
        "7 <= 7;",
        "9 >= 5;",
        "1 + 2 < 5;",
        "x * 2 > y;",
        "(a + b) <= (c - d);",
        "foo >= bar * 2;",
        "1 < 2 == true;",
        "x > 5 != false;",
        "a <= b == c >= d;",
        "1 + 2 * 3 > 5;",
        "(x + y) * z < w / 2;",
        "\"hello\" == \"world\" != true;"
    };
    
    std::cout << "Probando operadores relacionales..." << std::endl;
    
    for (const auto& input : testInputs) {
        std::cout << "Probando: \"" << input << "\" -> ";
        
        try {
            auto result = parser->parse(input);
            if (result) {
                std::cout << "✓ CORRECTO" << std::endl;
            } else {
                std::cout << "✗ FALLO (null result)" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "✗ ERROR: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n=== FIN ===" << std::endl;
    return 0;
}
