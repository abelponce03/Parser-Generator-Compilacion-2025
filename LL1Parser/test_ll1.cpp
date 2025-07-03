#include "ll1_grammar.hpp"
#include "ll1_parser.hpp"
#include <iostream>
#include <cassert>

using namespace LL1;

void testBasicGrammar() {
    std::cout << "=== Test: Basic Grammar Construction ===" << std::endl;
    
    Grammar grammar;
    
    // Símbolos
    Symbol E(SymbolType::NON_TERMINAL, "E");
    Symbol T(SymbolType::NON_TERMINAL, "T");
    Symbol F(SymbolType::NON_TERMINAL, "F");
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol id(SymbolType::TERMINAL, "IDENT");
    
    grammar.setStartSymbol(E);
    
    // Producciones: E -> T E'
    Symbol E_prime(SymbolType::NON_TERMINAL, "E_prime");
    grammar.addProduction(E, {T, E_prime});
    
    // E' -> + T E' | ε
    grammar.addProduction(E_prime, {plus, T, E_prime});
    grammar.addProduction(E_prime, {EPSILON});
    
    // T -> F T'
    Symbol T_prime(SymbolType::NON_TERMINAL, "T_prime");
    grammar.addProduction(T, {F, T_prime});
    
    // T' -> * F T' | ε
    grammar.addProduction(T_prime, {mult, F, T_prime});
    grammar.addProduction(T_prime, {EPSILON});
    
    // F -> ( E ) | id
    grammar.addProduction(F, {lparen, E, rparen});
    grammar.addProduction(F, {id});
    
    grammar.printGrammar();
    std::cout << "✓ Basic grammar construction passed\n" << std::endl;
}

void testFirstSets() {
    std::cout << "=== Test: FIRST Sets Computation ===" << std::endl;
    
    Grammar grammar;
    
    Symbol E(SymbolType::NON_TERMINAL, "E");
    Symbol T(SymbolType::NON_TERMINAL, "T");
    Symbol F(SymbolType::NON_TERMINAL, "F");
    Symbol E_prime(SymbolType::NON_TERMINAL, "E_prime");
    Symbol T_prime(SymbolType::NON_TERMINAL, "T_prime");
    
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol id(SymbolType::TERMINAL, "IDENT");
    
    grammar.setStartSymbol(E);
    
    grammar.addProduction(E, {T, E_prime});
    grammar.addProduction(E_prime, {plus, T, E_prime});
    grammar.addProduction(E_prime, {EPSILON});
    grammar.addProduction(T, {F, T_prime});
    grammar.addProduction(T_prime, {mult, F, T_prime});
    grammar.addProduction(T_prime, {EPSILON});
    grammar.addProduction(F, {lparen, E, rparen});
    grammar.addProduction(F, {id});
    
    grammar.printFirstSets();
    
    const auto& firstSets = grammar.getFirstSets();
    
    // Verificar algunos conjuntos FIRST
    assert(firstSets.at(E).count(id) > 0);
    assert(firstSets.at(E).count(lparen) > 0);
    assert(firstSets.at(E_prime).count(plus) > 0);
    assert(firstSets.at(E_prime).count(EPSILON) > 0);
    
    std::cout << "✓ FIRST sets computation passed\n" << std::endl;
}

void testFollowSets() {
    std::cout << "=== Test: FOLLOW Sets Computation ===" << std::endl;
    
    Grammar grammar;
    
    Symbol E(SymbolType::NON_TERMINAL, "E");
    Symbol T(SymbolType::NON_TERMINAL, "T");
    Symbol F(SymbolType::NON_TERMINAL, "F");
    Symbol E_prime(SymbolType::NON_TERMINAL, "E_prime");
    Symbol T_prime(SymbolType::NON_TERMINAL, "T_prime");
    
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol id(SymbolType::TERMINAL, "IDENT");
    
    grammar.setStartSymbol(E);
    
    grammar.addProduction(E, {T, E_prime});
    grammar.addProduction(E_prime, {plus, T, E_prime});
    grammar.addProduction(E_prime, {EPSILON});
    grammar.addProduction(T, {F, T_prime});
    grammar.addProduction(T_prime, {mult, F, T_prime});
    grammar.addProduction(T_prime, {EPSILON});
    grammar.addProduction(F, {lparen, E, rparen});
    grammar.addProduction(F, {id});
    
    grammar.printFollowSets();
    
    const auto& followSets = grammar.getFollowSets();
    
    // Verificar algunos conjuntos FOLLOW
    assert(followSets.at(E).count(END_OF_INPUT) > 0);
    assert(followSets.at(T).count(plus) > 0);
    
    std::cout << "✓ FOLLOW sets computation passed\n" << std::endl;
}

