
#include <iostream>
#include "JoyTree/JoyTree.h"

int main()
{
	std::cout << "Initialize BinarySearchTree" << std::endl;
	Joy::BinarySearchTree<int> tree;
	tree.Insert(6);
	tree.Insert(4);
	tree.Insert(1);
	tree.Insert(3);
	tree.Insert(5);
	tree.Insert(8);
	tree.Insert(7);
	tree.Insert(10);

	tree.PrintTree();

	std::cout << "Clone BinarySearchTree:" << std::endl;
	Joy::BinarySearchTree<int> copyTree(tree);
	copyTree.PrintTree();

	std::cout << "Add new TreeNode: 9" << std::endl;
	tree.Insert(9);
	tree.PrintTree();

	std::cout << "FindMin: " << tree.FindMin() << std::endl;
	std::cout << "FindMax: " << tree.FindMax() << std::endl;
	std::cout << "Contain: 2 -> is " << tree.Contain(2) << std::endl;
	std::cout << "Contain: 7 -> is " << tree.Contain(7) << std::endl;

	std::cout << "Remove TreeNode: 10" << std::endl;
	tree.Remove(10);
	tree.PrintTree();

	std::cout << "Remove TreeNode: 7" << std::endl;
	tree.Remove(7);
	tree.PrintTree();

	std::cout << "Remove TreeNode: 4" << std::endl;
	tree.Remove(4);
	tree.PrintTree();

	std::cout << "Print Copy BinarySearchTree:" << std::endl;
	copyTree.PrintTree();
	std::cout << "Remove CopyTree: 6" << std::endl;
	copyTree.Remove(6);
	copyTree.PrintTree();

	std::cin.get();
}