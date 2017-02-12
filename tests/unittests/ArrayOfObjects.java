class A {
  public int a;
  
  public static void main(String[] args)
  {
    int n = 10;
    int i = 0;
    A[] obj = new A[n];
    
    while (i < n)
    {
      obj[i] = new A();
      
      A tmp = obj[i];
      tmp.a = 10 - i;
      
      i = i + 1;
    }
    
    i = 0;
    while (i < n)
    {
      obj[i].doo();
      i = i + 1;
    }
  }

  public void doo()
  {
    System.out.println(a);
  }
}
