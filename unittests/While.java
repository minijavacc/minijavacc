class While {
  public static void main(String[] args) {
    While w = new While();
    System.out.println(w.foo3(0, 1));
  }
  
  public int foo1(int a) {
    while (a <= 3) {
      a = a + 1;
    }
    
    return a;
  }
  
  public int foo2(int a) {
    while (a <= 3) {
      a = a + 1;
    }

    while (a <= 11) {
      a = a + 1;
    }
    
    return a;
  }
  
  
  public int foo3(int a, int b) {
    MPI10 x = new MPI10();
    x.init(1337);
    
    MPI10 y = new MPI10();
    y.init(399996);
    
    x.plusEq(y);
    System.out.println(8888888);
    x.plusEq(y);
    System.out.println(8888888);
    x.plusEq(y);
    System.out.println(8888888);
    x.plusEq(y);
    
    x.print();
    y.print();
    
    return 0;
  }
}






class MPI10 {

	public void init(int value) {
		_M_size = 1;
		_M_capacity = 1;
		_M_data = new int[_M_size];
		_M_data[0] = value;
	}

	public void plusEq(MPI10 other) {
		int ndigits = 1;
		if (other._M_size > _M_size) {
			ndigits = ndigits + other._M_size;
		} else {
			ndigits = ndigits + _M_size;
		}
		_M_grow(ndigits);
		other._M_grow(ndigits);
		int i = 0;
		int carry = 0;
		int sum;
		while (i < ndigits) {
			sum = _M_data[i] + other._M_data[i] + carry;
      System.out.println(_M_data[i]);
			carry = sum / 10;
			_M_data[i] = sum % 10;
			i = i + 1;
		}
	}

	public void print() {
		int i = _M_size - 1;
		while ((i > 0) && (_M_data[i] == 0)) {
			i = i - 1;

		}
	}

	public int[] _M_data;
	public int _M_size;
	public int _M_capacity;

	public void _M_grow(int size) {
		if (size <= _M_capacity) {
			_M_size = size;
			return;
		}
		int capacity = 2 * _M_capacity;
		int[] temp = new int[capacity];
		int i = 0;
		while (i < _M_size) {
			temp[i] = _M_data[i];
			i = i + 1;
		}
		while (i < size) {
			temp[i] = 0;
			i = i + 1;
		}
		_M_data = temp;
		_M_size = size;
		_M_capacity = capacity;
	}

}
