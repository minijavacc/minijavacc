class A {
  public static void main(String[] args) {
    A a = new A();
    
    
    int i = 3;
    int j;
    
    while (i > 0)
    {
      j = 3;
      
      while (j > 0)
      {
        System.out.println(i);
        System.out.println(j);
        
        j = j - 1;
      }
      
      i = i - 1;
    }
  }
}
