public class CComTest implements test.TestCaseIntf {

	public String executeTest(compiler.FileReaderIntf fileReader) throws Exception {
		compiler.CComIntf cComTester = new compiler.CCom(fileReader);
		return cComTester.getStart() + "\r\n";
	}

}
