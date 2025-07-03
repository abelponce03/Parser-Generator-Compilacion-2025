#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createIntermediateHulkGrammar() {
    Grammar grammar;
    
    // === ESTRATEGIA: Gramática simple + LET expressions ===
    
    // No terminales - agregar comparaciones de igualdad
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    
    // Comparaciones (nuevo nivel)
    Symbol eq_expr(SymbolType::NON_TERMINAL, "eq_expr");
    Symbol eq_expr_prime(SymbolType::NON_TERMINAL, "eq_expr_prime");
    
    // Aritmética (como gramática simple - SIN unary_expr)
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    
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
    
    // Operadores de comparación (nuevo)
    Symbol eq(SymbolType::TERMINAL, "EQ");
    Symbol neq(SymbolType::TERMINAL, "NEQ");
    
    // Let expression keywords (por el momento comentado)
    // Symbol let_kw(SymbolType::TERMINAL, "LET");
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
    
    // 4. Expresiones - agregar comparaciones
    grammar.addProduction(expr, {eq_expr});
    
    // 5. Comparaciones de igualdad (nuevo nivel)
    grammar.addProduction(eq_expr, {add_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, add_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, add_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    
    // 6. Suma y resta 
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 7. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 8. Expresiones primarias (SIN let por ahora - como gramática simple)
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident});
    grammar.addProduction(primary_expr, {lparen, expr, rparen});
    
    return grammar;
}

std::unique_ptr<LL1Parser> ParserFactory::createIntermediateHulkParser() {
    Grammar grammar = createIntermediateHulkGrammar();
    auto parser = std::make_unique<LL1Parser>(grammar);
    // setupIntermediateHulkSemanticActions(*parser); // TODO: Implementar cuando sea LL(1)
    return parser;
}

} // namespace LL1
