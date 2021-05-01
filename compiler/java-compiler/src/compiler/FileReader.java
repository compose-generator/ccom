package compiler;

import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;

public class FileReader implements FileReaderIntf {
	private InputStream m_inputStream;
	private Reader m_inputStreamReader;
	private char[] m_nextChars = new char[4];
	private boolean isEndOfStream = false;

	public FileReader(InputStream inputStream) throws Exception {
		m_inputStream = inputStream;
		m_inputStreamReader = new InputStreamReader(m_inputStream);
		advance();
		advance();
		advance();
		advance();
	}

	public String lookAheadChar4() {
		return lookAheadChar3() + String.valueOf(m_nextChars[3]);
	}

	public String lookAheadChar3() {
		String str1 = String.valueOf(m_nextChars[0]);
		String str2 = String.valueOf(m_nextChars[1]);
		String str3 = String.valueOf(m_nextChars[2]);
		return str1 + str2 + str3;
	}

	public char lookAheadChar() {
		return m_nextChars[0];
	}

	public void advance() throws Exception {
		// III II_
		// IIII I__
		// IIIII ___

		int nextChar = -1;
		if (!isEndOfStream) {
			nextChar = m_inputStreamReader.read();
			if (nextChar == -1)
				isEndOfStream = true;
		}
		m_nextChars[0] = m_nextChars[1];
		m_nextChars[1] = m_nextChars[2];
		m_nextChars[2] = m_nextChars[3];
		m_nextChars[3] = (nextChar == -1) ? 0 : (char) nextChar;
	}

	public char expect(char c) throws Exception {
		if (m_nextChars[0] != c) {
			String msg = new String("unexpected character: '");
			msg += m_nextChars[0];
			msg += "'";
			msg += " expected: '";
			msg += c;
			msg += "'";
			throw new Exception(msg);
		}
		advance();
		return c;
	}

	public static FileReaderIntf fromFileName(String fileName) throws Exception {
		FileInputStream inputStream = new FileInputStream(fileName);
		compiler.FileReaderIntf fileReader = new compiler.FileReader(inputStream);
		return fileReader;
	}
}
