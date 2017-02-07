class Simple {
  public static void main(String[] args) {
    Simple s = new Simple();
    s.attr = new Attr();
    s.attr.anInt = 5;
    
    s.foo();
    
    System.out.println(s.attr.anInt);
  }
  
  public Attr attr;
  
  public void foo() {
    this.attr.anInt = 23;
  }

}


class Attr {
  public int anInt;
}
