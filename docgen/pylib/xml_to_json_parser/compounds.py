import logging

import settings
import parse_description
import parameters
import extract_namespace


# Compounds are basically everything, namespaces, classes, functions, variables...
class Compound:

    def __init__(self):
        self.id = ""
        self.name = ""
        self.surroundingNamespace = ""
        self.kind = ""
        self.briefDescription = ""
        self.detailedDescription = ""
        self.references = set()
        self.parameters = parameters.ParameterContainer()
        self.enumValues = parameters.EnumContainer()
        self.protectionLevel = ""
        self.qualifiers = []

    def fill(self, node, compoundName):
        logging.debug("Creating compound %s", compoundName)
        
        tempName = node.findtext('name', "")
        self.surroundingNamespace, self.name = \
            extract_namespace.splitNamespacesAndNames(compoundName, tempName)

        self.kind = node.get('kind', "")

        self.protectionLevel = node.get('prot', "")

        oldrefid = node.get('id')
        self.id = settings.referenceDictionary[oldrefid]

        # descriptions
        if node.find('briefdescription'):
            self.briefDescription = parse_description.parseDescription\
                (node.find('briefdescription'), self.references, self.parameters)
        if node.find('detaileddescription'):
            self.detailedDescription = parse_description.parseDescription\
                (node.find('detaileddescription'), self.references, self.parameters)

        # node type is used for return values for functions and variable types.
        # I only want to include it once!
        if self.kind != "variable" and self.kind != "typedef":
            self.parameters.addTypes(node, self.references)
        if self.kind == "enum":
            self.enumValues.addEnumValues(node, self.references)
            
        for q in settings.qualifierAttributes:
            qualifierValue = node.get(q, "")
            if qualifierValue != "" and qualifierValue != "no" and \
                qualifierValue != "non-virtual":
                self.qualifiers.append(q)


    def __str__(self):
        basics = "name = %s\nid = %s\nkind = %s\nbrief = %s\ndetailed = %s\n" \
            % (self.name, self.id, self.kind, self.briefDescription, \
            self.detailedDescription)

        referenceString = "references = "
        for r in self.references:
            referenceString += r + ", "
        referenceString += "\n"

        paramtersString = "Paramters: " + str(self.parameters) + "\n"

        return basics + referenceString + paramtersString


# Type is here either namespace, class or struct
class Type(Compound):

    def __init__(self):
        super(Type, self).__init__()
        self.innerGroups = []

    def addInnerTypes(self, nodes, name):
        if len(nodes) > 0:
            innerGroup = Group(name)
            for inner in nodes:
                innerReference = settings.referenceDictionary[inner.get('refid')]
                innerGroup.addMember(innerReference)
            self.innerGroups.append(innerGroup)
            
    def addInheritance(self, direction, nodes):
        if direction == "derived":
            self.derived = []
            inheritanceObject = self.derived
        else:
            self.base = []
            inheritanceObject = self.base
        
        for inner in nodes:
            text = parse_description.parseReference(inner, "", self.references)
            inheritanceObject.append(text)
            
    def fill(self, node):
        
        compoundName = node.findtext('compoundname')
        super().fill(node, compoundName)

        titleNode = node.find('title')
        if titleNode is not None:
            self.title = titleNode.text

        # get references from inner classes/namespaces (/structs = class here)
        innerTypeNodes = node.findall('innerclass')
        self.addInnerTypes(innerTypeNodes, 'Classes')

        innerTypeNodes = node.findall('innernamespace')
        self.addInnerTypes(innerTypeNodes, 'Namespaces')
        
        inheritanceNode = node.findall('derivedcompoundref')
        if inheritanceNode:
            self.addInheritance("derived", inheritanceNode)
            
        inheritanceNode = node.findall('basecompoundref')
        if inheritanceNode:
            self.addInheritance("base", inheritanceNode)


    def __str__(self):
        superString = super().__str__()
        groupString = "groups: "
        for group in self.innerGroups:
            groupString += str(group) + "\n"

        return superString + groupString


# Members are all Compounds, that are not Types (that is member variables, functions, typedefs, enums...)
# every Member is indirectly part of a Type by using Groups (see below)
class Member(Compound):

    def __init__(self):
        super(Member, self).__init__()
        # self.innerType = ""
        self.arguments = ""
        self.initializer = ""
        self.variableType = ""

    def fill(self, node):
        memberName = node.findtext('definition', "")
        if node.get("kind", "") == "enum":
            memberName = node.findtext('name', "")
        super().fill(node, memberName)

        type = ""
        if(node.find('type') is not None):
            type = parse_description.parseParaRecursive\
                (node.find('type'), type, self.references, None)
        else:
            logging.warning("Member.fill: Node 'type' not found in " + memberName)
            
        self.variableType = type
        if self.kind == "function" and type != "":
            self.parameters.setReturnParameter(type)
        
        self.arguments = node.findtext('argsstring')
        self.initializer = node.findtext('initializer')


    def __str__(self):
        superString = super().__str__()

        returnString = "arguments = %s\ninitializer = %s\nvariableType = %s\n" \
            % (self.arguments, self.initializer, self.variableType)

        return superString + returnString


# doxygen groupings of various kinds
# Bridge between Types and their Members
class Group:

    def __init__(self, newName):
        self.name = newName
        self.innerMemberRefs = []

    def addMember(self, reference):
        self.innerMemberRefs.append(reference)

    def __str__(self):
        returnString = "groupname:%s \n" % self.name
        for member in self.innerMemberRefs:
            returnString += str(member) + "\n"

        return returnString
