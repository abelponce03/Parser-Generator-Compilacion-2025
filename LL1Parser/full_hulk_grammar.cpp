#include "ll1_parser.hpp"
#include "semantic_nodes.hpp"
#include "../ast.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createFullHulkGrammar() {
    Grammar grammar;
    
    // === SÍMBOLOS NO TERMINALES ===
    
    // Programa principal
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt_list_prime(SymbolType::NON_TERMINAL, "stmt_list_prime");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol decl(SymbolType::NON_TERMINAL, "decl");
    
    // Expresiones con precedencia
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    Symbol assign_expr(SymbolType::NON_TERMINAL, "assign_expr");
    Symbol or_expr(SymbolType::NON_TERMINAL, "or_expr");
    Symbol or_expr_prime(SymbolType::NON_TERMINAL, "or_expr_prime");
    Symbol and_expr(SymbolType::NON_TERMINAL, "and_expr");
    Symbol and_expr_prime(SymbolType::NON_TERMINAL, "and_expr_prime");
    Symbol eq_expr(SymbolType::NON_TERMINAL, "eq_expr");
    Symbol eq_expr_prime(SymbolType::NON_TERMINAL, "eq_expr_prime");
    Symbol rel_expr(SymbolType::NON_TERMINAL, "rel_expr");
    Symbol rel_expr_prime(SymbolType::NON_TERMINAL, "rel_expr_prime");
    Symbol concat_expr(SymbolType::NON_TERMINAL, "concat_expr");
    Symbol concat_expr_prime(SymbolType::NON_TERMINAL, "concat_expr_prime");
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol add_expr_prime(SymbolType::NON_TERMINAL, "add_expr_prime");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol mult_expr_prime(SymbolType::NON_TERMINAL, "mult_expr_prime");
    Symbol pow_expr(SymbolType::NON_TERMINAL, "pow_expr");
    Symbol unary_expr(SymbolType::NON_TERMINAL, "unary_expr");
    Symbol postfix_expr(SymbolType::NON_TERMINAL, "postfix_expr");
    Symbol postfix_expr_prime(SymbolType::NON_TERMINAL, "postfix_expr_prime");
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    
    // Expresiones especiales
    Symbol let_expr(SymbolType::NON_TERMINAL, "let_expr");
    Symbol if_expr(SymbolType::NON_TERMINAL, "if_expr");
    Symbol elif_chain(SymbolType::NON_TERMINAL, "elif_chain");
    Symbol while_expr(SymbolType::NON_TERMINAL, "while_expr");
    Symbol for_expr(SymbolType::NON_TERMINAL, "for_expr");
    Symbol block_expr(SymbolType::NON_TERMINAL, "block_expr");
    
    // Listas y argumentos
    Symbol argument_list(SymbolType::NON_TERMINAL, "argument_list");
    Symbol argument_list_prime(SymbolType::NON_TERMINAL, "argument_list_prime");
    Symbol binding_list(SymbolType::NON_TERMINAL, "binding_list");
    Symbol binding_list_prime(SymbolType::NON_TERMINAL, "binding_list_prime");
    Symbol binding(SymbolType::NON_TERMINAL, "binding");
    Symbol ident_list(SymbolType::NON_TERMINAL, "ident_list");
    Symbol ident_list_prime(SymbolType::NON_TERMINAL, "ident_list_prime");
    
    // Declaraciones
    Symbol function_decl(SymbolType::NON_TERMINAL, "function_decl");
    Symbol class_decl(SymbolType::NON_TERMINAL, "class_decl");
    Symbol opt_inherits(SymbolType::NON_TERMINAL, "opt_inherits");
    Symbol class_body(SymbolType::NON_TERMINAL, "class_body");
    Symbol class_body_prime(SymbolType::NON_TERMINAL, "class_body_prime");
    Symbol member_def(SymbolType::NON_TERMINAL, "member_def");
    
    // === SÍMBOLOS TERMINALES ===
    
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
    Symbol pow(SymbolType::TERMINAL, "POW");
    
    // Operadores relacionales
    Symbol lt(SymbolType::TERMINAL, "LESS_THAN");
    Symbol gt(SymbolType::TERMINAL, "GREATER_THAN");
    Symbol le(SymbolType::TERMINAL, "LE");
    Symbol ge(SymbolType::TERMINAL, "GE");
    Symbol eq(SymbolType::TERMINAL, "EQ");
    Symbol neq(SymbolType::TERMINAL, "NEQ");
    
    // Operadores lógicos
    Symbol and_op(SymbolType::TERMINAL, "AND");
    Symbol or_op(SymbolType::TERMINAL, "OR");
    
    // Otros operadores
    Symbol concat(SymbolType::TERMINAL, "CONCAT");
    Symbol assign(SymbolType::TERMINAL, "ASSIGN");
    Symbol assign_destruct(SymbolType::TERMINAL, "ASSIGN_DESTRUCT");
    
    // Delimitadores
    Symbol lparen(SymbolType::TERMINAL, "LPAREN");
    Symbol rparen(SymbolType::TERMINAL, "RPAREN");
    Symbol lbrace(SymbolType::TERMINAL, "LBRACE");
    Symbol rbrace(SymbolType::TERMINAL, "RBRACE");
    Symbol semicolon(SymbolType::TERMINAL, "SEMICOLON");
    Symbol comma(SymbolType::TERMINAL, "COMMA");
    Symbol dot(SymbolType::TERMINAL, "DOT");
    Symbol arrow(SymbolType::TERMINAL, "ARROW");
    
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
    Symbol self_kw(SymbolType::TERMINAL, "SELF");
    Symbol base_kw(SymbolType::TERMINAL, "BASE");
    
    // === ESTABLECER SÍMBOLO INICIAL ===
    grammar.setStartSymbol(program);
    
    // === DEFINIR PRODUCCIONES ===
    
    // 1. Programa principal
    grammar.addProduction(program, {stmt_list});
    
    // 2. Lista de statements
    grammar.addProduction(stmt_list, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list, {EPSILON});
    grammar.addProduction(stmt_list_prime, {stmt, stmt_list_prime});
    grammar.addProduction(stmt_list_prime, {EPSILON});
    
    // 3. Statements
    grammar.addProduction(stmt, {expr, semicolon});
    grammar.addProduction(stmt, {decl});
    
    // 4. Declaraciones
    grammar.addProduction(decl, {function_decl});
    grammar.addProduction(decl, {class_decl});
    
    // 5. Expresiones (precedencia descendente)
    grammar.addProduction(expr, {assign_expr});
    
    // 6. Asignación
    grammar.addProduction(assign_expr, {or_expr});
    // Note: Para mantener LL(1), la asignación destructiva se maneja en postfix_expr
    
    // 7. OR lógico
    grammar.addProduction(or_expr, {and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {or_op, and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {EPSILON});
    
    // 8. AND lógico
    grammar.addProduction(and_expr, {eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {and_op, eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {EPSILON});
    
    // 9. Igualdad
    grammar.addProduction(eq_expr, {rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    
    // 10. Relacional
    grammar.addProduction(rel_expr, {concat_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {lt, concat_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {gt, concat_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {le, concat_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {ge, concat_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {EPSILON});
    
    // 11. Concatenación
    grammar.addProduction(concat_expr, {add_expr, concat_expr_prime});
    grammar.addProduction(concat_expr_prime, {concat, add_expr, concat_expr_prime});
    grammar.addProduction(concat_expr_prime, {EPSILON});
    
    // 12. Suma y resta
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 13. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {pow_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, pow_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, pow_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, pow_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 14. Potencia (asociativa por la derecha)
    grammar.addProduction(pow_expr, {unary_expr, pow, pow_expr});
    grammar.addProduction(pow_expr, {unary_expr});
    
    // 15. Expresiones unarias
    grammar.addProduction(unary_expr, {minus, unary_expr});
    grammar.addProduction(unary_expr, {postfix_expr});
    
    // 16. Expresiones postfix (llamadas, acceso a miembros, asignación)
    grammar.addProduction(postfix_expr, {primary_expr, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {lparen, argument_list, rparen, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {dot, ident, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {dot, ident, lparen, argument_list, rparen, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {dot, ident, assign_destruct, expr});
    grammar.addProduction(postfix_expr_prime, {assign_destruct, expr});
    grammar.addProduction(postfix_expr_prime, {EPSILON});
    
    // 17. Expresiones primarias
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident});
    grammar.addProduction(primary_expr, {self_kw});
    grammar.addProduction(primary_expr, {base_kw});
    grammar.addProduction(primary_expr, {lparen, expr, rparen});
    grammar.addProduction(primary_expr, {block_expr});
    grammar.addProduction(primary_expr, {let_expr});
    grammar.addProduction(primary_expr, {if_expr});
    grammar.addProduction(primary_expr, {while_expr});
    grammar.addProduction(primary_expr, {for_expr});
    grammar.addProduction(primary_expr, {new_kw, ident, lparen, argument_list, rparen});
    
    // 18. Expresiones especiales
    grammar.addProduction(let_expr, {let_kw, binding_list, in_kw, expr});
    
    grammar.addProduction(if_expr, {if_kw, lparen, expr, rparen, expr, elif_chain});
    grammar.addProduction(elif_chain, {else_kw, expr});
    grammar.addProduction(elif_chain, {elif_kw, lparen, expr, rparen, expr, elif_chain});
    
    grammar.addProduction(while_expr, {while_kw, lparen, expr, rparen, expr});
    
    grammar.addProduction(for_expr, {for_kw, lparen, ident, in_kw, expr, rparen, expr});
    
    grammar.addProduction(block_expr, {lbrace, stmt_list, rbrace});
    
    // 19. Listas de argumentos
    grammar.addProduction(argument_list, {expr, argument_list_prime});
    grammar.addProduction(argument_list, {EPSILON});
    grammar.addProduction(argument_list_prime, {comma, expr, argument_list_prime});
    grammar.addProduction(argument_list_prime, {EPSILON});
    
    // 20. Listas de bindings (para let)
    grammar.addProduction(binding_list, {binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {comma, binding, binding_list_prime});
    grammar.addProduction(binding_list_prime, {EPSILON});
    grammar.addProduction(binding, {ident, assign, expr});
    
    // 21. Listas de identificadores
    grammar.addProduction(ident_list, {ident, ident_list_prime});
    grammar.addProduction(ident_list, {EPSILON});
    grammar.addProduction(ident_list_prime, {comma, ident, ident_list_prime});
    grammar.addProduction(ident_list_prime, {EPSILON});
    
    // 22. Declaraciones de funciones
    grammar.addProduction(function_decl, {function_kw, ident, lparen, ident_list, rparen, arrow, expr, semicolon});
    grammar.addProduction(function_decl, {function_kw, ident, lparen, ident_list, rparen, lbrace, stmt_list, rbrace});
    
    // 23. Declaraciones de clases
    grammar.addProduction(class_decl, {type_kw, ident, opt_inherits, lbrace, class_body, rbrace});
    grammar.addProduction(opt_inherits, {inherits_kw, ident});
    grammar.addProduction(opt_inherits, {EPSILON});
    
    // 24. Cuerpo de clase
    grammar.addProduction(class_body, {member_def, class_body_prime});
    grammar.addProduction(class_body, {EPSILON});
    grammar.addProduction(class_body_prime, {member_def, class_body_prime});
    grammar.addProduction(class_body_prime, {EPSILON});
    
    // 25. Definiciones de miembros
    grammar.addProduction(member_def, {ident, assign, expr, semicolon}); // Atributo
    grammar.addProduction(member_def, {function_kw, ident, lparen, ident_list, rparen, arrow, expr, semicolon}); // Método inline
    grammar.addProduction(member_def, {function_kw, ident, lparen, ident_list, rparen, lbrace, stmt_list, rbrace}); // Método con cuerpo
    grammar.addProduction(member_def, {ident, lparen, ident_list, rparen, lbrace, stmt_list, rbrace}); // Método sin 'function'
    
    return grammar;
}

std::unique_ptr<LL1Parser> ParserFactory::createFullHulkParser() {
    Grammar grammar = createFullHulkGrammar();
    auto parser = std::make_unique<LL1Parser>(grammar);
    // setupFullHulkSemanticActions(*parser); // TODO: Implementar acciones semánticas
    return parser;
}

} // namespace LL1
