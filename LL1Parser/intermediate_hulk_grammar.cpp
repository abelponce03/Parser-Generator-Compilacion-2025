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
    
    // Comparaciones (expandir gradualmente)
    Symbol eq_expr(SymbolType::NON_TERMINAL, "eq_expr");
    Symbol eq_expr_prime(SymbolType::NON_TERMINAL, "eq_expr_prime");
    
    // Comparaciones relacionales (nuevo nivel)
    Symbol rel_expr(SymbolType::NON_TERMINAL, "rel_expr");
    Symbol rel_expr_prime(SymbolType::NON_TERMINAL, "rel_expr_prime");
    
    // Operadores lógicos (nuevo nivel por encima de igualdad)
    Symbol and_expr(SymbolType::NON_TERMINAL, "and_expr");
    Symbol and_expr_prime(SymbolType::NON_TERMINAL, "and_expr_prime");
    Symbol or_expr(SymbolType::NON_TERMINAL, "or_expr");
    Symbol or_expr_prime(SymbolType::NON_TERMINAL, "or_expr_prime");
    
    // Let expressions (nuevo)
    Symbol let_expr(SymbolType::NON_TERMINAL, "let_expr");
    Symbol binding_list(SymbolType::NON_TERMINAL, "binding_list");
    Symbol binding_list_prime(SymbolType::NON_TERMINAL, "binding_list_prime");
    Symbol binding(SymbolType::NON_TERMINAL, "binding");
    
    // Aritmética (como gramática simple - SIN unary_expr)
    Symbol arith_expr(SymbolType::NON_TERMINAL, "arith_expr");
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
    
    // Operadores de comparación
    Symbol eq(SymbolType::TERMINAL, "EQ");
    Symbol neq(SymbolType::TERMINAL, "NEQ");
    
    // Operadores relacionales (nuevo)
    Symbol lt(SymbolType::TERMINAL, "LESS_THAN");
    Symbol gt(SymbolType::TERMINAL, "GREATER_THAN");
    Symbol le(SymbolType::TERMINAL, "LE");
    Symbol ge(SymbolType::TERMINAL, "GE");
    
    // Operadores lógicos
    Symbol and_op(SymbolType::TERMINAL, "AND");
    Symbol or_op(SymbolType::TERMINAL, "OR");
    
    // Let expression keywords
    Symbol let_kw(SymbolType::TERMINAL, "LET");
    Symbol in_kw(SymbolType::TERMINAL, "IN");
    Symbol assign_destruct(SymbolType::TERMINAL, "ASSIGN_DESTRUCT"); // :=
    
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    Symbol comma(SymbolType::TERMINAL, "COMMA");
    
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
    
    // 4. Expresiones - jerarquía completa con operadores lógicos, relacionales, igualdad
    grammar.addProduction(expr, {or_expr});
    grammar.addProduction(expr, {let_expr});
    
    // 5. Expresiones OR (más baja precedencia)
    grammar.addProduction(or_expr, {and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {or_op, and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {EPSILON});
    
    // 6. Expresiones AND
    grammar.addProduction(and_expr, {eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {and_op, eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {EPSILON});
    
    // 7. Expresiones de igualdad
    grammar.addProduction(eq_expr, {rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    
    // 8. Expresiones relacionales
    grammar.addProduction(rel_expr, {arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {lt, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {gt, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {le, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {ge, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {EPSILON});
    
    // 9. Expresiones aritméticas (separadas de let para evitar conflictos)
    grammar.addProduction(arith_expr, {add_expr});
    
    // 10. Suma y resta 
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 11. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 12. Expresiones primarias - SIN let para evitar recursión y mantener LL(1)
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident});
    grammar.addProduction(primary_expr, {lparen, or_expr, rparen}); // Permite expresiones completas
    
    // 13. Let expressions - usando or_expr para permitir operadores completos
    grammar.addProduction(let_expr, {let_kw, binding_list, in_kw, or_expr});
    
    // 14. Lista de asignaciones (bindings)
    grammar.addProduction(binding_list, {binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {comma, binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {EPSILON});
    
    // 15. Asignaciones individuales - usando or_expr para permitir operadores completos
    grammar.addProduction(binding, {ident, assign_destruct, or_expr});
    
    return grammar;
}

std::unique_ptr<LL1Parser> ParserFactory::createIntermediateHulkParser() {
    Grammar grammar = createIntermediateHulkGrammar();
    auto parser = std::make_unique<LL1Parser>(grammar);
    // setupIntermediateHulkSemanticActions(*parser); // TODO: Implementar cuando sea LL(1)
    return parser;
}

} // namespace LL1
