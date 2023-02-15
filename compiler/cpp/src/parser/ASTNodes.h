/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

enum Operator {
  OP_EQUALS,
  OP_NOT_EQUALS,
  OP_GREATER,
  OP_LESS,
  OP_GREATER_EQUAL,
  OP_LESS_EQUAL,
};

class ASTRootNode {
public:
  // Enums
  enum TopLevelExprType { TOP_LEVEL_EXPR, CONTENT_EXPR, STMT_LST_EXPR };

  // Constructors
  ASTRootNode() : type(TOP_LEVEL_EXPR) {}

  // Public methods
  std::string serialize();

  // Members
  TopLevelExprType type;

protected:
  // Protected constructors
  explicit ASTRootNode(TopLevelExprType t) : type(t) {}
};

class ASTIdentifierExprNode {
public:
  // Constructors
  explicit ASTIdentifierExprNode(std::string Name) : name(std::move(Name)) {}
  ASTIdentifierExprNode(std::string name, int index) : name(std::move(name)), index(index) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::string name;
  int index = -1;
};

class ASTValueExprNode {
public:
  // Enums
  enum ValueExprType { VALUE_EXPR, NUMBER_EXPR, BOOLEAN_EXPR, STRING_EXPR };

  // Constructors
  ASTValueExprNode() : type(VALUE_EXPR) {}

  // Public methods
  std::string serialize();

  // Members
  ValueExprType type;

protected:
  // Protected constructors
  explicit ASTValueExprNode(ValueExprType t) : type(t) {}
};

class ASTKeyExprNode {
public:
  // Constructors
  explicit ASTKeyExprNode(std::vector<std::unique_ptr<ASTIdentifierExprNode>> stmts) : identifiers(std::move(stmts)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::vector<std::unique_ptr<ASTIdentifierExprNode>> identifiers;
};

class ASTStmtNode {
public:
  // Enums
  enum StmtExprType { STMT_EXPR, HAS_STMT_EXPR, COMP_STMT_EXPR, CONTAINS_STMT_EXPR };

  // Constructors
  ASTStmtNode() : type(STMT_EXPR) {}

  // Public methods
  std::string serialize();

  // Members
  StmtExprType type;

protected:
  // Protected constructors
  explicit ASTStmtNode(StmtExprType t) : type(t) {}
};

class ASTStmtListNode : public ASTRootNode {
public:
  // Constructors
  explicit ASTStmtListNode(std::vector<std::unique_ptr<ASTStmtNode>> stmts)
      : ASTRootNode(TopLevelExprType::STMT_LST_EXPR), stmts(std::move(stmts)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::vector<std::unique_ptr<ASTStmtNode>> stmts;
};

class ASTContentBlockExprNode {
public:
  // Enums
  enum ContentBlockExprType { CONTENT_BLOCK_EXPR, SECTION_EXPR, ARBITRARY_EXPR };

  // Constructors
  explicit ASTContentBlockExprNode() : type(CONTENT_BLOCK_EXPR) {}

  // Public methods
  std::string serialize();

  // Members
  ContentBlockExprType type;

protected:
  // Protected constructors
  explicit ASTContentBlockExprNode(ContentBlockExprType t) : type(t) {}
};

class ASTContentExprNode : public ASTRootNode {
public:
  // Constructors
  explicit ASTContentExprNode(std::vector<std::unique_ptr<ASTContentBlockExprNode>> sections)
      : ASTRootNode(TopLevelExprType::CONTENT_EXPR), contentBlocks(std::move(sections)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::vector<std::unique_ptr<ASTContentBlockExprNode>> contentBlocks;
};

class ASTArbitraryExprNode : public ASTContentBlockExprNode {
public:
  // Constructors
  explicit ASTArbitraryExprNode(std::string value)
      : ASTContentBlockExprNode(ContentBlockExprType::ARBITRARY_EXPR), value(std::move(value)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::string value;
};

class ASTPayloadExprNode {
public:
  // Constructors
  explicit ASTPayloadExprNode(std::string val) : value(std::move(val)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::string value;
};

class ASTIfBlockExprNode {
public:
  // Constructors
  ASTIfBlockExprNode(std::unique_ptr<ASTStmtListNode> stmtList, std::unique_ptr<ASTPayloadExprNode> payload)
      : stmtList(std::move(stmtList)), payload(std::move(payload)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::unique_ptr<ASTStmtListNode> stmtList;
  std::unique_ptr<ASTPayloadExprNode> payload;
};

class ASTComBlockExprNode {
public:
  // Enums
  enum ComBlockExprType { COM_BLOCK_EXPR, COM_LINE_BLOCK_EXPR, COM_BLOCK_BLOCK_EXPR };

