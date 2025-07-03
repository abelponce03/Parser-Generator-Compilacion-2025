#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createIntermediateHulkGrammar() {
    Grammar grammar;
    
    // === ESTRATEGIA: Expandir gradualmente desde gramática simple ===
    // Empezar con aritmética + comparaciones básicas (sin OR/AND aún)
    
    // No terminales
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    
    // Nivel 1: Comparaciones relacionales (nuevo nivel)
    Symbol rel_expr(SymbolType::NON_TERMINAL, "rel_expr");   
    Symbol rel_expr_prime(SymbolType::NON_TERMINAL, "rel_expr_prime");
    
    // Nivel 2: Comparaciones de igualdad
    Symbol eq_expr(SymbolType::NON_TERMINAL, "eq_expr");   
    Symbol eq_expr_prime(SymbolType::NON_TERMINAL, "eq_expr_prime");
    
    // Nivel 3: Aritmética (de la gramática simple)
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol unary_expr(SymbolType::NON_TERMINAL, "unary_expr");
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
    
    // Operadores relacionales (nuevos)
    Symbol lt(SymbolType::TERMINAL, "LESS_THAN");
    Symbol gt(SymbolType::TERMINAL, "GREATER_THAN");
    Symbol le(SymbolType::TERMINAL, "LE");
    Symbol ge(SymbolType::TERMINAL, "GE");
    
    // Operadores de comparación
    Symbol eq(SymbolType::TERMINAL, "EQ");
    Symbol neq(SymbolType::TERMINAL, "NEQ");
    
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    
    // === ESTABLECER SÍMBOLO INICIAL ===
    grammar.setStartSymbol(program);
    
    // === PRODUCCIONES (SIMPLIFICADAS) ===
    
    // 1. Programa principal
    grammar.addProduction(program, {stmt_list});
    
    // 2. Lista de statements
    grammar.addProduction(stmt_list, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list, {EPSILON});
    grammar.addProduction(stmt_list_prime, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list_prime, {EPSILON});
    
    // 3. Statements
    grammar.addProduction(stmt, {expr, semicolon});
    
    // 4. Expresiones - empezar con relacionales (menor precedencia)
    grammar.addProduction(expr, {rel_expr});
    
    // 5. Comparaciones relacionales (menor precedencia)
    grammar.addProduction(rel_expr, {eq_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {lt, eq_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {gt, eq_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {le, eq_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {ge, eq_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {EPSILON});
    
    // 6. Comparaciones de igualdad (mayor precedencia) 
    grammar.addProduction(eq_expr, {add_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, add_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, add_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    
    // 7. Suma y resta (de gramática simple)
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 8. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 9. Expresiones unarias
    grammar.addProduction(unary_expr, {minus, unary_expr});
    grammar.addProduction(unary_expr, {primary_expr});
    
    // 10. Expresiones primarias
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
