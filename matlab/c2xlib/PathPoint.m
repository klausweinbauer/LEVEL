classdef PathPoint < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [PathPoint] = stepImpl(obj, DeltaLatitude, DeltaLongitude, DeltaAltitude, PathDeltaTime) 
            PathPoint = int32(transpose([DeltaLatitude DeltaLongitude DeltaAltitude PathDeltaTime]));
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
        function [PathPoint] = getOutputSizeImpl(obj)
            PathPoint = [4 1];
        end 
        
        function [PathPoint] = isOutputFixedSizeImpl(obj)
            PathPoint = true;
        end
        
        function [PathPoint] = getOutputDataTypeImpl(obj)
            PathPoint = 'int32';
        end
        
        function [PathPoint] = isOutputComplexImpl(obj)
            PathPoint = false;
        end
    end
end