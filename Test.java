package wordsTool;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
public class Test {
	static List<String> li=new ArrayList<String>();;
	int c = 0;
	static void randomarry() throws IOException
	{
		int num_file = 0;
		File file = new File("D:/test.txt");
		BufferedReader br = new BufferedReader(new FileReader(file));
		while (br.readLine() != null) {
			num_file++;
		}
		br.close();
		int[] a= new int[num_file];
	  for (int i = 0; i < num_file; i++)
	  {
	    a[i] = 1234;
	  }
	  int index = 0;
	  while (a[num_file-1] == 1234)
	  {
	    int i = (int) (Math.random()*num_file) ;
	    for (int j = 0; j <= index; j++)
	    {
	      if (i == a[j])
	      {
	        break;
	      }
	    }
	    a[index] = i;
	    index++;
	  }
	  String str;
	  int t=0;
	  File file2 = new File("D:/test.txt");
	  while(li.size()<num_file) {
		BufferedReader br2 = new BufferedReader(new FileReader(file2));
		 int c=0;
		  while ((str=br2.readLine())!= null) {
			  c=c+1;
			  if(a[t]==c) {
				  t++;
				  System.out.println("第"+t+"个："+str);
				  li.add(str);
				  break;
			  }
		}
		  br2.close();
	  }
	  String string ="";
	for(String s:li) {
		string = string+s+"\r\n";
//		System.out.println(s);
	}
	  writeTxt("D:/book.txt", string);
	}
	public static void writeTxt(String txtPath, String content) {
		FileOutputStream fileOutputStream = null;
		File file = new File(txtPath);
		try {
			if (file.exists()) {
				file.createNewFile();
			}
			fileOutputStream = new FileOutputStream(file);
			fileOutputStream.write(content.getBytes());
			fileOutputStream.flush();
			fileOutputStream.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) throws IOException {
		File file = new File("D:/src.txt");
		BufferedReader br = new BufferedReader(new FileReader(file));
		String str = "", s = "", st = "";
		while ((s = br.readLine()) != null) {
			for (int i = 0; i < s.length() - 2; i++) {
				if (s.charAt(i) == ' '
						&& !(s.charAt(i + 2) <= 122 && s.charAt(i + 2) >= 97 || s.charAt(i + 2) == '.')) {
					str += "";
					st += "";
				} else if (i != 0 && s.charAt(i) == ' ' && s.charAt(i - 1) != ' ' && (s.charAt(i + 1) == 'n'
						|| s.charAt(i + 1) == 'a' || s.charAt(i + 1) == 'v' || s.charAt(i + 1) == 'p')) {
					str += " ";
					st = st + " ";
					str += s.charAt(i);
					st += s.charAt(i);
				} else {
					str += s.charAt(i);
					st += s.charAt(i);
				}
			}
			str += s.charAt(s.length() - 2);
			str += s.charAt(s.length() - 1);
			st += s.charAt(s.length() - 2);
			st += s.charAt(s.length() - 1);
			str += "\r\n";
			st += "\r\n";
			st = "";
		}
		br.close();
		writeTxt("D:/test.txt", str);
		randomarry();
		System.out.println("Work done!");
	}
}
