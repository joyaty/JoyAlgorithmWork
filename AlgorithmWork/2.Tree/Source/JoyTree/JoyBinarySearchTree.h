/**
 * 自定义ADT - 二叉搜索树
 * 二叉搜索树的定义：二叉搜索树是有特殊性质的二叉树
 * 特殊性质：对于树中的每个节点X，左子树上的每个节点都比节点X小，右子树上的每个节点都比节点X大。即，中序遍历下，节点元素是有序的。
 */

#pragma once

#include <iostream>

namespace Joy
{
	/// <summary>
	/// 二叉搜索树
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class BinarySearchTree
	{
	public:
		/// <summary>
		/// 默认构造函数
		/// </summary>
		BinarySearchTree()
			: m_Root(nullptr)
		{
		}

		/// <summary>
		/// 拷贝构造函数
		/// </summary>
		/// <param name="rhs"></param>
		BinarySearchTree(const BinarySearchTree& rhs)
		{
			operator=(rhs);
		}

		~BinarySearchTree() 
		{
			MakeEmpty();
		}

		/// <summary>
		/// 查找二叉搜索树的最大元素，有空树异常
		/// </summary>
		/// <returns></returns>
		const T& FindMax() const
		{
			return FindMax(m_Root)->elementData;
		}

		/// <summary>
		/// 查找二叉搜索树的最小元素，有空树异常
		/// </summary>
		/// <returns></returns>
		const T& FindMin() const noexcept
		{
			return FindMin(m_Root)->elementData;
		}

		/// <summary>
		/// 二叉搜索树是否包含指定元素
		/// </summary>
		/// <param name="element"></param>
		/// <returns></returns>
		bool Contain(const T& element) const
		{
			return Contain(element, m_Root);
		}

		/// <summary>
		/// 二叉搜索树是否为空
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const
		{
			return m_Root == nullptr;
		}

		/// <summary>
		/// 打印二叉搜索树
		/// </summary>
		void PrintTree() const
		{
			std::cout << "InOrder==========" << std::endl;
			PrintTreeInOrder(m_Root);
			std::cout << "PreOrder==========" << std::endl;
			PrintTreePreOrder(m_Root);
			std::cout << "PostOrder==========" << std::endl;
			PrintTreePostOrder(m_Root);
		}

		/// <summary>
		/// 清空二叉搜索树
		/// </summary>
		void MakeEmpty()
		{
			MakeEmpty(m_Root);
		}

		/// <summary>
		/// 向二叉搜索树添加一个元素节点
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
			Insert(element, m_Root);
		}

		/// <summary>
		/// 从二叉搜索树删除元素节点
		/// </summary>
		/// <param name="element"></param>
		void Remove(const T& element)
		{
			Remove(element, m_Root);
		}
		
		/// <summary>
		/// 重写赋值操作符
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		const BinarySearchTree& operator=(const BinarySearchTree& rhs)
		{
			MakeEmpty(m_Root);
			m_Root = Clone(rhs.m_Root);
			return *this;
		}

	private:
		/// <summary>
		/// 二叉树组成节点
		/// </summary>
		struct BinaryTreeNode
		{
			/// <summary>
			/// 节点数据
			/// </summary>
			T elementData;
			/// <summary>
			/// 左子节点
			/// </summary>
			BinaryTreeNode* leftChild{ nullptr };
			/// <summary>
			/// 右子节点
			/// </summary>
			BinaryTreeNode* rightChild{ nullptr };

			BinaryTreeNode(const T& element, BinaryTreeNode* leftNode, BinaryTreeNode* rightNode)
				: elementData(element), leftChild(leftNode), rightChild(rightNode)
			{
			}
		};

