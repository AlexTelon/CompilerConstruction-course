#include "optimize.hh"

/*** This file contains all code pertaining to AST optimisation. It currently
     implements a simple optimisation called "constant folding". Most of the
     methods in this file are empty, or just relay optimize calls downward
     in the AST. If a more powerful AST optimization scheme were to be
     implemented, only methods in this file should need to be changed. ***/


ast_optimizer *optimizer = new ast_optimizer();


/* The optimizer's interface method. Starts a recursive optimize call down
   the AST nodes, searching for binary operators with constant children. */
void ast_optimizer::do_optimize(ast_stmt_list *body)
{
    if (body != NULL) {
        body->optimize();
    }
}


/* Returns 1 if an AST expression is a subclass of ast_binaryoperation,
   ie, eligible for constant folding. */
bool ast_optimizer::is_binop(ast_expression *node)
{
    switch (node->tag) {
    case AST_ADD:
    case AST_SUB:
    case AST_OR:
    case AST_AND:
    case AST_MULT:
    case AST_DIVIDE:
    case AST_IDIV:
    case AST_MOD:
        return true;
    default:
        return false;
    }
}



/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
void ast_node::optimize()
{
    fatal("Trying to optimize abstract class ast_node.");
}

void ast_statement::optimize()
{
    fatal("Trying to optimize abstract class ast_statement.");
}

void ast_expression::optimize()
{
    fatal("Trying to optimize abstract class ast_expression.");
}

void ast_lvalue::optimize()
{
    fatal("Trying to optimize abstract class ast_lvalue.");
}

void ast_binaryoperation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryoperation.");
}

void ast_binaryrelation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryrelation.");
}



/*** The optimize methods for the concrete AST classes. ***/

/* Optimize a statement list. */
void ast_stmt_list::optimize()
{
    if (preceding != NULL) {
        preceding->optimize();
    }
    if (last_stmt != NULL) {
        last_stmt->optimize();
    }
}


/* Optimize a list of expressions. */
void ast_expr_list::optimize()
{
    /* Your code here */
    if (preceding != NULL) {
        preceding->optimize();
    }
    if (last_expr != NULL) {
        last_expr->optimize();
    }
}


/* Optimize an elsif list. */
void ast_elsif_list::optimize()
{
    /* Your code here */
    if (preceding != NULL) {
        preceding->optimize();
    }
    if (last_elsif != NULL) {
        last_elsif->optimize();
    }
}


/* An identifier's value can change at run-time, so we can't perform
   constant folding optimization on it unless it is a constant.
   Thus we just do nothing here. It can be treated in the fold_constants()
   method, however. */
void ast_id::optimize()
{
	// cout << "ast_id " << (this->tag == SYM_CONST) << endl;
	// cout << "a;kdlsjaslkdjasldkjasdlkjasdkljaskdlj" << endl;
}

void ast_indexed::optimize()
{
    /* Your code here */
	index->optimize();
}


/* This convenience method is used to apply constant folding to all
   binary operations. It returns either the resulting optimized node or the
   original node if no optimization could be performed. */
