#include "ll1_parser.hpp"
#include <iostream>

namespace LL1 {

Grammar ParserFactory::createHulkGrammar() {
    Grammar grammar;
    
    // === DEFINIR SÍMBOLOS ===
    
    // No terminales principales
    Symbol program(SymbolType::NON_TERMINAL, "program");
    Symbol stmt_list(SymbolType::NON_TERMINAL, "stmt_list");
    Symbol stmt(SymbolType::NON_TERMINAL, "stmt");
    Symbol decl(SymbolType::NON_TERMINAL, "decl");
    Symbol expr(SymbolType::NON_TERMINAL, "expr");
    Symbol expr_stmt(SymbolType::NON_TERMINAL, "expr_stmt");
    
    // Expresiones
    Symbol primary_expr(SymbolType::NON_TERMINAL, "primary_expr");
    Symbol postfix_expr(SymbolType::NON_TERMINAL, "postfix_expr");
    Symbol unary_expr(SymbolType::NON_TERMINAL, "unary_expr");
    Symbol mult_expr(SymbolType::NON_TERMINAL, "mult_expr");
    Symbol add_expr(SymbolType::NON_TERMINAL, "add_expr");
    Symbol rel_expr(SymbolType::NON_TERMINAL, "rel_expr");
    Symbol eq_expr(SymbolType::NON_TERMINAL, "eq_expr");
    Symbol and_expr(SymbolType::NON_TERMINAL, "and_expr");
    Symbol or_expr(SymbolType::NON_TERMINAL, "or_expr");
    Symbol assign_expr(SymbolType::NON_TERMINAL, "assign_expr");
    
    // Primes para LL(1)
    Symbol stmt_list_prime(SymbolType::NON_TERMINAL, "stmt_list_prime");
    Symbol postfix_expr_prime(SymbolType::NON_TERMINAL, "postfix_expr_prime");
    Symbol mult_expr_prime(SymbolType::NON_TERMINAL, "mult_expr_prime");
    Symbol add_expr_prime(SymbolType::NON_TERMINAL, "add_expr_prime");
    Symbol rel_expr_prime(SymbolType::NON_TERMINAL, "rel_expr_prime");
    Symbol eq_expr_prime(SymbolType::NON_TERMINAL, "eq_expr_prime");
    Symbol and_expr_prime(SymbolType::NON_TERMINAL, "and_expr_prime");
    Symbol or_expr_prime(SymbolType::NON_TERMINAL, "or_expr_prime");
    Symbol assign_expr_prime(SymbolType::NON_TERMINAL, "assign_expr_prime");
    
    // Listas y argumentos
    Symbol argument_list(SymbolType::NON_TERMINAL, "argument_list");
    Symbol argument_list_prime(SymbolType::NON_TERMINAL, "argument_list_prime");
    Symbol ident_list(SymbolType::NON_TERMINAL, "ident_list");
    Symbol ident_list_prime(SymbolType::NON_TERMINAL, "ident_list_prime");
    
    // Construcciones especiales
    Symbol let_expr(SymbolType::NON_TERMINAL, "let_expr");
    Symbol if_expr(SymbolType::NON_TERMINAL, "if_expr");
    Symbol while_expr(SymbolType::NON_TERMINAL, "while_expr");
    Symbol block_expr(SymbolType::NON_TERMINAL, "block_expr");
    Symbol function_decl(SymbolType::NON_TERMINAL, "function_decl");
    
    // Terminales básicos
    Symbol number(SymbolType::TERMINAL, "NUMBER");
    Symbol string_lit(SymbolType::TERMINAL, "STRING");
    Symbol ident(SymbolType::TERMINAL, "IDENT");
    Symbol true_lit(SymbolType::TERMINAL, "TRUE");
    Symbol false_lit(SymbolType::TERMINAL, "FALSE");
    
    // Operadores
    Symbol plus(SymbolType::TERMINAL, "PLUS");
    Symbol minus(SymbolType::TERMINAL, "MINUS");
    Symbol mult(SymbolType::TERMINAL, "MULT");
    Symbol div(SymbolType::TERMINAL, "DIV");
    Symbol mod(SymbolType::TERMINAL, "MOD");
    Symbol pow(SymbolType::TERMINAL, "POW");
    Symbol concat(SymbolType::TERMINAL, "CONCAT");
    
    // Comparadores
    Symbol eq(SymbolType::TERMINAL, "EQ");
    Symbol neq(SymbolType::TERMINAL, "NEQ");
    Symbol lt(SymbolType::TERMINAL, "LESS_THAN");
    Symbol gt(SymbolType::TERMINAL, "GREATER_THAN");
    Symbol le(SymbolType::TERMINAL, "LE");
    Symbol ge(SymbolType::TERMINAL, "GE");
    
    // Lógicos
    Symbol and_op(SymbolType::TERMINAL, "AND");
    Symbol or_op(SymbolType::TERMINAL, "OR");
    
    // Asignación
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
    
    // Palabras clave
    Symbol let(SymbolType::TERMINAL, "LET");
    Symbol in(SymbolType::TERMINAL, "IN");
    Symbol if_kw(SymbolType::TERMINAL, "IF");
    Symbol else_kw(SymbolType::TERMINAL, "ELSE");
    Symbol elif_kw(SymbolType::TERMINAL, "ELIF");
    Symbol while_kw(SymbolType::TERMINAL, "WHILE");
    Symbol for_kw(SymbolType::TERMINAL, "FOR");
    Symbol function(SymbolType::TERMINAL, "FUNCTION");
    Symbol arrow(SymbolType::TERMINAL, "ARROW");
    Symbol type_kw(SymbolType::TERMINAL, "TYPE");
    Symbol inherits(SymbolType::TERMINAL, "INHERITS");
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
    grammar.addProduction(stmt, {expr_stmt});
    grammar.addProduction(stmt, {decl});
    
    // 4. Expression statement
    grammar.addProduction(expr_stmt, {expr, semicolon});
    
    // 5. Declaraciones
    grammar.addProduction(decl, {function_decl});
    
    // 6. Función simple (inline)
    grammar.addProduction(function_decl, {function, ident, lparen, ident_list, rparen, arrow, expr, semicolon});
    
    // 7. Lista de identificadores
    grammar.addProduction(ident_list, {ident, ident_list_prime});
    grammar.addProduction(ident_list, {EPSILON});
    grammar.addProduction(ident_list_prime, {comma, ident, ident_list_prime});
    grammar.addProduction(ident_list_prime, {EPSILON});
    
    // 8. Expresiones (precedencia ascendente)
    grammar.addProduction(expr, {assign_expr});
    
    // 9. Asignación (simplificado por ahora)
    grammar.addProduction(assign_expr, {or_expr});
    
    // 10. OR lógico
    grammar.addProduction(or_expr, {and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {or_op, and_expr, or_expr_prime});
    grammar.addProduction(or_expr_prime, {EPSILON});
    
    // 11. AND lógico
    grammar.addProduction(and_expr, {eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {and_op, eq_expr, and_expr_prime});
    grammar.addProduction(and_expr_prime, {EPSILON});
    
    // 12. Igualdad
    grammar.addProduction(eq_expr, {rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {eq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {neq, rel_expr, eq_expr_prime});
    grammar.addProduction(eq_expr_prime, {EPSILON});
    
    // 13. Relacional
    grammar.addProduction(rel_expr, {add_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {lt, add_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {gt, add_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {le, add_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {ge, add_expr, rel_expr_prime});
    grammar.addProduction(rel_expr_prime, {EPSILON});
    
    // 14. Suma y resta
    grammar.addProduction(add_expr, {mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {plus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {minus, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {concat, mult_expr, add_expr_prime});
    grammar.addProduction(add_expr_prime, {EPSILON});
    
    // 15. Multiplicación, división, módulo
    grammar.addProduction(mult_expr, {unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mult, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {div, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {mod, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {pow, unary_expr, mult_expr_prime});
    grammar.addProduction(mult_expr_prime, {EPSILON});
    
    // 16. Expresiones unarias
    grammar.addProduction(unary_expr, {postfix_expr});
    grammar.addProduction(unary_expr, {minus, unary_expr});
    
    // 17. Expresiones postfix (llamadas, acceso a miembros)
    grammar.addProduction(postfix_expr, {primary_expr, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {lparen, argument_list, rparen, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {dot, ident, postfix_expr_prime});
    grammar.addProduction(postfix_expr_prime, {EPSILON});
    
    // 18. Lista de argumentos
    grammar.addProduction(argument_list, {expr, argument_list_prime});
    grammar.addProduction(argument_list, {EPSILON});
    grammar.addProduction(argument_list_prime, {comma, expr, argument_list_prime});
    grammar.addProduction(argument_list_prime, {EPSILON});
    
    // 19. Expresiones primarias
    grammar.addProduction(primary_expr, {number});
    grammar.addProduction(primary_expr, {string_lit});
    grammar.addProduction(primary_expr, {true_lit});
    grammar.addProduction(primary_expr, {false_lit});
    grammar.addProduction(primary_expr, {ident});
    grammar.addProduction(primary_expr, {self_kw});
    grammar.addProduction(primary_expr, {base_kw});
    grammar.addProduction(primary_expr, {lparen, expr, rparen});
    grammar.addProduction(primary_expr, {lbrace, stmt_list, rbrace});
    grammar.addProduction(primary_expr, {let_expr});
    grammar.addProduction(primary_expr, {if_expr});
    grammar.addProduction(primary_expr, {while_expr});
    grammar.addProduction(primary_expr, {new_kw, ident, lparen, argument_list, rparen});
    
    // 20. Let expression
    grammar.addProduction(let_expr, {let, ident, assign, expr, in, expr});
    
    // 21. If expression (simplificado)
    grammar.addProduction(if_expr, {if_kw, lparen, expr, rparen, expr, else_kw, expr});
    
    // 22. While expression
    grammar.addProduction(while_expr, {while_kw, lparen, expr, rparen, expr});
    
    return grammar;
}

void ParserFactory::setupHulkSemanticActions(LL1Parser& parser) {
    // Acciones semánticas básicas para construir AST
    
    // program -> stmt_list
    parser.setSemanticAction(0, [](const std::vector<Token>& tokens, std::stack<std::unique_ptr<SemanticNode>>& stack) {
        auto program = std::make_unique<Program>();
        // Obtener stmt_list del stack y agregarlo al programa
        // Para simplicidad, por ahora solo creamos un programa vacío
        // stack.push(program);  // Esta línea causaría problemas de tipos
    });
    
    // Más acciones semánticas se pueden agregar aquí...
}

std::unique_ptr<LL1Parser> ParserFactory::createHulkParser() {
    Grammar grammar = createHulkGrammar();
    auto parser = std::make_unique<LL1Parser>(grammar);
    setupHulkSemanticActions(*parser);
    return parser;
}

} // namespace LL1
