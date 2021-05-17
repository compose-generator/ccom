import java.io.FileInputStream;

public class CComManualMain {

	public static void main(String[] args) throws Exception {
		System.out.println("BEGIN");
		String fileName = args[0];
		FileInputStream inputStream = new FileInputStream(fileName);
		compiler.FileReaderIntf fileReader = new compiler.FileReader(inputStream);
		compiler.CComIntf ccom = new compiler.CCom(fileReader);
		String content = ccom.getStart();
		System.out.println(content);
		System.out.println("END");
	}

}
