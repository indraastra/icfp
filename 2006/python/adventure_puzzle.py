import sys
import pprint
 
import xml.dom.minidom
from xml.dom.minidom import Node

def extract_text(node):
    if len(node.childNodes) == 1:
        c_node = node.childNodes[0] 
        if c_node.TEXT_NODE == c_node.nodeType:
            text = c_node.data.strip()
            if text:
                return text

def get_attributes(node, attr):
    return list(children(node, node_name=attr))

def get_name(node):
    return extract_text(get_attributes(node, "name")[0])

def get_description(node):
    return extract_text(get_attributes(node, "description")[0])

def get_adjectives(node):
    adjectives = get_attributes(node, "adjectives")[0]
    adjectives = get_attributes(adjectives, "adjective")
    return [extract_text(adjective) for adjective in adjectives]

def get_condition(node):
    return get_attributes(node, "condition")

def get_piled_on(node):
    return get_attributes(node, "piled_on")
 
def get_item_attributes(node):
    return ( get_name(node), get_description(node), 
             get_adjectives(node), get_condition(node),
             get_piled_on(node) )

def find_first(filter, iterable):
    for i in iterable:
        if filter(i):
            return i
    return None

def children(node, node_name=None):
    for c in node.childNodes:
        if node_name and c.nodeName != node_name:
            continue
        else:
            yield c

class Simplify:
    def strategy1():
        pass

    def strategy2():
        pass

    def run():
        if not solved():
            self.strategy1()
            self.strategy2()
            ...

if __name__ == "__main__":
    doc = xml.dom.minidom.parse(sys.argv[1])
    item_nodes = doc.getElementsByTagName("item")
    for item in item_nodes:
        name, description, adjectives, condition, piled_on = get_item_attributes(item)
        print name
        print description
        print adjectives
        print condition
        print

        # make item
        # |--- make condition

