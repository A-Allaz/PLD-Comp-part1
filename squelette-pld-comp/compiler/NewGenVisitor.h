#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class  NewGenVisitor : public ifccBaseVisitor {
    public:
        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDeclaration_stmt(ifccParser::Declaration_stmtContext *ctx) override;
        virtual antlrcpp::Any visitAssignation_stmt(ifccParser::Assignation_stmtContext *ctx) override;
        virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override ;
        virtual antlrcpp::Any visitVariable(ifccParser::VariableContext *ctx) override ;
        virtual antlrcpp::Any visitMult_expression(ifccParser::Mult_expressionContext *ctx) override ;
        virtual antlrcpp::Any visitAdd_expression(ifccParser::Add_expressionContext *ctx) override ;
    private:
        typedef struct VarProperties
        {
            int index;
            bool used;
        } VarProperties ;

        std::unordered_map<std::string, VarProperties> symbolTable;
        int num_var;
};
