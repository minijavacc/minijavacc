#pragma once

#include <memory>
#include <vector>
#include <string>

namespace cmpl
{
  
  // abstract classes for categorization
  class Node
  {
    public:
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
  };
  
  class Leaf : public Node
  {
    public:
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
  };
  
  class UnaryNode : public Node
  {
    public:
      UnaryNode(std::unique_ptr<Node> node) : node(std::move(node)) {};
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
    
    protected:
      std::unique_ptr<Node> node;
  };

  class BinaryNode : public Node
  {
    public:
      BinaryNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right) : left(std::move(left)), right(std::move(right)) {};
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
    
    protected:
      std::unique_ptr<Node> left;
      std::unique_ptr<Node> right;
  };
  
  // n-ary Node
  class NaryNode : public Node
  {
    public:
      NaryNode(std::vector<std::unique_ptr<Node>> nodes) {
        this->nodes =  std::move(nodes);
      };
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
    
    protected:
      std::vector<std::unique_ptr<Node>> nodes;
  };

  /************* actual nodes *************/
  class ProgramNode : public NaryNode
  {
    public:
      using NaryNode::NaryNode; // use constructor of base class
      std::string getStringValue();
  };

  class ClassNode : public NaryNode
  {
    public:
      using NaryNode::NaryNode; // use constructor of base class
      std::string getStringValue() {
        return "";
      };
  };

  // actual nodes
  class ClassMemberNode : public NaryNode
  {
    public:
      using NaryNode::NaryNode; // use constructor of base class
      std::string getStringValue() {
        return "";
      };
  };

  // actual nodes
  class TypeNode : public NaryNode
  {
    public:
      using NaryNode::NaryNode; // use constructor of base class
      std::string getStringValue() {
        return "";
      };
  };
  // actual nodes
  class BasicTypeNode : public NaryNode
  {
    public:
      using NaryNode::NaryNode; // use constructor of base class
      std::string getStringValue() {
        return "";
      };
  };

}
