/**
 * 
 * @author Roland
 *
 */
class Test2 {

	public static void main(String[] args) {
		Test2 test2=new Test2();
		test2.integer();
	}
	
	public int integer(){
		return 5;
	}
	
	public void integer2(){
		integer();
		int int3=integer();	
	}
}
