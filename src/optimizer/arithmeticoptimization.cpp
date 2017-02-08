/*
 *  Optimizes following arithmetic operations:
 * 	x-0 = x
 *  0-x = -x
 * 	x-x = 0
 *  x+0 = x
 *  0+x = x
 *  x*0 = 0
 *  x*1 = x
 *  0*x = 0
 *  1*x = x
 *  0/x = x
 * 	x/x = 1
 * 	x/1 = x
 */
 
#include "arithmeticoptimization.h"
#include <libfirm/firm.h>
#include <stdio.h>
#include <string.h>
#include <queue>

using namespace cmpl;
using namespace std;

struct Env {
  queue<ir_node *> *q;
};

int getConstantValue(ir_node* node){
	ir_tarval* tarval=get_Const_tarval(node);
		if(tarval_is_long(tarval)){
			return get_tarval_long(tarval);
		}
	return -1;
}

ir_node* createTarvalNode(int value){
	 return new_Const(new_tarval_from_long(value, mode_Is));
}  


bool isSameVariable(ir_node* a, ir_node* b){
	return a==b;
}

bool isConstValueZero(ir_node* node){
	if(is_Const(node))
	{
		return getConstantValue(node)==0;
	}
	return false;
}

bool isConstValueOne(ir_node* node){
	if(is_Const(node))
	{
		return getConstantValue(node)==1;
	}
	return false;
}

void irgOptimizationNodeWalker(ir_node *node, void *env)
{
	Env *e = (Env *) env;
	if (is_Add(node)||is_Sub(node)||is_Mul(node)||is_Div(node)) {
		e->q->push(node);
	}
}

bool optimizeNode(ir_node* node, bool doTransformation){
	  if (is_Sub(node)) {	
	ir_node* left=get_Sub_left(node);
	ir_node* right=get_Sub_right(node);	
	//check x-0
	if(isConstValueZero(right)){
		if(doTransformation)
		{
		exchange(node, left);
		}
		return true;
	}
	//check 0-x
	else if(isConstValueZero(left)){
		if(doTransformation)
		{
		exchange(node,new_Minus(right));
		}
		return true;
	}
	//check x-x
	else if(isSameVariable(left,right)){
		if(doTransformation)
		{
		exchange(node,createTarvalNode(0));
		}
		return true;
	}
  }
  else if(is_Add(node)){
	ir_node* left=get_Add_left(node);
	ir_node* right=get_Add_right(node);	
	//check x+0
	if(isConstValueZero(right)){
		if(doTransformation)
		{
		exchange(node, left);
		}
		return true;
	}
	//check 0+x
	if(isConstValueZero(left)){
		if(doTransformation)
		{
		exchange(node, right);
		}
		return true;
	}
  }
  else if(is_Mul(node)){
	ir_node* left=get_Mul_left(node);
	ir_node* right=get_Mul_right(node);	
	//check x*0
	if(isConstValueZero(right)){
		if(doTransformation)
		{
		exchange(node, createTarvalNode(0));
		}
		return true;
	}
	//chek x*1
	if(isConstValueOne(right)){
		if(doTransformation)
		{
		exchange(node, left);
		}
		return true;
	}	
	//check 0*x
	if(isConstValueZero(left)){
		if(doTransformation)
		{
		exchange(node, createTarvalNode(0));
		}
		return true;
	}
	//chek 1*x
	if(isConstValueOne(left)){
		if(doTransformation)
		{
		exchange(node, right);
		}
		return true;
	}	
  }
  else if(is_Div(node)){
	ir_node* left=get_Div_left(node);
	ir_node* right=get_Div_right(node);	
	//check 0/x
 	if(isConstValueZero(left)){
		if(doTransformation)
		{
		exchange(node, createTarvalNode(0));
		}
		return true;
	}
	//check x/1
	if(isConstValueOne(right)){
		if(doTransformation)
		{
		exchange(node, left);
		}
		return true;
	}
	//check x/x
	if(isSameVariable(left,right)){
		if(doTransformation)
		{
		exchange(node, createTarvalNode(1));
		}
		return true;
	}	
  }  
  return false;
}

void ArithmeticOptimization::run() {
  
  set_current_ir_graph(irg);
  
  // activate all edges in graph
  edges_activate(irg);
  Env *env = new Env();
  env->q = new queue<ir_node *>();
  
  // walk nodes
  irg_walk_topological(irg, irgOptimizationNodeWalker,env);
  
  // Worklist
  while (!env->q->empty()) {
    ir_node *node = env->q->front();
    env->q->pop();
    if(optimizeNode(node,true)){
		foreach_out_edge_safe(node, edge) {
        ir_node *n = get_edge_src_irn(edge);
        env->q->push(n);
      }
	}
	
  }
  
  // deactivate edges
  edges_deactivate(irg);
}


