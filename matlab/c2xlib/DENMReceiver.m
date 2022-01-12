classdef DENMReceiver < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        Port = 1998;  
        EncodingType = 'DER_BER';
    end
    
    properties (Hidden, Constant)
        EncodingTypeSet = matlab.system.StringSet({'XER_BASIC', 'XER_CANONICAL', 'DER_BER'})
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getDENMHeader());
                encodingType = 0;
                if strcmp(obj.EncodingType, 'XER_CANONICAL')
                    encodingType = 1;
                elseif strcmp(obj.EncodingType, 'DER_BER')
                    encodingType = 2;
                end
                coder.ceval('startDENMReceiver', obj.Port, encodingType);
            end 
        end
        
        function stepImpl(obj)                   
        end
        
        function releaseImpl(~)  
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('stopDENMReceiver');
            end           
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'DENMReceiver';
        end
        
        function supported = isSupportedContext(buildContext)
            myTarget = {'rtw'};
            if  buildContext.isCodeGenTarget(myTarget)
                supported = true;
            else
                error('API only supported for mex, lib, exe, dll');
            end
        end
        
        function updateBuildInfo(buildInfo, buildContext)
            LibConfig.updateBuildInfoDENM(buildInfo, buildContext);
        end
    end
end