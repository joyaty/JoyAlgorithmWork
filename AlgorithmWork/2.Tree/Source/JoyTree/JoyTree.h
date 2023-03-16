/**
 * 自定义ADT - 二叉搜索树
 * 实现STL中vector
 */

namespace Joy
{
	/// <summary>
	/// 二叉搜索树
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class BinarySearchTree
	{
	public:
		BinarySearchTree()
			: m_Root(nullptr), m_Count(0), m_Deep(0)
		{
		}

		BinarySearchTree(const BinarySearchTree& rhs)
			: m_Count(rhs.m_Count), m_Deep(rhs.m_Deep)
		{
			operator=(rhs);
		}
		~BinarySearchTree() {}

		const T& FindMax() const
		{
		}

		const T& FindMin() const
		{
		}

		bool Contain(const T& element) const
		{
			return Contain(element, m_Root);
		}
		bool IsEmpty() const
		{
			return m_Root == nullptr; 
		}

		void PrintTree() const
		{

		}

		void MakeEmpty()
		{
			m_Count = 0;
			m_Deep = 0;
			m_Root = nullptr;
		}
		void Insert(const T& element)
		{
		}
		void Remove(const T& element)
		{
		}

		const BinarySearchTree& operator=(const BinarySearchTree& rhs)
		{
			// TODO 深拷贝树
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

			BinarySearchTree(const T& element, BinaryTreeNode* leftNode, BinarySearchTree* rightNode)
				: elementData(elementData), leftChild(leftNode), rightChild(rightNode)
			{
			}
		};

	private:
		bool Contain(const T& element, BinaryTreeNode* pNode)
		{
			if (pNode == nullptr)
			{
				return false;
			}
			if (pNode->elementData == element)
			{
				return true;
			}
			return pNode->elementData < element ? Contain(element, pNode->leftChild) : Contain(element, pNode->rightChild);
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