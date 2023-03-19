/**
 * 自定义ADT - 带有平衡的二叉搜索树
 * 二叉搜索树的定义：二叉搜索树是有特殊性质的二叉树
 * 特殊性质：对于树中的每个节点X，左子树上的每个节点都比节点X小，右子树上的每个节点都比节点X大。即，中序遍历下，节点元素是有序的。
 * 平衡条件：对于树中的每个节点X，左子树的深度与右子树的深度相差不超过1
 */

#pragma once

#include <iostream>

namespace Joy
{
	template<typename T> class JoyAVLTree
	{
	public:
		JoyAVLTree()
			: m_Root(nullptr)
		{
		}

		JoyAVLTree(const JoyAVLTree& rhs)
		{
			operator=(rhs);
		}

		~JoyAVLTree()
		{
			MakeEmpty(m_Root);
		}

	public:
		/// <summary>
		/// 清空二叉搜索树为空树
		/// </summary>
		void MakeEmpty()
		{
			MakeEmpty(m_Root);
		}

		/// <summary>
		/// 是否为空树
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const
		{
			return m_Root == nullptr;
		}

		/// <summary>
		/// 获取树的深度
		/// </summary>
		/// <returns></returns>
		int Deep() const
		{
			return Deep(m_Root);
		}

		/// <summary>
		/// 打印二叉搜索树
		/// </summary>
		void PrintTree() const
		{
			std::cout << "Tree Deep: " << Deep() << ", Max Deep Diff: " << MaxDepthDiff(m_Root) << std::endl;
			std::cout << "PreOrder =================" << std::endl;
			PrintTreePreOrder(m_Root);
			std::cout << "InOrder ==================" << std::endl;
			PrintTreeInOrder(m_Root);
			std::cout << "PostOrder ================" << std::endl;
			PrintTreePostOrder(m_Root);
		}

		/// <summary>
		/// 插入一个节点，并保持二叉搜索树平衡
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
			Insert(element, m_Root);
		}

		/// <summary>
		/// 删除一个节点，并保持二叉搜索树平衡
		/// </summary>
		/// <param name="element"></param>
		void Remove(const T& element)
		{
			// TODO
		}

	public:
		const JoyAVLTree& operator= (const JoyAVLTree& rhs)
		{
			MakeEmpty(this->m_Root);
			this->m_Root = Clone(rhs.m_Root);
			return *this;
		}

	private:
		/// <summary>
		/// 二叉搜索树节点数据结构
		/// </summary>
		struct AVLTreeNode
		{
			/// <summary>
			/// 节点数据
			/// </summary>
			T elementData;
			/// <summary>
			/// 左子节点
			/// </summary>
			AVLTreeNode* pLeftChild{ nullptr };
			/// <summary>
			/// 柚子节点
			/// </summary>
			AVLTreeNode* pRightChild{ nullptr };
			/// <summary>
			/// 树的深度
			/// </summary>
			int deep{ 0 };

