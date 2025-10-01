#ifndef RBT_HPP
#define RBT_HPP

#include <functional>
#include <fstream>

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
    Node(KeyT key, color_t color = color_t::RED, Node *parent = nullptr, Node *left = nullptr, Node *right = nullptr)
        : key_(key), color_(color), parent_(parent), left_(left), right_(right) {}

    ~Node()
    {
        delete left_;
        delete right_;
    }
    
    template <typename, typename>
    friend class SearchTree;
};


template <typename KeyT, typename Comp = std::less<KeyT>>
class SearchTree {
    Node<KeyT> *top_ = nullptr;

    void balance(Node<KeyT> *node);
    void gdumpNode(std::ofstream &ofs, Node<KeyT> *node) const;
    void LLRot(Node<KeyT> *node);
    void RRRot(Node<KeyT> *node);
    void BSTErase(KeyT key);
    Node<KeyT>* findMinInSubtree(Node<KeyT> *subRoot) const;

public:
    Node<KeyT>* successor(Node<KeyT>* node) const {
        if (!node) return nullptr;

        if (node->right_) {
            Node<KeyT>* cur = node->right_;
            while (cur->left_) cur = cur->left_;
            return cur;
        }

        Node<KeyT>* cur = node;
        Node<KeyT>* p = cur->parent_;
        while (p && cur == p->right_) {
            cur = p;
            p = p->parent_;
        }
        return p;
    }
    
    using iterator = Node<KeyT>*;
    iterator lower_bound(KeyT key) const;
    iterator upper_bound(KeyT key) const;
    int distance(iterator fst, iterator snd) const;

    void insert(KeyT key);
    void erase(KeyT key);
    void gdump() const;
};

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::insert(KeyT key)
{
    // std::cout << "here1" << std::endl;
    if (!top_) {
        std::cout << "top created" << std::endl;
        top_ = new Node<KeyT>{key, color_t::BLACK};
        return;
    }
    // std::cout << "here2" << std::endl;

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

    // std::cout << "here3" << std::endl;
    if (Comp{}(key, parent->key_))
    {
        // std::cout << "here4" << parent << std::endl;
        parent->left_ = new Node<KeyT>{key, color_t::RED, parent};
        std::cout << "here5" << parent << std::endl;
        balance(parent->left_);
        //  std::cout << "here6" << parent << std::endl;
    }
    else
    {
        parent->right_ = new Node<KeyT>{key, color_t::RED, parent};
        balance(parent->right_);
    }
}

