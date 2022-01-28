classdef DecodeCAM < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        EncodingType = 'DER_BER';
    end
    
    properties (Hidden, Constant)
        EncodingTypeSet = matlab.system.StringSet({'XER_BASIC', 'XER_CANONICAL', 'DER_BER'})
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [StationID] = stepImpl(obj, Buffer, BufferSize)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                StationID = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                encodingType = 0;
                if strcmp(obj.EncodingType, 'XER_CANONICAL')
                    encodingType = 1;
                elseif strcmp(obj.EncodingType, 'DER_BER')
                    encodingType = 2;
                end
                err = coder.ceval('decodeCAM', coder.ref(StationID), coder.ref(Buffer), BufferSize, encodingType);
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'DecodeCAM';
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
            LibConfig.updateBuildInfoCAM(buildInfo, buildContext);
        end
    end    
end