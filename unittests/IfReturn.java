class IfReturn {
  public static void main(String[] args) {
    IfReturn i = new IfReturn();
    System.out.println(i.foo1(3));
    System.out.println(i.foo2(3));
    System.out.println(i.foo3(3));
    System.out.println(i.foo4(3));
    System.out.println(i.foo1(13));
    System.out.println(i.foo2(13));
    System.out.println(i.foo3(13));
    System.out.println(i.foo4(13));
  }
  
  public int foo1(int a) {
    if (a == 3) {
      return 42;
    }
    
    return 23;
  }
  
  public int foo2(int a) {
    if (a == 3) {
      a = 5;
    }
    
    return a;
  }
  
  public int foo3(int a) {
    if (a == 3) {
      return 42;
    } else {
      return 23;
    }
  }
  
  public int foo4(int a) {
    if (a == 3) {
      return 42;
    } else {
      return 23;
    }
    
    return -1;
  }
}