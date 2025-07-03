#include "ll1_parser.hpp"
#include "intermediate_hulk_grammar.cpp"
#include <iostream>

using namespace LL1;

int main() {
    std::cout << "=== Test Operadores Lógicos ===" << std::endl;
    
    // Crear gramática intermedia con operadores lógicos
    Grammar grammar = ParserFactory::createIntermediateHulkGrammar();
    std::cout << "Gramática intermedia con operadores lógicos:" << std::endl;
    grammar.printGrammar();
    std::cout << std::endl;
    
    // Verificar si sigue siendo LL(1)
    bool isLL1 = grammar.isLL1();
    std::cout << "Gramática con operadores lógicos es LL(1): " << (isLL1 ? "SÍ" : "NO") << std::endl;
    
    if (!isLL1) {
        std::cout << "⚠️  CONFLICTO LL(1) detectado - necesita refactorización" << std::endl;
        return 1;
    }
    
    // Si es LL(1), probar el parser con operadores lógicos
    auto parser = ParserFactory::createIntermediateHulkParser();
    
    std::vector<std::string> testInputs = {
        // Operadores lógicos básicos
        "true && false;",
        "true || false;",
        
        // Combinaciones
        "true && false || true;",
        "false || true && false;",
        
        // Con comparaciones
        "x < 5 && y > 10;",
        "a == b || c != d;",
        "1 + 2 == 3 && x > 0;",
        
        // Con paréntesis
        "(true || false) && (x < y);",
        "a && (b || c) && d;",
        
        // Expresiones complejas
        "x < y && y <= z || a > b;",
        "(x + y) * 2 == z && w != 0;",
        "true || false && x == y || z > 5;"
    };
    
    std::cout << "\\nProbando operadores lógicos..." << std::endl;
    
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
    
    std::cout << "\\n=== FIN ===" << std::endl;
    return 0;
}
