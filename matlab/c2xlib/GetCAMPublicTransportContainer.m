classdef GetCAMPublicTransportContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        PtActivationDataSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [EmbarkationStatus, PtActivationType, PtActivationData] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                EmbarkationStatus = int32(0);
                PtActivationType = int32(0);
                PtActivationData = uint8(zeros(obj.PtActivationDataSize, 1));
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMPublicTransportContainer', StationID, coder.ref(EmbarkationStatus), ...
                    coder.ref(PtActivationType), coder.ref(PtActivationData), obj.PtActivationDataSize);
            end
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMPublicTransportContainer';
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
        function [EmbarkationStatus, PtActivationType, PtActivationData] = getOutputSizeImpl(obj)
            EmbarkationStatus = [1 1];
            PtActivationType = [1 1];
            PtActivationData = [obj.PtActivationDataSize 1];
        end 
        
        function [EmbarkationStatus, PtActivationType, PtActivationData] = isOutputFixedSizeImpl(obj)
            EmbarkationStatus = true;
            PtActivationType = true;
            PtActivationData = true;
        end
        
        function [EmbarkationStatus, PtActivationType, PtActivationData] = getOutputDataTypeImpl(obj)
            EmbarkationStatus = 'int32';
            PtActivationType = 'int32';
            PtActivationData = 'uint8';
        end
        
        function [EmbarkationStatus, PtActivationType, PtActivationData] = isOutputComplexImpl(obj)
            EmbarkationStatus = false;
            PtActivationType = false;
            PtActivationData = false;
        end
    end
end