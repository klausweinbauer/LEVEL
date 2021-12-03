classdef GetCAMRoadWorksContainerBasic < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        LightBarSirenInUseSize = 1;
        ClosedLaneDrivingLaneStatusSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [RoadworksSubCauseCode, LightBarSirenInUse, ClosedLaneInnerhardShoulderStatus, ...
            ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                RoadworksSubCauseCode = int32(0);
                LightBarSirenInUse = uint8(zeros(obj.LightBarSirenInUseSize, 1));
                ClosedLaneInnerhardShoulderStatus = int32(0);
                ClosedLaneOuterhardShoulderStatus = int32(0);
                ClosedLaneDrivingLaneStatus = uint8(zeros(obj.ClosedLaneDrivingLaneStatusSize, 1));
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMRoadWorksContainerBasic', StationID, coder.ref(RoadworksSubCauseCode), ...
                    coder.ref(LightBarSirenInUse), length(LightBarSirenInUse), coder.ref(ClosedLaneInnerhardShoulderStatus), ...
                    coder.ref(ClosedLaneOuterhardShoulderStatus), coder.ref(ClosedLaneDrivingLaneStatus), length(ClosedLaneDrivingLaneStatus));
            end
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMRoadWorksContainerBasic';
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
    methods (Access = protected)
        function [RoadworksSubCauseCode, LightBarSirenInUse, ClosedLaneInnerhardShoulderStatus, ...
            ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus] = getOutputSizeImpl(obj)
            RoadworksSubCauseCode = [1 1];
            LightBarSirenInUse = [obj.LightBarSirenInUseSize 1];
            ClosedLaneInnerhardShoulderStatus = [1 1];
            ClosedLaneOuterhardShoulderStatus = [1 1];
            ClosedLaneDrivingLaneStatus = [obj.ClosedLaneDrivingLaneStatusSize 1];
        end 
        
        function [RoadworksSubCauseCode, LightBarSirenInUse, ClosedLaneInnerhardShoulderStatus, ...
            ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus] = isOutputFixedSizeImpl(obj)
            RoadworksSubCauseCode = true;
            LightBarSirenInUse = true;
            ClosedLaneInnerhardShoulderStatus = true;
            ClosedLaneOuterhardShoulderStatus = true;
            ClosedLaneDrivingLaneStatus = true;
        end
        
        function [RoadworksSubCauseCode, LightBarSirenInUse, ClosedLaneInnerhardShoulderStatus, ...
            ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus] = getOutputDataTypeImpl(obj)
            RoadworksSubCauseCode = 'int32';
            LightBarSirenInUse = 'uint8';
            ClosedLaneInnerhardShoulderStatus = 'int32';
            ClosedLaneOuterhardShoulderStatus = 'int32';
            ClosedLaneDrivingLaneStatus = 'uint8';
            
        end
        
        function [RoadworksSubCauseCode, LightBarSirenInUse, ClosedLaneInnerhardShoulderStatus, ...
            ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus] = isOutputComplexImpl(obj)
            RoadworksSubCauseCode = false;
            LightBarSirenInUse = false;
            ClosedLaneInnerhardShoulderStatus = false;
            ClosedLaneOuterhardShoulderStatus = false;
            ClosedLaneDrivingLaneStatus = false;
        end
    end
end