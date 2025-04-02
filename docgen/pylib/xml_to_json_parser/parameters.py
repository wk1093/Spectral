import sys
import xml.etree.ElementTree as ET
import logging

import settings
import parse_description


# container for one parameter information used for for classes and functions
# also used for enum values
class Parameter:
    def __init__(self, name = "", description = "", group = "", defaultValue = ""):
        self.parametergroup = group # = template | in | out | enum
        self.name = name  # only set for in, out and enum
        self.decltype = "" # set for in, out - aka normal parameters
        self.description = description
        self.defaultValue = defaultValue # only set for in and out, and only in case there is one and for enums
        self.reference = "" # only set for enum values


    def parseParameterItemNodes(self, itemNode, references):
        nameListNode = itemNode.find("parameternamelist")
        if nameListNode is not None:
            nameNode = nameListNode.find("parametername") # hoping we will never really have a list there...
            if nameNode is not None:
                self.name = nameNode.text
                self.parametergroup = nameNode.get("direction", "")

        descrNode = itemNode.find("parameterdescription")
        if descrNode is not None:
            self.description = parse_description.parseDescription \
                (descrNode, references, None)
                
        return self.name, self.description
                
    def parseSingleEnumValue(self, enumValueNode, references):
        oldRef = enumValueNode.get("id")
        self.reference = settings.referenceDictionary[oldRef]

        self.name = enumValueNode.findtext("name", "")
        self.defaultValue = enumValueNode.findtext("initializer", "")
        self.description = parse_description.parseDescription \
            (enumValueNode.find("briefdescription"), references, None)
        self.description += parse_description.parseDescription \
            (enumValueNode.find("detaileddescription"), references, None)
        
        self.parametergroup = "enum"


    def __str__(self):
        str = f"p: group = {self.parametergroup}; name = {self.name};" + \
            f"type = {self.decltype}; descr = {self.description}; " + \
            f"defaultValue= {self.defaultValue}; reference= {self.reference}" #\
            # % (self.parametergroup, self.name, self.decltype, \
            # self.description, self.defaultValue, self.reference)
        return str

# container for the parameter objects... 
class ParameterContainer:
    def __init__(self):
        self.parameters = []
        self.templateParameters = []
        self.returnParameter = None
    
    def parseParameterList(self, parameterListNode, references):
        notAParameterDescription = ""
        kind = parameterListNode.get("kind")
        for item in parameterListNode.findall('parameteritem'):
            newParam = Parameter()
            name, description = newParam.parseParameterItemNodes(item, references)
            
            if kind == "templateparam":
                self.templateParameters.append(newParam)
            elif kind == "param":
                self.parameters.append(newParam)
            else: # It is a bit ugly to parse retval and exceptions here, because it doens't really belong here
                notAParameterDescription += "\n> **" + kind + ' ' + name +\
                    ":** " + description + "\n"
            
        return notAParameterDescription

    def setTypeOfParam(self, type, parameterName, defaultValue, list, addIfNotFound):        
        # search for correct parameterName in parameterList
        foundParameter = False
        for p in list:
            if parameterName == p.name:
                p.decltype = type
                p.defaultValue = defaultValue
                foundParameter = True
                break
            
            # This is because with templates, the type and name could be mixed up
            if type == p.name:
                p.decltype = parameterName
                p.defaultValue = defaultValue
                foundParameter = True
                break
        
        if not foundParameter and addIfNotFound:
            logging.info("parameters: I could not find parameter for description of " + \
                parameterName + " (" + type + ") and will now create one")
            newParam = Parameter()
            newParam.name = parameterName
            newParam.decltype = type
            newParam.defaultValue = defaultValue
            list.append(newParam)

    def getElementFromParamNode(self, identifier, paramNode, references):
        element = ""
        elementNode = paramNode.find(identifier)
        if elementNode is not None:
            # I know its not a para node, but it should still work 
            # executing only a small subset of that function
            element = parse_description.parseParaRecursive\
                (elementNode, element, references, None) 
        return element

    # only called for parameters
    def addTypesToParams(self, memberNode, references):
        for p in memberNode.findall('param'):
            type = self.getElementFromParamNode('type', p, references)
            
            name = self.getElementFromParamNode('declname', p, references)
                
            defaultValue = self.getElementFromParamNode('defval', p, references)
            
            if name != "":
                self.setTypeOfParam(type, name, defaultValue, self.parameters, True)
                
    # only called for templates
    def addTypesToTemplates(self, memberNode, references):
        for p in memberNode.findall('param'):
            type = self.getElementFromParamNode('type', p, references)
            if not type:
                return
            templateSubParts = type.split(" ")
            if len(templateSubParts) < 2:
                name = self.getElementFromParamNode('declname', p, references)
                if name != "":
                    self.setTypeOfParam(templateSubParts[0], \
                        name, "", self.templateParameters, False)
                else:
                    self.setTypeOfParam("", \
                        templateSubParts[0], "", self.templateParameters, False)
            else:
                self.setTypeOfParam(templateSubParts[0], \
                    templateSubParts[1], "", self.templateParameters, False)
                
    def addTypes(self, memberNode, references):
        templateParamNode = memberNode.find("templateparamlist")
        if templateParamNode is not  None:
            self.addTypesToTemplates(templateParamNode, references)
            
        self.addTypesToParams(memberNode, references)
        
    def setReturnParameter(self, type):
        if self.returnParameter is None:
            self.returnParameter = Parameter()
        self.returnParameter.parametergroup = "return"
        self.returnParameter.decltype = type
        
        
    def addReturnDescription(self, description):
        if self.returnParameter is None:
            self.returnParameter = Parameter()
        self.returnParameter.description = description
            
        
class EnumContainer:
    def __init__(self):
        self.values = []
    
    def addEnumValues(self, node, references):
        for enumValue in node.findall('enumvalue'):
            newParam = Parameter()
            newParam.parseSingleEnumValue(enumValue, references)
            self.values.append(newParam)