template <typename KeyT, typename Comp>
Node<KeyT>* SearchTree<KeyT, Comp>::findMinInSubtree(Node<KeyT> *subRoot) const
{
    Node<KeyT> *minNode = subRoot;

    while (minNode && minNode->left_) minNode = minNode->left_;

    // смотрим что это не корень поддерева,
    // иначе это означает что левых потомков в этом поддереве нет
    if (minNode == subRoot) return nullptr;
    return minNode;
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::BSTErase(KeyT key)
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
                std::cout << "here1111" << std::endl;
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
        Node<KeyT> *inorderedSuccessor = findMinInSubtree(deleted->right_);

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
        std::cout << "Удаляется элемент с правым потомком " << deleted->key_ << std::endl;
        replace(deleted, deleted->right_);
        std::cout << "удалён" << std::endl;
    }
    else // есть только левый потомок
    {
        replace(deleted, deleted->left_);
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::erase(KeyT key)
{
    BSTErase(key);
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::LLRot(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_;
    Node<KeyT> *g = p->parent_;

    if (g->parent_)
    {
        if (g == g->parent_->left_)
            g->parent_->left_ = p;
        else
            g->parent_->right_ = p;
            
        g->left_ = p->right_;
        g->parent_ = p;
        p->right_ = g;

        std::swap(p->color_, g->color_);
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::RRRot(Node<KeyT> *node)
{
    Node<KeyT> *p = node->parent_;
    Node<KeyT> *g = p->parent_;

    if (g->parent_)
    {
        if (g == g->parent_->left_)
            g->parent_->left_ = p;
        else
            g->parent_->right_ = p;
            
        g->right_ = p->left_;
        g->parent_ = p;
        p->left_ = g;
        std::swap(p->color_, g->color_);
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::balance(Node<KeyT> *node)
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

    if (u && u->color_ == color_t::RED)
    {
        p->color_ = color_t::BLACK;
        u->color_ = color_t::BLACK;

        if (g->parent_)
            g->color_ = color_t::RED;
        else
            g->color_ = color_t::BLACK;

        std::cout << "here7" << g << std::endl;
        balance(g);
    }

    else if (!u || u->color_ == color_t::BLACK)
    {
        if ((p == g->left_ && node == p->left_))
        {
            LLRot(node);
        }
        else if (p == g->right_ && node == p->right_)
        {
            RRRot(node);
        }
        else if (p == g->left_ && node == p->right_)
        {
            node->parent_ = g;
            g->left_ = node;

            p->right_ = node->left_;

            p->parent_ = node;
            node->left_ = p;

            LLRot(p);

        }
        else if (p == g->right_ && node == p->left_)
        {
            node->parent_ = g;
            g->right_ = node;

            p->left_ = node->right_;

            p->parent_ = node;
            node->right_ = p;

            RRRot(p);
        }
    }
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::gdump() const
{
    std::cout << "графическое представление дерева" << std::endl;
    static int counter = 0;
    std::string graphviz_file = "graphviz/tree" + std::to_string(counter) + ".dot";
    std::ofstream ofs(graphviz_file);
    if (!ofs) throw std::runtime_error("Failed to open graphviz_file file");
    ofs << "digraph G {\n";
    ofs << "    node [shape=circle, style=filled];\n";
    gdumpNode(ofs, top_);
    ofs << "}\n";
    ofs.close();
    std::cout << "графическое представление дерева" << std::endl;

    std::string dot_cmd = "dot -Tsvg graphviz/tree" + std::to_string(counter) + ".dot -o graphviz/tree" + std::to_string(counter + 1) + ".svg";
    system(dot_cmd.c_str());
    std::string rm_cmd = "rm graphviz/tree" + std::to_string(counter) + ".dot";
    system(rm_cmd.c_str());
    ++counter;
}

template <typename KeyT, typename Comp>
void SearchTree<KeyT, Comp>::gdumpNode(std::ofstream &ofs, Node<KeyT> *node) const
{
    if (!node) return;

    std::cout << "Print node " << node->key_ << std::endl;

    std::string color_attr = (node->color_ == color_t::RED) ? 
        "fillcolor=lightcoral, fontcolor=white" : 
        "fillcolor=lightgray, fontcolor=black";

    ofs << "    \"" << node << "\" [label=\"" << node->key_ << "\", " << color_attr << "];\n";
    std::cout << "gdumpNode    " << (node->parent_ ? node->parent_->key_ : -1) << std::endl;
    if (node->left_) {
        ofs << "    \"" << node << "\" -> \"" << node->left_ << "\";\n";
        gdumpNode(ofs, node->left_);
    }
    if (node->right_) {
        ofs << "    \"" << node << "\" -> \"" << node->right_ << "\";\n";
        gdumpNode(ofs, node->right_);
    }

    std::cout << "here2222" << std::endl;
}

template <typename KeyT, typename Comp>
typename SearchTree<KeyT, Comp>::iterator 
SearchTree<KeyT, Comp>::lower_bound(KeyT key) const {
    Node<KeyT>* res = nullptr;
    Node<KeyT>* cur = top_;
    while (cur) {
        if (!Comp{}(cur->key_, key)) { // cur->key_ >= key
            res = cur;
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return res;
}

template <typename KeyT, typename Comp>
typename SearchTree<KeyT, Comp>::iterator 
SearchTree<KeyT, Comp>::upper_bound(KeyT key) const {
    Node<KeyT>* res = nullptr;
    Node<KeyT>* cur = top_;
    while (cur) {
        if (Comp{}(key, cur->key_)) { // cur->key_ > key
            res = cur;
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return res;
}


template <typename C>
int mydistance(const C& tree, typename C::iterator fst, typename C::iterator snd) {
    int cnt = 0;
    auto cur = fst;
    while (cur && cur != snd) {
        ++cnt;
        cur = tree.successor(cur);
    }
    return cnt;
}

template <typename C, typename T>
int range_query(const C& s, T fst, T snd)
{
    using itt = typename C::iterator;
    itt start = s.lower_bound(fst);
    // first not less then fst
    itt fin = s.upper_bound(snd);
    // first greater then snd
    return mydistance(s, start, fin); // std::distance для set
}

} // namespace RBT



#endif // RBT_HPP