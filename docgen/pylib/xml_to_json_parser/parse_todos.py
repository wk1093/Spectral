import logging

import parse_description



def parseTodo(node, description, references):
    if node.findtext("xreftitle") != "Todo":
        logging.error("parse_todo: Found xrefsect without Todo title!")
        return description
    innerDescriptionNode = node.find("xrefdescription")
    text = parse_description.parseDescription\
        (innerDescriptionNode, references, None)
    description = parse_description.addHighlightToDescription\
        ("[Todo][todo]", description, text)
    
    references.add("todo")

    return description

def parseVariableList(variablelistNode, description, references):
    nodes = list(variablelistNode)
    
    for entry in nodes:
        if entry.tag == "varlistentry":
            termNode = entry.find("term")
            if termNode:
                text = parse_description.parseParaRecursive\
                    (termNode, "", references, None)
                description += "* " + text + '\n'
        elif entry.tag == "listitem":
            for p in entry.findall("para"):
                text = parse_description.parseParaRecursive\
                    (p, "", references, None)
                description += "    * " + text + '\n'
    
    return description
    