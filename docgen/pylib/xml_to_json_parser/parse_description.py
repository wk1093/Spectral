import sys
import xml.etree.ElementTree as ET
import re
import logging

import settings
import parameters
import parse_code
import parse_todos

#attention: This will sadly destroy XML nodes
def checkForTemplateBrackets(text):
    bracketRegex = r"((?P<bracket><)(?P<text>\w+))"
    text = re.sub(bracketRegex, "< \g<text>", text)
    bracketRegex = r"((?P<text>\w+)(?P<bracket>>))"
    text = re.sub(bracketRegex, "\g<text> >", text)
    return text

def replaceNodesWithMarkdown(node):
    if(node == None):
        return node

    nodeAsString = ET.tostring(node, encoding='unicode')

    codeRegex = r"(<computeroutput>\s*)"
    nodeAsString = re.sub(codeRegex, "`", nodeAsString)
    codeRegex = r"(\s*<\/computeroutput>)"
    nodeAsString = re.sub(codeRegex, "`", nodeAsString)
    
    codeRegex = r"(<verbatim>\s*)"
    nodeAsString = re.sub(codeRegex, "\n\n```\n", nodeAsString)
    codeRegex = r"(\s*<\/verbatim>)"
    nodeAsString = re.sub(codeRegex, "\n```\n\n", nodeAsString)

    italicRegex = r"(<emphasis>\s*)"
    nodeAsString = re.sub(italicRegex, "*", nodeAsString)
    italicRegex = r"(\s*<\/emphasis>)"
    nodeAsString = re.sub(italicRegex, "*", nodeAsString)

    boldRegex = r"(<bold>\s*)"
    nodeAsString = re.sub(boldRegex, "**", nodeAsString)
    boldRegex = r"(\s*<\/bold>)"
    nodeAsString = re.sub(boldRegex, "**", nodeAsString)
    
    # Headings will be set to a fixed #### level here
    # TODO: Maybe find out, what the correct level would be?
    headingRegex = r"(<heading level=\"[0-9]\">\s*)"
    nodeAsString = re.sub(headingRegex, "#### ", nodeAsString)
    headingRegex = r"(\s*<\/heading>)"
    nodeAsString = re.sub(headingRegex, "", nodeAsString)
    
    node = ET.fromstring(nodeAsString)
    return node
    
# Markdown code icons ` and the brackets for links [] are in the wrong order
# This function searches for this occurence and changes it if neccessary
def fixCodeReferenceOrder(description):
    problemRegex = r"(?P<text>`\[\w+((-\w+)|(::\w+))*\])(?P<link>\[\w+(-\w+)*\]`)"
    match = re.search(problemRegex, description)
    if match is not None and len(match.group("text")) > 2:
        logging.info("fixCodeReferenceOrder: Convert " + description + " ...")
        correctReference = "[`" + match.group("text")[2:-1] + "`]" + match.group("link")[:-1]
        description = description.replace(match[0], correctReference)
        logging.info("fixCodeReferenceOrder: ... to " + description)
    
    return description


def parseReference(node, description, references):
    refText = node.text
    if refText is None:
        refText = ""
    refText = checkForTemplateBrackets(refText)

    refid = node.get('refid')
    if refid is None:
        return description + refText
        
    if refid in settings.referenceDictionary:
        refid = settings.referenceDictionary[refid]
        references.add(refid)
        description += "[" + refText + "]" + "[" + refid + "]"
    else:
        settings.missingReferences.add(refid)
        description += refText

    return description

def addHighlightToDescription(title, description, text):
    description += "> **" + title + ":** " + text + '\n'
    return description

def parseSimpleSect(node, description, references, parameterList):    
    text = parseDescription(node, references, None)
    
    kind = node.get('kind')
    if kind == "return":
        if parameterList is not None:
            parameterList.addReturnDescription(text)
        else:
            logging.error("parse_description: Got None as parameterList when "\
                " trying to add a return description")
    else:
        description = addHighlightToDescription(kind, description, text)
        # TODO: Maybe printing the kind directly is not the most beautiful way

    return description
    

def parseProgramListing(node, description, references):
    text = parse_code.parseProgramListingNode(node, references)
    description += "\n```c++\n" + text + "\n```\n"
    return description

def parseItemizedList(itemizedlistNode, description, references):
    description += "\n"
    for listitemNode in itemizedlistNode.findall('listitem'):
        for paraNode in listitemNode.findall('para'):
            description += "- "
            description = parseParaRecursive\
                (paraNode, description, references, None)
        
        description += " \n\n"
    
    return description
    

def parseULink(ulinkNode, description):
    url = ulinkNode.get('url');
    linkText = ulinkNode.text;
    description += "[" + linkText + "]" + "(" + url + ")\n"
    return description
    

def parseParaRecursive(paraNode, description, references, parameterList):
    if(paraNode == None):
        return ""
    paraNode = replaceNodesWithMarkdown(paraNode)

    if paraNode.text:
        description += paraNode.text

    for inner in paraNode:
        if inner.tag == "ref":
            description = parseReference(inner, description, references)
        elif inner.tag == "parameterlist":
            descrString = parameterList.parseParameterList(inner, references)
            if descrString: # in case it is not a real parameter in that parameterList (exceptions, retvals)
                description += descrString + "\n"
        elif inner.tag == "simplesect":
            description = parseSimpleSect(inner, description, references, parameterList)
            description += '\n'
        elif inner.tag == "programlisting":
            description = parseProgramListing(inner, description, references)
            description += '\n'
        elif inner.tag == "xrefsect": # These are the todos
            description = parse_todos.parseTodo(inner, description, references)
            description += '\n'
        elif inner.tag == "itemizedlist":
            description = parseItemizedList(inner, description, references)
            description += '\n'
        elif inner.tag == "ulink":
            description = parseULink(inner, description)
            description += '\n'
        elif inner.tag == "variablelist":
            description = parse_todos.parseVariableList(inner, description, references)
            description += '\n'
        elif inner.tag != "anchor": # anchors are used in todo list, but not by  me
            logging.error("parse_description: Could not parse node in param: " + inner.tag)

        if inner.tail:
            description += inner.tail
            description += '\n'

    description = checkForTemplateBrackets(description)
    description = fixCodeReferenceOrder(description)
    # get rid of additional newlines at end of description
    # TODO: It is a bit uncoordinated to add and remove \n all the time, 
    # but sometimes I want them and sometimes I don't... Is there a way to clean that up?
    # also no matter which combination I choose, something always gets broken
    while len(description) > 0 and description[-1] == '\n':
        description = description[:-1]
        
    
    return description

def parseDescription(decriptionNode, references, parameterList):
    description = ""
    if decriptionNode.find('para') is None:
        return description
    
    for paraNode in decriptionNode.findall('para'):
        description = parseParaRecursive\
            (paraNode, description, references, parameterList)
        description += '\n\n'

    return description
