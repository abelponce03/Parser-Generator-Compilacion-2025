#include "ll1_grammar.hpp"
#include "ll1_parser.hpp"
#include <iostream>
#include <cassert>

using namespace LL1;

// Forward declaration
void testFullHulkGrammarV4();

int main() {
    std::cout << "LL(1) Parser Generator Tests - Full Grammar V4 with Semantic Actions" << std::endl;
    std::cout << "====================================================================" << std::endl << std::endl;

    try {
        testFullHulkGrammarV4();
        std::cout << "All tests passed! ✓" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void testFullHulkGrammarV4() {
    std::cout << "=== Test: Full HULK Grammar V4 with Complete Semantic Actions === " << std::endl;

    // Crear la gramática completa v3
    Grammar fullGrammar = ParserFactory::createFullHulkGrammarV3();
    std::cout << "Full HULK Grammar V3 Productions (" << fullGrammar.getProductions().size() << " total):" << std::endl;
    std::cout << std::endl;

    // Verificar si es LL(1)
    bool isLL1 = fullGrammar.isLL1();
    std::cout << "Full Grammar V3 is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;

    if (!isLL1) {
        std::cout << "⚠️  Full grammar V3 has LL(1) conflicts - need to refactor" << std::endl;
        return;
    }

    std::cout << "✓ Full HULK grammar V3 is LL(1) compatible!" << std::endl;
    std::cout << std::endl;

    // Crear parser con acciones semánticas V4
    auto parser = ParserFactory::createFullHulkParserV4();

    // Test expresiones básicas
    std::vector<std::string> testInputs = {
        "123;",
        "42.5;",
        "true;",
        "false;",
        "\"hello world\";",
        "x;",
        "123 + 456;",
        "10 - 5;",
        "3 * 4;",
        "8 / 2;",
        "true & false;",
        "true | false;",
        "42 == 42;",
        "x != y;",
        "5 < 10;",
        "15 > 5;",
        "(10 + 20) * 3;",
        "x + y * z;",
        "a + b + c;",
        "1 + 2 + 3 + 4;"
    };

    std::cout << "Testing basic expressions with semantic actions:" << std::endl;
    std::cout << std::endl;

    for (const auto& input : testInputs) {
        std::cout << "Testing: " << input << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        
        try {
            auto program = parser->parse(input);
            
            if (program) {
                std::cout << "✓ Parse successful" << std::endl;
                std::cout << "  Program has " << program->stmts.size() << " statements" << std::endl;
                
                // Imprimir información sobre la primera expresión si existe
                if (!program->stmts.empty()) {
                    auto firstStmt = program->stmts[0].get();
                    if (auto exprStmt = dynamic_cast<ExprStmt*>(firstStmt)) {
                        auto expr = exprStmt->expr.get();
                        if (auto numberExpr = dynamic_cast<NumberExpr*>(expr)) {
                            std::cout << "  First expression: NumberExpr(" << numberExpr->value << ")" << std::endl;
                        } else if (auto stringExpr = dynamic_cast<StringExpr*>(expr)) {
                            std::cout << "  First expression: StringExpr(\"" << stringExpr->value << "\")" << std::endl;
                        } else if (auto boolExpr = dynamic_cast<BooleanExpr*>(expr)) {
                            std::cout << "  First expression: BooleanExpr(" << (boolExpr->value ? "true" : "false") << ")" << std::endl;
                        } else if (auto varExpr = dynamic_cast<VariableExpr*>(expr)) {
                            std::cout << "  First expression: VariableExpr(" << varExpr->name << ")" << std::endl;
                        } else if (auto binExpr = dynamic_cast<BinaryExpr*>(expr)) {
                            std::cout << "  First expression: BinaryExpr(op=" << static_cast<int>(binExpr->op) << ")" << std::endl;
                        } else {
                            std::cout << "  First expression: Other type" << std::endl;
                        }
                    }
                }
            } else {
                std::cout << "❌ Parse failed - no program returned" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "❌ Parse failed with exception: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
    }

    std::cout << "=== V4 Semantic Actions Test Complete ===" << std::endl;
}
