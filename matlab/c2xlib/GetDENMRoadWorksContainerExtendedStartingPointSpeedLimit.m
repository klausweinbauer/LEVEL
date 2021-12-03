classdef GetDENMRoadWorksContainerExtendedStartingPointSpeedLimit < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [DeltaLatitude, DeltaLongitude, DeltaAltitude] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                DeltaLatitude = int32(0);
                DeltaLongitude = int32(0);
                DeltaAltitude = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMRoadWorksContainerExtendedStartingPointSpeedLimit', StationID, SequenceNumber, ...
                    coder.ref(DeltaLatitude), coder.ref(DeltaLongitude), coder.ref(DeltaAltitude));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMRoadWorksContainerExtendedStartingPointSpeedLimit';
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