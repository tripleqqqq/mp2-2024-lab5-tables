#pragma once
#include <iostream>

using std::size_t;

template<typename T>
class Node
{
public:
	T data; // данные узла
	Node* next; // указатель на следующий узел 

	Node(T _data = T(), Node* _next = nullptr) : data(_data), next(_next) {}
};

template<typename T>
class List
{
protected:
	Node<T>* first; // указатель на первый узел 
	size_t size; // количество элементов в списке
public:


	class iterator
	{
		Node<T>* current; // текущий узел 
	public:

		iterator(Node<T>* node) : current(node) {}

		iterator(const Node<T>* node) : current(const_cast<Node<T>*>(node)) {}

		iterator(const iterator& it) { current = it.current; }

		iterator(std::nullptr_t ptr) { current = nullptr; }


		Node<T>* get_current() const noexcept { return current; }

		iterator& operator++()
		{
			current = current->next;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		T& operator*()
		{
			return current->data;
		}
		const T& operator*() const
		{
			return current->data;
		}
		bool operator==(const iterator& it)
		{
			return this->current == it.current;
		}
		const bool operator==(const iterator& it) const
		{
			return this->current == it.current;
		}
		bool operator!=(const iterator& it)
		{
			return !(*this == it);
		}
	};
	iterator begin(bool k = 0) noexcept
	{
		if (k)
		{
			return iterator(first->next);
		}
		return iterator(first);
	}
	iterator end() noexcept
	{
		if (check_cycle())
		{
			return iterator(first);
		}
		return iterator(nullptr);
	}
	const iterator begin(bool k = 0) const noexcept
	{
		if (k)
		{
			return iterator(first->next);
		}
		return iterator(first);
	}
	const iterator end() const noexcept
	{
		if (check_cycle())
		{
			return iterator(first);
		}
		return iterator(nullptr);
	}


	List()
	{
		first = nullptr;
		size = 0;
	}
	List(Node<T>* first_node)
	{
		first = first_node;
		size = 1;
	}
	List(const List<T>& _list)
	{
		first = nullptr;
		size = 0;

		for (iterator it = _list.begin(); it != _list.end(); ++it)
			push_back(*it);
	}
	~List()
	{
		this->clear();
	}
	void clear()
	{
		while (first != nullptr)
		{
			Node<T>* temp = first;
			first = first->next;
			delete temp;
		}
		size = 0;
	}
	List<T>& operator=(const List<T>& _list)
	{
		if (this != &_list)
		{
			this->clear();
			for (iterator it = _list.begin(); it != _list.end(); ++it)
				push_back(*it);
		}
		return *this;
	}
	Node<T>* get_first() const noexcept { return first; }

	size_t get_size() const noexcept { return size; }

	void push_front(T elem)
	{
		first = new Node<T>(elem, first);
		size++;
	}
	void pop_front()
	{
		if (first == nullptr)
			throw std::logic_error("list is empty");

		Node<T>* temp = first;
		first = temp->next;

		delete temp;
		size--;
	}
	void insert(T elem, Node<T>* node)
	{
		if (node == nullptr)
			push_front(elem);
		else
		{
			node->next = new Node<T>(elem, node->next);

			size++;
		}
	}
	void erase(Node<T>* node)
	{
		if (node == nullptr)
			pop_front();
		else
		{
			Node<T>* temp = node->next;
			if (temp != nullptr)
			{
				node->next = temp->next;

				delete temp;
				size--;
			}
			else
				node->next = nullptr;
		}
	}
	Node<T>* get_last()
	{
		if (first == nullptr) return nullptr;
		Node<T>* current = first;
		Node<T>* next_node = first->next;

		while (next_node != nullptr && next_node != first)
		{
			current = next_node;
			next_node = next_node->next;
		}
		return current;
	}
	void push_back(T elem)
	{
		this->insert(elem, this->get_last());
	}
	bool check_cycle()
	{
		Node<T>* slow = first;
		Node<T>* fast = first;

		while (fast != nullptr)
		{
			fast = fast->next;
			slow = slow->next;
			if (fast == nullptr)
			{
				return false;
			}
			else
			{
				fast = fast->next;
			}
			if (fast == slow)
			{
				return true;
			}
		}
		return false;
	}
	const bool check_cycle() const
	{
		Node<T>* slow = first;
		Node<T>* fast = first;

		while (fast != nullptr)
		{
			fast = fast->next;
			slow = slow->next;
			if (fast == nullptr)
			{
				return false;
			}
			else
			{
				fast = fast->next;
			}
			if (fast == slow)
			{
				return true;
			}
		}
		return false;
	}
};