#include "ast_print.h"
#include "ast.h"
#include "sl_assert.h"
#include <stdio.h>

void ast_print_node_lite(AST_Node node) {
	switch (node.nodeType) {
		case AST_NODE_TYPE_UNDEFINED:
			printf("Lite: Undefined\n"); break;
		case AST_NODE_TYPE_PROGRAM:
			printf("Lite: Program\n"); break;
		case AST_NODE_TYPE_RESERVED:
			printf("Lite: Reserved: \"" SV_Fmt "\"\n", SV_Arg(node.reserved.token.text)); break;
		case AST_NODE_TYPE_TERMINAL:
			printf("Lite: Terminal\n"); break;
		case AST_NODE_TYPE_TERM:
			printf("Lite: Term\n"); break;
		case AST_NODE_TYPE_OPERATOR:
			printf("Lite: Operator\n"); break;
		case AST_NODE_TYPE_STATEMENT_EXPRESSION:
			printf("Lite: StatementExpression\n"); break;
		case AST_NODE_TYPE_PROCEDURE_DEF:
			printf("Lite: ProcedureDef\n"); break;
		case AST_NODE_TYPE_PROCEDURE_CALL:
			printf("Lite: ProcedureCall\n"); break;
		case AST_NODE_TYPE_IFF:
			printf("Lite: Iff\n"); break;
		case AST_NODE_TYPE_SWITCH:
			printf("Lite: Switch\n"); break;
		case AST_NODE_TYPE_CASE:
			printf("Lite: Case\n"); break;
		case AST_NODE_TYPE_BLOCK:
			printf("Lite: Block\n"); break;
		case AST_NODE_TYPE_SWITCH_BLOCK:
			printf("Lite: SwitchBlock\n"); break;
	}
}

void ast_print_node(AST_Node node, int depth) {
	switch (node.nodeType) {
	case AST_NODE_TYPE_UNDEFINED:      			printf("Node {UNDEFINED}\n"); break;
	case AST_NODE_TYPE_PROGRAM: 			 			ast_print_program(node.program, depth); break;
	case AST_NODE_TYPE_RESERVED:       			ast_print_reserved(node.reserved, depth); break;
	case AST_NODE_TYPE_TERMINAL:       			ast_print_terminal(node.terminal, depth); break;
	case AST_NODE_TYPE_TERM:           			ast_print_term(node.term, depth); break;
	case AST_NODE_TYPE_OPERATOR:       			ast_print_operator(node.op, depth); break;
	case AST_NODE_TYPE_PROCEDURE_DEF:  			ast_print_procedure_def(node.procDef, depth); break;
	case AST_NODE_TYPE_STATEMENT_EXPRESSION:ast_print_stmt_expr(node.stmtExpr, depth); break;
	// case AST_NODE_TYPE_STATEMENT:           ast_print_statement(node.statement, depth); break;
	// case AST_NODE_TYPE_PROCEDURE_CALL:      ast_print_procedure_call(node.procedureCall, depth); break;
	case AST_NODE_TYPE_IFF: 					      ast_print_iff(node.iff, depth); break;	
	case AST_NODE_TYPE_SWITCH:  			      ast_print_switch(node.switchf, depth); break; 
	case AST_NODE_TYPE_CASE: 					      ast_print_switch_case(node.casef, depth); break; 
	case AST_NODE_TYPE_BLOCK: 				      ast_print_block(node.block, depth); break; 
	case AST_NODE_TYPE_SWITCH_BLOCK:        ast_print_switch_block(node.switchBlock, depth); break;
	default:   												      assert(0 && "Unimplemented ast node type");
	}
}

void ast_print_program  			(Program prog, int depth) {
	printf("%*cProgram: \n", depth * 2, ' ');
	for (AST_Node* it = cvector_begin(prog.p); it != cvector_end(prog.p); it++) {
		ast_print_node(*it, depth+1);
	}
}

void ast_print_reserved       (Reserved reserved, int depth) {
	printf("%*cReserved: \'" SV_Fmt "\'\n", depth * 2, ' ', SV_Arg(reserved.token.text));
}

