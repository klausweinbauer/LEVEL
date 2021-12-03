classdef GetDENMSituationContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [InformationQuality, CauseCode, SubCauseCode] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                InformationQuality = int32(0);
                CauseCode = int32(0);
                SubCauseCode = int32(0);
                coder.cinclude('c2xdenm.h');
                coder.ceval('getDENMSituationContainer', StationID, SequenceNumber, coder.ref(InformationQuality), coder.ref(CauseCode), coder.ref(SubCauseCode));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMSituationContainer';
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