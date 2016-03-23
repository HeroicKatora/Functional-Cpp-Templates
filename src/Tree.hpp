/*
 * Tree.hpp
 *
 *  Created on: 02.04.2015
 *      Author: Andreas Molzer
 */

#pragma once
#include "traits.hpp"
#include <cstring>

namespace{
	template<typename ElPtr, typename CmpT, unsigned size = 2>
	class _Node {
	private:
		using OwnT = _Node<ElPtr, CmpT, size>;
		using OwnTPtr = _Node<ElPtr, CmpT, size> *;

		static_assert(size > 0, "Each node needs to have at least branching of 1");

		union {
			bool isNotLeaf;
			unsigned childCnt;
		} state;
		ElPtr element; // refers to the minimum for non-leaves and to the element for leaves
		OwnTPtr parent; //nullptr for top node, pointer to parent else
		OwnTPtr children [2*size];

		/**
		 * Inserts a node at a position
		 */
		OwnTPtr _insert(OwnTPtr node, int index){
			OwnTPtr ret = nullptr;
			if(state.childCnt == 2*size){ // New node needs to be created
				ret = new _Node<ElPtr, CmpT, size>();
				if(index > size){ // Node would be in new node
					index -= size;
					std::memcpy(ret->children, children + size, index);
					std::memcpy(ret->children+index+1, children+index, size-index);
					std::memset(ret->children+size, 0, size);
					ret->_setNodeTo(node, index);
					state.childCnt -= size;
					ret->state.childCnt = size+1;
					return ret;
				}
				std::memcpy(ret->children, children + size, size);
				std::memset(ret->children+size, 0, size);
				state.childCnt -= size;
				ret->state.childCnt = size;
			}
			std::memmove(children+index+1, children+index, state.childCnt - index);
			_setNodeTo(node, index);
			state.childCnt++;
			return ret;
		}

		void _setNodeTo(OwnTPtr node, int index){
			children[index] = node;
			node->parent = this;
		}

	public:
		struct ResultType {
			bool found;
			ElPtr element;
			ResultType():found(false), ElPtr(nullptr){};
			ResultType(ElPtr element):found(true), ElPtr(element){};
		};

		_Node() = delete;
		_Node(OwnTPtr parent, ElPtr element):state(0), parent(parent), element(element){};
		~_Node(){
			for(int i = 0;i < state.childCnt;i++){
				delete(children[i]);
			}
			delete(element);
		}

		/**
		 * Returns a new node that should be inserted if necessary, nullptr otherwise
		 * The new node exists next to this one.
		 */
		OwnTPtr insert(ElPtr element){
			if(!state.isNotLeaf){
				if(*(this->element) > *element) swap(this->element, element);
				return new OwnT(parent, element);
			}
			if(*element < *(this->element)){
				element = this->element;
			}
			int i = state.childCnt-1;
			for(;i>=0;i--){
				if(*(children[i].element) < *element) break;
			}
			i = i < 0? 0 : i;
			OwnTPtr nNode = children[i].insert(element);
			return _insert(nNode, i+1);
		}

		/**
		 * Inserts a new node that was created by splitting this or nullptr
		 */
		OwnTPtr insert(OwnTPtr node){
			if(node == nullptr)
				return node;
			int index = 0;
			for(;index < state.childCnt;index++){
				if(*(children[index].element) < *element) {
					break;
				}
			}
			return _insert(node, index);
		}

		ResultType search(CmpT criteria){

		}

		ResultType remove(CmpT criteria){

		}
	};
}

template<typename ElementT,
	typename _ElPtr = ElementT *,
	typename _CmpT = ElementT &,
	typename _NdT = _Node<_ElPtr, _CmpT>>
class Tree {
protected:
	_NdT *topNode;
	int size;
public:
	using ResultType = typename _NdT::ResultType;

	Tree():topNode(nullptr), size(0){}
	virtual ~Tree(){}

	void add(_ElPtr element){
		size ++;
		if(!topNode){
			topNode = new _NdT(nullptr, element);
			return;
		}
		_NdT *newElement = topNode->insert(element);
		if(newElement){
			_NdT *node = new _NdT(nullptr, topNode->element);
			node->insert(topNode);
			node->insert(newElement);
			topNode = node;
		}
	};

	ResultType search(_CmpT criteria){}

	ResultType remove(_CmpT criteria){};

	void clear(){
		if(topNode){
			delete(topNode);
		}
	}
};

/*
template<typename Integral, Integral notfound>
class Tree<Integral, Integral, Integral, notfound> {

};
*/
