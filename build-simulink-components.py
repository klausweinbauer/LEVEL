import re
import os.path

########## Parameters ##########

libName = 'c2xcam'
headerFile = './include/c2xcam.h'
templateDirectory = './matlab/templates'
defaultTemplate = 'simulink-component.template'
outputDirectory = './matlab/c2xlib/'

simulinkBlockDefine = "SIMULINK_BLOCK"
simulinkNontunableProperty = "SIMULINK_NONTUNABLE_PROPERTY"

################################

def getClassName(functionDefinition):
    fName = functionDefinition[1]
    if fName.startswith(("get", "set", "add")):
        fName = fName[3:]
    return fName

class Parameter:
    isPointer = False
    isNontunable = False
    name = ""
    dataType = ""

def getParameterList(functionDefinition):
    parameters = []
    tmpRawParameters = functionDefinition[2].replace('\n', '').split(',')
    for rP in tmpRawParameters:
        raw = rP.strip()
        param = Parameter()
        param.isPointer = '*' in raw
        dtMatch = re.search("(int|uint8_t)", raw)
        if not dtMatch:
            print("[ERROR] Unknown data type in function definition: " + rP)
            exit(0)
        param.dataType = dtMatch.group()            
        param.name = re.search("([a-zA-Z]+)$", raw).group()
        param.isNontunable = simulinkNontunableProperty in raw
        parameters.append(param)
    return parameters

def getCFunctionName(functionDefinition):
    return functionDefinition[1]

def fName(name):
    return name[0].capitalize() + name[1:]

def getOutputParameterList(paramList):
    out = []
    for param in paramList:
        if param.isPointer and not param.isNontunable:
            out.append(fName(param.name))
    return ", ".join(out)

def getInputParameterList(paramList):
    out = []
    for param in paramList:
        if not param.isPointer and not param.isNontunable:
            out.append(fName(param.name))
    return ", ".join(out)

def getNontunableProperties(paramList):
    out = []
    for param in paramList:
        if param.isNontunable:
            out.append(fName(param.name) + " = 0;")
    return "\n".join(out)

def getCoderParamList(paramList):
    out = []
    for param in paramList:
        if param.isPointer:
            out.append("coder.wref(" + fName(param.name) + ")")
        elif param.isNontunable:
            out.append("obj." + fName(param.name))
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
        '__libName__': libName,
        '__nontunableProperties__': getNontunableProperties(paramList)
    }

def getFileName(functionDefinition):
    return getClassName(functionDefinition) + ".m"


def loadTemplate(functionDefinition):
    content = ""
    path = os.path.join(templateDirectory, defaultTemplate)
    fPath = os.path.join(templateDirectory, getClassName(functionDefinition)) + ".template"
    if os.path.isfile(fPath):
        print("Found Template for " + getClassName(functionDefinition))
        path = fPath
    if not os.path.isfile(path):
        print("[ERROR] No template found.")
        exit(-1)
    with open(path) as f:
        content = f.read()
    return content
    

functionDefinitionRegex = "(int|void).*" + simulinkBlockDefine + ".*[ ]([a-zA-Z_]+)\(([a-zA-Z0-9, *\n_]*)\);"

headerFileTxt = ""
with open(headerFile) as f:
    headerFileTxt = f.read()

functionDefinitions = re.findall(functionDefinitionRegex, headerFileTxt)
for functionDefinition in functionDefinitions:
    paramList = getParameterList(functionDefinition)
    matlabFileContent = loadTemplate(functionDefinition)
    templateObjectMappings = resolveTemplateObjects(functionDefinition, paramList)
    for key in templateObjectMappings.keys():
        matlabFileContent = matlabFileContent.replace(key, templateObjectMappings[key])
    match = re.search("__([a-zA-Z]+)__", matlabFileContent)
    if match:
        print("[ERROR] Unknown template object: " + match.group())
        exit(-1)
    with open(outputDirectory + getFileName(functionDefinition), "w") as oFile:
        oFile.write(matlabFileContent)

    