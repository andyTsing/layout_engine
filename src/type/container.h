#ifndef LE_CONTAINER_H_
#define LE_CONTAINER_H_

#include <list>
#include <algorithm>

namespace le
{

template<typename NodeType>
class Container
{
 public:
  //void InsertBefore(NodeType* new_node, NodeType* ref_node);
  void InsertBefore(NodeType* new_node, typename std::list<NodeType*>::iterator iter);
  void Append(NodeType* node);
  void Remove(NodeType* node);
  
  NodeType* GetFirstNode() const;
  NodeType* GetLastNode() const;
  
 private:
  std::list<NodeType*> list_;
  
};

template<typename NodeType>
void Container<NodeType>::InsertBefore(NodeType* new_node, typename std::list<NodeType*>::iterator iter)
{
  auto it = list_.emplace(iter, new_node);
}

template<typename NodeType>
void Container<NodeType>::Append(NodeType* node)
{
  list_.emplace_back(node);
}


// template<typename NodeType>
// void Container<NodeType>::InsertBefore(NodeType* new_node, NodeType* ref_node)
// {
//   if( ref_node == nullptr ) {
//     list_.emplace_back(new_node);
//   }
//   else {
//     auto it = ref_node->GetIterator();
//     it = list_.emplace(it,new_node);
    
//   }
// }

template<typename NodeType>
void Container<NodeType>::Remove(NodeType* node)
{
  auto it = node->GetIterator();
  list_.remove(it);
}

template<typename NodeType>
NodeType* Container<NodeType>::GetFirstNode() const
{
  if( list_.empty() )
    return nullptr;
  else
    return list_.front();
}

template<typename NodeType>
NodeType* Container<NodeType>::GetLastNode() const
{
  if( list_.empty() )
    return nullptr;
  else
    return list_.back();
}

template<typename ContainerType, typename NodeType>
class Node
{
 public:
  Node() : container_(nullptr) {}
  virtual ~Node() {}
  
  NodeType* GetNext() { return *(++iter_); }
  NodeType* GetPrev() { return *(--iter_); }

  ContainerType* GetContainer() { return container_; }
  void SetContainer(ContainerType* container) { container_ = container; }

  typename std::list<NodeType*>::iterator GetIterator() const { return iter_; }
  void SetIterator(typename std::list<NodeType*>::iterator iter) { iter_ = iter; }
  
 private:
  typename std::list<NodeType*>::iterator iter_;
  ContainerType* container_;
  
};











// template<typename Container, typename Node, typename NodeDelegate>
// class ContainerDelegate {
//  public:
//   void InsertBefore(NodeDelegate* new_delegate, NodeDelegate* ref_delegate);
//  private:
//   std::list<NodeDelegate*> list_;
//   Container* container_;
// };

// template<typename Container, typename Node, typename NodeDelegate>
// void ContainerDelegate::InsertBefore(NodeDelegate* new_delegate, NodeDelegate* ref_delegate)
// {
//   if( ref_delegate == nullptr ) {
//     list_.emplace_back(new_delegate);
//   } else {
//     auto it = list_.emplace(ref_delegate->GetIterator(), new_delegate);
//     new_delegate->SetIterator(it);
//     new_delegate->
//   }

// }

// template<typename Container, typename Node, typename ContainerDelegate>
// class NodeDelegate {
//  public:

  
//   typename std::list<NodeDelegate*>::iterator GetIterator() const { return iter_; }
//   void SetIterator(typename std::list<NodeDelegate*>::iterator iter) { iter_ = iter; }

//   void SetContainerDelegate(ContainerDelegate* container_delegate) { container_delegate_ = container_delegate; }
//   ContainerDelegate* GetContainerDelegate() const { return container_delegate_; }
  
//  private:
//   typename std::list<NodeDelegate*>::iterator iter_;
//   ContainerDelegate* container_delegate_;
//   Node* node_;
// };

}

#endif /* LE_CONTAINER_H_ */
