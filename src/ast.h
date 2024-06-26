#ifndef AST_H
#define AST_H

/**
 *    ---------------- TERMINALS ----------------------
 *    id     				 := [a-zA-Z][a-zA-Z0-9_]*
 * 		hexlit         := 0x[0-9a-fA-F]+
 *    dbllit         := [0-9]+\\.[0-9]+
 * 		declit         := [0-9]+
 *    strlit         := \\"([^\\"]|\n)*\\"
 *    chrlit         := \\'.\\'
 * 		stack_op       := ',' | '.'
 * 		arith_op       := '+' | '-' | '*' | '/' | '%'
 * 		logic_op       := "&&" | "||" | '>' | '<' | "==" | ">=" | "<="
 *    -------------- NON-TERMINALS ----------------------
 * 		term           := <declit> 
 * 										| <hexlit> 
 * 										| <dbllit> 
 * 										| <strlit> 
 * 										| <chrlit>
 * 		operator       := <arith_op> 
 * 										| <logic_op>
 *    expression     := <expression> <expression> <operator> 
 *    								| <operator> 
 *    								| <term> 
 *    								| <stack_op> 
 *    								| <procedure_call>
 *    procedure_call := <expression> <id>
 * 		procedure      := <id> <block>
 * 		statement  		 := <if> 
 * 										| <switch>
 * 		statements     := <statement> <statements> 
 * 		                | null
 * 		block          := '{' <statements> '}'
 *    if   					 := if <expression> <block>
 *
 *    -------- FUTURE ----------
 *    switch				 := switch <stack_op> <switch-block>
 * 		switch-case    := <term> ':' <expression>
 * 		switch-block   := <switch-case> <block>
 */

#include "cvector.h"
#include "sv.h"
#include "tokenizer.h"
// These directly corrolate to the grammar above
typedef enum TerminalType {
	TERMINAL_TYPE_IDENTIFIER, // <id>
	TERMINAL_TYPE_HEX_LIT,    // <hex_lit>
	TERMINAL_TYPE_DOUBLE_LIT, // <dbl_lit>
	TERMINAL_TYPE_DEC_LIT,    // <dec_lit>
	TERMINAL_TYPE_STRING_LIT, // <str_lit>
	TERMINAL_TYPE_CHAR_LIT,   // <chr_lit>
	// TERMINAL_TYPE_OPERATOR,   // <logic_op> | <stack_op> | <arith_op>
	// TERMINAL_TYPE_RESERVED,   // other...
} TerminalType;
typedef enum TermType {
	TERM_TYPE_DEC_LIT,        // term := <dec_lit>
	TERM_TYPE_HEX_LIT,        // term := <hex_lit>
	TERM_TYPE_DOUBLE_LIT,     // term := <dbl_lit>
	TERM_TYPE_STRING_LIT,     // term := <str_lit>
	TERM_TYPE_CHR_LIT         // term := <chr_lit>
} TermType;
typedef enum OperatorType {
	OPERATOR_TYPE_ARITH,      // operator := <arith_op>
	OPERATOR_TYPE_LOGIC,      // operator := <logic_op>
	OPERATOR_TYPE_STACK			  // operator := <stack_op> ??
} OperatorType;
typedef enum StackOpType {
	STACK_OP_TYPE_COMMA_SEQ,
	STACK_OP_TYPE_PERIOD_SEQ,
	STACK_OP_TYPE_SEMI_SEQ,
} StackOpType;
typedef enum ExpressionType {
	EXPRESSION_TYPE_PROC_CALL,// expression := <expression> <id>
	EXPRESSION_TYPE_EEO,      // expression := <expression> <expression> <operator>
	EXPRESSION_TYPE_TERM,     // expression := <term>
	EXPRESSION_TYPE_STACK_OP, // expression := <stack_op>
} ExpressionType;
typedef enum StatementType {
	// STATEMENT_TYPE_EXPRESSION,   // statement := <expression>   // Not really
	STATEMENT_TYPE_PROCEDURE_DEF,   // statement := <procedure_def>
	// STATEMENT_TYPE_PROCEDURE_CALL,  // statement := <procedure_call>
	STATEMENT_TYPE_IFF,             // statement := <iff>
	STATEMENT_TYPE_SWITCH,          // statement := <switch>
	STATEMENT_TYPE_CASE,            // statement := <case>
	STATEMENT_TYPE_BLOCK            // statement := <block>
} StatementType;
typedef enum StatementExpressionType {
	STATEMENT_EXPR_TYPE_EXPRESSION,
	STATEMENT_EXPR_TYPE_STATEMENT
} StatementExpressionType;

