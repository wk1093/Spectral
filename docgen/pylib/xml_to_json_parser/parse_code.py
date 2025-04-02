import sys
import xml.etree.ElementTree as ET
import logging

import parse_description

def replaceSpaceNodes(node):
    nodeAsString = ET.tostring(node, encoding='unicode')

    nodeAsString = nodeAsString.replace("<sp/>", ' ')
    nodeAsString = nodeAsString.replace("<sp />", ' ')

    node = ET.fromstring(nodeAsString)
    return node


def parseProgramListingNode(node, references):
    codeAsText = ""
    for codelineNode in node.findall('codeline'):
        if codeAsText:
            codeAsText += "\n"
        for highlightnode in codelineNode.findall('highlight'):
            highlightnode = replaceSpaceNodes(highlightnode)
            if highlightnode.text:
                codeAsText += highlightnode.text

            for inner in highlightnode:
                if inner.tag == "ref":
                    codeAsText = parse_description.parseReference(inner, codeAsText, references)
                else:
                    logging.warning("parse_code: I got an element in a codeline" +\
                        " I didn't expect: Tag = %s", inner.tag)

                if inner.tail:
                    codeAsText += inner.tail

    return codeAsText
