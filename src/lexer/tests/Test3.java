//TEST
class Test3 {

	public static void main(String[] args) {
		int i=0;
		if(true==false)
		{
			if(true)
			{
				if(false)
				{
					i=1;
				}
			}
		}
		else if(false==true)
		{
			i=2;
		}
		else
		{
			if(i>=0)
			{
				i=i+3;
			}
			else
			{
				//nothing
			}
		}
	}
	
}
