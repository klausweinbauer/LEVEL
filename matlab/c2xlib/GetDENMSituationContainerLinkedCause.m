classdef GetDENMSituationContainerLinkedCause < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [CauseCode, SubCauseCode] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                CauseCode = int32(0);
                SubCauseCode = int32(0);
                coder.cinclude('c2xdenm.h');
                coder.ceval('getDENMSituationContainerLinkedCause', StationID, SequenceNumber, coder.ref(CauseCode), coder.ref(SubCauseCode));
            end            
        end
        
        function releaseImpl(~)            
        end       

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMSituationContainerLinkedCause';
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
            libName =  strcat('c2xdenm', linkLibExt);
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