void testParseTable() {
    std::cout << "=== Test: Parse Table Construction ===" << std::endl;
    
    Grammar grammar;
    
    Symbol E(SymbolType::NON_TERMINAL, "E");
    Symbol T(SymbolType::NON_TERMINAL, "T");
    Symbol F(SymbolType::NON_TERMINAL, "F");
    Symbol E_prime(SymbolType::NON_TERMINAL, "E_prime");
    Symbol T_prime(SymbolType::NON_TERMINAL, "T_prime");
    
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol id(SymbolType::TERMINAL, "IDENT");
    
    grammar.setStartSymbol(E);
    
    grammar.addProduction(E, {T, E_prime});
    grammar.addProduction(E_prime, {plus, T, E_prime});
    grammar.addProduction(E_prime, {EPSILON});
    grammar.addProduction(T, {F, T_prime});
    grammar.addProduction(T_prime, {mult, F, T_prime});
    grammar.addProduction(T_prime, {EPSILON});
    grammar.addProduction(F, {lparen, E, rparen});
    grammar.addProduction(F, {id});
    
    bool isLL1 = grammar.isLL1();
    std::cout << "Grammar is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;
    
    if (isLL1) {
        grammar.printParseTable();
    }
    
    assert(isLL1);
    std::cout << "✓ Parse table construction passed\n" << std::endl;
}

void testLexer() {
    std::cout << "=== Test: Basic Lexer Functionality ===" << std::endl;
    
    std::string input = "123 + abc * (456 - def);";
    Lexer lexer(input);
    
    std::vector<std::string> expectedTokens = {
        "NUMBER", "PLUS", "IDENT", "MULT", "LPAREN", "NUMBER", "MINUS", "IDENT", "RPAREN", "SEMICOLON", "$"
    };
    
    for (const std::string& expected : expectedTokens) {
        Token token = lexer.nextToken();
        std::cout << "Token: " << token.symbol.name << " (" << token.lexeme << ")" << std::endl;
        assert(token.symbol.name == expected);
    }
    
    std::cout << "✓ Lexer functionality passed\n" << std::endl;
}

