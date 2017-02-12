class A {
  public int a;
  
  public static void main(String[] args) {
    int x = new A().five();
    
    System.out.println(x);
  }
  
  
  public int five() {
    return 5;
  }
}
