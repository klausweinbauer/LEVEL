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
        
        function [ProtectedZoneType, ExpiryTime, ProtectedZoneLatitude, ProtectedZoneLongitude, ...
            ProtectedZoneRadius, ProtectedZoneID] = stepImpl(obj, ProtectedCommunicationZone) 
            if length(ProtectedCommunicationZone) ~= 7
                error("ProtectedCommunicationZone must be of length 7.");
            end
            ProtectedZoneType = int32(ProtectedCommunicationZone(1));
            ExpiryTime = int32(ProtectedCommunicationZone(2));
            ProtectedZoneLatitude = int32(ProtectedCommunicationZone(3));
            ProtectedZoneLongitude = int32(ProtectedCommunicationZone(4));
            ProtectedZoneRadius = int32(ProtectedCommunicationZone(5));
            ProtectedZoneID = int32(ProtectedCommunicationZone(6));
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
end