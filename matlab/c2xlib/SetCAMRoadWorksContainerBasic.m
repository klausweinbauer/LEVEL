classdef SetCAMRoadWorksContainerBasic < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, RoadworksSubCauseCode, LightBarSirenInUse, ... 
            ClosedLaneInnerhardShoulderStatus, ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpLightBarSirenInUse = uint8(LightBarSirenInUse);
                TmpClosedLaneDrivingLaneStatus = uint8(ClosedLaneDrivingLaneStatus);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('setCAMRoadWorksContainerBasic', StationID, RoadworksSubCauseCode, coder.ref(TmpLightBarSirenInUse), ...
                    length(TmpLightBarSirenInUse), ClosedLaneInnerhardShoulderStatus, ClosedLaneOuterhardShoulderStatus, ...
                    coder.ref(TmpClosedLaneDrivingLaneStatus), length(TmpClosedLaneDrivingLaneStatus));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetCAMRoadWorksContainerBasic';
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