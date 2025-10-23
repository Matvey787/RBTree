#ifndef RBT_HPP
#define RBT_HPP

#include <gtest/gtest.h>
#include <functional>
#include <fstream>
#include <stack>

namespace RBT {

enum class color_t { RED, BLACK };

template <typename KeyT>
struct Node
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
    void gdumpNode  (std::ofstream &ofs, Node<KeyT> *node) const;
    void LLRot      (Node<KeyT> *node);
    void RRRot      (Node<KeyT> *node);
    void BSTErase(const KeyT& key);
    Node<KeyT>* findMinInSubtree(Node<KeyT> *subRoot) const;

public:
    
    using iterator = Node<KeyT>*;
    iterator successor(Node<KeyT>* node) const;
    iterator lower_bound(KeyT key) const;
    iterator upper_bound(KeyT key) const;

    void insert (KeyT key);
    void erase  (KeyT key);
    bool exists (KeyT key) const;
    void clear  ();
    void gdump  () const;

    ~RBTree()
    {
        clear();
    }
};


// --------------------------------------- балансировка ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::LLRot(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_;
    Node<KeyT> *g = p->parent_;

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
void RBTree<KeyT, Comp>::RRRot(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_;
    Node<KeyT> *g = p->parent_;

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
    Node<KeyT> *p = node->parent_; // parent
    if (!p) return;
    if (p->color_ == color_t::BLACK) return;

    Node<KeyT> *g = p->parent_; // grandparent

    Node<KeyT> *u = nullptr;

    if (g->left_ != p)
        u = g->left_; // uncle
    else
        u = g->right_; // uncle

    if (u && u->color_ == color_t::RED) // случай, если дядя красный
    {
        p->color_ = color_t::BLACK;
        u->color_ = color_t::BLACK;

        if (g->parent_)
            g->color_ = color_t::RED;
        else
            g->color_ = color_t::BLACK;

        balance(g);
    }

    else if (!u || u->color_ == color_t::BLACK) // случай, если дядя черный
    {
        if ((p == g->left_ && node == p->left_)) // отец левый, сын левый
        {
            LLRot(node);
        }
        else if (p == g->right_ && node == p->right_) // отец правый, сын правый
        {
            
            RRRot(node);
        }
        else if (p == g->left_ && node == p->right_)
        {
            node->parent_ = g;
            g->left_ = node;

            p->right_ = node->left_;
            if (node->left_) node->left_->parent_ = p;

            p->parent_ = node;
            node->left_ = p;

            LLRot(p);
        }
        else if (p == g->right_ && node == p->left_) // отец правый, сын левый
        {
            node->parent_ = g;
            g->right_ = node;

            p->left_ = node->right_;
            if (node->right_) node->right_->parent_ = p;

            p->parent_ = node;
            node->right_ = p;

            RRRot(p);
        }
    }
}



// --------------------------------------- вставка ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::insert(KeyT key)
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



// --------------------------------------- удаление ---------------------------------------

