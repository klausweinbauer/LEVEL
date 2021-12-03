classdef GetCAMSafetyCarContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        LightBarSirenInUseSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [LightBarSirenInUse, CauseCode, SubCauseCode, TrafficRule, SpeedLimit] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                LightBarSirenInUse = uint8(zeros(obj.LightBarSirenInUseSize, 1));
                CauseCode = int32(0);
                SubCauseCode = int32(0);
                TrafficRule = int32(0);
                SpeedLimit = int32(0);
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMSafetyCarContainer', StationID, coder.ref(LightBarSirenInUse), length(LightBarSirenInUse), ...
                    coder.ref(CauseCode), coder.ref(SubCauseCode), coder.ref(TrafficRule), coder.ref(SpeedLimit));
            end
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMSafetyCarContainer';
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
        function [LightBarSirenInUse, CauseCode, SubCauseCode, TrafficRule, SpeedLimit] = getOutputSizeImpl(obj)
            LightBarSirenInUse = [obj.LightBarSirenInUseSize 1];
            CauseCode = [1 1];
            SubCauseCode = [1 1];
            TrafficRule = [1 1];
            SpeedLimit = [1 1];
        end 
        
        function [LightBarSirenInUse, CauseCode, SubCauseCode, TrafficRule, SpeedLimit] = isOutputFixedSizeImpl(obj)
            LightBarSirenInUse = true;
            CauseCode = true;            
            SubCauseCode = true;            
            TrafficRule = true;            
            SpeedLimit = true;
        end
        
        function [LightBarSirenInUse, CauseCode, SubCauseCode, TrafficRule, SpeedLimit] = getOutputDataTypeImpl(obj)
            LightBarSirenInUse = 'uint8'; 
            CauseCode = 'int32';
            SubCauseCode = 'int32';
            TrafficRule = 'int32';
            SpeedLimit = 'int32';
        end
        
        function [LightBarSirenInUse, CauseCode, SubCauseCode, TrafficRule, SpeedLimit] = isOutputComplexImpl(obj)
            LightBarSirenInUse = false;
            CauseCode = false;
            SubCauseCode = false;
            TrafficRule = false;
            SpeedLimit = false;
        end
    end
end