void ast_print_terminal       (Terminal terminal, int depth) {
	printf("%*cTerminal:\n", depth * 2, ' ');
	switch (terminal.type) {
		case TERMINAL_TYPE_IDENTIFIER: printf(" |   Identifier = " SV_Fmt, SV_Arg(terminal.id));         break;
		case TERMINAL_TYPE_DEC_LIT:    printf(" |   DecLit = %d", terminal.integer_lit);                    break;
		case TERMINAL_TYPE_DOUBLE_LIT: printf(" |   DblLit = %.8f", terminal.dbl_lit);                      break;
		case TERMINAL_TYPE_HEX_LIT:    printf(" |   HexLit = 0x%X", terminal.integer_lit);                  break;
		case TERMINAL_TYPE_STRING_LIT: printf(" |   StrLit = " SV_Fmt, SV_Arg(terminal.str_lit));        break;
		case TERMINAL_TYPE_CHAR_LIT:   printf(" |   ChrLit = " SV_Fmt, SV_Arg(terminal.chr_lit));        break;
	}
}
void ast_print_term           (Term term, int depth) {
	printf("%*cTerm:\n", depth * 2, ' ');
	switch (term.type) {
		case TERM_TYPE_DEC_LIT:    printf("%*cDecLit = %d\n",         (depth + 1) * 2, ' ', term._integer); break;
		case TERM_TYPE_DOUBLE_LIT: printf("%*cDblLit = %.8f\n",       (depth + 1) * 2, ' ', term._double); break;
		case TERM_TYPE_HEX_LIT:    printf("%*cHexLit = 0x%X\n",       (depth + 1) * 2, ' ', term._integer); break;
		case TERM_TYPE_STRING_LIT: printf("%*cStrLit = " SV_Fmt "\n", (depth + 1) * 2, ' ', SV_Arg(term._string)); break;
		case TERM_TYPE_CHR_LIT:    printf("%*cChrLit = " SV_Fmt "\n", (depth + 1) * 2, ' ', SV_Arg(term._chr)); break;
	}                                                                 
}
void ast_print_operator       (Operator op, int depth) {
	printf("%*cOperator: \n", depth * 2, ' ');
	switch (op.type) {
		case OPERATOR_TYPE_ARITH: printf("%*cArithOp = " SV_Fmt "\n", (depth + 1) * 2, ' ', SV_Arg(op.op)); break;
		case OPERATOR_TYPE_LOGIC: printf("%*cLogicOp = " SV_Fmt "\n", (depth + 1) * 2, ' ', SV_Arg(op.op)); break;
		case OPERATOR_TYPE_STACK: printf("%*cStackOp = " SV_Fmt "\n", (depth + 1) * 2, ' ', SV_Arg(op.op)); break;
		default: printf("%*cUnknown: %d\n", (depth + 1) * 2, ' ', op.type);
	}
}
void ast_print_stmt_expr(StatementExpression stmtExpr, int depth) {
	printf("%*cStmtExpr: \n", depth * 2, ' ');
	switch (stmtExpr.type) {
		case STATEMENT_EXPR_TYPE_EXPRESSION: ast_print_expression(stmtExpr.expr, depth + 1); break;
		case STATEMENT_EXPR_TYPE_STATEMENT: ast_print_statement(stmtExpr.stmt, depth + 1); break;
	}
}
void ast_print_expression     (Expression *expr, int depth) {
	switch (expr->type) {
		case EXPRESSION_TYPE_EEO:
			printf("%*cExpression(EEO): \n", depth * 2, ' ');
			ast_print_expression(expr->EEO.left, depth + 1);
			ast_print_expression(expr->EEO.right, depth + 1);
			ast_print_operator(expr->EEO.operation, depth + 1);
			break;
		case EXPRESSION_TYPE_TERM:
			printf("%*cExpression(Term): \n", depth * 2, ' ');
			ast_print_term(expr->ETerm.term, depth + 1);
			break;
		case EXPRESSION_TYPE_STACK_OP:
			/* ast_print_operator(expr->EEO.operation, depth + 1); */
			break;
		case EXPRESSION_TYPE_PROC_CALL:
			printf("%*cExpression(ProcCall): \n", depth * 2, ' ');
			ast_print_procedure_call(&expr->EProcCall.proc_call, depth + 1);
			break;
	}
}
void ast_print_procedure_def  (ProcedureDef *proc_def, int depth) {
}
void ast_print_statement      (Statement *stmt, int depth) {
	printf("%*cStatement (Print not implemented)\n", depth * 2, ' ');
	switch (stmt->type) {
		case STATEMENT_TYPE_PROCEDURE_DEF:  break;
		// case STATEMENT_TYPE_PROCEDURE_CALL: break;
		case STATEMENT_TYPE_IFF:            break;
		case STATEMENT_TYPE_SWITCH:         break;
		case STATEMENT_TYPE_CASE:         	break;
		case STATEMENT_TYPE_BLOCK:         	break;
	}
}
void ast_print_procedure_call (ProcedureCall *proc_call, int depth) {
	if (!proc_call)
		return;
	printf("%*cProcedureCall: \n", depth * 2, ' ');
	printf("%*cName: " SV_Fmt "\n", (depth + 1) * 2, ' ', SV_Arg(proc_call->name));
}
void ast_print_iff            (Iff *iff, int depth) {
	// Print expression
	// Print block
}
void ast_print_switch         (Switch *sswitch, int depth) {
}
void ast_print_switch_case    (SwitchCase *ccase, int depth) {
}
void ast_print_block   				(Block block, int depth) {
	printf("%*cBlock: [%lu]\n", depth * 2, ' ', cvector_size(block.items));
	for (StatementExpression* it = cvector_begin(block.items); it != cvector_end(block.items); it++) {
		switch (it->type) {
			case STATEMENT_EXPR_TYPE_EXPRESSION: ast_print_expression(it->expr, depth + 1); break;
			case STATEMENT_EXPR_TYPE_STATEMENT: ast_print_statement(it->stmt, depth + 1); break;
			default: sl_assert(0, "Something went very wrong");
		}
	}
}
void ast_print_switch_block   (SwitchBlock *sw_block, int depth) {
}