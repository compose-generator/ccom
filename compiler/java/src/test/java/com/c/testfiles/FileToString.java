package com.c.testfiles;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class FileToString {

    private static final String TEST_FOLDER_PATH = "./src/test/java/com/c/testfiles/";

    /**
     * Reads in file as a string.
     *
     * @param filename name of the file in the testfiles folder
     * @return file content as string
     * @throws IOException if the file does not exist in the testfiles folder
     */
    public static String fileToString(String filename) throws IOException {
        Path filePath = Path.of(TEST_FOLDER_PATH + filename);
        return Files.readString(filePath);
    }

}