ast_expression *ast_optimizer::fold_constants(ast_expression *node)
{
    /* Your code here */
  if (node == NULL) return NULL;
	if (is_binop(node)) { // kinda unneccesary
		ast_binaryoperation *binop_node = node->get_ast_binaryoperation();
		if (binop_node == NULL) {
			return NULL;
		}
		sym_index left_type = binop_node->left->type;
		sym_index right_type = binop_node->right->type;
		long left_int, right_int;
		double left_real, right_real;
		
		// need to be int or real to be foldable TODO check ast_id, sida 114
		if ((left_type != integer_type || left_type != real_type) &&
			(right_type != integer_type || right_type != real_type)) {

			if (left_type == integer_type) {
				if(binop_node->left->get_ast_integer() != NULL){
					left_int = binop_node->left->get_ast_integer()->value;
				} else if (binop_node->left->get_ast_id() != NULL) {
					ast_id *ast_thing = binop_node->left->get_ast_id();
					symbol* tmp_sym = sym_tab->get_symbol(ast_thing->sym_p);
					if (tmp_sym->tag == SYM_CONST) {
						constant_symbol *const_sym = tmp_sym->get_constant_symbol();
						left_int = const_sym->const_value.ival;
					} else {
						return NULL;
					}
				} else {
					return NULL;
				}
				left_real = (double)left_int;

			} else {
				if(binop_node->left->get_ast_real() != NULL){
					left_real = binop_node->left->get_ast_real()->value;
				} else if (binop_node->left->get_ast_id() != NULL) {
					ast_id *ast_thing = binop_node->left->get_ast_id();
					symbol* tmp_sym = sym_tab->get_symbol(ast_thing->sym_p);
					if (tmp_sym->tag == SYM_CONST) {
						constant_symbol *const_sym = tmp_sym->get_constant_symbol();
						left_real = const_sym->const_value.rval;
					} else {
						return NULL;
					}
				} else {
					return NULL;
				}
			}

			if (right_type == integer_type) {
				if(binop_node->right->get_ast_integer() != NULL){
					right_int = binop_node->right->get_ast_integer()->value;
				} else if (binop_node->right->get_ast_id() != NULL) {
					ast_id *ast_thing = binop_node->right->get_ast_id();
					symbol* tmp_sym = sym_tab->get_symbol(ast_thing->sym_p);
					if (tmp_sym->tag == SYM_CONST) {
						constant_symbol *const_sym = tmp_sym->get_constant_symbol();
						right_int = const_sym->const_value.ival;
					} else {
						return NULL;
					}
				} else {
					return NULL;
				}
				right_real = (double)left_int;
			} else {
				if(binop_node->right->get_ast_real() != NULL){
					right_real = binop_node->right->get_ast_real()->value;
				} else if (binop_node->right->get_ast_id() != NULL) {
					ast_id *ast_thing = binop_node->right->get_ast_id();
					symbol* tmp_sym = sym_tab->get_symbol(ast_thing->sym_p);
					if (tmp_sym->tag == SYM_CONST) {
						constant_symbol *const_sym = tmp_sym->get_constant_symbol();
						right_real = const_sym->const_value.rval;
					} else {
						return NULL;
					}
				} else {
					return NULL;
				}
			}

			if (left_type == integer_type && right_type == integer_type) {
				long retval;
				switch (binop_node->tag) {
				case AST_ADD: retval = left_int + right_int; break;
				case AST_SUB: retval = left_int - right_int; break;
				case AST_OR: retval = left_int || right_int; break;
				case AST_AND: retval = left_int && right_int; break;
				case AST_MULT: retval = left_int * right_int; break;
				case AST_DIVIDE:  return new ast_real(binop_node->pos, left_int / right_int); // TODO if right is 0??
				case AST_IDIV: retval = left_int / right_int; break;
				case AST_MOD: retval = left_int % right_int; break;
				default:
					type_error(binop_node->pos) << "what is this? You suck!" << endl;
				}
				return new ast_integer(binop_node->pos, retval);				
			} else {
				double retval;
				switch (binop_node->tag) {
				case AST_ADD: retval = left_real + right_real; break;
				case AST_SUB: retval = left_real - right_real; break;

				case AST_OR:
				case AST_MOD: 
				case AST_AND:  type_error(binop_node->pos) << "Invalid operation,"
													 << " to OR, AND or MOD with reals in optimization."
													 << endl; break;

				case AST_MULT: retval = left_real * right_real; break;
				case AST_DIVIDE:  return new ast_real(binop_node->pos, left_real / right_real); // TODO if right is 0??
				case AST_IDIV: type_error(binop_node->pos) << "Invalid operation, IDIV with reals in optimization."
													 << endl; break;

				default:
					type_error(binop_node->pos) << "what is this? You suck!" << endl;
				}
				return new ast_real(binop_node->pos, retval);				
			}


		// type_error(node->pos) << "batman left type " << left->type << endl; //node->left << endl;
		// type_error(node->pos) << "batman right " << right << endl;

		}


	}
    return NULL;
}


/* All the binary operations should already have been detected in their parent
   nodes, so we don't need to do anything at all here. */
void ast_add::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;
}

void ast_sub::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_mult::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_divide::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_or::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_and::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_idiv::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_mod::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}



/* We can apply constant folding to binary relations as well. */
void ast_equal::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_notequal::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_lessthan::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}

void ast_greaterthan::optimize()
{
    /* Your code here */
	ast_expression* new_ast_left = optimizer->fold_constants(this->left);
	ast_expression* new_ast_right = optimizer->fold_constants(this->right);
	if (new_ast_left != NULL) this->left = new_ast_left;
	if (new_ast_right != NULL) this->right = new_ast_right;

}



/*** The various classes derived from ast_statement. ***/

void ast_procedurecall::optimize()
{
    /* Your code here */
	if (parameter_list != NULL) {
		parameter_list->optimize();
	}
}


void ast_assign::optimize()
{
    /* Your code here */
	lhs->optimize();
	rhs->optimize();
	ast_expression* new_ast_right = optimizer->fold_constants(this->rhs);
	if (new_ast_right != NULL) this->rhs = new_ast_right;
	
}


void ast_while::optimize()
{
    /* Your code here */
	condition->optimize();
	body->optimize();

	ast_expression* new_ast_cond = optimizer->fold_constants(this->condition);
	if (new_ast_cond != NULL) this->condition = new_ast_cond;

}


void ast_if::optimize()
{
    /* Your code here */
	condition->optimize();
	body->optimize();

	ast_expression* new_ast_cond = optimizer->fold_constants(this->condition);
	if (new_ast_cond != NULL) this->condition = new_ast_cond;

	if (elsif_list != NULL) {
		elsif_list->optimize();
	}

	if (else_body != NULL) {
		else_body->optimize();
	}
}


void ast_return::optimize()
{
  /* Your code here */
  if (value != NULL) {
    value->optimize();
  }

  ast_expression* new_ast_value = optimizer->fold_constants(this->value);
  if (new_ast_value != NULL) this->value = new_ast_value;
}


void ast_functioncall::optimize()
{
    /* Your code here */
	if (parameter_list != NULL) {
		parameter_list->optimize();
	}
}

void ast_uminus::optimize()
{
    /* Your code here */
	expr->optimize();
}

void ast_not::optimize()
{
    /* Your code here */
	expr->optimize();
}


void ast_elsif::optimize()
{
  /* Your code here */
  condition->optimize();
  if(body != NULL){	
    body->optimize();
  }
}



void ast_integer::optimize()
{
    /* Your code here */
	// do nothing.
}

void ast_real::optimize()
{
    /* Your code here */
	// do nothing.
}

/* Note: See the comment in fold_constants() about casts and folding. */
void ast_cast::optimize()
{
    /* Your code here */
}



void ast_procedurehead::optimize()
{
    fatal("Trying to call ast_procedurehead::optimize()");
}


void ast_functionhead::optimize()
{
    fatal("Trying to call ast_functionhead::optimize()");
}
