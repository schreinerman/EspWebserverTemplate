import os
import distutils.dir_util
import shutil
import argparse
import json
import fileinput

parser = argparse.ArgumentParser(description='Generate a new webserver project')
parser.add_argument('--name',required=True, help='project name')
parser.add_argument('--conf',required=True, help='application config file')
args = parser.parse_args()

scriptPath = os.path.dirname(os.path.realpath(__file__))

from_dir = os.path.join(scriptPath,"template")
to_dir = args.name
appconfigJson = os.path.join(args.name,"appconfig.json")

cmdUpdateConfig = os.path.join(args.name,"utils","update-appconfig.py")
cmdUpdateFavicon = os.path.join(args.name,"utils","update-favicon.py")
cmdUpdateWebstore = os.path.join(args.name,"utils","update-webstore.py")

distutils.dir_util.copy_tree(from_dir, to_dir)
os.rename(os.path.join(args.name,"template.ino"),os.path.join(args.name,args.name + ".ino"))
shutil.copyfile(args.conf,appconfigJson)

os.system("python3 " + cmdUpdateConfig)
os.system("python3 " + cmdUpdateFavicon)
os.system("python3 " + cmdUpdateWebstore)
