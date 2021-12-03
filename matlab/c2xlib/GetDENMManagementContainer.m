classdef GetDENMManagementContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [DetectionTime, ReferenceTime, Termination, RelevanceDistance, ...
                RelevanceTrafficDirection, ValidityDuration, TransmissionInterval, StationType] ...
                = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                DetectionTime = int32(0);
                ReferenceTime = int32(0);
                Termination = int32(0);
                RelevanceDistance = int32(0);
                RelevanceTrafficDirection = int32(0);
                ValidityDuration = int32(0);
                TransmissionInterval = int32(0);
                StationType = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('getDENMManagementContainer', StationID, SequenceNumber, coder.ref(DetectionTime), ...
                    coder.ref(ReferenceTime), coder.ref(Termination), coder.ref(RelevanceDistance), coder.ref(RelevanceTrafficDirection), ...
                    coder.ref(ValidityDuration), coder.ref(TransmissionInterval), coder.ref(StationType));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMManagementContainer';
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