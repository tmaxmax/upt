from __future__ import annotations
from collections.abc import Callable
from dataclasses import dataclass, field
from functools import reduce
from typing import Any


def flatten(it):
    try:
        yield from next(it)
        yield from flatten(it)
    except StopIteration:
        pass


print([*flatten(iter([[1, 2, 3], [4, 5], [6, 7, 8]]))])


@dataclass(eq=False)
class Node:
    data: Any
    children: list[Node] = field(default_factory=list)

    def inorder(self):
        match self.children:
            case []:
                yield self
            case [c, *rest]:
                yield from c.inorder()
                yield self
                yield from flatten(map(Node.inorder, rest))

    def preorder(self):
        yield self
        yield from flatten(map(Node.preorder, self.children))

    def postorder(self):
        yield from flatten(map(Node.postorder, self.children))
        yield self


class BNode(Node):
    def __init__(self, data, left: BNode = None, right: BNode = None):
        children = []
        if left is not None:
            children.append(left)
        if right is not None:
            children.append(right)
        super(BNode, self).__init__(data, children)

    def left(self) -> BNode:
        if len(self.children) > 0:
            return self.children[0]
        return None

    def right(self) -> BNode:
        if len(self.children) > 1:
            return self.children[1]
        return None


def for_each(fn, it):
    reduce(lambda _, e: fn(e), it, None)


def get_nodes_with_single_child(root: Node):
    return [*filter(lambda n: len(n.children) == 1, root.inorder())]


def count_nodes(root: Node):
    return sum(map(lambda _: 1, root.preorder()))


def print_indented(root: Node, level=0):
    if root is None:
        return
    print(("  " * level) + str(root.data))
    for_each(lambda n: print_indented(n, level + 1), root.children)


def tree_filter(root: Node | None, cond: Callable[[Node], bool]):
    if root is None or not cond(root):
        return None
    children = filter(
        lambda n: n is not None, map(lambda n: tree_filter(n, cond), root.children)
    )
    return Node(root.data, [*children])


def remove_value(root: Node | None, value):
    return tree_filter(root, lambda n: n.data != value)


test_tree = BNode(
    2,
    BNode(7, None, BNode(6, BNode(5, None, None), BNode(11, None, None))),
    BNode(4, None, None),
)

print("Test tree:")
print_indented(test_tree)
print(f"\nNumber of nodes: {count_nodes(test_tree)}", end="\n\n")
print("Nodes with single child:")
for_each(print_indented, get_nodes_with_single_child(test_tree))
print("\nTree with node with value 6 removed:")
print_indented(remove_value(test_tree, 6))
