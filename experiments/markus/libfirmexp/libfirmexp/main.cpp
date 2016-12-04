//
//  main.cpp
//  libfirmexp
//
//  Created by Markus Schlegel on 23/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include <iostream>
#include <firm.h>
#include <assert.h>







int main(int argc, const char * argv[]) {
  ir_init();
  set_optimize(0);
  
  
  ir_mode *int_mode = mode_Is;
  ir_type *int_type = new_type_primitive(int_mode);
  
  ir_type *t = new_type_method(1, 1, false, cc_cdecl_set, mtp_no_property);
  set_method_param_type(t, 0, int_type);
  set_method_res_type(t, 0, int_type);
  ir_entity *ent = new_entity(get_glob_type(), new_id_from_str("james"), t);
  
  ir_graph *g = new_ir_graph(ent, 1);
  set_current_ir_graph(g);
  
  ir_node *args = get_irg_args(g);
  ir_node *proj_param = new_Proj(args, mode_Is, 0);
  
  ir_type   *frame_type = get_irg_frame_type(g);
  ir_entity *param_entity = new_parameter_entity(frame_type, 0, int_type);
  
  
  
  ir_node   *frame    = get_irg_frame(g);
  ir_node   *sel      = new_Member(frame, param_entity);
  
  
  // store something in parameter
  ir_node *c = new_Const(new_tarval_from_long(42, mode_Is));
  set_value(0, c);
//  ir_node *st = new_Store(get_store(), sel, c, int_type, cons_none);
//  ir_node *m = new_Proj(st, mode_M, pn_Store_M);
//  set_store(m);
  
  
  // condition
  ir_node *d = new_Const(new_tarval_from_long(13, mode_Is));
  ir_node *cmp = new_Cmp(proj_param, d, ir_relation_equal);
  ir_node *cond = new_Cond(cmp);
  
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  
  // store 4 in parameter in if branch
  set_cur_block(trueBlock);
  ir_node *c4 = new_Const(new_tarval_from_long(4, mode_Is));
  set_value(0, c4);
//  ir_node *st4 = new_Store(get_store(), sel, c4, int_type, cons_none);
//  ir_node *m4 = new_Proj(st4, mode_M, pn_Store_M);
//  set_store(m4);
  
  ir_node *jmpIf = new_Jmp();
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  add_immBlock_pred(trueBlock, tnode);
  
  // store 5 in parameter in else branch
  set_cur_block(falseBlock);
  ir_node *c5 = new_Const(new_tarval_from_long(5, mode_Is));
  set_value(0, c5);
//  ir_node *st5 = new_Store(get_store(), sel, c5, int_type, cons_none);
//  ir_node *m5 = new_Proj(st5, mode_M, pn_Store_M);
//  set_store(m5);
  
  ir_node *jmpElse = new_Jmp();
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  add_immBlock_pred(falseBlock, fnode);
  
  
  
  add_immBlock_pred(nextBlock, jmpIf);
  mature_immBlock(trueBlock);
  add_immBlock_pred(nextBlock, jmpElse);
  mature_immBlock(falseBlock);
  
  set_cur_block(nextBlock);
  
  
  
  ir_node *out[1] = {get_value(0, mode_Is)};
  ir_node *ret = new_Return(get_store(), 1, out);
  ir_node *end = get_irg_end_block(g);
  add_immBlock_pred(end, ret);
  mature_immBlock(get_r_cur_block(g));
  
  irg_finalize_cons(g);
  dump_ir_graph(g, "whatever");

  be_main(stdout, "src name");              // emit code
  ir_finish();                            // clean up
}




