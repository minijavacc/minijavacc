class A {
  public int a;
  
  public static void main(String[] args)
  {
    A a = new A();
    A b = new A();
    A c = new A();
    
    a.a = 1;
    b.a = 2;
    c.a = 3;
    
    System.out.println(a.a);
    System.out.println(b.a);
    System.out.println(c.a);
    
    a.increment();
    b.increment();
    c.increment();
    
    System.out.println(a.a);
    System.out.println(b.a);
    System.out.println(c.a);
  }

  public void increment()
  {
    a = a + 1;
  }
}
