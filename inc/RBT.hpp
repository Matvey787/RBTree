#ifndef RBT_HPP
#define RBT_HPP

#include <gtest/gtest.h>
#include <functional>
#include <fstream>
#include <stack>

namespace RBT {

enum class color_t { RED, BLACK };

template <typename KeyT>
struct Node final
{
private:
    KeyT key_;
    color_t color_;
    Node *parent_;
    Node *left_;
    Node *right_;

public:
    Node(KeyT key,
         color_t color = color_t::RED,
         Node *parent = nullptr,
         Node *left = nullptr,
         Node *right = nullptr) : 
         key_(key),
         color_(color),
         parent_(parent),
         left_(left),
         right_(right) {}

    ~Node() = default;

    const KeyT& key() const { return key_; }
    color_t color() const { return color_; }
    template <typename, typename>
    friend class RBTree;
    
    // only for GTests
    const Node* left() const { return left_; }
    const Node* right() const { return right_; }
    const Node* parent() const { return parent_; }
};


template <typename KeyT, typename Comp = std::less<KeyT>>
class RBTree {
    Node<KeyT> *top_ = nullptr;

    void balance    (Node<KeyT> *node);
    void gdumpNode(std::ostream &os, const Node<KeyT> *node) const;
    void LLRot      (Node<KeyT>& node);
    void RRRot      (Node<KeyT>& node);
    void BSTErase(const KeyT& key);
    Node<KeyT>* successor(Node<KeyT>* node) const;
    Node<KeyT>* findMinInSubtree(Node<KeyT> *subRoot) const;

public:
    
    using iterator = Node<KeyT>*;
    iterator lower_bound(const KeyT& key) const;
    iterator upper_bound(const KeyT& key) const;

    void insert (const KeyT& key);
    void erase  (const KeyT& key);
    bool exists (const KeyT& key) const;
    void clear  ();
    void gdump(std::ostream &os) const;

    template <typename C>
    friend int mydistance(const C& tree, typename C::iterator fst, typename C::iterator snd);

