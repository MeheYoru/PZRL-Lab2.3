#include "BST.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left,
                             Node *right)
    : keyValuePair(std::make_pair(key, value)),
      parent(parent),
      left(left),
      right(right) {}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair),
      parent(nullptr),
      left(nullptr),
      right(nullptr) {
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
    return keyValuePair == other.keyValuePair &&
           ((!left && !other.left) ||
            (left && other.left && *left == *other.left)) &&
           ((!right && !other.right) ||
            (right && other.right && *right == *other.right));
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left) left->output_node_tree();
    std::cout << "[" << keyValuePair.first << ": " << keyValuePair.second
              << "] ";
    if (right) right->output_node_tree();
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        if (left)
            left->insert(key, value);
        else
            left = new Node(key, value, this);
    } else {
        if (right)
            right->insert(key, value);
        else
            right = new Node(key, value, this);
    }
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other)
    : _size(other._size) {
    if (other._root) _root = new Node(*other._root);
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this != &other) {
        BinarySearchTree temp(other);
        _root = new Node(*other._root);
        _size = other._size;
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    std::swap(_root, other._root);
    std::swap(_size, other._size);
}

BinarySearchTree &BinarySearchTree::operator=(
    BinarySearchTree &&other) noexcept {
    std::swap(_root, other._root);
    std::swap(_size, other._size);
    other.~BinarySearchTree();
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    std::stack<Node *> nodes;
    if (_root) nodes.push(_root);

    while (!nodes.empty()) {
        Node *current = nodes.top();
        nodes.pop();
        if (current->left) nodes.push(current->left);
        if (current->right) nodes.push(current->right);
        delete current;
    }  
    _size.~size_t();
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) return *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
    } else {
        Node *parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
    } else {
        Node *parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }

    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    if (!this) return Iterator(nullptr);
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return !(*this == other);
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node)
    : _node(node) {}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*()
    const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->()
    const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (!_node) return *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
    } else {
        const Node *parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(
    int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node) {
        return *this;
    }

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
    } else {
        const Node *parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(
    int) {
    if (!this) return ConstIterator(nullptr);
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(
    const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(
    const ConstIterator &other) const {
    return !(*this == other);
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (!_root) {
        _root = new Node(key, value);
        Node *_endNode = new Node(key + 1, 0, _root);
        _root->right = _endNode;
        _size = 1;
    } else {
        Node *current = _root;
        Node *parent = nullptr;
        while (current) {
            parent = current;
            if (key < current->keyValuePair.first) {
                current = current->left;
            } else if (key > current->keyValuePair.first) {
                current = current->right;
            } else {
                current = current->right;
            }
        }
        Node *currentMax = _root;
        while (currentMax->right) currentMax = currentMax->right;

        if (parent != currentMax) {
            if (key < parent->keyValuePair.first) {
                parent->left = new Node(key, value, parent);
            } else {
                parent->right = new Node(key, value, parent);
            }
            _size++;
        } else {
            Node *temp = parent;
            parent = new Node(key, value, temp->parent, nullptr, temp);
            parent->parent->right = parent;
            temp->parent = parent;
            temp->keyValuePair.first = key + 1;
            _size++;
        }
    }
}

void BinarySearchTree::erase(const Key &key) {
    if (!_root) return;
    bool nodeFound = true;
    while (nodeFound && _root) {
        nodeFound = false;
        Node *parent = nullptr;
        Node *current = _root;
        while (current) {
            if (current->keyValuePair.first == key) {
                nodeFound = true;
                break;
            }
            parent = current;
            current = (key < current->keyValuePair.first) ? current->left
                                                          : current->right;
        }
        if (!nodeFound) break;
        if (current->left && current->right) {
            // Node has two children
            Node *successorParent = current;
            Node *successor = current->right;

            while (successor->left) {
                successorParent = successor;
                successor = successor->left;
            }
            current->keyValuePair = successor->keyValuePair;
            current = successor;
            parent = successorParent;
        }

        // Node has one or zero children
        Node *child = current->left ? current->left : current->right;
        if (parent) {
            if (parent->left == current)
                parent->left = child;
            else
                parent->right = child;
        } else {
            _root = child;
        }

        if (child) child->parent = parent;

        delete current;
        _size--;
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *current = _root;
    while (current) {
        if (key < current->keyValuePair.first)
            current = current->left;
        else if (key > current->keyValuePair.first)
            current = current->right;
        else
            return ConstIterator(current);
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *current = _root;
    while (current) {
        if (key < current->keyValuePair.first)
            current = current->left;
        else if (key > current->keyValuePair.first)
            current = current->right;
        else
            return Iterator(current);
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
    Iterator lower = find(key);
    Iterator upper = lower;

    if (lower != end()) {
        while (upper != end() && upper->first == key) {
            ++upper;
        }
    }
    return {lower, upper};
}
std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator lower = find(key);
    ConstIterator upper = lower;

    if (lower != cend()) {
        while (upper != cend() && upper->first == key) {
            ++upper;
        }
    }

    return {lower, upper};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    if (!_root) return ConstIterator(nullptr);
    const Node *current = _root;
    while (current->left) current = current->left;
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (!_root) return ConstIterator(nullptr);
    const Node *current = _root;
    while (current->right) current = current->right;
    return ConstIterator(current->parent);
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    ConstIterator it = find(key);
    ConstIterator itMin = find(key);
    it++;
    while (it->first == key) {
        itMin = (itMin->second < it->second ? itMin : it);
        it++;
    }
    return itMin;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    ConstIterator it = find(key);
    ConstIterator itMin = find(key);
    it++;
    while (it->first == key) {
        itMin = (itMin->second > it->second ? itMin : it);
        it++;
    }
    return itMin;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    if (!_root) return Iterator(nullptr);
    Node *current = _root;
    while (current->left) current = current->left;
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    if (!_root) return Iterator(nullptr);
    Node *current = _root;
    while (current->right) current = current->right;
    return Iterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    if (!_root) return cend();
    const Node *current = _root;
    while (current->left) current = current->left;
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    if (!_root) return cend();
    const Node *current = _root;
    while (current->right) current = current->right;
    return ConstIterator(current);
}

size_t BinarySearchTree::size() const { return _size; }

void BinarySearchTree::output_tree() {
    if (_root) {
        _root->output_node_tree();
        std::cout << std::endl;
    }
}
size_t BinarySearchTree::max_height() const {
    if (!_root) return 0;

    auto max_height_helper = [](const Node *node, auto &&self) {
        if (!node) return 0;
        return 1 + std::max(self(node->left, self), self(node->right, self));
    };

    return max_height_helper(_root, max_height_helper);
}