import sys
import xml.etree.ElementTree as ET
import logging

import settings
import compounds

def parseMember(member, surroundingNamespace):
    currentMember = compounds.Member()

    currentMember.fill(member)
    
    # enums don't have their namespace saved in the xml
    if currentMember.surroundingNamespace == "":
        currentMember.surroundingNamespace = surroundingNamespace

    currentReference = currentMember.id
    if currentReference in settings.compoundDictionary:
        logging.warning("parse_file: This is strange... I already have" +\
            " added this member to the dictionary: %s", str(currentReference))
    settings.compoundDictionary[currentReference] = currentMember

    return currentReference


def parseSection(section, surroundingNamespace):
    kind = section.get('kind')
    currentGroup = compounds.Group(settings.kindToName[kind])
    if kind == "user-defined":
        currentGroup = compounds.Group(section.findtext('header', ""))

    for memberDef in section.findall('memberdef'):
        memberReference = parseMember(memberDef, surroundingNamespace)
        currentGroup.addMember(memberReference)

    return currentGroup


def parse(filename):
    tree =  ET.parse(settings.pathToXML + "/" + filename)
    root = tree.getroot()

    for compoundNode in root:

        currentElement = compounds.Type()

        currentElement.fill(compoundNode)

        for section in compoundNode.findall('sectiondef'):
            newNamespace = currentElement.name
            if currentElement.surroundingNamespace != "":
                newNamespace = currentElement.surroundingNamespace + "::" + newNamespace
            newGroup = parseSection(section, newNamespace)
            currentElement.innerGroups.append(newGroup)

        currentReference = currentElement.id
        if currentReference in settings.compoundDictionary:
            logging.warning("parse_file: This is strange... I already have" +\
                " added this member to the dictionary: %s", str(currentReference))
        settings.compoundDictionary[currentReference] = currentElement