template <typename KeyT, typename Comp>
Node<KeyT>* RBTree<KeyT, Comp>::findMinInSubtree(Node<KeyT> *subRoot) const
{
    Node<KeyT> *minNode = subRoot;

    while (minNode && minNode->left_) minNode = minNode->left_;

    // смотрим что это не корень поддерева,
    // иначе это означает что левых потомков в этом поддереве нет
    if (minNode == subRoot) return nullptr;
    return minNode;
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::BSTErase(const KeyT& key)
{
    
    auto replace = [this](Node<KeyT>* oldNode, Node<KeyT>* newNode) {
        if (!oldNode || !newNode) return;

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
    while (deleted) // ищем узел для удаления
    {
        if (Comp{}(key, deleted->key_))
            deleted = deleted->left_;
        else if (Comp{}(deleted->key_, key))
            deleted = deleted->right_;
        else
            break;
    }

    if (!deleted) return;

    if(!deleted->left_ && !deleted->right_) // если это лист
    {
        if (deleted->parent_->left_ == deleted)
            deleted->parent_->left_ = nullptr;
        else
            deleted->parent_->right_ = nullptr;

        delete deleted;
    }
    else if (deleted->right_ && deleted->left_)
    {
        Node<KeyT> *inorderedSuccessor =  successor(deleted);

        if (!inorderedSuccessor) // минимальный элемент в правом поддереве не найден
        {
            replace(deleted, deleted->right_);
        }
        else
        {
            replace(deleted, inorderedSuccessor);
        }
    }
    else if (deleted->right_) // есть только правый потомок
    {
        
        replace(deleted, deleted->right_);
        
    }
    else // есть только левый потомок
    {
        replace(deleted, deleted->left_);
    }
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::erase(KeyT key)
{
    BSTErase(key);
}



// --------------------------------------- визуализация ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::gdump() const
{
    static int counter = 0;
    std::string graphviz_file = "graphviz/tree" + std::to_string(counter) + ".dot";
    std::ofstream ofs(graphviz_file);
    if (!ofs) throw std::runtime_error("Failed to open graphviz_file file");
    ofs << "digraph G {\n";
    ofs << "    node [shape=circle, style=filled];\n";
    gdumpNode(ofs, top_);
    ofs << "}\n";
    ofs.close();

    // generate dot file
    std::string dot_cmd = "dot -Tsvg graphviz/tree" + std::to_string(counter) + ".dot -o graphviz/tree" + std::to_string(counter + 1) + ".svg";
    system(dot_cmd.c_str());
    std::string rm_cmd = "rm graphviz/tree" + std::to_string(counter) + ".dot";
    system(rm_cmd.c_str());
    ++counter;
}

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::gdumpNode(std::ofstream &ofs, Node<KeyT> *node) const
{
    if (!node) return;

    std::string color_attr = (node->color_ == color_t::RED) ? 
        "fillcolor=lightcoral, fontcolor=white" : 
        "fillcolor=lightgray, fontcolor=black";

    ofs << "    \"" << node << "\" [label=\"" << node->key_ << "\", " << color_attr << "];\n";
    if (node->left_) {
        ofs << "    \"" << node << "\" -> \"" << node->left_ << "\";\n";
        gdumpNode(ofs, node->left_);
    }
    if (node->right_) {
        ofs << "    \"" << node << "\" -> \"" << node->right_ << "\";\n";
        gdumpNode(ofs, node->right_);
    }

}



// --------------------------------------- поиск ---------------------------------------

template <typename KeyT, typename Comp>
typename RBTree<KeyT, Comp>::iterator 
RBTree<KeyT, Comp>::lower_bound(KeyT key) const {
    Node<KeyT>* res = nullptr;
    Node<KeyT>* cur = top_;
    while (cur) {
        if (!Comp{}(cur->key_, key)) // cur->key_ >= key
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
RBTree<KeyT, Comp>::upper_bound(KeyT key) const {
    Node<KeyT>* res = nullptr;
    Node<KeyT>* cur = top_;
    while (cur) {
        if (Comp{}(key, cur->key_)) // cur->key_ > key
        { 
            res = cur;
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return res;
}

// поиск следующего с более большим ключом
template <typename KeyT, typename Comp>
typename RBTree<KeyT, Comp>::iterator
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

    // std::cout << "Range query [" << (start ? start->key() : -1) << ", " << (fin ? fin->key() : -1) << "]: " << std::endl;

    return mydistance(s, start, fin); // std::distance для set
}

template <typename KeyT, typename Comp>
bool RBTree<KeyT, Comp>::exists(KeyT key) const {
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

// --------------------------------------- линейная очистка ---------------------------------------

template <typename KeyT, typename Comp>
void RBTree<KeyT, Comp>::clear()
{
    std::stack<RBTree::iterator> stack;
    if(top_ != nullptr) stack.push(top_);

    while(!stack.empty())
    {
        RBTree::iterator currNode = stack.top();
        stack.pop();

        if (currNode->left_ != nullptr) stack.push(currNode->left_);
        if (currNode->right_ != nullptr) stack.push(currNode->right_);

        delete currNode;
    }

}


} // namespace RBT

#endif // RBT_HPP