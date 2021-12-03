classdef CAMMessage < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        StationID = 0;
        HeighFrequencyContainerType = 0;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude('c2xcam.h');
                err = coder.ceval('createCAM', obj.StationID, obj.HeighFrequencyContainerType);
                obj.printErrorCode(err);
            end 
        end
        
        function [StationID] = stepImpl(obj)
            StationID = obj.StationID;                       
        end
        
        function releaseImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude('c2xcam.h');
                err = coder.ceval('deleteCAM', obj.StationID);
                obj.printErrorCode(err);
            end    
        end

        function printErrorCode(~, err)
            if (err < 0)
                MsgBytes = uint8(zeros(255, 1));
                MsgLength = int32(0);
                coder.ceval('getLastErrMsg', coder.ref(MsgBytes), length(MsgBytes), coder.ref(MsgLength));
                error(char(MsgBytes));
            end
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMMessage';
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