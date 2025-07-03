#include "ll1_grammar.hpp"
#include "ll1_parser.hpp"
#include <iostream>
#include <iomanip>

using namespace LL1;

// Programa auxiliar para mapear IDs de producción
int main() {
    std::cout << "=== HULK Grammar V3 Production ID Mapping ===" << std::endl;
    std::cout << std::endl;
    
    // Crear la gramática V3
    Grammar grammar = ParserFactory::createFullHulkGrammarV3();
    
    // Obtener las producciones
    const auto& productions = grammar.getProductions();
    
    std::cout << "Total productions: " << productions.size() << std::endl;
    std::cout << std::endl;
    
    // Mapear cada producción con su ID
    for (size_t i = 0; i < productions.size(); ++i) {
        const auto& prod = productions[i];
        
        std::cout << "Production ID " << std::setw(2) << i << ": ";
        std::cout << prod.lhs.name << " -> ";
        
        if (prod.isEpsilonProduction()) {
            std::cout << "ε";
        } else {
            for (size_t j = 0; j < prod.rhs.size(); ++j) {
                if (j > 0) std::cout << " ";
                std::cout << prod.rhs[j].name;
            }
        }
        
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "=== Key Productions for Semantic Actions ===" << std::endl;
    std::cout << std::endl;
    
    // Identificar producciones clave
    for (size_t i = 0; i < productions.size(); ++i) {
        const auto& prod = productions[i];
        
        // Identificar producciones importantes para construcción de AST
        if (prod.lhs.name == "program" || 
            prod.lhs.name == "stmt_list" || 
            prod.lhs.name == "stmt" ||
            prod.lhs.name == "primary_expr" ||
            prod.lhs.name == "or_expr" ||
            prod.lhs.name == "and_expr" ||
            prod.lhs.name == "add_expr" ||
            prod.lhs.name == "mult_expr" ||
            (prod.rhs.size() == 1 && prod.rhs[0].name == "NUMBER") ||
            (prod.rhs.size() == 1 && prod.rhs[0].name == "STRING") ||
            (prod.rhs.size() == 1 && prod.rhs[0].name == "TRUE") ||
            (prod.rhs.size() == 1 && prod.rhs[0].name == "FALSE") ||
            (prod.rhs.size() == 1 && prod.rhs[0].name == "IDENT")) {
            
            std::cout << "⭐ ID " << std::setw(2) << i << ": " << prod.lhs.name << " -> ";
            
            if (prod.isEpsilonProduction()) {
                std::cout << "ε";
            } else {
                for (size_t j = 0; j < prod.rhs.size(); ++j) {
                    if (j > 0) std::cout << " ";
                    std::cout << prod.rhs[j].name;
                }
            }
            
            std::cout << std::endl;
        }
    }
    
    return 0;
}
