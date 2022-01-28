classdef CAMTransmitter < matlab.System & coder.ExternalDependency
    % IDsToTransmit is a vector of StationIDs which should be sent from this instance
    
    properties (Nontunable)
        Port = 1997;
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
                coder.cinclude(LibConfig.getCAMHeader());
                encodingType = 0;
                if strcmp(obj.EncodingType, 'XER_CANONICAL')
                    encodingType = 1;
                elseif strcmp(obj.EncodingType, 'DER_BER')
                    encodingType = 2;
                end
                coder.ceval('startCAMTransmitter', obj.Port, encodingType);
            end 
        end
        
        function [] = stepImpl(obj, IDsToTransmit, f_send) 
            Size = length(IDsToTransmit);
            tmpIDsToTransmit = int32(IDsToTransmit);
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('setCAMTransmissionFrequency', f_send);
                coder.ceval('setCAMIDsForTransmission', coder.ref(tmpIDsToTransmit), Size);
            end            
        end
        
        function releaseImpl(~)   
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('stopCAMTransmitter');
            end          
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMTransmitter';
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