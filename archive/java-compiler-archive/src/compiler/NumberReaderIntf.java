package compiler;

public interface NumberReaderIntf {

	// create a NumberReader for the given fileReader
	// NumberReader(FileReaderIntf fileReader)

	// reads number from fileReader
	int getNumber() throws Exception;

	// is given character a digit 0..9
	boolean isDigit(char c);

}