classdef SetCAMPublicTransportContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, EmbarkationStatus, PtActivationType, PtActivationData) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpPtActivationData = uint8(PtActivationData);
                coder.cinclude('c2xcam.h');
                err = coder.ceval('setCAMPublicTransportContainer', StationID, int32(EmbarkationStatus), ...
                    int32(PtActivationType), coder.ref(TmpPtActivationData), length(TmpPtActivationData));
                obj.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

        function printErrorCode(~, err)
            if (err ~= 0)
                MsgBytes = uint8(zeros(255, 1));
                MsgLength = int32(0);
                coder.ceval('getLastErrMsg', coder.ref(MsgBytes), length(MsgBytes), coder.ref(MsgLength));
                Msg = ascii2str(MsgBytes);
                error(Msg);
            end
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetCAMPublicTransportContainer';
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