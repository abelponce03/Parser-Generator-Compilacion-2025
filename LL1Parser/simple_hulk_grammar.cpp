#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createSimpleHulkGrammar() {
    Grammar grammar;
    
    // === SÍMBOLOS BÁSICOS ===
    
    // No terminales
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    
    // Primes para LL(1)
    Symbol stmt_list_prime(SymbolType::NON_TERMINAL, "stmt_list_prime");
    Symbol add_expr_prime(SymbolType::NON_TERMINAL, "add_expr_prime");
    Symbol mult_expr_prime(SymbolType::NON_TERMINAL, "mult_expr_prime");
    
    // Terminales
    Symbol number(SymbolType::TERMINAL, "NUMBER");
    Symbol ident(SymbolType::TERMINAL, "IDENT");
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol minus(SymbolType::TERMINAL, "MINUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol div(SymbolType::TERMINAL, "DIV");
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    
    // === ESTABLECER SÍMBOLO INICIAL ===
    grammar.setStartSymbol(program);
    
    // === PRODUCCIONES ===
    
    // 1. program -> stmt_list
    grammar.addProduction(program, {stmt_list});
    
    // 2. stmt_list -> stmt stmt_list_prime | ε
    grammar.addProduction(stmt_list, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list, {EPSILON});
    
    // 3. stmt_list_prime -> stmt stmt_list_prime | ε
    grammar.addProduction(stmt_list_prime, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list_prime, {EPSILON});
    
    // 4. stmt -> expr semicolon
    grammar.addProduction(stmt, {expr, semicolon});
    
    // 5. expr -> add_expr
    grammar.addProduction(expr, {add_expr});
    
    // 6. add_expr -> mult_expr add_expr_prime
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    
    // 7. add_expr_prime -> plus mult_expr add_expr_prime | minus mult_expr add_expr_prime | ε
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 8. mult_expr -> primary_expr mult_expr_prime
    grammar.addProduction(mult_expr, {primary_expr, mult_expr_prime});
    
    // 9. mult_expr_prime -> mult primary_expr mult_expr_prime | div primary_expr mult_expr_prime | ε
    grammar.addProduction(mult_expr_prime, {mult, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 10. primary_expr -> number | ident | lparen expr rparen
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {ident});
    grammar.addProduction(primary_expr, {lparen, expr, rparen});
    
    return grammar;
}

void ParserFactory::setupSimpleHulkSemanticActions(LL1Parser& parser) {
    // Por ahora, acciones semánticas vacías para verificar el parsing básico
    std::cout << "Setting up basic semantic actions..." << std::endl;
}

std::unique_ptr<LL1Parser> ParserFactory::createSimpleHulkParser() {
    Grammar grammar = createSimpleHulkGrammar();
    auto parser = std::make_unique<LL1Parser>(grammar);
    setupSimpleHulkSemanticActions(*parser);
    return parser;
}

} // namespace LL1
