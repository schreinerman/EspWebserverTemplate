import os
import distutils.dir_util
import argparse
import json
import fileinput

dictCVarType = {
        "String32":"char*",
        "String64":"char*",
        "String128":"char*",
        "UInt8":"uint8_t",
        "Int8":"int8_t",
        "UInt16":"uint16_t",
        "Int16":"int16_t",
        "UInt32":"uint32_t",
        "Int32":"int32_t",
        "UInt64":"uint64_t",
        "Int64":"int64_t",
        "Bool":"bool"
        }

dictStringSize = {
        "String32":32,
        "String64":64,
        "String128":128,
        "UInt8":0,
        "Int8":0,
        "UInt16":0,
        "Int16":0,
        "UInt32":0,
        "Int32":0,
        "UInt64":0,
        "Int64":0,
        "Bool":0
        }

dictCCopyFunc = {
        "String32":"strncpy(stcAppConfig.%VAR_NAME%,%VAR_NAME%,32);",
        "String64":"strncpy(stcAppConfig.%VAR_NAME%,%VAR_NAME%,64);",
        "String128":"strncpy(stcAppConfig.%VAR_NAME%,%VAR_NAME%,128);",
        "UInt8":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "Int8":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "UInt16":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "Int16":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "UInt32":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "Int32":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "UInt64":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "Int64":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;",
        "Bool":"stcAppConfig.%VAR_NAME% = %VAR_NAME%;"
        }
        
dictWebVarType = {
        "String32":"enWebConfigTypeStringLen32",
        "String64":"enWebConfigTypeStringLen64",
        "String128":"enWebConfigTypeStringLen128",
        "UInt8":"enWebConfigTypeUInt8",
        "Int8":"enWebConfigTypeInt8",
        "UInt16":"enWebConfigTypeUInt16",
        "Int16":"enWebConfigTypeInt16",
        "UInt32":"enWebConfigTypeUInt32",
        "Int32":"enWebConfigTypeInt32",
        "UInt64":"enWebConfigTypeUInt64",
        "Int64":"enWebConfigTypeInt64",
        "Bool":"enWebConfigTypeBool"
        }

def getDataInit(varName,varType,varInit):
    varCType = dictCVarType.get(varType)
    if varCType == "char*":
        return "\"" + varInit + "\""
    return varInit

def genDataInit(varName,varType,varInit):
    return "  " + getDataInit(varName,varType,varInit) + ", // " + varName + "\r\n"
    
def genWebDefinitions(varName,varType,varDescription):
    varWebType = dictWebVarType.get(varType)
    webDef = ""
    webDef += "    {%WEB_VAR_TYPE%,\"%VAR_NAME%\",\"%VAR_DESCRIPTION%\"},\r\n"
    webDef = webDef.replace("%VAR_NAME%",varName)
    webDef = webDef.replace("%WEB_VAR_TYPE%",varWebType)
    webDef = webDef.replace("%VAR_DESCRIPTION%",varDescription)
    return webDef

def genFuncImplementation(varName,varType,varDescription):
    varCType = dictCVarType.get(varType)
    varCopy = dictCCopyFunc.get(varType)
    funcImplementation =  ""
    funcImplementation += "/**********************************************\r\n"
    funcImplementation += " * Get %VAR_NAME% - %VAR_DESCRIPTION%\r\n"
    funcImplementation += " * \r\n"
    funcImplementation += " * \\return %VAR_NAME%\r\n"
    funcImplementation += " **********************************************\r\n" 
    funcImplementation += " */\r\n"
    funcImplementation += "%VAR_TYPE% AppConfig_Get%VAR_NAME%(void)\r\n"
    funcImplementation += "{\r\n"
    funcImplementation += "  if (bInitDone == false)\r\n"
    funcImplementation += "  {\r\n"
    funcImplementation += "    AppConfig_Init(NULL);\r\n"
    funcImplementation += "  }\r\n"
    funcImplementation += "  return stcAppConfig.%VAR_NAME%;\r\n"
    funcImplementation += "}\r\n"
    funcImplementation += "\r\n"
    funcImplementation += "/*********************************************\r\n"
    funcImplementation += " * Set %VAR_NAME% - %VAR_DESCRIPTION%\r\n"
    funcImplementation += " * \r\n"
    funcImplementation += " * \\param %VAR_NAME% %VAR_DESCRIPTION%\r\n"
    funcImplementation += " * \r\n"
    funcImplementation += " ********************************************* \r\n"
    funcImplementation += " */\r\n"
    funcImplementation += "void AppConfig_Set%VAR_NAME%(%VAR_TYPE% %VAR_NAME%)\r\n"
    funcImplementation += "{\r\n"
    funcImplementation += "  if (bInitDone == false)\r\n"
    funcImplementation += "  {\r\n"
    funcImplementation += "    AppConfig_Init(NULL);\r\n"
    funcImplementation += "  }\r\n"
    funcImplementation += "  %VAR_COPY%\r\n"
    funcImplementation += "  AppConfig_Write();\r\n"
    funcImplementation += "}\r\n"
    funcImplementation = funcImplementation.replace("%VAR_COPY%",varCopy)
    funcImplementation = funcImplementation.replace("%VAR_NAME%",varName)
    funcImplementation = funcImplementation.replace("%VAR_TYPE%",varCType)
    funcImplementation = funcImplementation.replace("%VAR_DESCRIPTION%",varDescription)
    return funcImplementation

