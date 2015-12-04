#include "semantic.hh"

semantic *type_checker = new semantic();


/* Used to check that all functions contain return statements.
   Static means that it is only visible inside this file.
   It is set to false in do_typecheck() (ie, every time we start type checking
   a new block) and set to true if we find an ast_return node. See below. */
static bool has_return = false;


/* Interface for type checking a block of code represented as an AST node. */
void semantic::do_typecheck(symbol *env, ast_stmt_list *body)
{
    // Reset the variable, since we're checking a new block of code.
    has_return = false;
    if (body) {
        body->type_check();
    }

    // This is the only case we need this variable for - a function lacking
    // a return statement. All other cases are already handled in
    // ast_return::type_check(); see below.
    if (env->tag == SYM_FUNC && !has_return) {
        // Note: We could do this by overloading the do_typecheck() method -
        // one for ast_procedurehead and one for ast_functionhead, but this
        // will do... Hopefully people won't write empty functions often,
        // since in that case we won't have position information available.
        if (body != NULL) {
            type_error(body->pos) << "A function must return a value.\n";
        } else {
            type_error() << "A function must return a value.\n";
        }
    }
}


/* Compare formal vs. actual parameters recursively. */
bool semantic::chk_param(ast_id *env,
                        parameter_symbol *formals,
                        ast_expr_list *actuals)
{
    /* Your code here, DONE */

	if (formals == NULL && actuals == NULL)
		return true;
 
	if (formals == NULL) {
		type_error(env->pos) << "More actual than formal parameters." << endl;
		return false;
	}

	if (actuals == NULL) {
		type_error(env->pos) << "More formal than actual parameters." << endl;
		return false;
	}

	if (formals->type != actuals->last_expr->type) {
		type_error(env->pos) << "Type discrepancy between formal and actual parameters." << endl;
		//formal parameters does not match actual parameters


		//<< " fomals type: " << formals->type
							  // << endl << "last_expr : " << actuals->last_expr << endl
							  // << endl << "last_expr type : " << actuals->last_expr->type << endl;
		
		return false;
	} else {
		return chk_param(env, formals->preceding, actuals->preceding);
	}
}


/* Check formal vs. actual parameters at procedure/function calls. */
void semantic::check_parameters(ast_id *call_id,
                                ast_expr_list *param_list)
{
    /* Your code here, DONE */
    sym_index sym_p = call_id->sym_p;
	sym_type tag = sym_tab->get_symbol_tag(sym_p);
	symbol *sym = sym_tab->get_symbol(sym_p);
	parameter_symbol *last_parameter;
	function_symbol *function;
	procedure_symbol *procedure;
	if (tag == SYM_FUNC) {
		function = sym->get_function_symbol();
		last_parameter = function->last_parameter;
	} else if (tag == SYM_PROC) {
		procedure = sym->get_procedure_symbol();
		last_parameter = procedure->last_parameter;
	}

	if (chk_param(call_id, last_parameter, param_list)) {
		// all is fine, chk_param returned true
	} else {
								  // << "first:" <<  last_parameter << endl << "second: "
								  // << param_list->last_expr << endl << "this: " << this << endl  << endl;
	}
}



/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
sym_index ast_node::type_check()
{
    fatal("Trying to type check abstract class ast_node.");
    return void_type;
}

sym_index ast_statement::type_check()
{
    fatal("Trying to type check abstract class ast_statement.");
    return void_type;
}

sym_index ast_expression::type_check()
{
    fatal("Trying to type check abstract class ast_expression.");
    return void_type;
}

sym_index ast_lvalue::type_check()
{
    fatal("Trying to type check abstract class ast_lvalue.");
    return void_type;
}

sym_index ast_binaryoperation::type_check()
{
    fatal("Trying to type check abstract class ast_binaryoperation.");
    return void_type;
}

sym_index ast_binaryrelation::type_check()
{
    fatal("Trying to type check abstract class ast_binaryrelation.");
    return void_type;
}



/* Type check a list of statements. */
sym_index ast_stmt_list::type_check()
{
    if (preceding != NULL) {
        preceding->type_check();
    }
    if (last_stmt != NULL) {
        last_stmt->type_check();
    }
    return void_type;
}


/* Type check a list of expressions. */
sym_index ast_expr_list::type_check()
{
    /* Your code here, DONE */
   if (preceding != NULL) {
        preceding->type_check();
    }
    if (last_expr != NULL) {
        last_expr->type_check();
    }
     return void_type;
}



/* Type check an elsif list. */
sym_index ast_elsif_list::type_check()
{
    /* Your code here, DONE */
   if (preceding != NULL) {
        preceding->type_check();
    }
    if (last_elsif != NULL) {
        last_elsif->type_check();
    }
    return void_type;
}


