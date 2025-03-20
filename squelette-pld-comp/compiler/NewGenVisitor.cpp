#include "NewGenVisitor.h"

antlrcpp::Any NewGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{

    num_var = 0;
    #ifdef __APPLE__
    std::cout<< ".globl _main\n" ;
    std::cout<< " _main: \n" ;
    #else
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;
    #endif

    std::cout << "    pushq   %rbp\n";
    std::cout << "    movq	   %rsp, %rbp\n";

    int size = ctx->children.size() - 6;

    for(int i = 0; i < size; i++)
    {
        visit( ctx->statement(i) );
    }

    std::cout << "    popq    %rbp\n";
    
    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any NewGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expression());

    return 0;
}

antlrcpp::Any NewGenVisitor::visitDeclaration_stmt(ifccParser::Declaration_stmtContext *ctx)
{
    std::string var_name = ctx->VAR()->getText();

    if(symbolTable.find(var_name) == symbolTable.end())
    {
        symbolTable[var_name].index = (num_var + 1) * 4;
        num_var++;

    } else {
        std::cerr << "error : redefinition of variable '" << var_name << "'\n";

        exit(1);
    }

    if(ctx->expression()){
        visit(ctx->expression());
        std::cout << "    movl    %eax, -" << symbolTable[var_name].index << "(%rbp)\n";
    }

    return 0;
}

antlrcpp::Any NewGenVisitor::visitAssignation_stmt(ifccParser::Assignation_stmtContext *ctx)
{
    std::string left_var_name = ctx->VAR()->getText();

    if(symbolTable.find(left_var_name) == symbolTable.end())
    {
        std::cerr << "error : variable '" << left_var_name << "' is undefined\n";

        exit(1);
    }

    symbolTable[left_var_name].used = false;

    visit(ctx->expression());

    std::cout << "    movl    %eax, -" << symbolTable[left_var_name].index << "(%rbp)\n";

    return 0;
}

antlrcpp::Any NewGenVisitor::visitConst(ifccParser::ConstContext *ctx)
{
    std::string value = ctx->CONST()->getText();

    std::cout << "    movl    $" << value << ", %eax \n";

    return 0;
}

antlrcpp::Any NewGenVisitor::visitVariable(ifccParser::VariableContext *ctx)
{
    std::string var_name = ctx->VAR()->getText();
    if(symbolTable.find(var_name) == symbolTable.end()){
        std::cout << "error : Variable '" << var_name << "' is undefined\n";

        exit(1);
    }
    symbolTable[var_name].used = true;

    std::cout << "    movl    -" << symbolTable[var_name].index << "(%rbp), %eax \n";

    return 0;
}

antlrcpp::Any NewGenVisitor::visitMult_expression(ifccParser::Mult_expressionContext *ctx)
{
    visit(ctx->expression(0));

    num_var++;
    int temp_var = num_var;
    std::cout << "    movl    %eax, -" << temp_var * 4 << "(%rbp)\n";

    visit(ctx->expression(1));

    num_var++;
    std::cout << "    movl    %eax, -" << num_var * 4 << "(%rbp)\n";
    std::cout << "    movl    -" << temp_var * 4 << "(%rbp), %eax\n";
    
    if(ctx->children[1]->getText() == "*"){
        std::cout << "    imull   -" << num_var * 4 << "(%rbp), %eax\n";
    } else {
        std::cout << "    cltd\n";
        std::cout << "    idivl   -" << num_var * 4 << "(%rbp)\n";
    }
    return 0;
}

antlrcpp::Any NewGenVisitor::visitAdd_expression(ifccParser::Add_expressionContext *ctx)
{
    visit(ctx->expression(0));

    num_var++;
    int temp_var = num_var;
    std::cout << "    movl    %eax, -" << temp_var * 4 << "(%rbp)\n";

    visit(ctx->expression(1));
    if(ctx->children[1]->getText() == "+"){
        std::cout << "    addl    -" << temp_var * 4 << "(%rbp), %eax\n";
    } else {
        std::cout << "    subl    %eax, -" << temp_var * 4 << "(%rbp)\n";
        std::cout << "    movl    -" << temp_var * 4 << "(%rbp), %eax\n";
    }
    return 0;
}