class Calls
{
  public int a;
  
  public static void main(String[] args)
  {
    Calls c = new Calls();
    
    c.a = c.f(0, 1, 2, 3, 4, 5, 6, 7, c);
    
    System.out.println(c.a);
  }
  
  /*
  - more the five parameters (this + 5 = 6 parameters come in registers, others on stack)
  - different number of parameters on stack (ensure 16-byte alignment)
  - add (odd) number of local variables (ensure 16-byte alignment)
  */
  
  public int f(int a, int b, int c, int d, int e, int f, int g, int h, Calls call)
  {
    return call.g(a, b, c, d, e, f, g, h);
  }
  
  public int g(int a, int b, int c, int d, int e, int f, int g, int h)
  {
    return h(a, b, c, d, e, f, g, h);
  }
  
  public int h(int a, int b, int c, int d, int e, int f, int g, int h)
  {
    a = a + 1;
    
    voidFunction();
    
    return a + b + c + d + e + f + g + h;
  }
  
  public void voidFunction()
  {
    
  }
}
