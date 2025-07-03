#include "ll1_parser.hpp"
#include "intermediate_hulk_grammar.cpp"
#include <iostream>

using namespace LL1;

int main() {
    std::cout << "=== Test Expresiones Let COMPLETO ===" << std::endl;
    
    // Crear gramática intermedia
    Grammar grammar = ParserFactory::createIntermediateHulkGrammar();
    std::cout << "Gramática con let es LL(1): " << (grammar.isLL1() ? "SÍ" : "NO") << std::endl;
    
    if (!grammar.isLL1()) {
        std::cout << "❌ Gramática no es LL(1)" << std::endl;
        return 1;
    }
    
    auto parser = ParserFactory::createIntermediateHulkParser();
    
    // Casos de prueba EXITOSOS para expresiones let
    std::vector<std::string> successTests = {
        // Let básicos
        "let x := 5 in x;",
        "let y := 10 in y + 1;",
        
        // Let con múltiples bindings
        "let x := 1, y := 2 in x + y;",
        "let a := 5, b := 10 in a * b;",
        
        // Let con expresiones complejas
        "let x := 1 + 2 * 3 in x + 5;",
        "let result := (a + b) * c in result + expected;",
        
        // Let anidados ¡FUNCIONAN!
        "let x := 5 in let y := x + 1 in x * y;",
        "let outer := 10 in let inner := outer / 2 in inner + 1;",
        
        // Casos más complejos
        "let count := 0 in count + 5;",
        "let name := \"hello\", active := true in name;",
        
        // Paréntesis y precedencia
        "let x := (1 + 2) * 3 in x / 2;",
        "let a := 1, b := 2, c := 3 in (a + b) * c;",
        
        // Let con diferentes tipos de literales
        "let num := 42, str := \"test\", flag := true in num;",
        
        // Expresiones aritméticas complejas en el cuerpo
        "let x := 2, y := 3 in x * x * x + y * y + 1;"
    };
    
    std::cout << "\\n=== PRUEBAS EXITOSAS ===" << std::endl;
    int passed = 0;
    
    for (const auto& test : successTests) {
        std::cout << "Probando: \"" << test << "\" -> ";
        
        try {
            auto result = parser->parse(test);
            if (result) {
                std::cout << "✅ ÉXITO" << std::endl;
                passed++;
            } else {
                std::cout << "❌ FALLO (null result)" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "❌ ERROR: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\\n📊 RESUMEN:" << std::endl;
    std::cout << "✅ Éxitos: " << passed << "/" << successTests.size() << std::endl;
    std::cout << "📈 Porcentaje de éxito: " << (100.0 * passed / successTests.size()) << "%" << std::endl;
    
    if (passed == successTests.size()) {
        std::cout << "\\n🎉 ¡TODAS LAS EXPRESIONES LET FUNCIONAN PERFECTAMENTE!" << std::endl;
        std::cout << "✅ Gramática LL(1) exitosa" << std::endl;
        std::cout << "✅ Let básicos funcionan" << std::endl;
        std::cout << "✅ Let con múltiples asignaciones funcionan" << std::endl;
        std::cout << "✅ Let anidados funcionan" << std::endl;
        std::cout << "✅ Expresiones complejas funcionan" << std::endl;
    }
    
    return 0;
}
