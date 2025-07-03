#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createFullHulkGrammarV2() {
    Grammar grammar;
    
    // === ESTRATEGIA: Expandir gramática intermedia exitosa ===
    // Basándonos en intermediate_hulk_grammar.cpp que es LL(1)
    
    // No terminales básicos
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    
    // Declaraciones (nuevo nivel)
    Symbol decl(SymbolType::NON_TERMINAL, "decl");
    Symbol function_decl(SymbolType::NON_TERMINAL, "function_decl");
    Symbol function_body(SymbolType::NON_TERMINAL, "function_body"); // Para left-factoring
    Symbol class_decl(SymbolType::NON_TERMINAL, "class_decl");
    
    // Jerarquía de expresiones (de la gramática intermedia exitosa)
    Symbol or_expr(SymbolType::NON_TERMINAL, "or_expr");
    Symbol or_expr_prime(SymbolType::NON_TERMINAL, "or_expr_prime");
    Symbol and_expr(SymbolType::NON_TERMINAL, "and_expr");
    Symbol and_expr_prime(SymbolType::NON_TERMINAL, "and_expr_prime");
    Symbol eq_expr(SymbolType::NON_TERMINAL, "eq_expr");
    Symbol eq_expr_prime(SymbolType::NON_TERMINAL, "eq_expr_prime");
    Symbol rel_expr(SymbolType::NON_TERMINAL, "rel_expr");
    Symbol rel_expr_prime(SymbolType::NON_TERMINAL, "rel_expr_prime");
    Symbol arith_expr(SymbolType::NON_TERMINAL, "arith_expr");
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    Symbol ident_suffix(SymbolType::NON_TERMINAL, "ident_suffix"); // Para left-factoring
    
    // Primes para LL(1)
    Symbol stmt_list_prime(SymbolType::NON_TERMINAL, "stmt_list_prime");
    Symbol add_expr_prime(SymbolType::NON_TERMINAL, "add_expr_prime");
    Symbol mult_expr_prime(SymbolType::NON_TERMINAL, "mult_expr_prime");
    
    // Expresiones complejas (expandir gradualmente)
    Symbol let_expr(SymbolType::NON_TERMINAL, "let_expr");
    Symbol if_expr(SymbolType::NON_TERMINAL, "if_expr");
    Symbol elif_chain(SymbolType::NON_TERMINAL, "elif_chain");
    Symbol while_expr(SymbolType::NON_TERMINAL, "while_expr");
    Symbol for_expr(SymbolType::NON_TERMINAL, "for_expr");
    Symbol block_expr(SymbolType::NON_TERMINAL, "block_expr");
    
    // Listas auxiliares
    Symbol binding_list(SymbolType::NON_TERMINAL, "binding_list");
    Symbol binding_list_prime(SymbolType::NON_TERMINAL, "binding_list_prime");
    Symbol binding(SymbolType::NON_TERMINAL, "binding");
    Symbol param_list(SymbolType::NON_TERMINAL, "param_list");
    Symbol param_list_prime(SymbolType::NON_TERMINAL, "param_list_prime");
    Symbol arg_list(SymbolType::NON_TERMINAL, "arg_list");
    Symbol arg_list_prime(SymbolType::NON_TERMINAL, "arg_list_prime");
    
    // === TERMINALES ===
    
    // Literales
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
    Symbol lt(SymbolType::TERMINAL, "LESS_THAN");
    Symbol gt(SymbolType::TERMINAL, "GREATER_THAN");
    Symbol le(SymbolType::TERMINAL, "LE");
    Symbol ge(SymbolType::TERMINAL, "GE");
    
    // Operadores lógicos
    Symbol and_op(SymbolType::TERMINAL, "AND");
    Symbol or_op(SymbolType::TERMINAL, "OR");
    
    // Palabras clave
    Symbol let_kw(SymbolType::TERMINAL, "LET");
    Symbol in_kw(SymbolType::TERMINAL, "IN");
    Symbol if_kw(SymbolType::TERMINAL, "IF");
    Symbol else_kw(SymbolType::TERMINAL, "ELSE");
    Symbol elif_kw(SymbolType::TERMINAL, "ELIF");
    Symbol while_kw(SymbolType::TERMINAL, "WHILE");
    Symbol for_kw(SymbolType::TERMINAL, "FOR");
    Symbol function_kw(SymbolType::TERMINAL, "FUNCTION");
    Symbol type_kw(SymbolType::TERMINAL, "TYPE");
    Symbol inherits_kw(SymbolType::TERMINAL, "INHERITS");
    Symbol new_kw(SymbolType::TERMINAL, "NEW");
    
    // Operadores especiales
    Symbol assign_destruct(SymbolType::TERMINAL, "ASSIGN_DESTRUCT"); // :=
    Symbol arrow(SymbolType::TERMINAL, "ARROW"); // =>
    
    // Delimitadores
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol lbrace(SymbolType::TERMINAL, "LBRACE");
    Symbol rbrace(SymbolType::TERMINAL, "RBRACE");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    Symbol comma(SymbolType::TERMINAL, "COMMA");
    Symbol dot(SymbolType::TERMINAL, "DOT");
    
    // === ESTABLECER SÍMBOLO INICIAL ===
    grammar.setStartSymbol(program);
    
    // === PRODUCCIONES (basadas en gramática intermedia exitosa) ===
    
    // 1. Programa principal
    grammar.addProduction(program, {stmt_list});
    
    // 2. Lista de statements (permitir declaraciones)
    grammar.addProduction(stmt_list, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list, {EPSILON});
    grammar.addProduction(stmt_list_prime, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list_prime, {EPSILON});
    
    // 3. Statements (expresiones Y declaraciones)
    grammar.addProduction(stmt, {expr, semicolon});
    grammar.addProduction(stmt, {decl});
    
    // 4. Declaraciones
    grammar.addProduction(decl, {function_decl});
    // grammar.addProduction(decl, {class_decl}); // TODO: Agregar después
    
    // 5. Expresiones (misma jerarquía que gramática intermedia)
    grammar.addProduction(expr, {or_expr});
    grammar.addProduction(expr, {let_expr});
    grammar.addProduction(expr, {if_expr});
    grammar.addProduction(expr, {while_expr});
    grammar.addProduction(expr, {for_expr});
    grammar.addProduction(expr, {block_expr});
    
    // 6. Expresiones OR (más baja precedencia)
    grammar.addProduction(or_expr, {and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {or_op, and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {EPSILON});
    
    // 7. Expresiones AND
    grammar.addProduction(and_expr, {eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {and_op, eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {EPSILON});
    
    // 8. Expresiones de igualdad
    grammar.addProduction(eq_expr, {rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    
    // 9. Expresiones relacionales
    grammar.addProduction(rel_expr, {arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {lt, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {gt, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {le, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {ge, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {EPSILON});
    
    // 10. Expresiones aritméticas
    grammar.addProduction(arith_expr, {add_expr});
    
    // 11. Suma y resta 
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 12. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 13. Expresiones primarias (left-factorizadas para evitar conflictos LL(1))
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident, ident_suffix}); // Left-factorizado
    grammar.addProduction(primary_expr, {lparen, or_expr, rparen}); // Permite expresiones completas
    grammar.addProduction(primary_expr, {new_kw, ident, lparen, arg_list, rparen}); // Instanciación
    
    // 13b. Sufijo de identificador (para resolver conflicto LL(1))
    grammar.addProduction(ident_suffix, {lparen, arg_list, rparen}); // Llamada a función
    grammar.addProduction(ident_suffix, {EPSILON}); // Identificador simple
    
    // 14. Let expressions
    grammar.addProduction(let_expr, {let_kw, binding_list, in_kw, or_expr});
    
    // 15. If expressions
    grammar.addProduction(if_expr, {if_kw, lparen, or_expr, rparen, or_expr, elif_chain});
    grammar.addProduction(elif_chain, {else_kw, or_expr});
    grammar.addProduction(elif_chain, {elif_kw, lparen, or_expr, rparen, or_expr, elif_chain});
    grammar.addProduction(elif_chain, {EPSILON}); // if sin else
    
    // 16. While expressions
    grammar.addProduction(while_expr, {while_kw, lparen, or_expr, rparen, or_expr});
    
    // 17. For expressions
    grammar.addProduction(for_expr, {for_kw, lparen, ident, in_kw, or_expr, rparen, or_expr});
    
    // 18. Block expressions
    grammar.addProduction(block_expr, {lbrace, stmt_list, rbrace});
    
    // 19. Function declarations (left-factorizadas)
    grammar.addProduction(function_decl, {function_kw, ident, lparen, param_list, rparen, function_body});
    grammar.addProduction(function_body, {arrow, or_expr, semicolon}); // Función expresión
    grammar.addProduction(function_body, {block_expr}); // Función bloque
    
    // 20. Lista de parámetros
    grammar.addProduction(param_list, {ident, param_list_prime});
    grammar.addProduction(param_list, {EPSILON});
    grammar.addProduction(param_list_prime, {comma, ident, param_list_prime});
    grammar.addProduction(param_list_prime, {EPSILON});
    
    // 21. Lista de argumentos
    grammar.addProduction(arg_list, {or_expr, arg_list_prime});
    grammar.addProduction(arg_list, {EPSILON});
    grammar.addProduction(arg_list_prime, {comma, or_expr, arg_list_prime});
    grammar.addProduction(arg_list_prime, {EPSILON});
    
    // 22. Lista de asignaciones (bindings)
    grammar.addProduction(binding_list, {binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {comma, binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {EPSILON});
    
    // 23. Asignaciones individuales
    grammar.addProduction(binding, {ident, assign_destruct, or_expr});
    
    return grammar;
}

std::unique_ptr<LL1Parser> ParserFactory::createFullHulkParserV2() {
    Grammar grammar = createFullHulkGrammarV2();
    auto parser = std::make_unique<LL1Parser>(grammar);
    // setupFullHulkSemanticActions(*parser); // TODO: Implementar cuando sea LL(1)
    return parser;
}

} // namespace LL1
