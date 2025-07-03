#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createIntermediateHulkGrammar() {
    Grammar grammar;
    
    // === ESTRATEGIA: Gramática simple + LET expressions ===
    
    // No terminales
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    
    // Aritmética (como gramática simple)
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol unary_expr(SymbolType::NON_TERMINAL, "unary_expr");
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    
    // Let expressions (nuevo)
    Symbol let_expr(SymbolType::NON_TERMINAL, "let_expr");
    Symbol binding(SymbolType::NON_TERMINAL, "binding");
    
    // Primes para LL(1)
    Symbol stmt_list_prime(SymbolType::NON_TERMINAL, "stmt_list_prime");
    Symbol add_expr_prime(SymbolType::NON_TERMINAL, "add_expr_prime");
    Symbol mult_expr_prime(SymbolType::NON_TERMINAL, "mult_expr_prime");
    
    // Terminales básicos
    Symbol number(SymbolType::TERMINAL, "NUMBER");
    Symbol string_lit(SymbolType::TERMINAL, "STRING");
    Symbol true_lit(SymbolType::TERMINAL, "TRUE"); 
    Symbol false_lit(SymbolType::TERMINAL, "FALSE");
    Symbol ident(SymbolType::TERMINAL, "IDENT");
    
    // Operadores aritméticos
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol minus(SymbolType::TERMINAL, "MINUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol div(SymbolType::TERMINAL, "DIV");
    Symbol mod(SymbolType::TERMINAL, "MOD");
    
    // Let expression keywords
    Symbol let_kw(SymbolType::TERMINAL, "LET");
    Symbol in_kw(SymbolType::TERMINAL, "IN");
    Symbol assign(SymbolType::TERMINAL, "ASSIGN");
    
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    
    // === ESTABLECER SÍMBOLO INICIAL ===
    grammar.setStartSymbol(program);
    
    // === PRODUCCIONES ===
    
    // 1. Programa principal
    grammar.addProduction(program, {stmt_list});
    
    // 2. Lista de statements
    grammar.addProduction(stmt_list, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list, {EPSILON});
    grammar.addProduction(stmt_list_prime, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list_prime, {EPSILON});
    
    // 3. Statements
    grammar.addProduction(stmt, {expr, semicolon});
    
    // 4. Expresiones (como gramática simple)
    grammar.addProduction(expr, {add_expr});
    
    // 5. Suma y resta 
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 6. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 7. Expresiones unarias
    grammar.addProduction(unary_expr, {minus, unary_expr});
    grammar.addProduction(unary_expr, {primary_expr});
    
    // 8. Expresiones primarias (incluyendo let)
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident});
    grammar.addProduction(primary_expr, {lparen, expr, rparen});
    grammar.addProduction(primary_expr, {let_expr}); // NUEVO
    
    // 9. Let expressions (simplificado - solo un binding)
    grammar.addProduction(let_expr, {let_kw, binding, in_kw, expr});
    grammar.addProduction(binding, {ident, assign, expr});
    
    return grammar;
}

std::unique_ptr<LL1Parser> ParserFactory::createIntermediateHulkParser() {
    Grammar grammar = createIntermediateHulkGrammar();
    auto parser = std::make_unique<LL1Parser>(grammar);
    // setupIntermediateHulkSemanticActions(*parser); // TODO: Implementar cuando sea LL(1)
    return parser;
}

} // namespace LL1
