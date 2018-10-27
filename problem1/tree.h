#pragma once

#include <memory>

namespace bintree {
    template <typename T>
    struct TNode : std::enable_shared_from_this<TNode<T>> {
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;
        using TNodeWeakPtr = std::weak_ptr<TNode<T>>;

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
            return !parent.expired();
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
            return parent.lock();
        }

        TNodeConstPtr getParent() const {
            return parent.lock();
        }

        static TNodePtr createLeaf(T v) {
            // std::make_shared requires a public constructor
            // So we cann't use it
            TNodePtr ptr(new TNode(v));
            return ptr;
        }

        // If we use raw pointer in this method
        // the problomem of double creation of shared_ptr from raw pointer
        // would be occured
        static TNodePtr fork(T v, TNodePtr left, TNodePtr right) {
            // std::make_shared requires a public constructor
            // So we cann't use it
            TNodePtr ptr (new TNode(v, left, right));
            setParent(ptr->getLeft(), ptr);
            setParent(ptr->getRight(), ptr);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
            // the idea of creating pointer like TNodePtr(this)
            // is a bad idea becouse of the same problem as
            // doublie ccreation shared_ptr from raw pointer
            setParent(l, this->shared_from_this());
            setParent(left, nullptr);
            std::swap(l, left);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, this->shared_from_this());
            setParent(right, nullptr);
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }
        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        TNodeWeakPtr parent; // prevent cyclic pointers

        TNode(T v)
            : value(v)
        {
        }

        // We can't use a raw pointers becouse of the same problem as in
        // fork method
        TNode(T v, TNodePtr left, TNodePtr right)
            : value(v)
            , left(left)
            , right(right)
        {
        }

        static void setParent(TNodePtr node, TNodePtr parent) {
            if (node)
                node->parent = parent;
        }
    };
}
