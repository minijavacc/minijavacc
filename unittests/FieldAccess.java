class A {
  public B b;
  public int x;
  
  public static void main(String[] args) {
    B b = new B();
    A a = new A();
    
    b.a = a;
    a.b = b;
    
    b.doo(3);
    a.doo();
  }
  
  public void doo()
  {
    System.out.println(this.x);
  }
}

class B {
  public A a;
  
  public void doo(int x)
  {
    a.x = x;
  }
}
