#include "ll1_parser.hpp"
#include <iostream>

using namespace LL1;

int main() {
    std::cout << "=== Test Expresiones Let ===" << std::endl;
    
    // Crear gramática intermedia con expresiones let
    Grammar grammar = ParserFactory::createIntermediateHulkGrammar();
    std::cout << "Gramática intermedia con expresiones let:" << std::endl;
    grammar.printGrammar();
    std::cout << std::endl;
    
    // Verificar si sigue siendo LL(1)
    bool isLL1 = grammar.isLL1();
    std::cout << "Gramática con expresiones let es LL(1): " << (isLL1 ? "SÍ" : "NO") << std::endl;
    
    if (!isLL1) {
        std::cout << "⚠️  CONFLICTO LL(1) detectado - necesita refactorización" << std::endl;
        
        // Mostrar conjuntos FIRST y FOLLOW para debug del conflicto
        std::cout << "=== DEBUG: FIRST and FOLLOW sets ===" << std::endl;
        grammar.printFirstSets();
        grammar.printFollowSets();
        std::cout << "=== END DEBUG ===" << std::endl;
        
        return 1;
    }
    
    // Si es LL(1), probar el parser con expresiones let
    auto parser = ParserFactory::createIntermediateHulkParser();
    
    std::vector<std::string> testInputs = {
        // Expresiones let básicas
        "let x := 5 in x;",
        "let y := 10 in y + 1;",
        "let z := true in z && false;",
        
        // Let con múltiples asignaciones
        "let x := 1, y := 2 in x + y;",
        "let a := 5, b := 10, c := 15 in a * b + c;",
        
        // Let con expresiones complejas (solo aritméticas)
        "let x := 1 + 2 * 3 in x + 5;",
        "let result := (a + b) * c in result + expected;",
        
        // Let anidados
        "let x := 5 in let y := x + 1 in x * y;",
        "let outer := 10 in let inner := outer / 2 in inner + 1;",
        
        // Combinaciones con expresiones aritméticas
        "let x := 5, y := 10 in x + y;",
        "let count := 0 in count + 5;",
        
        // Let con strings y literales (sin comparaciones)
        "let name := \"hello\", active := true in name;",
        
        // Expresiones complejas en el cuerpo del let
        "let base := 2, exp := 3 in base * base * base + exp * exp + 1;"
    };
    
    std::cout << "\\nProbando expresiones let..." << std::endl;
    
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
