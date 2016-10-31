#pragma once

#include <memory>
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
      UnaryNode(std::shared_ptr<Node> node) : node(node) {};
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
    
    protected:
      std::shared_ptr<Node> node;
  };

  class BinaryNode : public Node
  {
    public:
      BinaryNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : left(left), right(right) {};
      virtual std::string getStringValue() = 0; // must be implemented by subclasses
    
    protected:
      std::shared_ptr<Node> left;
      std::shared_ptr<Node> right;
  };

  // actual nodes
  // TODO: implement nodes, only examples so far
  class Addition : public BinaryNode
  {
    public:
      using BinaryNode::BinaryNode; // use constructor of base class
      std::string getStringValue();
  };

}