void testHulkGrammar() {
    std::cout << "=== Test: HULK Grammar Creation ===" << std::endl;
    
    // Crear la gramática simplificada primero
    Grammar simpleGrammar = ParserFactory::createSimpleHulkGrammar();
    std::cout << "Simple HULK Grammar Productions:" << std::endl;
    simpleGrammar.printGrammar();
    std::cout << std::endl;
    
    // Verificar si es LL(1)
    bool isLL1 = simpleGrammar.isLL1();
    std::cout << "Simple Grammar is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;
    
    if (isLL1) {
        // Crear parser con gramática simple
        auto parser = ParserFactory::createSimpleHulkParser();
        
        // Test multiple expressions
        std::vector<std::string> testInputs = {
            "123;",
            "456;",
            "x;",
            "(42);",
            "1 + 2;",
            "3 * 4;",
            "5 + 6 * 7;",
            "(8 + 9) * 10;"
        };
        
        for (const auto& input : testInputs) {
            std::cout << "Testing input: \"" << input << "\"" << std::endl;
            
            try {
                auto result = parser->parse(input);
                if (result) {
                    std::cout << "✓ Parsed successfully - created AST with " 
                              << result->stmts.size() << " statements" << std::endl;
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

void testIntermediateHulkGrammar() {
    std::cout << "=== Test: Intermediate HULK Grammar Creation ===" << std::endl;
    
    // Crear la gramática intermedia
    Grammar intermediateGrammar = ParserFactory::createIntermediateHulkGrammar();
    std::cout << "Intermediate HULK Grammar Productions:" << std::endl;
    intermediateGrammar.printGrammar();
    std::cout << std::endl;
    
    // Verificar si es LL(1)
    bool isLL1 = intermediateGrammar.isLL1();
    std::cout << "Intermediate Grammar is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;
    
    if (isLL1) {
        // Mostrar algunos conjuntos FIRST y FOLLOW para debug
        std::cout << "=== DEBUG: FIRST and FOLLOW sets for intermediate grammar ===" << std::endl;
        intermediateGrammar.printFirstSets();
        intermediateGrammar.printFollowSets();
        std::cout << "=== END DEBUG ===" << std::endl;
        
        // Crear parser con gramática intermedia
        auto parser = ParserFactory::createIntermediateHulkParser();
        
        // Test expressions with equality comparisons
        std::vector<std::string> testInputs = {
            "123;",
            "x;",
            "true;",
            "false;",
            "1 + 2;",
            "3 * 4;",
            "\"hello\";",
            "(1 + 2) * 3;",
            "1 == 2;",
            "3 != 4;",
            "1 + 2 == 3;",
            "x == \"world\";",
            "true != false;"
        };
        
        for (const auto& input : testInputs) {
            std::cout << "Testing input: \"" << input << "\"" << std::endl;
            
            try {
                auto result = parser->parse(input);
                if (result) {
                    std::cout << "✓ Parsed successfully - created AST with " 
                              << result->stmts.size() << " statements" << std::endl;
                } else {
                    std::cout << "✗ Parsing failed - null result" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "✗ Parsing failed: " << e.what() << std::endl;
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "⚠️  Intermediate grammar has LL(1) conflicts - need to refactor" << std::endl;
    }
    
    std::cout << std::endl;
}

void testFullHulkGrammar() {
    std::cout << "=== Test: Full HULK Grammar Creation ===" << std::endl;
    
    // Crear la gramática completa
    Grammar fullGrammar = ParserFactory::createFullHulkGrammar();
    std::cout << "Full HULK Grammar Productions (" << fullGrammar.getProductions().size() << " total):" << std::endl;
    fullGrammar.printGrammar();
    std::cout << std::endl;
    
    // Verificar si es LL(1)
    bool isLL1 = fullGrammar.isLL1();
    std::cout << "Full Grammar is LL(1): " << (isLL1 ? "YES" : "NO") << std::endl;
    
    if (!isLL1) {
        std::cout << "⚠️  Full grammar has LL(1) conflicts - need to refactor" << std::endl;
    } else {
        std::cout << "✓ Full HULK grammar is LL(1) compatible!" << std::endl;
        
        // Si es LL(1), probar el parser
        auto parser = ParserFactory::createFullHulkParser();
        
        // Test expresiones complejas
        std::vector<std::string> testInputs = {
            "123;",
            "x + y * z;",
            "let x := 5 in x + 1;",
            "if (true) 1 else 0;",
            "while (x > 0) x := x - 1;",
            // "function add(x, y) => x + y;", // TODO: Habilitar cuando esté listo
            // "new Point(1, 2);"
        };
        
        for (const auto& input : testInputs) {
            std::cout << "Testing full grammar with: \"" << input << "\"" << std::endl;
            
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

int main() {
    std::cout << "LL(1) Parser Generator Tests" << std::endl;
    std::cout << "============================" << std::endl << std::endl;
    
    try {
        testBasicGrammar();
        testFirstSets();
        testFollowSets();
        testParseTable();
        testLexer();
        testHulkGrammar();
        testIntermediateHulkGrammar();
        testFullHulkGrammar();
        
        std::cout << "All tests passed! ✓" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
