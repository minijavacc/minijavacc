//
//  main.cpp
//  libfirmexp
//
//  Created by Markus Schlegel on 23/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include <iostream>
#include <firm.h>







int main(int argc, const char * argv[]) {
  ir_init();
  
  
  ir_mode *int_mode = mode_Is;
  ir_type *int_type = new_type_primitive(int_mode);
  
  
  
  // new type
  // -----------------
  
  ir_type *type = new_type_struct(new_id_from_str("yo"));
  
  
  
  
  // new simple method
  // -----------------
  
  ir_type *t = new_type_method(1, 1, false, cc_cdecl_set, mtp_no_property);
  set_method_param_type(t, 0, new_type_primitive(int_mode));
  set_method_res_type(t, 0, new_type_primitive(int_mode));
  ir_entity *ent = new_entity(get_glob_type(), new_id_from_str("james"), t);
  
  
  ir_graph *fun_graph = new_ir_graph(ent, 1); // create a new graph
  set_current_ir_graph(fun_graph);
  
  // keep track of the current block
  ir_node *block = get_r_cur_block(fun_graph);
  // set the start block to be the current block
  set_r_cur_block(fun_graph, get_irg_start_block(fun_graph));
  // get a reference to the arguments node
  ir_node *args = get_irg_args(fun_graph);
  
  ir_node *proj = new_Proj(args, int_mode, 0);
  
  
  
  
  // handle body
  ir_tarval *tv = new_tarval_from_long(42, mode_Is);
  ir_node *lhs = new_Const(tv);
  
  ir_node *rhs = proj;
  
  ir_node *node = new_Add(lhs, rhs);
  
  
  // the result of the function is the result of the body
  ir_node *results[1] = { node };
  ir_node *store = get_store();
  ir_node *ret = new_Return(store, 1, results);               // create a return node
  ir_node *end = get_irg_end_block(fun_graph);                // get hold of the end block
  // set the return node to be its predecessor
  add_immBlock_pred(end, ret);
  
  mature_immBlock(get_r_cur_block(fun_graph));          // mature the current block
  
  irg_finalize_cons(fun_graph);                               // finalize the construction
  
  
  //  dump_all_ir_graphs("");
  dump_ir_graph(fun_graph, "whatever");
  
  be_main(stdout, "src name");              // emit code
  ir_finish();                            // clean up
}




