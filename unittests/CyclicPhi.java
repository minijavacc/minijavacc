class A {
  public static void main(String[] args) {
    A a = new A();
    
    
    int i = 3;
    int j = 0;
    
    while (i > 0)
    {
      int tmp = i;
      i = j;
      j = tmp;
    }
    
    System.out.println(j);
    System.out.println(i);
  }
}
