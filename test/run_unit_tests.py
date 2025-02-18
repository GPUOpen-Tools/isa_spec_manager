# Copyright (c) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
from optparse import OptionParser
import os
import subprocess

# Option help messages.
HELP_MSG_AMDISA_TEST_BIN_PATH = 'AMDISA unit test executable path'
HELP_XML_FILE_PATH            = 'AMDISA XML file path'
HELP_XML_DIR_PATH_OUTPUT      = 'AMDISA XML files directory path'

# Error messages.
ERR_MSG_INVALID_ARGUMENTS            = 'Invalid arguments provided: '
ERR_MSG_NO_TEST_FILE_OR_DIR_PROVIDED = 'No AMDISA XML test file or file directory provided'
ERR_MSG_AMDISA_TEST_BIN_NOT_PROVIDED = 'AMDISA test executable not provided'
ERR_MSG_EXE_DOES_NOT_EXIST           = 'AMDISA test executable does not exists: {}'
ERR_MSG_XML_FILE_DOES_NOT_EXIST      = 'AMDISA XML file path does not exists: {}'
ERR_MSG_NOT_XML_FILE                 = 'The provided file type is not supported'
ERR_MSG_NO_XML_FILES_FOUND           = 'No XML files found in the provided test directory: {}'
ERR_MSG_XMLS_DIR_NOT_EXIST           = 'AMDISA XMLs directory path does not exists: {}'
ERR_MSG_XML_UNIT_TEST_FAIL           = 'Unit tests FAILED for {}'

# Info messages.
INFO_ALL_XMLS_UNIT_TESTS_PASSED = "Unit tests PASSED!"

# Executable command
AMDISA_TEST_CMD = '{} -I {}'

# Parse arguments
def parse_args():

    # Setup OptionParser
    parser = OptionParser()
    parser.add_option("-e", "--test-path", dest="amdisa_test", help=HELP_MSG_AMDISA_TEST_BIN_PATH)
    parser.add_option("-f", "--xml-file", dest="xml_file_path", help=HELP_XML_FILE_PATH)
    parser.add_option("-d", "--dir", dest="xmls_dir_path", help=HELP_XML_DIR_PATH_OUTPUT)
    (options, args) = parser.parse_args()
    
    # Initialize parse variables
    is_parse_successful = False
    error_message = ERR_MSG_INVALID_ARGUMENTS
    exec_path = "" 
    xml_file = ""
    xml_files_dir = ""
    
    if not options.amdisa_test:
        # Path to amdisa_test required
        error_message += ERR_MSG_AMDISA_TEST_BIN_NOT_PROVIDED
    else: 
        # Parse amdisa_test path and check if exists
        if not options.amdisa_test is None:
            if os.path.exists(options.amdisa_test):
                is_parse_successful = True
                exec_path = options.amdisa_test
            else:
                error_message += ERR_MSG_EXE_DOES_NOT_EXIST.format(options.amdisa_test)

    # Parse xml file path or xmls directory path and check if exists
    if (is_parse_successful and (options.xml_file_path or options.xmls_dir_path)):
        if not options.xml_file_path is None:
            if os.path.exists(options.xml_file_path):
                xml_file = options.xml_file_path
            else:
                is_parse_successful = False
                error_message += ERR_MSG_XML_FILE_DOES_NOT_EXIST.format(options.xml_file_path)

        if not options.xmls_dir_path is None:
            if os.path.exists(options.xmls_dir_path):
                xml_files_dir = options.xmls_dir_path
            else:
                is_parse_successful = False
                error_message += ERR_MSG_XMLS_DIR_NOT_EXIST.format(options.xmls_dir_path)
    else:
        # Fail if no xml file path or xmls dir path is provided
        is_parse_successful = False
        error_message += ERR_MSG_NO_TEST_FILE_OR_DIR_PROVIDED

    # Print help
    if not is_parse_successful:        
        parser.print_help()

    return is_parse_successful, error_message, exec_path, xml_file, xml_files_dir

# Perform unit tests on AMDISA XML file
def test_xml_file(exec_path, spec_file):
    is_pass = False
    file_name = os.path.basename(spec_file).split('/')[-1]
    error_message = ERR_MSG_XML_UNIT_TEST_FAIL.format(file_name)
    if file_name.endswith(".xml"):
        cli_command = AMDISA_TEST_CMD.format(exec_path, spec_file).split()
        test = subprocess.Popen(cli_command)
        test.communicate()
        is_pass = (test.returncode == 0)
    else:
        # Fail if an AMDISA XML file is not provided
        error_message += ": " + ERR_MSG_NOT_XML_FILE

    return is_pass, error_message

# Perform unit tests on all the AMDISA XML files in the given directory
def test_xml_files_dir(exec_path, xml_files_dir):
    is_pass = False
    error_message = ERR_MSG_NO_XML_FILES_FOUND.format(xml_files_dir)
    for xml_file in os.listdir(xml_files_dir):
        if xml_file.endswith(".xml"):
            is_pass, error_message = test_xml_file(exec_path, os.path.join(xml_files_dir, xml_file))
            if not is_pass:
                # Exit even if one of the AMDISA XML file fails the unit tests
                break
    return is_pass, error_message

def main():
    is_pass = False
    error_message = ""
    
    # Parse arguments
    is_pass, error_message, exec_path, xml_file, xml_files_dir = parse_args()

    # Test AMDISA XML file or AMDISA XML directory
    if is_pass:
        if xml_file:
            is_pass, error_message = test_xml_file(exec_path, xml_file)
        if xml_files_dir:
            is_pass, error_message = test_xml_files_dir(exec_path, xml_files_dir)
    
    if is_pass:
        print(INFO_ALL_XMLS_UNIT_TESTS_PASSED)
    else:
        print(error_message)
        exit(1)

if __name__ == "__main__":
    main()
