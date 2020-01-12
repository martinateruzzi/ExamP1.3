#include <iostream>
#include <iterator>
#include <functional>
#include <utility>
#include <memory>
#include <vector>

#ifndef __BST__HH__
#define __BST__HH__

template <typename K, typename V, typename C=std::less<K>>
class bst {

	C cmp;	
	
	struct node {

		std::pair<const K, V> p;
		std::unique_ptr<node> left;
		std::unique_ptr<node> right;
		node* parent;

		explicit node(const std::pair<const K, V>& pp, node* dad) noexcept : p{pp}, left{}, right{}, parent{dad} {}

		explicit node(std::pair<const K, V>&& pp, node* dad) noexcept : p{std::move(pp)},left{}, right{}, parent{dad} {}

		explicit node(const std::unique_ptr<node>& from): p{from->p}, left{}, right{}, parent{} {
		
			if(from->left){
				left = std::make_unique<node>(from->left);
				left->parent = this;
			}
			if(from->right){
				right = std::make_unique<node>(from->right);
				right->parent = this;
			}
		}

	};

	std::unique_ptr<node> root;

	void fill(const std::vector<std::pair<const K, V>>& v, int begin, int end);

public:

	bst() noexcept = default;

	bst(const bst& b): root{} { if(b.root) root = std::make_unique<node>(b.root);}	
	bst& operator=(const bst& b){

		root.reset();
		auto tmp{b};
		*this = std::move(tmp);	
		return *this;
	}

	bst(bst && b) noexcept = default;	
	bst& operator=(bst&& l) noexcept = default;

	template <typename O>
	class __iterator;

	using iterator = __iterator< std::pair<const K, V> >;
	using const_iterator = __iterator<const std::pair<const K, V> >;

	iterator begin() noexcept {

		if(!root) return iterator{nullptr};
		auto tmp = root.get();
	 	while(tmp->left) tmp = tmp->left.get();
		return iterator{tmp};
	}
	iterator end() noexcept { return iterator{nullptr}; }

	const_iterator begin() const noexcept { 
		
		if(!root) return const_iterator{nullptr};
		auto tmp = root.get();
		while(tmp->left) tmp = tmp->left.get();
		return const_iterator{tmp};
	}
	const_iterator end() const noexcept { return const_iterator{nullptr}; }

	const_iterator cbegin() const noexcept { 
	
		if(!root) return const_iterator{nullptr};	
		auto tmp = root.get();
		while(tmp->left) tmp = tmp->left.get();
		return const_iterator{tmp};
	}
	const_iterator cend() const noexcept{ return const_iterator{nullptr}; }

	iterator find(const K &x);

	const_iterator find(const K &x) const;

	std::pair<iterator,bool> insert(const std::pair<const K, V>& x);

	std::pair<iterator,bool> insert(std::pair<const K, V>&& x);

	template <typename... Types>
	std::pair<iterator,bool> emplace(Types&&... args);

	template <typename OT>
	V& operator[](OT&& x);

	friend std::ostream& operator<<(std::ostream& os, const bst& x){
	
		for(const auto i : x) os << i.first << " ";
		os << std::endl;
	
	return os;
	}

	void clear() noexcept {root.reset();}

	void balance();

};

template <typename K, typename V, typename C>
void bst<K, V, C>::fill(const std::vector<std::pair<const K, V>>& v, int begin, int end){

	insert(v[(end+begin)/2]);

	if(begin>=end) return;
	fill(v,begin,(end+begin)/2-1);
	fill(v,(end+begin)/2+1,end);
}

template <typename K, typename V, typename C>
typename bst<K, V, C>::iterator bst<K, V, C>::find(const K &x){
		
	auto tmp = root.get();
	while(tmp){

		if(cmp(tmp->p.first,x)) tmp = (tmp->right).get();
		else if(cmp(x,tmp->p.first)) tmp = (tmp->left).get();
		else return iterator{tmp};
	}
	return end();
}

template <typename K, typename V, typename C>
typename bst<K, V, C>::const_iterator bst<K, V, C>::find(const K &x) const {return const_iterator{find(x)};}

