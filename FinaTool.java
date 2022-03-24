package wordsTool;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.concurrent.TimeUnit;

public class FinalTool {
	public static String web_add(String word) throws IOException {
		URL url = new URL("https://dict.youdao.com/w/"+word+"/#keyfrom=dict2.top");
	      BufferedReader reader = new BufferedReader(new InputStreamReader(url.openConnection().getInputStream(),"UTF-8"));//.openStream()
	      String line,symbol="";
	      while ((line = reader.readLine()) != null) {
	    	  if(line.contains("[")) {
	    		  symbol=line.substring(line.indexOf("["),line.indexOf("]")+1);
	    		  break;
	    	  }
	      }
	      reader.close();
		return symbol;
	}
	public static String format(String s) {
		String s2="";
		s2=s.substring(0,s.indexOf("]")+1);
		for(int i=s.indexOf("]")+1;i<s.length();i++) {
			s2+=s.charAt(i);
			if((i<s.length()-3&&i>0&&((s.charAt(i+1)=='n'&&s.charAt(i+2)=='.')&&s.charAt(i)!=' '||(s.charAt(i+1)=='v'&&(s.charAt(i+3)=='.'||s.charAt(i+2)=='.')&&s.charAt(i)!='d')||((s.charAt(i+1)=='a')&&s.charAt(i+4)=='.'))&&s.charAt(i-1)!=' '&&s.charAt(i)!=' ')){
				s2+="  ";
			}
		}
		System.out.println(s2);
		return s2;
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

	public static void main(String[] args) throws Exception {
		File file = new File("D:/text.csv");
		BufferedReader br = new BufferedReader(new FileReader(file));
		String str = "", s = "", st = "";
		int f = 0;
		while ((s = br.readLine()) != null) {
			if(s.contains("\"")) {
				f++;
			}
			str+=s;
			if(f%2==0) {
				str=str.replaceFirst(",", web_add(str.substring(0,str.indexOf(","))));
				str = str.replace("\"", "" );
				str= format(str).replaceAll("；", ";");
				str+="\n";
				st+=str;
				System.out.println(st);
				TimeUnit.SECONDS.sleep((int)Math.random()*3);
				str="";
			}
		}
		br.close();
		writeTxt("D:/book.txt",st);
	}

}
