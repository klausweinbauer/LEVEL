classdef ProtectedCommunicationZone < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [ProtectedCommunicationZone] = stepImpl(obj, ProtectedZoneType, ExpiryTime, ...
            ProtectedZoneLatitude, ProtectedZoneLongitude, ProtectedZoneRadius, ProtectedZoneID) 
            ProtectedCommunicationZone = int32(transpose([ProtectedZoneType, ExpiryTime, ...
                ProtectedZoneLatitude, ProtectedZoneLongitude, ProtectedZoneRadius, ProtectedZoneID]));
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetCAMBasicContainer';
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
        function [ProtectedCommunicationZone] = getOutputSizeImpl(obj)
            ProtectedCommunicationZone = [6 1];
        end 
        
        function [ProtectedCommunicationZone] = isOutputFixedSizeImpl(obj)
            ProtectedCommunicationZone = true;
        end
        
        function [ProtectedCommunicationZone] = getOutputDataTypeImpl(obj)
            ProtectedCommunicationZone = 'int32';
        end
        
        function [ProtectedCommunicationZone] = isOutputComplexImpl(obj)
            ProtectedCommunicationZone = false;
        end
    end
end