template <typename K, typename V, typename C>
std::pair<typename bst<K, V, C>::iterator,bool> bst<K, V, C>::insert(const std::pair<const K, V>& x){

	auto tmp = root.get();
	while(tmp){

		if(cmp(tmp->p.first,x.first)){
			if(tmp->right) tmp = (tmp->right).get();
			else{
			     tmp->right.reset(new node(x,tmp));
			     return std::make_pair<iterator,bool>(iterator{tmp->right.get()},true);
			}
		}
		else if(cmp(x.first,tmp->p.first)){
			if(tmp->left) tmp = (tmp->left).get();
			else{
				tmp->left.reset(new node(x,tmp));
				return std::make_pair<iterator,bool>(iterator{tmp->left.get()},true);
			}
		}
		else return std::make_pair<iterator,bool>(iterator{tmp},false);
	}

	root.reset(new node(x,nullptr));
	return std::make_pair<iterator,bool>(iterator{root.get()},true);
}

template <typename K, typename V, typename C>
std::pair<typename bst<K, V, C>::iterator,bool> bst<K, V, C>::insert(std::pair<const K, V>&& x){

	auto tmp = root.get();
	while(tmp){

		if(cmp(tmp->p.first,x.first)){
			if(tmp->right) tmp = (tmp->right).get();
			else{
			     tmp->right.reset(new node(std::move(x),tmp));
			     return std::make_pair<iterator,bool>(iterator{tmp->right.get()},true);
			}
		}
		else if(cmp(x.first,tmp->p.first)){
			if(tmp->left) tmp = (tmp->left).get();
			else{
				tmp->left.reset(new node(std::move(x),tmp));
				return std::make_pair<iterator,bool>(iterator{tmp->left.get()},true);
			}
	
		}
		else return std::make_pair<iterator,bool>(iterator{tmp},false);
	}

	root.reset(new node(std::move(x),nullptr));
	return std::make_pair<iterator,bool>(iterator{root.get()},true);
}

template <typename K, typename V, typename C>
template <typename... Types>
std::pair<typename bst<K, V, C>::iterator,bool> bst<K, V, C>::emplace(Types&&... args){
	return insert(std::pair<const K, V>{std::forward<Types>(args)...});
}

template <typename K, typename V, typename C>
template <typename OT>
V& bst<K, V, C>::operator[](OT&& x){

	iterator it = find(x);
	if(it == end()) it = (emplace(std::forward<OT>(x),V{})).first;
	return (*it).second;
}

template <typename K, typename V, typename C>
void bst<K, V, C>::balance(){

	if(!root) return;
	std::vector<std::pair<const K, V>> v;
	for(auto x: (*this)) v.push_back(x);

	clear();
	fill(v,0,v.size()-1);
}

template <typename K, typename V, typename C>
template <typename O>
class bst<K, V, C>::__iterator {

	using node = typename bst<K, V, C>::node;
	node* current;

	public:
	explicit __iterator(node* x) noexcept : current{x} {}

	using value_type = O;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;
	using reference = value_type&;
	using pointer = value_type*;

	reference operator*() const noexcept { return current->p; }
	pointer operator->() const noexcept { return &(*(*this)); }

	__iterator& operator++() noexcept {  // pre increment
		
		auto tmp = current->right.get();
		if(tmp){ //successor is the farthest left node of right sub-tree
			while(tmp->left) tmp = tmp->left.get();
			current = tmp;
		}
		else{ //we have to go up in the tree
			tmp = current->parent;	
			while(tmp && current == tmp->right.get()){ //as far as parent is not nullptr and we are at its right
			
				current = tmp;
				tmp = tmp->parent;
			
			}	
			current = tmp; //we found our nearest neighbor, either real one or nullptr (in this case we are end)	
		}	
		
	return *this;
	}

	__iterator operator++(int) noexcept {
		__iterator tmp{current};
		++(*this);
		return tmp;
	}

	friend bool operator==(const __iterator& a, const __iterator& b) { return a.current == b.current; }

	friend bool operator!=(const __iterator& a, const __iterator& b) { return !(a == b); }
};

#endif