def genFuncPrototype(varName,varType,varDescription):
    varCType = dictCVarType.get(varType)
    funcPrototype =  ""
    funcPrototype += "%VAR_TYPE% AppConfig_Get%VAR_NAME%(void);\r\n"
    funcPrototype += "void AppConfig_Set%VAR_NAME%(%VAR_TYPE% %VAR_NAME%);\r\n"
    funcPrototype = funcPrototype.replace("%VAR_NAME%",varName)
    funcPrototype = funcPrototype.replace("%VAR_TYPE%",varCType)
    funcPrototype = funcPrototype.replace("%VAR_DESCRIPTION%",varDescription)
    return funcPrototype

def genInitSetupImpl(varName,varType,varInit):
    varCType = dictCVarType.get(varType)
    varInit = getDataInit(varName,varType,varInit)
    setupImpl =  ""
    setupImpl += "      AppConfig_Set%VAR_NAME%(%VAR_INIT%);\r\n"
    setupImpl = setupImpl.replace("%VAR_NAME%",varName)
    setupImpl = setupImpl.replace("%VAR_TYPE%",varCType)
    setupImpl = setupImpl.replace("%VAR_INIT%",varInit)
    return setupImpl

def genVarDefinition(varName,varType,varDescription):
    varCType = dictCVarType.get(varType)
    varStrLen = dictStringSize.get(varType)
    if varStrLen > 0:
        return "  char " + varName + "[" + str(varStrLen) + "];\r\n"
    return "  " + varCType + " " + varName + ";\r\n"

parser = argparse.ArgumentParser(description='Generate a new webserver project')
parser.add_argument('--name',required=True, help='project name')
parser.add_argument('--conf',required=True, help='application config file')
args = parser.parse_args()

from_dir = "template"
to_dir = args.name
distutils.dir_util.copy_tree(from_dir, to_dir)
os.rename(os.path.join(args.name,"template.ino"),os.path.join(args.name,args.name + ".ino"))
os.rename(os.path.join(args.name,"template.yaml"),os.path.join(args.name,args.name + ".yaml"))

# Opening JSON file
f = open(args.conf,)
  
# returns JSON object as 
# a dictionary
data = json.load(f)
  
dataFuncProto = "\r\n"
dataFuncImpl = "\r\n"
dataWebDef = "\r\n"
dataVarInit = "\r\n"
dataVarSetup = "\r\n"
dataVarDef = "\r\n"
for var_data in data['variables']:
    dataFuncProto += genFuncPrototype(varName=var_data["name"],varType=var_data["type"],varDescription=var_data["description"])
    dataFuncImpl += genFuncImplementation(varName=var_data["name"],varType=var_data["type"],varDescription=var_data["description"])
    dataWebDef += genWebDefinitions(varName=var_data["name"],varType=var_data["type"],varDescription=var_data["description"])
    dataVarInit += genDataInit(varName=var_data["name"],varType=var_data["type"],varInit=var_data["initial"])
    dataVarSetup += genInitSetupImpl(varName=var_data["name"],varType=var_data["type"],varInit=var_data["initial"])
    dataVarDef += genVarDefinition(varName=var_data["name"],varType=var_data["type"],varDescription=var_data["description"])
# Closing file
f.close()

fname_appconfig_cpp = os.path.join(args.name,"appconfig.cpp")
fname_appconfig_h = os.path.join(args.name,"appconfig.h")

with open(fname_appconfig_cpp, 'r') as file :
  fdata_appconfig_cpp = file.read()
with open(fname_appconfig_h, 'r') as file :
  fdata_appconfig_h = file.read()

# Replace the target string
fdata_appconfig_h = fdata_appconfig_h.replace('MYAPP', args.name)
fdata_appconfig_h = fdata_appconfig_h.replace('/*APPFUNC_PROTOTYPES*/', dataFuncProto)
fdata_appconfig_h = fdata_appconfig_h.replace('/*APPVARS_DEFINITION*/', dataVarDef)
fdata_appconfig_cpp = fdata_appconfig_cpp.replace('/*APPFUNC_IMPLEMENT*/', dataFuncImpl)
fdata_appconfig_cpp = fdata_appconfig_cpp.replace('/*APPVARS_INIT*/', dataVarInit)
fdata_appconfig_cpp = fdata_appconfig_cpp.replace('/*APPVARS_INIT_SETUP*/', dataVarSetup)
fdata_appconfig_cpp = fdata_appconfig_cpp.replace('/*APPVARS_WEB_DEFINITION*/', dataWebDef)

# Write the file out again
with open(fname_appconfig_cpp, 'w') as file:
  file.write(fdata_appconfig_cpp)
# Write the file out again
with open(fname_appconfig_h, 'w') as file:
  file.write(fdata_appconfig_h)