/* "type check" an indentifier. We need to separate nametypes from other types
   here, since all nametypes are of type void, but should return an index to
   itself in the symbol table as far as typechecking is concerned. */
sym_index ast_id::type_check()
{
    if (sym_tab->get_symbol(sym_p)->tag != SYM_NAMETYPE) {
        return type;
    }
    return sym_p;
}


sym_index ast_indexed::type_check()
{
	if (index->type_check() != integer_type) {
		type_error(index->pos) << "index must be of integer "
							   << "type.\n";
	}
    return id->type_check();
}



/* This convenience function is used to type check all binary operations
   in which implicit casting of integer to real is done: plus, minus,
   multiplication. We synthesize type information as well. */
sym_index semantic::check_binop1(ast_binaryoperation *node)
{
    /* Your code here, DONE */
	sym_index left_type, right_type;
	left_type = node->left->type_check();
	right_type = node->right->type_check();

	if (left_type == right_type) return left_type;
	if (left_type == real_type && right_type == integer_type) {
		node->right = new ast_cast(node->pos, node->right);
		return real_type;
	}
	if (right_type == real_type && left_type == integer_type) {
		node->left = new ast_cast(node->pos, node->left);
		return real_type;
	}
    return void_type; // You don't have to use this method but it might be convenient
}


sym_index ast_add::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binop1(this);

	if (return_type == void_type) {
		type_error(pos) << "Return from add must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		type = return_type;
		return type;
	}
}

sym_index ast_sub::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binop1(this);

	if (return_type == void_type) {
		type_error(pos) << "Return from sub must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		this->type = return_type;
		return type;
	}

}

sym_index ast_mult::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binop1(this);

	if (return_type == void_type) {
		type_error(pos) << "Return from mult must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		type = return_type;
		return type;
	}
}

/* Divide is a special case, since it always returns real. We make sure the
   operands are cast to real too as needed. */
sym_index ast_divide::type_check()
{
    /* Your code here, DONE */
	if ((right->type_check() == integer_type || right->type_check() == real_type) &&
		(left->type_check() == integer_type || left->type_check() == real_type)) {

		if (right->type_check() == integer_type) {
			right = new ast_cast(pos, right);
		}
		if (left->type_check() == integer_type) {
			left = new ast_cast(pos, left);
		}
	} else {
		type_error(pos) << "Data to div must be of integer or real" 
						<< "type.\n";
	}
	type = real_type;
	return real_type;
}



/* This convenience method is used to type check all binary operations
   which only accept integer operands: AND, OR, MOD, DIV.
   The second argument is the name of the operator, so we can generate a
   good error message.
   All of these return integers, so we synthesize that.
   */
sym_index semantic::check_binop2(ast_binaryoperation *node, string s)
{
    /* Your code here, DONE */
	sym_index left_type, right_type;
	left_type = node->left->type_check();
	right_type = node->right->type_check();
	if (left_type == integer_type && right_type == integer_type) {
		node->type = integer_type;
		return integer_type;
	} else {
		type_error(node->pos) << "operands to " << s <<" must be of integer" 
						<< "type.\n";
		return void_type; 
	}
}

sym_index ast_or::type_check()
{
    /* Your code here, DONE */
    return type_checker->check_binop2(this, "OR");
}

sym_index ast_and::type_check()
{
    /* Your code here, DONE */
    return type_checker->check_binop2(this, "AND");
}

sym_index ast_idiv::type_check()
{
    /* Your code here, DONE */
    return type_checker->check_binop2(this, "IDIV");
}

sym_index ast_mod::type_check()
{
    /* Your code here, DONE */
    return type_checker->check_binop2(this, "MOD");
}



/* Convienience method for all binary relations, since they're all typechecked
   the same way. They all return integer types, 1 = true, 0 = false. */
sym_index semantic::check_binrel(ast_binaryrelation *node)
{
    /* Your code here, DONE */
	sym_index left_type, right_type;
	left_type = node->left->type_check();
	right_type = node->right->type_check();

	if ((right_type == integer_type || right_type == real_type) &&
		(left_type == integer_type || left_type == real_type)) {
	
		if (left_type == right_type) return left_type;

		if (left_type == real_type && right_type == integer_type) {
			node->right = new ast_cast(node->pos, node->right);
			return real_type;
		}
		if (right_type == real_type && left_type == integer_type) {
			node->left = new ast_cast(node->pos, node->left);
			return real_type;
		}
	}

    return void_type;
}

sym_index ast_equal::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binrel(this);

	if (return_type == void_type) {
		type_error(pos) << "operands to EQUAL must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		type = integer_type; // always integer type 
		return type;
	}
    return void_type;
}

