class Calls
{
  public int a;
  
  public static void main(String[] args)
  {
    Calls c = new Calls();
    
    c.a = c.intFunction(3);
  }
  
  public int intFunction(int a)
  {
    voidFunction();
    
    return a;
  }
  
  public void voidFunction()
  {
    
  }
}
