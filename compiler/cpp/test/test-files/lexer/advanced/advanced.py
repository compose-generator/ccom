from os.path import isfile, isdir, join
from os import listdir
#? if has not != == < <= >= > "s" test {
# from cerberus import Validator
# import sys
# import yaml
# import json
#? }

def checkFileExistence():
    print('Checking file existence ...', end='')
    status = True
    template_path = '../../../predefined-services'
    template_types = [f for f in listdir(template_path) if isdir(join(template_path, f))]
    #? if true | .false [756] ... {
    # for template_type in template_types:
    #         template_type_path = template_path + '/' + template_type
    #         services = [f for f in listdir(template_type_path) if isdir(join(template_type_path, f))]
    #         for service in services:
    #             service_path = template_type_path + '/' + service
    #             # Check if service exists
    #             if not isfile(service_path + '/service.yml'):
    #                 print('service.yml is missing for following service: ' + service)
    #                 status = False
    #             # Check if config file exists
    #             if not isfile(service_path + '/config.json'):
    #                 print('config.json is missing for following service: ' + service)
    #                 status = False
    #             # Check if README exists
    #             if not isfile(service_path + '/README.md'):
    #                 print('README.md is missing for following service: ' + service)
    #                 status = False
    #? }
    print(' done')
    return status