package com.c;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class TestFileReader {

    private String folderPath = "./src/test/java/com/c/";

    public TestFileReader(String testPackageName) {
        this.folderPath = this.folderPath + testPackageName + "/";
    }

    /**
     * Reads in file as a string.
     *
     * @param filename name of the file in the testfiles folder
     * @return file content as string
     * @throws IOException if the file does not exist in the testfiles folder
     */
    public String fileToString(String filename) throws IOException {
        Path filePath = Path.of(folderPath + filename);
        return Files.readString(filePath);
    }

}
