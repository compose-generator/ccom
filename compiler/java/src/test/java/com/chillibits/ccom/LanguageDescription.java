package com.chillibits.ccom;

public class LanguageDescription {
    private final String fileExtension;
    private final String commentLineIdentifier;
    private final String commentBlockOpenIdentifier;
    private final String commentBlockCloseIdentifier;

    public LanguageDescription(String languageName, String commentLineIdentifier, String commentBlockOpenIdentifier, String commentBlockCloseIdentifier) {
        this.fileExtension = languageName;
        this.commentLineIdentifier = commentLineIdentifier;
        this.commentBlockOpenIdentifier = commentBlockOpenIdentifier;
        this.commentBlockCloseIdentifier = commentBlockCloseIdentifier;
    }

    public String getFileExtension() {
        return fileExtension;
    }

    public String getCommentLineIdentifier() {
        return commentLineIdentifier;
    }

    public String getCommentBlockOpenIdentifier() {
        return commentBlockOpenIdentifier;
    }

    public String getCommentBlockCloseIdentifier() {
        return commentBlockCloseIdentifier;
    }

}
