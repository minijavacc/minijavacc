//
//  ConstantPropagation.cpp
//  mjcc
//
//  Created by Markus Schlegel on 09/01/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "constantpropagation.h"
#include <libfirm/firm.h>
#include <queue>


using namespace cmpl;
using namespace std;


struct Env {
  queue<ir_node *> *q;
  map<ir_node *, ir_tarval *> *m;
};


ir_tarval *supremum(ir_tarval *tv1, ir_tarval *tv2) {
  if (tv1 == tarval_bad || tv2 == tarval_bad) {
    return tarval_bad;
  }
  
  if (tarval_is_constant(tv1) && tarval_is_constant(tv2)) {
    if (tv1 == tv2) {
      return tv1;
    } else {
      return tarval_bad;
    }
  }
  
  if (tarval_is_constant(tv1)) {
    return tv1;
  }
  
  if (tarval_is_constant(tv2)) {
    return tv2;
  }
  
  return tarval_unknown;
}


bool transferConst(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  auto tvo  = m->at(n);
  auto tvo_ = get_Const_tarval(n);
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transferAdd(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_node *l = get_Add_left(n);
  ir_node *r = get_Add_right(n);
  
  ir_tarval *tvl = m->at(l);
  ir_tarval *tvr = m->at(r);
  
  ir_tarval *tvo = m->at(n);
  ir_tarval *tvo_;
  
  if (tvl == tarval_unknown || tvr == tarval_unknown) {
    tvo_ = tarval_unknown;
  } else if (tarval_is_constant(tvl) && tarval_is_constant(tvr)) {
    tvo_ = tarval_add(tvl, tvr);
  } else {
    tvo_ = tarval_bad;
  }
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transferSub(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_node *l = get_Sub_left(n);
  ir_node *r = get_Sub_right(n);
  
  ir_tarval *tvl = m->at(l);
  ir_tarval *tvr = m->at(r);
  
  ir_tarval *tvo = m->at(n);
  ir_tarval *tvo_;
  
  if (tvl == tarval_unknown || tvr == tarval_unknown) {
    tvo_ = tarval_unknown;
  } else if (tvl == tarval_bad || tvr == tarval_bad) {
    tvo_ = tarval_bad;
  } else {
    tvo_ = tarval_sub(tvl, tvr);
  }
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transferMul(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_node *l = get_Mul_left(n);
  ir_node *r = get_Mul_right(n);
  
  ir_tarval *tvl = m->at(l);
  ir_tarval *tvr = m->at(r);
  
  ir_tarval *tvo = m->at(n);
  ir_tarval *tvo_;
  
  if (tvl == tarval_unknown || tvr == tarval_unknown) {
    tvo_ = tarval_unknown;
  } else if (tarval_is_constant(tvl) && tarval_is_constant(tvr)) {
    tvo_ = tarval_mul(tvl, tvr);
  } else {
    tvo_ = tarval_bad;
  }
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transferDiv(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_node *l = get_Div_left(n);
  ir_node *r = get_Div_right(n);
  
  ir_tarval *tvl = m->at(l);
  ir_tarval *tvr = m->at(r);
  
  ir_tarval *tvo = m->at(n);
  ir_tarval *tvo_;
  
  if (tvl == tarval_unknown || tvr == tarval_unknown) {
    tvo_ = tarval_unknown;
  } else if (tvl == tarval_bad || tvr == tarval_bad) {
    tvo_ = tarval_bad;
  } else {
    tvo_ = tarval_div(tvl, tvr);
  }
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transferMod(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_node *l = get_Mod_left(n);
  ir_node *r = get_Mod_right(n);
  
  ir_tarval *tvl = m->at(l);
  ir_tarval *tvr = m->at(r);
  
  ir_tarval *tvo = m->at(n);
  ir_tarval *tvo_;
  
  if (tvl == tarval_unknown || tvr == tarval_unknown) {
    tvo_ = tarval_unknown;
  } else if (tvl == tarval_bad || tvr == tarval_bad) {
    tvo_ = tarval_bad;
  } else {
    tvo_ = tarval_mod(tvl, tvr);
  }
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transferPhi(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_tarval *tvo  = m->at(n);
  ir_tarval *tvo_ = tarval_unknown;
  
  for (int i = 0; i < get_Phi_n_preds(n); i++) {
    ir_node *pred = get_Phi_pred(n, i);
    ir_tarval *tv = m->at(pred);
    tvo_ = supremum(tvo_, tv);
  }
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


bool transfer(ir_node *n, map<ir_node *, ir_tarval *> *m) {
  ir_tarval *tvo  = m->at(n);
  
  if (is_Const(n)) {
    return transferConst(n, m);
  }
  
  if (is_Add(n)) {
    return transferAdd(n, m);
  }
  
  if (is_Sub(n)) {
    return transferSub(n, m);
  }
  
  if (is_Mul(n)) {
    return transferMul(n, m);
  }
  
  if (is_Div(n)) {
    return transferDiv(n, m);
  }
  
  if (is_Mod(n)) {
//    return transferMod(n, m);
  }
  
  if (is_Phi(n)) {
//    return transferPhi(n, m);
  }
  
  // constant tarval not possible for this node
  ir_tarval *tvo_ = tarval_bad;
  
  (*m)[n] = tvo_;
  return tvo != tvo_;
}


// for use with irg_walk_topological()
void irgNodeCollector(ir_node *node, void *env)
{
  Env *e = (Env *) env;
  
  (*e->m)[node] = tarval_unknown;
  
  if (is_Const(node)) {
    e->q->push(node);
  }
}


void ConstantPropagation::run() {
  
  set_current_ir_graph(irg);
  
  // Worklist queue and lattice map
  Env *env = new Env();
  env->q = new queue<ir_node *>();
  env->m = new map<ir_node *, ir_tarval *>();
  
  // activate all edges in graph
  edges_activate(irg);
  
  // Collect nodes
  irg_walk_topological(irg, irgNodeCollector, env);
  
  // Worklist
  while (!env->q->empty()) {
    ir_node *node = env->q->front();
    env->q->pop();
    
    // libfirm bug causes End Block to have weird out edges
    if (is_Block(node) || is_End(node)) {
      continue;
    }
    
    bool changed = transfer(node, env->m);
    if (changed) {
      foreach_out_edge_safe(node, edge) {
        ir_node *n = get_edge_src_irn(edge);
        env->q->push(n);
      }
    }
  }
  
  // Transform graph
  for (const auto& pair : *env->m) {
    ir_node *node = pair.first;
    ir_tarval *tv = env->m->at(node);
    if (tarval_is_constant(tv)) {
      ir_node *node_ = new_Const(tv);
      exchange(node, node_);
    }
  }
  
  // deactivate edges
  edges_deactivate(irg);
}