	private:
		/// <summary>
		/// 检查二叉搜索树是否存在数据节点element
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		/// <returns></returns>
		bool Contain(const T& element, BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr)
			{
				return false;
			}
			if (pNode->elementData < element)
			{
				return Contain(element, pNode->rightChild);
			}
			if (element < pNode->elementData)
			{
				return Contain(element, pNode->leftChild);
			}
			return true;
		}

		/// <summary>
		/// 查找二叉搜索树的最小节点，一直遍历左子树，递归方法
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		BinaryTreeNode* FindMin(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr)
			{
				return nullptr;
			}
			if (pNode->leftChild == nullptr)
			{
				return pNode;
			}
			return FindMin(pNode->leftChild);
		}

		/// <summary>
		/// 查找二叉搜索树的最大节点，一直遍历右子树，非递归方法
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		BinaryTreeNode* FindMax(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr)
			{
				return nullptr;
			}
			while (pNode->rightChild != nullptr)
			{
				pNode = pNode->rightChild;
			}
			return pNode;
		}

		/// <summary>
		/// 插入新元素节点到二叉搜索树
		/// </summary>
		/// <param name="element">新元素数据</param>
		/// <param name="pNode">当前位置节点的引用，即是当前节点，也是父节点的子节点引用</param>
		void Insert(const T& element, BinaryTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				pNode = new BinaryTreeNode(element, nullptr, nullptr);
			}
			else if (pNode->elementData < element)
			{
				Insert(element, pNode->rightChild);
			}
			else if(element < pNode->elementData)
			{
				Insert(element, pNode->leftChild);
			}
		}

		/// <summary>
		/// 移除二叉搜索树的某个元素节点
		/// </summary>
		/// <param name="element">待删除的元素</param>
		/// <param name="pNode">即是当前节点引用，也是父节点的子节点引用</param>
		void Remove(const T& element, BinaryTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}

			if (pNode->elementData < element)
			{
				Remove(element, pNode->rightChild);
			}
			else if (element < pNode->elementData)
			{
				Remove(element, pNode->leftChild);
			}
			else
			{
				// 有左右两个子树
				if (pNode->leftChild != nullptr && pNode->rightChild != nullptr)
				{
					// 右子树最小节点上提到删除节点，然后删除右子树的最小节点
					pNode->elementData = RemoveMin(pNode->rightChild);
				}
				else
				{
					// 叶子节点或者只有一个子树，保留子树关联到父节点上，移除当前节点
					BinaryTreeNode* pTempNode = pNode;
					pNode = pNode->leftChild != nullptr ? pNode->leftChild : pNode->rightChild;
					delete pTempNode;
					pTempNode = nullptr;
				}
			}
		}

		/// <summary>
		/// 删除二叉搜索树最小节点，返回最小节点的元素
		/// </summary>
		/// <param name="pNode">即是当前节点引用，也是父节点的子节点引用</param>
		/// <returns>返回删除的节点元素</returns>
		T RemoveMin(BinaryTreeNode*& pNode)
		{
			// 找到最小节点
			if (pNode->leftChild != nullptr)
			{
				return RemoveMin(pNode->leftChild);
			}
			else
			{
				// 移除最小节点，返回最小节点的元素
				BinaryTreeNode* pTempNode = pNode;
				T element = pTempNode->elementData;
				pNode = pNode->rightChild;
				delete pTempNode;
				pTempNode = nullptr;

				return element;
			}
		}

		/// <summary>
		/// 清空二叉搜索树
		/// </summary>
		/// <param name="pNode">即是当前节点引用，也是父节点的子节点引用</param>
		void MakeEmpty(BinaryTreeNode*& pNode)
		{
			if (pNode == nullptr)
			{
				return;
			}
			// 移除右节点
			MakeEmpty(pNode->rightChild);
			// 移除左节点
			MakeEmpty(pNode->leftChild);
			// 移除自身节点
			delete pNode;
			pNode = nullptr;
		}

		/// <summary>
		/// 打印二叉搜索树 - 中序遍历
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			PrintTreeInOrder(pNode->leftChild);
			std::cout << pNode->elementData << std::endl;
			PrintTreeInOrder(pNode->rightChild);
		}

		/// <summary>
		/// 打印二叉搜索树 - 前序遍历
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			std::cout << pNode->elementData << std::endl;
			PrintTreePreOrder(pNode->leftChild);
			PrintTreePreOrder(pNode->rightChild);
		}

		/// <summary>
		/// 打印二叉搜索树 - 后序遍历
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(BinaryTreeNode* pNode) const
		{
			if (pNode == nullptr) { return; }
			PrintTreePostOrder(pNode->leftChild);
			PrintTreePostOrder(pNode->rightChild);
			std::cout << pNode->elementData << std::endl;
		}

		/// <summary>
		/// 克隆二叉搜索树
		/// </summary>
		/// <param name="pNode"></param>
		BinaryTreeNode* Clone(BinaryTreeNode* pSrcNode) const
		{
			if (pSrcNode == nullptr) { return nullptr; }
			return new BinaryTreeNode(pSrcNode->elementData, Clone(pSrcNode->leftChild), Clone(pSrcNode->rightChild));
		}

	private:
		/// <summary>
		/// 根节点
		/// </summary>
		BinaryTreeNode* m_Root{ nullptr };
		/// <summary>
		/// 树节点数量
		/// </summary>
		int m_Count{ 0 };
		/// <summary>
		/// 树的深度
		/// </summary>
		int m_Deep{ 0 };
	};
}