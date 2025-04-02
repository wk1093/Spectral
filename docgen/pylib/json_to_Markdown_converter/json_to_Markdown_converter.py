import sys
import json
import os
import logging
import argparse

import prepare_structure
import write_markdown
import settings

# small function to setup a dictionary combining the refs of enums (found as keys
# in the json file) and their values (that don't have their own objects in json)
# and therefore can't be found that easily
def setupEnumRefDictionary(objects):
    enumValueDictionary = dict()
    for ref, obj in objects.items():
        if obj["kind"] is None:
            continue
        elif obj["kind"] == "enum":
            for v in obj["enumValues"]["values"]:
                enumValueDictionary[v["reference"]] = ref
    return enumValueDictionary


def main():
    # arguments
    parser = argparse.ArgumentParser(description='Take the JSON file from ' + \
        'xml_to_json_parser and create Markdown files with the information.')
    
    parser.add_argument('input_file', default="output.json", \
        help='The JSON file to parse')
    parser.add_argument('output_directory', \
        help='The directory where the Markdown output should be created', \
        default="./Markdown/")
    parser.add_argument('-l', dest='logfile', default="json_to_Markdown_converter.log", \
        help='The filename for the created logfile')
    parser.add_argument('--loglevel', dest='loglevel', default='info', \
        help='The level of logging (debug, info, warning, error or critical)' + \
        ' Default is info')
    parser.add_argument('--undocumented', dest='undocumented', \
        help='If set it will also add members without documentation to the output.' + \
        ' Default: off', action='store_true')
    
    args = parser.parse_args()
    
    settings.setUndocumented(args.undocumented)
        
    # logging
    numericLogLevel = getattr(logging, args.loglevel.upper(), None)
    if not isinstance(numericLogLevel, int):
        raise ValueError('Invalid log level: %s' % args.loglevel)
    logging.basicConfig(filename=args.logfile, filemode='w', level=numericLogLevel)
    
    logging.info("arguments: " + str(args))

    # paths
    if not args.output_directory.startswith('/'): # path is relative!
        currentWorkingDir = os.getcwd()
        # outputPath = currentWorkingDir + '/' + outputPath
        logging.debug("output path: " + args.output_directory)
    if args.output_directory.endswith('/'):
        args.output_directory = args.output_directory[:-1]

    file_object = open(args.input_file, 'r')
    objects = json.load(file_object)

    # logging.debug("Got this: " + str(type(objects)))
    
    #start with setting up
    objects = prepare_structure.prepare_structure(objects, args.output_directory)
    
    enumValueDictionary = setupEnumRefDictionary(objects)
    
    # fill with contents
    for ref, obj in objects.items():
        if obj["kind"] is None:
            logging.warning("Found none kind: " + ref + " so it will not be added")
            continue
        
        kind = obj["kind"]
        if kind == "namespace" or kind == "class" or kind == "page" or kind == "struct":
            if obj["mdFilename"] == "":
                logging.warning("Still missing filename " + \
                    obj["surroundingNamespace"] + "::" + obj["name"])
            write_markdown.writeType(objects, obj, enumValueDictionary)
                
                
if __name__ == '__main__':
    main()