    ~RBTree()
    {
        clear();
    }
};


// --------------------------------------- balance ---------------------------------------


template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::LLRot(Node<KeyT>& node)
{
    Node<KeyT> *p = node.parent_;
    if (!p) throw std::runtime_error("LLRot: node has no parent");
    Node<KeyT> *g = p->parent_;
    if (!g) throw std::runtime_error("LLRot: parent has no grandparent");

    if (g->parent_) {
        if (g == g->parent_->left_)
            g->parent_->left_ = p;
        else
            g->parent_->right_ = p;
    } else {
        top_ = p;
    }

    p->parent_ = g->parent_;

    g->left_ = p->right_;
    if (p->right_) p->right_->parent_ = g;

    p->right_ = g;
    g->parent_ = p;

    std::swap(p->color_, g->color_);
}


template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::RRRot(Node<KeyT>& node)
{
    Node<KeyT> *p = node.parent_;
    if (!p) throw std::runtime_error("RRRot: node has no parent");
    Node<KeyT> *g = p->parent_;
    if (!g) throw std::runtime_error("RRRot: parent has no grandparent");

    if (g->parent_) {
        if (g == g->parent_->left_)
            g->parent_->left_ = p;
        else
            g->parent_->right_ = p;
    } else {
        top_ = p;
    }

    p->parent_ = g->parent_;

    g->right_ = p->left_;
    if (p->left_) p->left_->parent_ = g;

    p->left_ = g;
    g->parent_ = p;

    std::swap(p->color_, g->color_);
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::balance(Node<KeyT> *node)
{
    if (!node) throw std::runtime_error("balance: node is nullptr");
    Node<KeyT> *p = node->parent_;
    if (!p) return;
    if (p->color_ == color_t::BLACK) return;

    Node<KeyT> *g = p->parent_;

    Node<KeyT> *u = nullptr;

    if (g->left_ != p)
        u = g->left_;
    else
        u = g->right_;

    if (u && u->color_ == color_t::RED)
    {
        p->color_ = color_t::BLACK;
        u->color_ = color_t::BLACK;

        if (g->parent_)
            g->color_ = color_t::RED;
        else
            g->color_ = color_t::BLACK;

        balance(g);
    }

    else if (!u || u->color_ == color_t::BLACK)
    {
        if ((p == g->left_ && node == p->left_)) // father left, son left
        {
            LLRot(*node);
        }
        else if (p == g->right_ && node == p->right_) // father right, son right
        {
            
            RRRot(*node);
        }
        else if (p == g->left_ && node == p->right_) // father left, son right
        {
            node->parent_ = g;
            g->left_ = node;

            p->right_ = node->left_;
            if (node->left_) node->left_->parent_ = p;

            p->parent_ = node;
            node->left_ = p;

            LLRot(*p);
        }
        else if (p == g->right_ && node == p->left_) // father right, son left
        {
            node->parent_ = g;
            g->right_ = node;

            p->left_ = node->right_;
            if (node->right_) node->right_->parent_ = p;

            p->parent_ = node;
            node->right_ = p;

            RRRot(*p);
        }
    }
}



// --------------------------------------- insert ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::insert(const KeyT& key)
{
    if (!top_)
    {
        top_ = new Node<KeyT>{key, color_t::BLACK};
        return;
    }

    if (exists(key)) return;

    Node<KeyT> *current = top_;
    Node<KeyT> *parent = nullptr;

    while (current)
    {
        parent = current;

        if (Comp{}(key, current->key_))
            current = current->left_;
        else
            current = current->right_;
    }

    if (Comp{}(key, parent->key_))
    {
        parent->left_ = new Node<KeyT>{key, color_t::RED, parent};
        balance(parent->left_);
    }
    else
    {
        parent->right_ = new Node<KeyT>{key, color_t::RED, parent};
        balance(parent->right_);
    }
}



// --------------------------------------- erase ---------------------------------------

template <typename KeyT, typename Comp>
Node<KeyT>* RBTree<KeyT, Comp>::findMinInSubtree(Node<KeyT> *subRoot) const
{
    if (!subRoot) return nullptr;

    Node<KeyT> *minNode = subRoot;

    while (minNode && minNode->left_) minNode = minNode->left_;

    // check that this is not the root of the subtree,
    // otherwise it means that there are no left descendants in this subtree
    if (minNode == subRoot) return nullptr;
    return minNode;
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::BSTErase(const KeyT& key)
{
    auto replace = [this](Node<KeyT>* oldNode, Node<KeyT>* newNode) {
        if (!oldNode) return;

        if (!newNode)
        {
            if (!oldNode->parent_)
            {
                top_ = nullptr;
            }
            else
            {
                if (oldNode->parent_->left_ == oldNode)
                    oldNode->parent_->left_ = nullptr;
                else
                    oldNode->parent_->right_ = nullptr;
            }

            oldNode->left_ = nullptr;
            oldNode->right_ = nullptr;
            delete oldNode;
            return;
        }

        if (!newNode->left_ && oldNode->left_) newNode->left_ = oldNode->left_;

        newNode->parent_ = oldNode->parent_;
        if (!oldNode->parent_)
        {
            top_ = newNode;
        }
        else
        {
            if (oldNode->parent_->left_ == oldNode)
                oldNode->parent_->left_ = newNode;
            else
            {
                oldNode->parent_->right_ = newNode;
            }
        }
        oldNode->left_ = nullptr;
        oldNode->right_ = nullptr;
        delete oldNode;
    };

    Node<KeyT> *deleted = top_;
    while (deleted)
    {
        if (Comp{}(key, deleted->key_))
            deleted = deleted->left_;
        else if (Comp{}(deleted->key_, key))
            deleted = deleted->right_;
        else
            break;
    }

    if (!deleted) return;

    if(!deleted->left_ && !deleted->right_) // if it's leaf
    {
        if (!deleted->parent_)
        {
            delete deleted;
            top_ = nullptr;
            return;
        }

        if (deleted->parent_->left_ == deleted)
            deleted->parent_->left_ = nullptr;
        else
            deleted->parent_->right_ = nullptr;

        delete deleted;
    }
    else if (deleted->right_ && deleted->left_)
    {
        Node<KeyT> *inorderedSuccessor =  successor(deleted);

        if (!inorderedSuccessor)
        {
            replace(deleted, deleted->right_);
        }
        else
        {
            replace(deleted, inorderedSuccessor);
        }
    }
    else if (deleted->right_) 
    {
        replace(deleted, deleted->right_);
    }
    else 
    {
        replace(deleted, deleted->left_);
    }
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::erase(const KeyT& key)
{
    BSTErase(key);
}


// --------------------------------------- visualization ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::gdump(std::ostream &os) const
{
    os << "digraph G {\n";
    os << "    node [shape=circle, style=filled];\n";
    gdumpNode(os, top_);
    os << "}\n";
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::gdumpNode(std::ostream &os, const Node<KeyT> *node) const
{
    if (!node) return;

    os << "    \"" << node << "\" [label=\"" << node->key_ << "\", "
       << (node->color_ == color_t::RED ? "fillcolor=lightcoral, fontcolor=white" : "fillcolor=lightgray, fontcolor=black")
       << "];\n";

    if (node->left_) {
        os << "    \"" << node << "\" -> \"" << node->left_ << "\";\n";
        gdumpNode(os, node->left_);
    }
    if (node->right_) {
        os << "    \"" << node << "\" -> \"" << node->right_ << "\";\n";
        gdumpNode(os, node->right_);
    }

}



// --------------------------------------- searching ---------------------------------------

template <typename KeyT, typename Comp>
typename RBTree<KeyT, Comp>::iterator 
RBTree<KeyT, Comp>::lower_bound(const KeyT& key) const {
    Node<KeyT>* res = nullptr;
    Node<KeyT>* cur = top_;
    while (cur) {
        if (!Comp{}(cur->key_, key))
        { 
            res = cur;
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return res;
}

template <typename KeyT, typename Comp>
typename RBTree<KeyT, Comp>::iterator 
RBTree<KeyT, Comp>::upper_bound(const KeyT& key) const {
    Node<KeyT>* res = nullptr;
    Node<KeyT>* cur = top_;
    while (cur) {
        if (Comp{}(key, cur->key_))
        { 
            res = cur;
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return res;
}

// search next element after Node<KeyT>* node in subtree
template <typename KeyT, typename Comp>
Node<KeyT>*
RBTree<KeyT, Comp>::successor(Node<KeyT>* node) const
{
    if (!node) return nullptr;

    if (node->right_)
    {
        Node<KeyT>* cur = node->right_;
        while (cur->left_) cur = cur->left_;
        return cur;
    }

    Node<KeyT>* cur = node;
    Node<KeyT>* p = cur->parent_;
    while (p && (cur == p->right_))
    {
        cur = p;
        p = p->parent_;
    }
    return p;
}

template <typename C>
int mydistance(const C& tree, typename C::iterator fst, typename C::iterator snd) {
    int cnt = 0;
    auto cur = fst;
    while (cur && cur != snd)
    {
        ++cnt;
        cur = tree.successor(cur);
    }
    return cnt;
}

template <typename C, typename T>
int range_query(const C& s, T fst, T snd)
{
    if (fst > snd) return 0;

    using itt = typename C::iterator;
    itt start = s.lower_bound(fst);
    
    itt fin = s.upper_bound(snd);

    return mydistance(s, start, fin); // std::distance для set
}

template <typename KeyT, typename Comp>
bool RBTree<KeyT, Comp>::exists(const KeyT& key) const {
    Node<KeyT>* cur = top_;
    while (cur) {
        if (cur->key_ == key) return true;
        if (Comp{}(key, cur->key_)) {
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return false;
}

// --------------------------------------- destructor ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::clear()
{
    Node<KeyT>* curr = top_;
    while (curr)
    {
        if (curr->left_)
        {
            curr = curr->left_;
            continue;
        }
        if (curr->right_)
        {
            curr = curr->right_;
            continue;
        }

        // leaf node
        Node<KeyT>* parent = curr->parent_;
        if (parent)
        {
            if (parent->left_ == curr) parent->left_ = nullptr;
            else parent->right_ = nullptr;
        }
        delete curr;
        if (!parent) { curr = nullptr; top_ = nullptr; }
        else curr = parent;
    }

    top_ = nullptr;
}

} // namespace RBT

#endif // RBT_HPP