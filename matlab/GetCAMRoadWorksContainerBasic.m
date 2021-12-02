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
                coder.ceval('getCAMRoadWorksContainerBasic', StationID, coder.ref(EmbarkationStatus), ...
                    coder.ref(PtActivationType), coder.ref(PtActivationData), obj.PtActivationDataSize);
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
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            libPath = 'C:\Program Files\Polyspace\R2021a\extern\lib\win64\c2x';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libPath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end
    end
    methods (Access = protected)
        function [RoadworksSubCauseCode, LightBarSirenInUse, ClosedLaneInnerhardShoulderStatus, ...
            ClosedLaneOuterhardShoulderStatus, ClosedLaneDrivingLaneStatus] = getOutputSizeImpl(obj)
            RoadworksSubCauseCode = [1 1];
            LightBarSirenInUse = [1 1];
            ClosedLaneInnerhardShoulderStatus = [1 1];
            ClosedLaneOuterhardShoulderStatus = [1 1];
            ClosedLaneDrivingLaneStatus = [1 1];
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
            LightBarSirenInUse = 'int32';
            ClosedLaneInnerhardShoulderStatus = 'int32';
            ClosedLaneOuterhardShoulderStatus = 'int32';
            ClosedLaneDrivingLaneStatus = 'int32';
            
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