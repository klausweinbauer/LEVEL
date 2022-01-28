classdef DENMTransmitter < matlab.System & coder.ExternalDependency
    % IDsToTransmit is a vector of StationIDs which should be sent from this instance
    
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
                coder.ceval('startDENMTransmitter', obj.Port, encodingType);
            end 
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, f_send) 
            tmpStationID = int32(StationID);
            tmpSequenceNumber = int32(SequenceNumber);
            if coder.target('Rtw') || coder.target('Sfun')
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('setDENMTransmissionFrequency', f_send);
                coder.ceval('setDENMTransmissionSource', tmpStationID, tmpSequenceNumber);                
            end            
        end
        
        function releaseImpl(~)   
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('stopDENMTransmitter');
            end          
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'DENMTransmitter';
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