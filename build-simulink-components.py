import re

########## Parameters ##########

libName = 'c2xcam'
headerFile = './include/c2xcam.h'
templateFile = 'simulink-component.template'
simulinkBlockDefine = "SIMULINK_BLOCK"
outputDirectory = './matlab/c2xlib/'

################################

def getClassName(functionDefinition):
    fName = functionDefinition[1]
    if fName.startswith(("get", "set", "add")):
        fName = fName[3:]
    return fName;

class Parameter:
    isPointer = False
    name = ""
    dataType = ""

def getParameterList(functionDefinition):
    parameters = []
    tmpRawParameters = functionDefinition[2].replace('\n', '').split(',')
    for rP in tmpRawParameters:
        raw = rP.strip()
        param = Parameter()
        param.isPointer = '*' in raw
        param.dataType = re.search("^([a-zA-Z0-9_]+)", raw).group()
        param.name = re.search("([a-zA-Z]+)$", raw).group()
        parameters.append(param)
        
    return parameters

def getCFunctionName(functionDefinition):
    return functionDefinition[1]

def fName(name):
    return name[0].capitalize() + name[1:]

def getOutputParameterList(paramList):
    out = []
    for param in paramList:
        if (param.isPointer):
            out.append(fName(param.name))
    return ", ".join(out)

def getInputParameterList(paramList):
    out = []
    for param in paramList:
        if not param.isPointer:
            out.append(fName(param.name))
    return ", ".join(out)

def getCoderParamList(paramList):
    out = []
    for param in paramList:
        if param.isPointer:
            out.append("coder.wref(" + fName(param.name) + ")")
        else:
            out.append(fName(param.name))
    return ", ".join(out)

def getNextTemplateObject(templateText):
    return re.search("__([a-zA-Z]+)__", templateText)

def resolveTemplateObjects(functionDefinition, paramList):
    return {
        '__className__': getClassName(functionDefinition),
        '__outputParameterList__': getOutputParameterList(paramList),
        '__inputParameterList__': getInputParameterList(paramList),
        '__cFunctionName__': getCFunctionName(functionDefinition),
        '__coderParameterList__': getCoderParamList(paramList),
        '__libName__': libName
    }

def getFileName(functionDefinition):
    return getClassName(functionDefinition) + ".m"

functionDefinitionRegex = "(int|void).*" + simulinkBlockDefine + ".*[ ]([a-zA-Z_]+)\(([a-zA-Z0-9, *\n_]*)\);"

headerFileTxt = ""
with open(headerFile) as f:
    headerFileTxt = f.read()

templateFileTxt = ""
with open(templateFile) as f:
    templateFileTxt = f.read()

functionDefinitions = re.findall(functionDefinitionRegex, headerFileTxt)
for functionDefinition in functionDefinitions:
    paramList = getParameterList(functionDefinition)
    matlabFileTxt = templateFileTxt
    templateObjectMappings = resolveTemplateObjects(functionDefinition, paramList)
    for key in templateObjectMappings.keys():
        matlabFileTxt = matlabFileTxt.replace(key, templateObjectMappings[key])
    match = re.search("__([a-zA-Z]+)__", matlabFileTxt)
    if match:
        print("[ERROR] Unknown template object: " + match.group())
        exit(-1)
    with open(outputDirectory + getFileName(functionDefinition), "w") as oFile:
        oFile.write(matlabFileTxt)

    