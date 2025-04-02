import sys
import xml.etree.ElementTree as ET
import json
import logging
import argparse

import settings
import parse_file
import compounds
import jsonSerialization as jS


def main():
    # arguments
    parser = argparse.ArgumentParser(description='Parse the Doxygen XML ' + \
        'output, create a dictionary with all members and save it to a JSON file.')
    
    parser.add_argument('input_directory', \
        help='The directory of the Doxygen XML output. Must contain index.xml', \
        default="./xml/")
    parser.add_argument('output_file', default="output.json", \
        help='The name of the JSON file that should be created')
    parser.add_argument('-l', dest='logfile', default="xml_to_json_parser.log", \
        help='The filename for the created logfile')
    parser.add_argument('--loglevel', dest='loglevel', default='info', \
        help='The level of logging (debug, info, warning, error or critical)' + \
        ' Default is info')
    
    args = parser.parse_args()
        
    # logging
    numericLogLevel = getattr(logging, args.loglevel.upper(), None)
    if not isinstance(numericLogLevel, int):
        raise ValueError('Invalid log level: %s' % args.loglevel)
    logging.basicConfig(filename=args.logfile, filemode='w', level=numericLogLevel)
    
    logging.info("arguments: " + str(args))

    # start setting up
    settings.init()

    # get path to index.xml
    indexFilename = "index.xml"

    if args.input_directory.endswith(indexFilename):
        settings.pathToXML = args.input_directory.replace(indexFilename, '')
        if not settings.pathToXML:
            # pick current directory if user just specified "index.xml"
            settings.pathToXML = '.'
    else:
        settings.pathToXML = args.input_directory


    # parse index.xml and ...
    tree = ET.parse(settings.pathToXML + "/index.xml")
    indexRoot = tree.getroot()
    # ... save all compound-references (with old references as keys and new ones as values)
    settings.setupReferences(indexRoot)

    # loop over all types referenced in index and parse them, creating objects
    # for every compound in them
    for compound in indexRoot:

        compoundKind = compound.get('kind')
        if(compoundKind != 'file' and \
            compoundKind != 'dir'):

            compoundpath = compound.get('refid') + ".xml"
            parse_file.parse(compoundpath)


    # save compound objects as json file
    file_object = open(args.output_file, 'w')
    json.dump(settings.compoundDictionary, file_object, cls=jS.MyEncoder, sort_keys=True, indent=4)

    for r, e in settings.referenceDictionary.items():
        logging.debug("References: r " + r + " -> " + e)

    logging.info("Missing references: %s", settings.missingReferences)

if __name__ == '__main__':
    main()
