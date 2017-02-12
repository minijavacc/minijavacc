class A {
  public static void main(String[] args)
  {
    int c = System.in.read();
    
    if (c == -1)
      System.out.println(42);
    else
      System.out.write(c);
    System.out.flush();
  }
}
