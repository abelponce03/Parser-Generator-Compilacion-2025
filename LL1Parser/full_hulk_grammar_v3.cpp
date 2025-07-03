#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

// Forward declarations for grammar creation
Grammar createFullHulkGrammarV3();
std::unique_ptr<LL1Parser> createFullHulkParserV3();

Grammar ParserFactory::createFullHulkGrammarV3() {
    Grammar grammar;

    // === ESTRATEGIA: Refactorización de V2 para ser LL(1) ===
    // Cambios clave:
    // 1. Simplificar `stmt_list` para eliminar recursión izquierda indirecta y ambigüedad.
    // 2. Eliminar el no-terminal `expr` y mover sus producciones a `stmt` para resolver conflictos.
    // 3. Refactorizar `if-else` para manejar la opcionalidad del `else` sin conflictos.

    // --- NO TERMINALES ---
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol decl(SymbolType::NON_TERMINAL, "decl");
    Symbol function_decl(SymbolType::NON_TERMINAL, "function_decl");
    Symbol function_body(SymbolType::NON_TERMINAL, "function_body");
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
    Symbol add_expr_prime(SymbolType::NON_TERMINAL, "add_expr_prime");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol mult_expr_prime(SymbolType::NON_TERMINAL, "mult_expr_prime");
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    Symbol ident_suffix(SymbolType::NON_TERMINAL, "ident_suffix");
    Symbol let_expr(SymbolType::NON_TERMINAL, "let_expr");
    Symbol if_expr(SymbolType::NON_TERMINAL, "if_expr");
    Symbol else_part(SymbolType::NON_TERMINAL, "else_part"); // Para if-else
    Symbol while_expr(SymbolType::NON_TERMINAL, "while_expr");
    Symbol for_expr(SymbolType::NON_TERMINAL, "for_expr");
    Symbol block_expr(SymbolType::NON_TERMINAL, "block_expr");
    Symbol binding_list(SymbolType::NON_TERMINAL, "binding_list");
    Symbol binding_list_prime(SymbolType::NON_TERMINAL, "binding_list_prime");
    Symbol binding(SymbolType::NON_TERMINAL, "binding");
    Symbol param_list(SymbolType::NON_TERMINAL, "param_list");
    Symbol param_list_prime(SymbolType::NON_TERMINAL, "param_list_prime");
    Symbol arg_list(SymbolType::NON_TERMINAL, "arg_list");
    Symbol arg_list_prime(SymbolType::NON_TERMINAL, "arg_list_prime");

    // --- TERMINALES ---
    Symbol number(SymbolType::TERMINAL, "NUMBER");
    Symbol string_lit(SymbolType::TERMINAL, "STRING");
    Symbol true_lit(SymbolType::TERMINAL, "TRUE");
    Symbol false_lit(SymbolType::TERMINAL, "FALSE");
    Symbol ident(SymbolType::TERMINAL, "IDENT");
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol minus(SymbolType::TERMINAL, "MINUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol div(SymbolType::TERMINAL, "DIV");
    Symbol mod(SymbolType::TERMINAL, "MOD");
    Symbol eq(SymbolType::TERMINAL, "EQ");
    Symbol neq(SymbolType::TERMINAL, "NEQ");
    Symbol lt(SymbolType::TERMINAL, "LESS_THAN");
    Symbol gt(SymbolType::TERMINAL, "GREATER_THAN");
    Symbol le(SymbolType::TERMINAL, "LE");
    Symbol ge(SymbolType::TERMINAL, "GE");
    Symbol and_op(SymbolType::TERMINAL, "AND");
    Symbol or_op(SymbolType::TERMINAL, "OR");
    Symbol let_kw(SymbolType::TERMINAL, "LET");
    Symbol in_kw(SymbolType::TERMINAL, "IN");
    Symbol if_kw(SymbolType::TERMINAL, "IF");
    Symbol else_kw(SymbolType::TERMINAL, "ELSE");
    Symbol elif_kw(SymbolType::TERMINAL, "ELIF");
    Symbol while_kw(SymbolType::TERMINAL, "WHILE");
    Symbol for_kw(SymbolType::TERMINAL, "FOR");
    Symbol function_kw(SymbolType::TERMINAL, "FUNCTION");
    Symbol new_kw(SymbolType::TERMINAL, "NEW");
    Symbol assign_destruct(SymbolType::TERMINAL, "ASSIGN_DESTRUCT"); // :=
    Symbol arrow(SymbolType::TERMINAL, "ARROW");
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol lbrace(SymbolType::TERMINAL, "LBRACE");
    Symbol rbrace(SymbolType::TERMINAL, "RBRACE");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    Symbol comma(SymbolType::TERMINAL, "COMMA");

    // === SÍMBOLO INICIAL ===
    grammar.setStartSymbol(program);

    // === PRODUCCIONES (Refactorizadas para LL(1)) ===

    // 1. Program -> StmtList
    grammar.addProduction(program, {stmt_list});

    // 2. StmtList -> Stmt StmtList | ε
    grammar.addProduction(stmt_list, {stmt, stmt_list});
    grammar.addProduction(stmt_list, {EPSILON});

    // 3. Stmt -> Decl | LetExpr; | IfExpr; | WhileExpr; | ForExpr; | BlockExpr; | OrExpr;
    grammar.addProduction(stmt, {decl});
    grammar.addProduction(stmt, {let_expr, semicolon});
    grammar.addProduction(stmt, {if_expr, semicolon});
    grammar.addProduction(stmt, {while_expr, semicolon});
    grammar.addProduction(stmt, {for_expr, semicolon});
    grammar.addProduction(stmt, {block_expr, semicolon});
    grammar.addProduction(stmt, {or_expr, semicolon});

    // 4. Decl -> FunctionDecl
    grammar.addProduction(decl, {function_decl});

    // 5. Jerarquía de expresiones (sin cambios)
    grammar.addProduction(or_expr, {and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {or_op, and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {EPSILON});
    grammar.addProduction(and_expr, {eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {and_op, eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {EPSILON});
    grammar.addProduction(eq_expr, {rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    grammar.addProduction(rel_expr, {arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {lt, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {gt, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {le, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {ge, arith_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {EPSILON});
    grammar.addProduction(arith_expr, {add_expr});
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    grammar.addProduction(mult_expr, {primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, primary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});

    // 6. PrimaryExpr (con left-factoring)
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident, ident_suffix});
    grammar.addProduction(primary_expr, {lparen, or_expr, rparen});
    grammar.addProduction(primary_expr, {new_kw, ident, lparen, arg_list, rparen});

    // 7. IdentSuffix (para resolver conflicto de llamada a función vs. variable)
    grammar.addProduction(ident_suffix, {lparen, arg_list, rparen});
    grammar.addProduction(ident_suffix, {EPSILON});

    // 8. LetExpr
    grammar.addProduction(let_expr, {let_kw, binding_list, in_kw, or_expr});

    // 9. IfExpr (refactorizado para ser LL(1))
    grammar.addProduction(if_expr, {if_kw, lparen, or_expr, rparen, or_expr, else_part});
    grammar.addProduction(else_part, {else_kw, or_expr});
    grammar.addProduction(else_part, {elif_kw, lparen, or_expr, rparen, or_expr, else_part});
    grammar.addProduction(else_part, {EPSILON});

    // 10. WhileExpr
    grammar.addProduction(while_expr, {while_kw, lparen, or_expr, rparen, or_expr});

    // 11. ForExpr
    grammar.addProduction(for_expr, {for_kw, lparen, ident, in_kw, or_expr, rparen, or_expr});

    // 12. BlockExpr
    grammar.addProduction(block_expr, {lbrace, stmt_list, rbrace});

    // 13. FunctionDecl (con left-factoring)
    grammar.addProduction(function_decl, {function_kw, ident, lparen, param_list, rparen, function_body});
    grammar.addProduction(function_body, {arrow, or_expr, semicolon});
    grammar.addProduction(function_body, {block_expr});

    // 14. Listas (sin cambios)
    grammar.addProduction(param_list, {ident, param_list_prime});
    grammar.addProduction(param_list, {EPSILON});
    grammar.addProduction(param_list_prime, {comma, ident, param_list_prime});
    grammar.addProduction(param_list_prime, {EPSILON});
    grammar.addProduction(arg_list, {or_expr, arg_list_prime});
    grammar.addProduction(arg_list, {EPSILON});
    grammar.addProduction(arg_list_prime, {comma, or_expr, arg_list_prime});
    grammar.addProduction(arg_list_prime, {EPSILON});
    grammar.addProduction(binding_list, {binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {comma, binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {EPSILON});
    grammar.addProduction(binding, {ident, assign_destruct, or_expr});

    return grammar;
}

std::unique_ptr<LL1Parser> ParserFactory::createFullHulkParserV3() {
    Grammar grammar = createFullHulkGrammarV3();
    auto parser = std::make_unique<LL1Parser>(grammar);
    // setupFullHulkSemanticActionsV3(*parser); // Deshabilitado temporalmente
    return parser;
}

std::unique_ptr<LL1Parser> ParserFactory::createFullHulkParserV4() {
    Grammar grammar = createFullHulkGrammarV3();
    auto parser = std::make_unique<LL1Parser>(grammar);
    setupCompleteSemanticActionsV4(*parser); // Habilitar acciones semánticas V4
    return parser;
}

} // namespace LL1
