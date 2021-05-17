package compiler;

public class NumberReader implements NumberReaderIntf {
	private FileReaderIntf m_reader;

	public NumberReader(FileReaderIntf reader) {
		m_reader = reader;
	}
	
	public int getNumber() throws Exception {
		int number = 0;
		char nextChar = m_reader.lookAheadChar();
		if (!isDigit(nextChar)) {
			throw new Exception("not a number");
		}
		do {
			m_reader.advance();
			number *= 10;
			number += (int)(nextChar - '0');
			nextChar = m_reader.lookAheadChar();
		} while (isDigit(nextChar));
		return number;
	}
	
	public boolean isDigit(char c) {
		return ('0' <= c && c <= '9');
	}
}