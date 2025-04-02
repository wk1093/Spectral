import re
import logging

def removeTemplateBracketSpaces(compoundName):
    templateBracketRegex = r"(<\s*)"
    compoundName = re.sub(templateBracketRegex, "<", compoundName)
    templateBracketRegex = r"(\s*>)"
    compoundName = re.sub(templateBracketRegex, ">", compoundName)
    
    return compoundName

def removeTemplateInnerSpaces(compoundName):
    templateSpaceRegex = r"<(\w+\s\w+)+>"
    match = re.search(templateSpaceRegex, compoundName)
    if not match:
        return compoundName
    
    innerTemplateString = match.string[match.start():match.end()]
    innerTemplateString = innerTemplateString.replace(' ', '-------')
    compoundName = compoundName[:match.start()] + innerTemplateString \
        + compoundName[(match.end()):]
        
    return compoundName
    
def restoreTemplateSpaces(compoundName):
    templateBracketRegex = r"<"
    compoundName = re.sub(templateBracketRegex, "< ", compoundName)
    templateBracketRegex = r">"
    compoundName = re.sub(templateBracketRegex, " >", compoundName)
    
    compoundName = compoundName.replace('-------', ' ')
        
    return compoundName

def addPointerSpaces(compoundName):
    pointerRegex = r"(?P<ptr>\*)(?P<text>\w+)"
    compoundName = re.sub(pointerRegex, "* \g<text>", compoundName)
    pointerRegex = r"(?P<text>\w+)(?P<ptr>\*)"
    compoundName = re.sub(pointerRegex, "\g<text> *", compoundName)
    
    return compoundName

# takes the whole compound name (that is with possible return values, 
# identifiers, templates, default values, etc) and returns the namespace and name
# of the compound
def splitNamespacesAndNames(compoundName, temporaryName):
    savetmp = compoundName
    
    compoundName = removeTemplateBracketSpaces(compoundName)
    compoundName = removeTemplateInnerSpaces(compoundName)
            
    allDefinitionElements = compoundName.split(" ")
    namespaceElements = [d for d in allDefinitionElements if "::" in d]
    
    name = compoundName
    if(len(namespaceElements) < 1):
        logging.info("extract_namespace: No namespace parts for name: " + compoundName)
    
    if len(namespaceElements) > 0:
        # This checks in which string the temporary name (name node in XML) is
        # contained and picks that one if existing
        if(temporaryName != ""):
            namesWithTemp = [d for d in namespaceElements if temporaryName in d]
            if len(namesWithTemp) < 1:
                name = namespaceElements[0]
            else:
                name = namesWithTemp[0]
        else:
            # no temporary sting given, best choice might be the first one:
            name = namespaceElements[0]       

    name = restoreTemplateSpaces(name)
    name = addPointerSpaces(name)
    
    allNameParts = name.split("::")
        
    logging.debug("extract_namespace: %s, %s", "::".join(allNameParts[:-1]), allNameParts[-1] + "( " + \
        savetmp + " )")
    return "::".join(allNameParts[:-1]), allNameParts[-1]