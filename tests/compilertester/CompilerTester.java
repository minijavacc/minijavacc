import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.List;
import java.util.ArrayList;

public class CompilerTester {

	private static String COMPILER_SCRIPT = "";

	public static void main(String[] args) throws UnsupportedEncodingException {
		System.out.println("Starting minijavacc CompilerTester");
		String path = CompilerTester.class.getProtectionDomain().getCodeSource().getLocation().getPath();
		String decodedPath = URLDecoder.decode(path, "UTF-8");
		String testFilePath = "";
		if (args != null && args.length == 2) {
			COMPILER_SCRIPT = decodedPath + args[0];
			testFilePath = args[1];
		} else {
			System.err
					.println("Wrong parameters. First parameter must be path to compiler executable. Second parameter must be path to testcases folder.");
			return;
		}
		String folder = decodedPath + testFilePath;
		System.out.println("Search for testcases in path: " + folder);
		File file = new File(folder);
		int testnr = 1;
		int success = 0;
		List<String> fails=new ArrayList<String>();
		for (File f : file.listFiles()) {
			if (f.isFile()) {
				if (f.getName().toLowerCase().endsWith(".java")) {
					String t = "[Test#" + testnr + "]";
					System.out.println(t + "-- Read: " + f.getName());
					BufferedReader in;
					try {
						in = new BufferedReader(new FileReader(f));
						String line = in.readLine();
						while (line != null) {
							System.out.println(t + ">--- " + line);
							line = in.readLine();
						}
						in.close();
					} catch (FileNotFoundException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					}
					System.out.println(t + "-- Compile: " + f.getName());
					try {
						if(runCompiler(f))
						{
						success++;
						}
						else
						{
						fails.add(f.getName());
						}
					} catch (IOException e) {
						e.printStackTrace();
						fails.add(f.getName());
					}
					System.out.println("");
					testnr++;
				}
			}
		}
		testnr--;
	System.out.println("=> Tests summary:");
		if(success==testnr)
{
		System.out.println("All tests passed!");
}
else
{
for(String fail: fails)
{
System.out.println("Failed Test: "+fail);
}
		System.out.println("Failed "+(testnr-success)+" from "+testnr+" Tests! ");
}
	}

	private static boolean runCompiler(File f) throws IOException {
		boolean success=false;
		ProcessBuilder ps = new ProcessBuilder(COMPILER_SCRIPT , "--compile-firm" , f.getPath());
		ps.redirectErrorStream(true);
		Process pr = ps.start();
		BufferedReader in = new BufferedReader(new InputStreamReader(pr.getInputStream()));
		String line;
		while ((line = in.readLine()) != null) {
			System.out.println(line);
			if(line.toLowerCase().startsWith("created binary")){
				success=true;
			}
		}
		in.close();
		return success;
	}
}
