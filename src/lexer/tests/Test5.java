
class Test5 {

	public static void main(String[] args) {
		Test5 t5=new Test5();
		t5.voids(t5.booleans(t5.ints(12)));
		t5.test5(t5.test55());
	}
	
	public void voids(boolean b)
	{
		
	}
	
	public boolean booleans(int b)
	{
		return b>0;
	}
	
	public int ints(int s)
	{
		return s>>s/2;
	}
	
	public Test5 test5(Test5 test)
	{
		test.voids(5==6);
		return new Test5();
	}
	
	public Test5 test55()
	{
		return this;
	}
}
