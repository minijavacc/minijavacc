class A {
  public static void main(String[] args)
  {
    A a = new A();
    
    System.out.println(a.div(10,3));
    System.out.println(a.mod(10,3));
    System.out.println(a.mul(10,3));
  }

  public int div(int a, int b)
  {
    return a / b;
  }
  
  public int mod(int a, int b)
  {
    return a % b;
  }
  
  public int mul(int a, int b)
  {
    return a * b;
  }
}
