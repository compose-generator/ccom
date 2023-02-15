/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "UnexpectedTokenException.h"

std::string UnexpectedTokenException::getNameFromTokenType(int type) {
  switch (type) {
  case TOK_EOF:
    return "end of file";
  case TOK_IF:
    return "'if'";
  case TOK_HAS:
    return "'has'";
  case TOK_NOT:
    return "'not'";
  case TOK_TRUE:
  case TOK_FALSE:
    return "boolean ('true' / 'false')";
  case TOK_OR:
    return "'|'";
  case TOK_EQUALS:
  case TOK_NOT_EQUALS:
    return "comparison operator ('==' / '!=')";
  case TOK_GREATER:
  case TOK_LESS:
  case TOK_GREATER_EQUAL:
  case TOK_LESS_EQUAL:
    return "comparison operand ('>' / '<' / '>=' / '<=')";
  case TOK_IDENTIFIER:
    return "identifier";
  case TOK_NUMBER:
    return "number";
  case TOK_STRING:
    return "string";
  case TOK_DOT:
    return "'.'";
  case TOK_BRACE_OPEN:
    return "'{'";
  case TOK_BRACE_CLOSE:
    return "'}";
  case TOK_INDEX:
    return "index";
  case TOK_COM_IDEN_PAYLOAD:
    return "payload section";
  case TOK_COM_LINE_IDEN:
    return "comment line identifier";
  case TOK_COM_BLOCK_IDEN_OPEN:
    return "comment block identifier open";
  case TOK_COM_BLOCK_IDEN_CLOSE:
    return "comment block identifier close";
  case TOK_ARBITRARY:
    return "arbitrary section";
  default:
    return "Unknown token";
  }
}