  // Constructors
  ASTComBlockExprNode() : type(COM_BLOCK_EXPR) {}

  // Public methods
  std::string serialize();

  // Members
  ComBlockExprType type;

protected:
  // Protected constructors
  explicit ASTComBlockExprNode(ComBlockExprType t) : type(t) {}
};

class ASTComLineBlockExprNode : public ASTComBlockExprNode {
public:
  // Constructors
  explicit ASTComLineBlockExprNode(std::unique_ptr<ASTIfBlockExprNode> ifBlock)
      : ASTComBlockExprNode(ComBlockExprType::COM_LINE_BLOCK_EXPR), ifBlock(std::move(ifBlock)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::unique_ptr<ASTIfBlockExprNode> ifBlock;
};

class ASTComBlockBlockExprNode : public ASTComBlockExprNode {
public:
  // Constructors
  explicit ASTComBlockBlockExprNode(std::unique_ptr<ASTIfBlockExprNode> ifBlock)
      : ASTComBlockExprNode(ComBlockExprType::COM_BLOCK_BLOCK_EXPR), ifBlock(std::move(ifBlock)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::unique_ptr<ASTIfBlockExprNode> ifBlock;
};

class ASTCompStmtNode : public ASTStmtNode {
public:
  // Constructors
  ASTCompStmtNode(std::unique_ptr<ASTKeyExprNode> key, Operator op, std::unique_ptr<ASTValueExprNode> value)
      : ASTStmtNode(StmtExprType::COMP_STMT_EXPR), key(std::move(key)), op(op), value(std::move(value)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  Operator op;
  std::unique_ptr<ASTKeyExprNode> key;
  std::unique_ptr<ASTValueExprNode> value;
};

class ASTContainsStmtNode : public ASTStmtNode {
public:
  // Constructors
  ASTContainsStmtNode(std::unique_ptr<ASTKeyExprNode> listKey, std::unique_ptr<ASTKeyExprNode> valueKey,
                      std::unique_ptr<ASTValueExprNode> value, Operator op, bool inverted)
      : ASTStmtNode(StmtExprType::CONTAINS_STMT_EXPR), listKey(std::move(listKey)), valueKey(std::move(valueKey)),
        value(std::move(value)), op(op), isInverted(inverted) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  Operator op;
  std::unique_ptr<ASTKeyExprNode> listKey;
  std::unique_ptr<ASTKeyExprNode> valueKey;
  std::unique_ptr<ASTValueExprNode> value;
  bool isInverted;
};

class ASTHasStmtNode : public ASTStmtNode {
public:
  // Constructors
  ASTHasStmtNode(std::unique_ptr<ASTKeyExprNode> key, bool inverted)
      : ASTStmtNode(StmtExprType::HAS_STMT_EXPR), key(std::move(key)), isInverted(inverted) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::unique_ptr<ASTKeyExprNode> key;
  bool isInverted;
};

class ASTNumberExprNode : public ASTValueExprNode {
public:
  // Constructors
  explicit ASTNumberExprNode(int val) : ASTValueExprNode(ValueExprType::NUMBER_EXPR), value(val) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  int value;
};

class ASTSectionExprNode : public ASTContentBlockExprNode {
public:
  // Constructors
  explicit ASTSectionExprNode(std::vector<std::unique_ptr<ASTComBlockExprNode>> comBlocks)
      : ASTContentBlockExprNode(ContentBlockExprType::SECTION_EXPR), comBlocks(std::move(comBlocks)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::vector<std::unique_ptr<ASTComBlockExprNode>> comBlocks;
};

class ASTStringExprNode : public ASTValueExprNode {
public:
  // Constructors
  explicit ASTStringExprNode(std::string val) : ASTValueExprNode(ValueExprType::STRING_EXPR), value(std::move(val)) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  std::string value;
};

class ASTBooleanExprNode : public ASTValueExprNode {
public:
  // Constructors
  explicit ASTBooleanExprNode(bool val) : ASTValueExprNode(ValueExprType::BOOLEAN_EXPR), value(val) {}

  // Public methods
  [[nodiscard]] std::string serialize() const;

  // Members
  bool value;
};