sym_index ast_notequal::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binrel(this);

	if (return_type == void_type) {
		type_error(pos) << "operands to NOTEQUAL must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		type = return_type;
		return type;
	}
    return void_type;
}

sym_index ast_lessthan::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binrel(this);

	if (return_type == void_type) {
		type_error(pos) << "operands to LESSTHAN must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		type = return_type;
		return type;
	}
    return void_type;
}

sym_index ast_greaterthan::type_check()
{
    /* Your code here, DONE */
	sym_index return_type =	type_checker->check_binrel(this);

	if (return_type == void_type) {
		type_error(pos) << "operands to GREATERTHAN must be of integer or real" 
							  << "type.\n";
		return void_type;
	} else {
		type = return_type;
		return type;
	}
    return void_type;
}



/*** The various classes derived from ast_statement. ***/

sym_index ast_procedurecall::type_check()
{
    /* Your code here, DONE */
	id->type_check();

    if (parameter_list != NULL) {
        parameter_list->type_check();
    }

	type_checker->check_parameters(id, parameter_list);
    return void_type;
}


sym_index ast_assign::type_check()
{
    /* Your code here, DONE */
	sym_index lvalue_type =	lhs->type_check();
	sym_index rvalue_type =	rhs->type_check();
	if ((lvalue_type == integer_type || lvalue_type == real_type) &&
		(rvalue_type == integer_type || rvalue_type == real_type)) {

		if (lvalue_type == real_type && rvalue_type == integer_type) {
			rhs = new ast_cast(pos, rhs);
			return lvalue_type;
		}
		if (lvalue_type == integer_type && rvalue_type == real_type) {
			type_error(pos) << "Can't assign a real value to an integer variable.\n";
			return void_type;
		}

		return lvalue_type;
	} else {
		cout << lhs << endl;
		type_error(pos) << "operands to assign not of real or integer type.";
		return void_type;
	}
}


sym_index ast_while::type_check()
{
    if (condition->type_check() != integer_type) {
        type_error(condition->pos) << "while predicate must be of integer "
                                   << "type.\n";
    }

    if (body != NULL) {
        body->type_check();
    }
    return void_type;
}


sym_index ast_if::type_check()
{
    /* Your code here, DONE */
	sym_index cond_type = condition->type_check();
	if (cond_type != integer_type) {
        type_error(condition->pos) << "if predicate must be of integer "
                                   << "type.\n";
	}

	body->type_check();

	if (elsif_list != NULL) {
		elsif_list->type_check();
	}

	if (else_body != NULL) {
		else_body->type_check();
	}

    return void_type;
}


sym_index ast_return::type_check()
{
    // This static global (meaning it is global for all methods in this file,
    // but not visible outside this file) variable is reset to 0 every time
    // we start type checking a new block of code. If we find a return
    // statement, we set it to 1. It is checked in the do_typecheck() method
    // of semantic.cc.
    has_return = true;

    // Get the current environment. We don't yet know if it's a procedure or
    // a function.
    symbol *tmp = sym_tab->get_symbol(sym_tab->current_environment());
    if (value == NULL) {
        // If the return value is NULL,
        if (tmp->tag != SYM_PROC)
            // ...and we're not inside a procedure, something is wrong.
        {
            type_error(pos) << "Must return a value from a function.\n";
        }
        return void_type;
    }

    sym_index value_type = value->type_check();

    // The return value is not NULL,
    if (tmp->tag != SYM_FUNC) {
        // ...so if we're not inside a function, something is wrong too.
        type_error(pos) << "Procedures may not return a value.\n";
        return void_type;
    }

    // Now we know it's a function and can safely downcast.
    function_symbol *func = tmp->get_function_symbol();

    // Must make sure that the return type matches the function's
    // declared return type.
    if (func->type != value_type) {
        type_error(value->pos) << "Bad return type from function.\n";
    }

    return void_type;
}


sym_index ast_functioncall::type_check()
{
    /* Your code here, DONE */
	if (parameter_list != NULL) {
		parameter_list->type_check();
	}

	type_checker->check_parameters(id, parameter_list);
    return id->type_check();
}

sym_index ast_uminus::type_check()
{
    /* Your code here, DONE */
    return expr->type_check();
}

sym_index ast_not::type_check()
{
    /* Your code here, DONE */
    return expr->type_check();
}


sym_index ast_elsif::type_check()
{
    /* Your code here, DONE */
	sym_index cond_type = condition->type_check();
	if (cond_type != integer_type) {
        type_error(condition->pos) << "elsif predicate must be of integer "
                                   << "type.\n";
	}

	body->type_check();

    return void_type;
}



sym_index ast_integer::type_check()
{
    return integer_type;
}

sym_index ast_real::type_check()
{
    return real_type;
}
