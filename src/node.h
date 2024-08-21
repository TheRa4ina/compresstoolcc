#pragma once
#include <memory>

template<class T>
class Node {
public:
	Node();
	Node(T value);
	Node(T value, T left, T right);

	const T getValue() const;
	void setValue(T val);

	const std::shared_ptr<const Node<T>> getLeft() const;
	void setLeft(Node<T> new_left);
	const std::shared_ptr<const Node<T>> getRight() const;
	void setRight(Node<T> new_right);

	bool isLeaf() const;

private:

	T value;
	std::shared_ptr<Node<T>> left = nullptr;
	std::shared_ptr<Node<T>> right = nullptr;
};

template<class T>
inline Node<T>::Node()
	: value(T()) {}

template<class T>
inline Node<T>::Node(T value)
	:value(value) {}

template<class T>
inline Node<T>::Node(T value, T left, T right)
	: value(value), left(std::make_shared<Node<T>>(left)), right(std::make_shared<Node<T>>(right))
{
}

template<class T>
inline const T Node<T>::getValue() const
{
	return value;
}

template<class T>
inline void Node<T>::setValue(T val)
{
	value = val;
}

template<class T>
inline const std::shared_ptr<const Node<T>> Node<T>::getLeft() const
{
	return left;
}

template<class T>
inline void Node<T>::setLeft(Node<T> new_left)
{
	left = std::make_shared<Node<T>>(std::move(new_left));
}

template<class T>
inline const std::shared_ptr<const Node<T>> Node<T>::getRight() const
{
	return right;
}

template<class T>
inline void Node<T>::setRight(Node<T> new_right)
{
	right = std::make_shared<Node<T>>(std::move(new_right));
}

template<class T>
inline bool Node<T>::isLeaf() const
{
	return left==nullptr && right == nullptr;
}