			AVLTreeNode(const T& element, AVLTreeNode* pLeftNode = nullptr, AVLTreeNode* pRightNode = nullptr, int height = 0)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightNode), deep(height)
			{}
		};

	private:
		/// <summary>
		/// 清空二叉搜索树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(AVLTreeNode*& pNode)
		{
			if (pNode == nullptr) { return; }
			MakeEmpty(pNode->pLeftChild);
			MakeEmpty(pNode->pRightChild);
			delete pNode;
			pNode = nullptr;
		}

		/// <summary>
		/// 获取节点子树的深度
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		int Deep(AVLTreeNode* pNode) const
		{
			return pNode == nullptr ? -1 : pNode->deep;
		}

		/// <summary>
		/// 前序遍历二叉搜索树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(AVLTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			std::cout << pNode->elementData << std::endl;
			PrintTreePreOrder(pNode->pLeftChild);
			PrintTreePreOrder(pNode->pRightChild);
		}

		/// <summary>
		/// 中序遍历二叉搜索树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(AVLTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			PrintTreeInOrder(pNode->pLeftChild);
			std::cout << pNode->elementData << std::endl;
			PrintTreeInOrder(pNode->pRightChild);
		}

		/// <summary>
		/// 后续遍历二叉搜索树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(AVLTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			PrintTreePostOrder(pNode->pLeftChild);
			PrintTreePostOrder(pNode->pRightChild);
			std::cout << pNode->elementData << std::endl;
		}

		/// <summary>
		/// 插入一个节点
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void Insert(const T& element, AVLTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				pNode = new AVLTreeNode(element);
			}
			else if (pNode->elementData < element)
			{
				Insert(element, pNode->pRightChild);
				// 左右子树的高度差值大于1，需要平衡
				if (Deep(pNode->pRightChild) - Deep(pNode->pLeftChild) > 1)
				{
					if (element < pNode->pRightChild->elementData)
					{
						// 插入位置当前节点右儿子左子树，双旋转一次实现再平衡
						DoubleWithRightChild(pNode);
					}
					else
					{
						// 插入位置当前节点右儿子右子树，单旋转一次实现再平衡
						RotateWithRightChild(pNode);
					}
				}
			}
			else if (element < pNode->elementData)
			{
				Insert(element, pNode->pLeftChild);
				// 左右子树的高度差值大于1，需要平衡
				if (Deep(pNode->pLeftChild) - Deep(pNode->pRightChild) > 1)
				{
					if (element < pNode->pLeftChild->elementData)
					{
						// 插入位置当前节点左儿子左子树，单旋转一次实现再平衡
						RotateWithLeftChild(pNode);
					}
					else
					{
						// 插入位置当前节点左儿子右子树，双旋转一次实现再平衡
						DoubleWithLeftChild(pNode);
					}
				}
			}
			// 更新当前节点的深度
			pNode->deep = std::max(Deep(pNode->pLeftChild), Deep(pNode->pRightChild)) + 1;
		}

		/// <summary>
		/// 插入位置在左儿子的左子树，单旋转逻辑（右旋）
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithLeftChild(AVLTreeNode*& pNode)
		{
			AVLTreeNode* pTempNode = pNode->pLeftChild;
			pNode->pLeftChild = pTempNode->pRightChild;
			pTempNode->pRightChild = pNode;
			// 更新两个节点的深度
			pNode->deep = std::max(Deep(pNode->pLeftChild), Deep(pNode->pRightChild)) + 1;
			pTempNode->deep = std::max(Deep(pTempNode->pLeftChild), Deep(pTempNode->pRightChild)) + 1;
			pNode = pTempNode;
		}

		/// <summary>
		/// 插入位置在左儿子的右子树，双旋转逻辑(左右旋)
		/// </summary>
		/// <param name="pNode"></param>
		void DoubleWithLeftChild(AVLTreeNode*& pNode)
		{
			// 先左旋
			RotateWithRightChild(pNode->pLeftChild);
			// 再右旋
			RotateWithLeftChild(pNode);
		}

		/// <summary>
		/// 插入位置在右儿子的右子树，单旋转逻辑（左旋）
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithRightChild(AVLTreeNode*& pNode)
		{
			AVLTreeNode* pTempNode = pNode->pRightChild;
			pNode->pRightChild = pTempNode->pLeftChild;
			pTempNode->pLeftChild = pNode;
			// 更新两个节点的深度
			pNode->deep = std::max(Deep(pNode->pLeftChild), Deep(pNode->pRightChild)) + 1;
			pTempNode->deep = std::max(Deep(pTempNode->pLeftChild), Deep(pTempNode->pRightChild)) + 1;
			pNode = pTempNode;
		}

		/// <summary>
		/// 插入位置在右儿子的左子树，双旋转逻辑(右左旋)
		/// </summary>
		/// <param name="pNode"></param>
		void DoubleWithRightChild(AVLTreeNode*& pNode)
		{
			// 先右旋
			RotateWithLeftChild(pNode->pRightChild);
			// 再左旋
			RotateWithRightChild(pNode);
		}

		/// <summary>
		/// 克隆树节点
		/// </summary>
		/// <param name="pNode"></param>
		AVLTreeNode* Clone(AVLTreeNode* pNode) const
		{
			if (pNode == nullptr) { return nullptr; }
			return new AVLTreeNode(pNode->elementData, Clone(pNode->pLeftChild), Clone(pNode->pRightChild), pNode->deep);
		}

		/// <summary>
		/// 最大深度差
		/// </summary>
		/// <returns></returns>
		int MaxDepthDiff(AVLTreeNode* pNode) const
		{
			int deepDiff = std::abs(Deep(pNode->pLeftChild) - Deep(pNode->pRightChild));
			int deepLeftDiff = pNode->pLeftChild == nullptr ? -1 : MaxDepthDiff(pNode->pLeftChild);
			int deepRightDiff = pNode->pRightChild == nullptr ? -1 : MaxDepthDiff(pNode->pRightChild);
			return std::max(std::max(deepLeftDiff, deepRightDiff), deepDiff);
		}

	private:
		/// <summary>
		/// 二叉搜索树根节点
		/// </summary>
		AVLTreeNode* m_Root{ nullptr };
	};
}
