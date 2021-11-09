import re
import os.path

########## Parameters ##########

libName = 'c2xcam'
headerFile = './include/c2xcam.h'
templateDirectory = './matlab/templates'
defaultTemplate = 'simulink-component.template'
outputDirectory = './matlab/c2xlib/'

simulinkBlockDefine = "SIMULINK_BLOCK"
simulinkNontunableProperty = "SIMULINK_NONTUN_PROP"
simulinkInputProperty = "SIMULINK_INPUT"
simulinkOutputMatrixProperty = "SIMULINK_OUTPUT_MATRIX"

errorCodes = {
    -1: "ERR_MSG_NOT_FOUND",
    -2: "ERR_ALLOC_FAILED",
    -3: "ERR_NULL",
    -4: "ERR_BUFFER_OVERFLOW",
    -5: "ERR_ENCODE",
    -6: "ERR_DECODE",
    -7: "ERR_RECEIVER_START",
    -8: "ERR_TRANSMITTER_START",
    -9: "ERR_ARG_NULL",
    -20: "ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE",
    -21: "ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE",
    -22: "ERR_CAM_ALREADY_EXISTS",
    -40: "ERR_DENM_ALREADY_EXISTS"
}

################################

errorCodeFunctionName = "printErrorCode"

def getClassName(functionDefinition):
    return functionDefinition[1][0].capitalize() + functionDefinition[1][1:] 

class Parameter:
    isPointer = False
    isNontunable = False
    isInput = False
    isOutput = False
    outputSize = ["1", "1"]
    name = ""
    dataType = ""

def getParameterList(functionDefinition):
    parameters = []
    tmpRawParameters = functionDefinition[2].replace('\n', '').split(',')
    for rP in tmpRawParameters:
        raw = rP.strip()
        param = Parameter()
        param.isPointer = '*' in raw
        param.isNontunable = simulinkNontunableProperty in raw
        param.isInput = (not param.isPointer or simulinkInputProperty in raw) and not param.isNontunable 
        param.isOutput = param.isPointer and not param.isNontunable and not param.isInput
        if simulinkOutputMatrixProperty in raw:
            mProp = re.search(simulinkOutputMatrixProperty + "_([A-Z0-9]+)_([A-Z0-9]+)", raw)
            param.outputSize = [mProp.group(1), mProp.group(2)]
        dtMatch = re.search("(int|uint8_t)", raw)
        if not dtMatch:
            print("[ERROR] Unknown data type in function definition: " + rP)
            exit(0)
        param.dataType = dtMatch.group()            
        param.name = re.search("([a-zA-Z]+)$", raw).group()
        param.name = param.name[0].capitalize() + param.name[1:]
        parameters.append(param)
    for i in range(len(parameters)):
            offset0 = ord(parameters[i].outputSize[0]) - (ord("A") - 1)
            offset1 = ord(parameters[i].outputSize[1]) - (ord("A") - 1)
            if offset0 > 0:
                parameters[i].outputSize[0] = "obj." + parameters[i + offset0].name
            if offset1 > 0:
                parameters[i].outputSize[1] = "obj." + parameters[i + offset0].name
    return parameters

def getCFunctionName(functionDefinition):
    return functionDefinition[1]

def fName(name):
    return name[0].capitalize() + name[1:]

def getOutputParameterList(paramList):
    out = []
    for param in paramList:
        if param.isOutput:
            out.append(fName(param.name))
    return ", ".join(out)

def getInputParameterList(paramList):
    out = []
    for param in paramList:
        if param.isInput:
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
        '__nontunableProperties__': getNontunableProperties(paramList),
        '__errorCodeFunction__': createErrorFunction(),
        '__errorCodeFunctionName__': errorCodeFunctionName,
        '__outputSizeImpl__' : getOutputSizeImpl(paramList),
        '__outputFixedSizeImpl__' : getOutputFixedSizeImpl(paramList),
        '__outputDataTypeImpl__' : getOutputDataTypeImpl(paramList),
        '__outputComplexImpl__' : getOutputComplexImpl(paramList)
    }

def getFileName(functionDefinition):
    return getClassName(functionDefinition) + ".m"


def loadTemplate(functionDefinition):
    content = ""
    path = os.path.join(templateDirectory, defaultTemplate)
    fPath = os.path.join(templateDirectory, getClassName(functionDefinition)) + ".template"
    if os.path.isfile(fPath):
        print("[INFO] Found custom template for " + getClassName(functionDefinition))
        path = fPath
    if not os.path.isfile(path):
        print("[ERROR] No template found.")
        exit(-1)
    with open(path) as f:
        content = f.read()
    return content
    
def createErrorFunction():
    code = "function " + errorCodeFunctionName + "(err)\n"
    code += "   if "
    for err in errorCodes:
        code += "(err == " + str(err) + ")\n"
        code += "       error('" + str(errorCodes[err]) + "')\n"
        code += "   elseif "
    code = code[:len(code) - 7]
    code += "end\nend\n"
    return code

def getOutputSizeImpl(parameterList):
    output = ""
    for i in range(len(parameterList)):
        param = parameterList[i]
        if param.isOutput:
            output += param.name + " = [" + param.outputSize[0] + ", " + param.outputSize[1] + "];\n";
    return output

def getOutputFixedSizeImpl(parameterList):
    output = ""
    for i in range(len(parameterList)):
        param = parameterList[i]
        if param.isOutput:
            output += "true;\n"
    return output

def getOutputDataTypeImpl(parameterList):
    output = ""
    for i in range(len(parameterList)):
        param = parameterList[i]
        if param.isOutput:
            if param.dataType == "int":
                output += "'int32';\n"
            elif param.dataType == "uint8_t":
                output += "'uint8';\n"
            else:
                print("[ERROR] " + param.dataType + " not implemented.")
                exit(-1)
    return output

def getOutputComplexImpl(parameterList):
    output = ""
    for i in range(len(parameterList)):
        param = parameterList[i]
        if param.isOutput:
            output += "false;\n"
    return output

functionDefinitionRegex = "(int|void).*" + simulinkBlockDefine + ".*[ ]([a-zA-Z_]+)\(([a-zA-Z0-9, *\n_]*)\);"

headerFileTxt = ""
with open(headerFile) as f:
    headerFileTxt = f.read()

functionDefinitions = re.findall(functionDefinitionRegex, headerFileTxt)
for functionDefinition in functionDefinitions:
    print("[INFO] ##### Build matlab class for " + getClassName(functionDefinition))
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

    