typedef struct Program Program;
typedef struct Reserved Reserved;
typedef struct Terminal Terminal;
typedef struct Term Term;
typedef struct Operator Operator;
typedef struct StackOp StackOp;
typedef struct Expression Expression;
typedef struct ProcedureDef ProcedureDef;
typedef struct Statement Statement;
typedef struct StatementExpression StatementExpression;
typedef struct ProcedureCall ProcedureCall;
typedef struct Iff Iff;
typedef struct Switch Switch;
typedef struct SwitchCase SwitchCase;
typedef struct Block Block;
typedef struct SwitchBlock SwitchBlock;

typedef enum AST_NodeType {
	AST_NODE_TYPE_STACK_UNDERFLOW = -1,
	AST_NODE_TYPE_UNDEFINED = 0,
	AST_NODE_TYPE_PROGRAM,
	AST_NODE_TYPE_RESERVED,
	AST_NODE_TYPE_TERMINAL,
	AST_NODE_TYPE_TERM,
	AST_NODE_TYPE_OPERATOR,
	AST_NODE_TYPE_STACK_OPERATOR,
	AST_NODE_TYPE_PROCEDURE_DEF,
	AST_NODE_TYPE_STATEMENT_EXPRESSION,
	AST_NODE_TYPE_PROCEDURE_CALL,
	AST_NODE_TYPE_IFF,
	AST_NODE_TYPE_SWITCH,
	AST_NODE_TYPE_CASE,
	AST_NODE_TYPE_BLOCK,
	AST_NODE_TYPE_SWITCH_BLOCK,
} AST_NodeType;

typedef struct AST_Node AST_Node;

struct Program {
	tokenizer_state state;
	cvector_vector_type(AST_Node) p;
};

struct Reserved {
	tokenizer_state state;
	token token;
	// String_View str;
};

struct Operator {
	tokenizer_state state;
	OperatorType type;
	String_View op_str;
};

struct Terminal {
	tokenizer_state state;
	TerminalType type;
	union {
		// Reserved reserved;
		String_View id;
		String_View str_lit;
		String_View chr_lit;
		int integer_lit;
		double dbl_lit;
		Operator operatorr;
	};
};

struct Term {
	tokenizer_state state;
	TermType type;
	union {
		int         _integer;
		double      _double;
		String_View _string;
		String_View _ident;
		String_View _chr;
	};
};

struct ProcedureCall {
	tokenizer_state state;
	String_View name;
	int argumentCount;
};

struct StackOp {
	StackOpType type;
	Operator op;
};

/** 
 *   when type == TERM
 *     - only term is guarunteed to be a valid value
 *   when type == STACK_OP
 *     - only operation is guarunteed to be valid
 *   when type == COMPOUND
 *     - only term, term2, and operation are guarunteed to be valid
 */
struct Expression {
	tokenizer_state state;
	ExpressionType type;
	union {
		StackOp stackOp;
		// struct {
		// 	StackOpType type;
		// 	Operator op;
		// 	int repeat;
		// } StackOp;
		struct {
			Expression *left, *right;
			Operator operation;
		} EEO;
		struct {
			ProcedureCall proc_call;
		} EProcCall;
		struct {
			Term term;
		} ETerm;
	};
};

struct Block {
	tokenizer_state state;
	cvector_vector_type(StatementExpression) items;
};

struct SwitchBlock {
	tokenizer_state state;
	cvector_vector_type(SwitchCase) cases;
};

struct ProcedureDef {
	tokenizer_state state;
	String_View name;
	cvector_vector_type(String_View) params;
	Block block;
};

struct Iff {
	tokenizer_state state;
	Expression* expression;
	Block block;
};

struct Switch {
	tokenizer_state state;
	Expression expr;
	SwitchBlock block;
};

struct SwitchCase {
	tokenizer_state state;
	Term value;
	Block b;
};

struct Statement {
	StatementType type;
	tokenizer_state state;
	union {
		Iff iff;
		Switch switchh;
		ProcedureCall procCall;
	};
};

// Able to be both a statement and an expression at once
struct StatementExpression {
	StatementExpressionType type;
	union {
		Statement* stmt;
		Expression* expr;
	};
};

struct AST_Node {
	AST_NodeType nodeType;
	tokenizer_state state;
	union {
		Program program;
		Terminal terminal;
		Operator op;
		StackOp stackOp;
		Reserved reserved;
		Term term;
		ProcedureDef *procDef;
		StatementExpression stmtExpr;
		// Expression *expression;
		// Statement *statement;
		// ProcedureCall *procedureCall;
		// Iff *iff;
		Switch *switchf;
		SwitchCase *casef;
		Block block;
		SwitchBlock *switchBlock;
	};
};

#endif
